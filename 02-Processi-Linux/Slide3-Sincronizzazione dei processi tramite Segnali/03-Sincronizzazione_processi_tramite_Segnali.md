# Sincronizzazione dei processi tramite Segnali Linux

*Come usare SIGUSR1, SIGCHLD e sigsuspend() per far collaborare processi concorrenti, senza race condition.*

## 1. Introduzione

Quando più processi cooperano per raggiungere uno scopo comune non basta che "funzionino": devono anche **coordinarsi**. Un processo deve aspettare che un altro abbia finito una fase, due processi devono alternarsi nell'uso di una risorsa, un padre deve sapere quando un figlio è terminato. Questo coordinamento si chiama **sincronizzazione**.

Unix/Linux offre diversi meccanismi di comunicazione e sincronizzazione tra processi (IPC): pipe, FIFO, code di messaggi, semafori, memoria condivisa, socket. Il più antico e al tempo stesso il più "leggero" è il **segnale**: una notifica asincrona che il kernel (o un altro processo) recapita a un processo, interrompendone il normale flusso di esecuzione.

In questo articolo vedremo:

- che cosa sono i segnali e quali sono i principali;
- come inviarli e come gestirli in C con `kill()` e `sigaction()`;
- perché una sincronizzazione ingenua con `pause()` è **sbagliata** e come correggerla con `sigprocmask()` e `sigsuspend()`;
- tre esempi completi (ping-pong padre/figlio, gestione di `SIGCHLD`, `trap` in Bash);
- best practice, tip & trick, esercizi e un quiz di autovalutazione con le risposte finali.

> **Prerequisiti:** conoscenza di base del linguaggio C, della shell Linux e delle chiamate di sistema `fork()` e `wait()`.

### Perché serve sincronizzare

Immaginiamo due processi che aggiornano lo stesso contatore salvato in un file. Entrambi leggono il valore 10, lo incrementano e lo riscrivono: il risultato finale è 11 invece di 12. Quando il risultato dipende dall'ordine temporale in cui i processi eseguono le operazioni si parla di **race condition**, e la sincronizzazione serve proprio a evitarla.

| Termine | Significato |
| --- | --- |
| **Sincronizzazione** | Imporre un ordine alle azioni di processi concorrenti |
| **Race condition** | Risultato che dipende dalla temporizzazione relativa dei processi |
| **Sezione critica** | Porzione di codice che accede a una risorsa condivisa e deve essere eseguita da un solo processo per volta |
| **Attesa passiva** | Il processo dorme finché l'evento atteso non si verifica |
| **Attesa attiva** (*busy waiting*) | Il processo controlla di continuo una condizione, sprecando CPU |

I segnali permettono l'attesa passiva: un processo si addormenta e il kernel lo risveglia quando riceve la notifica, senza cicli di controllo e senza consumo di CPU.

## 2. Gli interrupt software: dall'hardware ai segnali

Un interrupt interrompe il flusso normale della CPU, fa eseguire una routine di servizio (**ISR**) e poi riprende da dove ci si era fermati. I segnali applicano lo stesso schema ai processi: capire l'uno aiuta a capire l'altro.

| Tipo | Chi lo genera | Esempio |
| --- | --- | --- |
| **Interrupt hardware** | Una periferica, in modo asincrono | Tasto premuto, pacchetto di rete |
| **Eccezione** | La CPU, durante un'istruzione | Divisione per zero, accesso a memoria non valido |
| **Interrupt software (trap)** | Il programma, volontariamente | Chiamata di sistema (`syscall`, `int 0x80`, `svc`) |
| **Segnale Unix** | Il kernel o un altro processo, verso un processo | `SIGINT`, `SIGCHLD` |

L'eccezione è il ponte tra hardware e segnali. Se un programma scrive tramite un puntatore nullo, la CPU genera un *page fault*, il kernel riconosce l'accesso illegale e invia al processo `SIGSEGV`: la shell stampa "Segmentation fault" e `echo $?` mostra 139 (128 + 11).

Le analogie con i segnali sono dirette: la ISR corrisponde al *signal handler*, il mascheramento degli interrupt a `sigprocmask()`, gli interrupt non mascherabili a `SIGKILL` e `SIGSTOP`.

### Modalità utente e modalità kernel

La CPU lavora in almeno due modalità: i programmi girano in **modalità utente**, con accesso limitato alle risorse, mentre il kernel gira in **modalità privilegiata**. Interrupt, eccezioni e chiamate di sistema sono i tre modi per passare dalla prima alla seconda. Quando il kernel ha finito il suo lavoro e sta per restituire il controllo al processo, controlla se ci sono segnali in attesa di consegna: è questo il momento in cui un signal handler viene eseguito.

## 3. Che cosa sono i segnali

Un segnale è un **interrupt software**: un piccolo messaggio, identificato da un numero intero, che non trasporta dati (a parte il fatto stesso di essere arrivato). Può essere generato:

- dal **kernel**, a seguito di un evento hardware o di un errore (`SIGSEGV` per un accesso illegale alla memoria, `SIGFPE` per una divisione per zero);
- dal **terminale**, a seguito di una combinazione di tasti (`Ctrl+C` genera `SIGINT`, `Ctrl+Z` genera `SIGTSTP`);
- da **un altro processo** (o da se stesso) con la chiamata `kill()` o il comando `kill`;
- da un **timer** (`alarm()` genera `SIGALRM`);
- dal **kernel verso il padre** quando un figlio termina (`SIGCHLD`).

Per vedere l'elenco completo dei segnali del proprio sistema:

```bash
kill -l
```

### I segnali più importanti

| Segnale | N. (x86) | Azione di default | Significato |
| --- | --- | --- | --- |
| `SIGHUP` | 1 | Termina | Chiusura del terminale; spesso usato per far rileggere la configurazione ai demoni |
| `SIGINT` | 2 | Termina | Interruzione da tastiera (`Ctrl+C`) |
| `SIGKILL` | 9 | Termina | Terminazione forzata: **non catturabile, non ignorabile** |
| `SIGUSR1` | 10 | Termina | Segnale a disposizione dell'applicazione |
| `SIGSEGV` | 11 | Termina + core | Accesso non valido alla memoria |
| `SIGUSR2` | 12 | Termina | Secondo segnale a disposizione dell'applicazione |
| `SIGPIPE` | 13 | Termina | Scrittura su pipe senza lettori |
| `SIGALRM` | 14 | Termina | Scadenza di un timer impostato con `alarm()` |
| `SIGTERM` | 15 | Termina | Richiesta "gentile" di terminazione (default di `kill`) |
| `SIGCHLD` | 17 | Ignora | Un processo figlio è terminato o si è fermato |
| `SIGCONT` | 18 | Riprende | Riprende un processo fermato |
| `SIGSTOP` | 19 | Ferma | Sospensione forzata: **non catturabile, non ignorabile** |

> **Nota:** i numeri variano a seconda dell'architettura (ad esempio su alcune piattaforme `SIGUSR1` non vale 10). Nel codice usate **sempre i nomi simbolici**, mai i numeri.

### Le tre "disposizioni" di un segnale

Per ogni segnale un processo può scegliere:

1. **azione di default** (`SIG_DFL`): di norma terminare, ignorare o fermare il processo;
2. **ignorare** il segnale (`SIG_IGN`);
3. **catturarlo** installando una funzione, detta **signal handler**, che il kernel eseguirà al suo arrivo.

`SIGKILL` e `SIGSTOP` fanno eccezione: non possono essere né catturati né ignorati né bloccati, così l'amministratore ha sempre un modo per riprendere il controllo di un processo.

### Il ciclo di vita di un segnale

1. **Generazione**: il kernel o un processo produce il segnale.
2. **Pendente**: il segnale viene registrato come in attesa di consegna per il processo destinatario.
3. **Consegna**: quando il processo sta per tornare in esecuzione in modalità utente, il kernel controlla i segnali pendenti non bloccati e ne consegna uno.
4. **Gestione**: si applica la disposizione scelta (azione di default, ignora, handler).

Un segnale **bloccato** (presente nella maschera) resta pendente finché non viene sbloccato; non va confuso con un segnale **ignorato**, che viene semplicemente scartato. Questa distinzione è la base della sincronizzazione corretta che vedremo nella sezione 6.

### Segnali, `fork()` ed `exec()`

| Evento | Disposizioni (handler) | Maschera dei segnali | Segnali pendenti |
| --- | --- | --- | --- |
| `fork()` | Ereditate dal figlio | Ereditata dal figlio | **Non** ereditati (il figlio parte senza pendenti) |
| `exec()` | Gli handler tornano al default; i segnali ignorati restano ignorati | Conservata | Conservati |

L'ereditarietà della maschera è sfruttata nell'esempio ping-pong (sezione 7): bloccando `SIGUSR1` prima della `fork()`, anche il figlio parte con il segnale bloccato.

## 4. Inviare un segnale

### Da shell

```bash
kill -SIGUSR1 1234      # invia SIGUSR1 al processo con PID 1234
kill -TERM 1234         # equivalente a: kill 1234
kill -9 1234            # SIGKILL: ultima spiaggia
killall -USR1 miodemone # invia il segnale a tutti i processi con quel nome
```

### Da programma C

```c
#include <signal.h>

int kill(pid_t pid, int sig);   // invia sig al processo pid
int raise(int sig);             // invia sig al processo stesso
unsigned int alarm(unsigned int secondi); // SIGALRM tra N secondi
```

Il valore di `pid` ha significati particolari: se è maggiore di 0 il segnale va a quel processo; se è 0 va a tutti i processi del gruppo del chiamante; se è -1 a tutti i processi a cui si ha il permesso di inviarlo. Con `sig = 0` non viene inviato nulla, ma `kill()` controlla se il processo esiste ed è raggiungibile: un trucco utile per verificare che un PID sia ancora vivo.

### Valori di `pid` in `kill()`

| Valore di `pid` | Destinatari |
| --- | --- |
| maggiore di 0 | Il processo con quel PID |
| 0 | Tutti i processi del gruppo del chiamante |
| -1 | Tutti i processi a cui il chiamante può inviare segnali (tranne init) |
| minore di -1 | Tutti i processi del gruppo `-pid` |

### Permessi ed errori

Un processo può inviare un segnale solo ai processi dello stesso utente (o a qualunque processo se è eseguito da root). In caso di fallimento `kill()` restituisce -1 e imposta `errno`:

| `errno` | Significato |
| --- | --- |
| `ESRCH` | Il processo (o il gruppo) non esiste |
| `EPERM` | Non si hanno i permessi per inviare il segnale |
| `EINVAL` | Numero di segnale non valido |

### Trovare i PID da shell

```bash
pgrep -l miodemone                   # elenca PID e nome dei processi che corrispondono
pkill -USR1 -f "python3 server.py"   # invia SIGUSR1 ai processi con quella riga di comando
```

### Esempio: un timeout con `alarm()`

```c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// handler per SIGALRM: scrive un messaggio e termina il processo
static void on_alarm(int sig) {
    // il cast a void serve a evitare warning di compilazione per variabili non usate
    (void)sig;
    // Stampa ricevuto segnale SIGALRM
    // printf("Ricevuto segnale SIGALRM (%d)\n", sig);
    printf("Tempo scaduto!\n");
    // write() è sicura dentro un handler, printf() no
    // STDOUT_FILENO è il file descriptor della stdout
    // 15 è la lunghezza della stringa
    write(STDOUT_FILENO, "Tempo scaduto!\n", 15);
}

int main(void) {
    // installa l'handler per SIGALRM
    struct sigaction sa = {0};
    sa.sa_handler = on_alarm;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    // imposta un timer: tra 3 secondi il kernel invia SIGALRM
    alarm(3);                          // tra 3 secondi il kernel invia SIGALRM
    printf("Attendo 3 secondi...\n");
    pause();                           // dorme finché arriva un segnale
    return 0;
}
```

Il programma stampa il messaggio dopo tre secondi e termina. Qui `pause()` è tollerabile perché il segnale arriva tre secondi dopo; per eventi che possono arrivare in qualunque momento serve la tecnica della sezione 6.

## 5. Gestire un segnale: `sigaction()`

Esistono due modi per installare un handler: la vecchia `signal()` e la moderna `sigaction()`. La semantica di `signal()` **varia tra le versioni di Unix**; `sigaction()` è portabile e precisa, e va sempre preferita.

```c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t ricevuto = 0;

static void handler(int sig) {
    (void)sig;
    ricevuto = 1;          // l'handler fa il minimo indispensabile
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);   // nessun segnale extra bloccato nell'handler
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("PID %d: premi Ctrl+C\n", getpid());
    while (!ricevuto)
        pause();               // dorme finché non arriva un segnale
    printf("Segnale ricevuto, esco.\n");
    return 0;
}
```
Punti da notare:

- l'handler si limita a impostare un **flag**; il lavoro vero lo fa il programma principale;
- il flag è dichiarato `volatile sig_atomic_t`: `volatile` impedisce al compilatore di tenerlo in un registro, `sig_atomic_t` garantisce che la lettura e la scrittura siano atomiche;
- `pause()` sospende il processo fino all'arrivo di un qualunque segnale gestito.

esempio di prima, ma utilizzando la funzione `signal()`.
```c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t ricevuto = 0;

static void handler(int sig) {
    (void)sig;
    ricevuto = 1;
}

int main(void) {
    signal(SIGINT, handler);
    printf("PID %d: premi Ctrl+C\n", getpid());
    while (!ricevuto)
        pause();
    printf("Segnale ricevuto, esco.\n");
    return 0;
}
```


### Cosa succede mentre l'handler è in esecuzione

Il kernel blocca automaticamente il segnale che l'handler sta gestendo (a meno di `SA_NODEFER`) e quelli indicati in `sa_mask`. In questo modo lo stesso handler non viene riattivato in modo ricorsivo: i segnali arrivati nel frattempo restano pendenti (al massimo uno per tipo) e vengono consegnati al ritorno, quando la maschera originale è ripristinata.

### I flag più usati di `sa_flags`

| Flag | Effetto |
| --- | --- |
| `SA_RESTART` | Riavvia automaticamente le chiamate di sistema lente interrotte (`read()`, `wait()`...) invece di farle fallire con `EINTR` |
| `SA_NOCLDSTOP` | Non genera `SIGCHLD` quando un figlio si ferma o riprende |
| `SA_NODEFER` | Non blocca il segnale durante l'esecuzione del suo handler |
| `SA_RESETHAND` | Ripristina l'azione di default dopo la prima consegna |
| `SA_SIGINFO` | Usa un handler a tre argomenti con informazioni sul mittente |

### Sapere chi ha inviato il segnale: `SA_SIGINFO`

```c
static volatile sig_atomic_t mittente = 0;

static void handler(int sig, siginfo_t *si, void *ctx) {
    (void)sig; (void)ctx;
    mittente = si->si_pid;        // PID del processo che ha inviato il segnale
}

/* nel main: */
struct sigaction sa = {0};
sa.sa_sigaction = handler;        // non sa_handler: i due campi sono alternativi
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_SIGINFO;
sigaction(SIGUSR1, &sa, NULL);
```

Il campo `si_pid` permette, ad esempio, a un figlio di rispondere al mittente senza conoscerne il PID in anticipo; `si_uid` ne indica l'utente.

## 6. Il problema: sincronizzare con `pause()` è una race condition

Immaginiamo di voler far aspettare un figlio finché il padre non gli invia `SIGUSR1`. L'idea ingenua è:

```c
// ❌ VERSIONE SBAGLIATA
if (!ricevuto) {
    // <-- se il segnale arriva QUI, l'handler imposta il flag...
    pause();   // ...ma noi ci addormentiamo comunque, per sempre!
}
```

Tra il controllo del flag e la chiamata a `pause()` esiste una piccola finestra temporale. Se il segnale arriva proprio in quel momento, l'handler viene eseguito, ma subito dopo il processo entra in `pause()` e **nessun altro segnale arriverà più**: il processo resta bloccato. È una **race condition** (corsa critica), difficilissima da riprodurre e da debuggare, perché compare solo con una particolare sequenza temporale.

### La soluzione: bloccare, poi `sigsuspend()`

La strategia corretta ha tre passi:

1. **bloccare** il segnale con `sigprocmask()` *prima* di poter perdere qualcosa: se arriva mentre è bloccato, il kernel lo tiene **pendente**;
2. controllare la condizione (o semplicemente procedere);
3. chiamare `sigsuspend()`, che **in modo atomico** sostituisce la maschera dei segnali e mette il processo in attesa. Se esiste un segnale pendente, viene consegnato subito.

```c
sigset_t blocco, vecchia;
sigemptyset(&blocco);
sigaddset(&blocco, SIGUSR1);
sigprocmask(SIG_BLOCK, &blocco, &vecchia);  // 1. blocca SIGUSR1

/* ... eventuali operazioni ... */

sigsuspend(&vecchia);   // 3. sblocca e attende ATOMICAMENTE
```

Il segnale non può più "scivolare" nella finestra critica, perché durante tutto il tempo in cui è bloccato resta pendente, e `sigsuspend()` lo vedrà.

### Anatomia della corsa critica

1. Il processo controlla il flag `ricevuto` e lo trova a 0.
2. Subito dopo, prima di `pause()`, arriva il segnale: l'handler imposta `ricevuto = 1`.
3. Il processo esegue `pause()`, ma ormai non arriveranno altri segnali.
4. Il processo resta addormentato per sempre.

Il bug compare solo con questa precisa sequenza temporale, quindi può passare inosservato per mesi e manifestarsi sotto carico: per questo la prevenzione, e non il collaudo, è l'unica difesa.

### Alternativa: `sigwait()`

Quando si attende un solo evento, `sigwait()` è ancora più semplice: attende in modo sincrono uno dei segnali di un insieme, che deve essere **bloccato**, senza handler e senza flag.

```c
sigset_t insieme;
int sig;
sigemptyset(&insieme);
sigaddset(&insieme, SIGUSR1);
sigprocmask(SIG_BLOCK, &insieme, NULL);   // il segnale deve essere bloccato

sigwait(&insieme, &sig);                  // dorme finché non arriva SIGUSR1
printf("Ricevuto il segnale %d\n", sig);
```

Il segnale resta pendente se arriva prima della chiamata, quindi anche qui non c'è alcuna finestra critica.

## 7. Esempio completo: ping-pong tra padre e figlio

Padre e figlio si alternano in modo rigoroso: il padre stampa *ping* e sveglia il figlio, il figlio stampa *pong* e risveglia il padre. Ogni processo aspetta il segnale dell'altro prima di procedere.

```c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROUND 5

static void handler(int sig) { (void)sig; }   // serve solo a "catturare" il segnale

int main(void) {
    // installa l'handler per SIGUSR1
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    // Blocchiamo SIGUSR1 PRIMA della fork: il figlio eredita la maschera,
    // quindi nessun segnale potrà arrivare "troppo presto".
    sigset_t blocco, vecchia;
    sigemptyset(&blocco);
    sigaddset(&blocco, SIGUSR1);
    sigprocmask(SIG_BLOCK, &blocco, &vecchia);

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(EXIT_FAILURE); }

    if (pid == 0) {                       
        /* ---- FIGLIO ---- */
        pid_t padre = getppid();
        for (int i = 1; i <= ROUND; i++) {
            sigsuspend(&vecchia);         // attende il ping del padre
            printf("    Figlio: pong %d\n", i);
            fflush(stdout);
            kill(padre, SIGUSR1);         // risveglia il padre
        }
        _exit(EXIT_SUCCESS);
        // il codice del figlio termina qui, il padre continua dopo il blocco `if`
    }

    /* ---- PADRE ---- */
    for (int i = 1; i <= ROUND; i++) {
        printf("Padre: ping %d\n", i);
        // forza la stampa prima del pong, senza questo il buffer 
        // potrebbe essere stampato tutto insieme alla fine
        fflush(stdout); 
        kill(pid, SIGUSR1);               // risveglia il figlio
        sigsuspend(&vecchia);             // attende il pong
    }
    wait(NULL);
    printf("Fine.\n");
    return 0;
}
```

Compilazione ed esecuzione:

```bash
# -Wall serve a mostrare tutti i warning, -Wextra anche quelli meno importanti
# -o pingpong imposta il nome dell'eseguibile
gcc -Wall -Wextra -o pingpong pingpong.c
./pingpong
```

Output atteso:

```
Padre: ping 1
    Figlio: pong 1
Padre: ping 2
    Figlio: pong 2
...
Fine.
```

**Perché funziona sempre?** Il segnale è bloccato dall'inizio. Se il pong del figlio arriva prima che il padre abbia chiamato `sigsuspend()`, resta pendente e viene consegnato appena il padre si mette in attesa. Nessun segnale va perso, e l'ordine di stampa è deterministico.

### Provare per credere

Per vedere quanto conta il blocco preventivo, togliete la chiamata a `sigprocmask()` (inizializzando `vecchia` con `sigemptyset()`) e lanciate il programma molte volte:

```bash
for i in $(seq 1 50); do
    timeout 5 ./pingpong > /dev/null || { echo "Bloccato alla prova $i"; pkill pingpong; }
done
```

Senza blocco il programma funziona quasi sempre, ma ogni tanto un segnale arriva prima che il figlio sia in attesa e viene perso: i due processi restano bloccati e `timeout` interviene dopo 5 secondi. Con il blocco il ciclo termina sempre senza errori.

### Per esplorare ancora

- Usate `strace -f -e trace=signal ./pingpong` per vedere in tempo reale i segnali scambiati dai due processi.
- Sostituite `sigsuspend()` con `sigwait()` e confrontate il codice: quale risulta più semplice?

## 8. Esempio: `SIGCHLD` e i processi zombie

Quando un figlio termina, il kernel ne conserva una piccola traccia (lo stato di uscita) finché il padre non la legge con `wait()`. Nel frattempo il figlio è uno **zombie**. Un padre che genera molti figli senza raccoglierli riempie la tabella dei processi.

Un padre che deve fare altro, e non può bloccarsi in `wait()`, può farsi avvisare dal kernel con `SIGCHLD`:

```c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void on_sigchld(int sig) {
    (void)sig;
    int salvato = errno;                    // l'handler non deve alterare errno
    while (waitpid(-1, NULL, WNOHANG) > 0)  // raccoglie TUTTI i figli terminati
        ;
    errno = salvato;
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = on_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {
            sleep(i + 1);                   // il figlio lavora per i+1 secondi
            _exit(0);
        }
    }
    for (int s = 0; s < 6; s++) {           // il padre fa il suo lavoro
        printf("Padre al lavoro... %d\n", s);
        sleep(1);
    }
    return 0;
}
```

Perché il `while` con `WNOHANG` e non un singolo `waitpid()`? Perché i segnali standard **non vengono accodati**: se due figli terminano quasi insieme, il padre potrebbe ricevere un solo `SIGCHLD`. Il ciclo raccoglie tutti i figli già terminati.

> **Tip:** se non vi interessa lo stato di uscita dei figli, basta `signal(SIGCHLD, SIG_IGN)` (o `sa_flags = SA_NOCLDWAIT`): il kernel eliminerà i figli terminati senza lasciare zombie.

Per verificare che non ci siano zombie, in un altro terminale:

```bash
ps -o pid,ppid,stat,cmd --ppid <PID_PADRE>
```

Gli zombie compaiono con stato `Z`.

### Leggere lo stato di uscita del figlio

`waitpid()` restituisce anche il motivo per cui il figlio è terminato, che si decodifica con alcune macro:

| Macro | Significato |
| --- | --- |
| `WIFEXITED(stato)` | Il figlio è terminato normalmente (`exit()` o `return`) |
| `WEXITSTATUS(stato)` | Codice di uscita del figlio |
| `WIFSIGNALED(stato)` | Il figlio è stato terminato da un segnale |
| `WTERMSIG(stato)` | Numero del segnale che lo ha terminato |

```c
int stato;
pid_t p;
while ((p = waitpid(-1, &stato, WNOHANG)) > 0) {
    if (WIFEXITED(stato))
        printf("Figlio %d: uscita con codice %d\n", p, WEXITSTATUS(stato));
    else if (WIFSIGNALED(stato))
        printf("Figlio %d: ucciso dal segnale %d\n", p, WTERMSIG(stato));
}
```

Questo ciclo va eseguito nel programma principale, dopo che l'handler di `SIGCHLD` ha impostato un flag: `printf()` non è sicura dentro un handler.

## 9. Segnali anche in Bash: il comando `trap`

La sincronizzazione tramite segnali funziona anche negli script di shell. Il comando `trap` associa un'azione a un segnale:

```bash
#!/bin/bash
# attendi.sh - attende SIGUSR1 prima di partire

trap 'echo "[$$] SIGUSR1 ricevuto: parto!"; exit 0' USR1
trap 'echo "[$$] pulizia e uscita"; exit 1' INT TERM

echo "[$$] in attesa di SIGUSR1..."
while true; do
    sleep 1
done
```

Esecuzione:

```bash
./attendi.sh &            # lo script stampa il suo PID
kill -USR1 <PID>          # lo risveglia
```

> **Tip:** Bash esegue il `trap` solo al termine del comando in primo piano. Con `sleep 1000` lo script reagirebbe dopo 1000 secondi! Per una risposta immediata si usa il pattern `sleep 1000 & wait $!`: il builtin `wait` viene interrotto subito dal segnale.

### Gestire i trap

| Comando | Effetto |
| --- | --- |
| `trap -l` | Elenca i segnali disponibili |
| `trap -p` | Mostra i trap attualmente impostati |
| `trap - USR1` | Ripristina l'azione di default per `SIGUSR1` |
| `trap '' USR1` | Ignora `SIGUSR1` |

### Pulizia con `EXIT`

Lo pseudo-segnale `EXIT` esegue un comando alla terminazione dello script, qualunque ne sia il motivo (fine normale, `exit`, errore): è il posto giusto per cancellare i file temporanei.

```bash
#!/bin/bash
TMP=$(mktemp)
trap 'rm -f "$TMP"; echo "pulizia fatta"' EXIT
trap 'exit 1' INT TERM      # Ctrl+C e SIGTERM passano comunque dal trap su EXIT

echo "dati" > "$TMP"
sleep 30
```

Anche senza gestire `SIGKILL`, che non si può catturare, lo script lascia il sistema pulito in tutti gli altri casi.

## 10. Limiti dei segnali come strumento di sincronizzazione

I segnali sono comodi, ma hanno limiti precisi:

- **Non trasportano informazioni**: arriva "un SIGUSR1", niente altro. Per scambiare dati servono pipe, code di messaggi o memoria condivisa.
- **I segnali standard non sono accodati**: 5 `SIGUSR1` inviati mentre il segnale è bloccato o l'handler sta girando possono diventare 1 sola consegna. Per un conteggio affidabile si usano i *segnali real-time* (`SIGRTMIN`...`SIGRTMAX`) con `sigqueue()`, che vengono accodati e possono portare un valore.
- **Gli handler sono asincroni**: possono interrompere il programma in qualunque istruzione, quindi dentro un handler si possono chiamare solo funzioni *async-signal-safe* (ad esempio `write()`, `_exit()`, `kill()`). `printf()` e `malloc()` **non** lo sono.
- **Scalano male**: con molti processi, il codice diventa intricato. Per sincronizzare più processi su risorse condivise sono più adatti i **semafori** POSIX o i mutex.

### Segnali real-time

Linux offre anche i segnali real-time, da `SIGRTMIN` a `SIGRTMAX`. A differenza dei segnali standard sono **accodati** (ogni invio produce una consegna), vengono consegnati in ordine (prima il numero più basso) e possono trasportare un valore intero o un puntatore. Si inviano con `sigqueue()` e si ricevono con un handler `SA_SIGINFO`.

```c
union sigval valore;
valore.sival_int = 42;
sigqueue(pid, SIGRTMIN, valore);      // mittente

/* nell'handler (SA_SIGINFO), il destinatario legge: */
/* int v = si->si_value.sival_int; */
```

### Segnali o altri meccanismi?

| Esigenza | Meccanismo adatto |
| --- | --- |
| Avvisare che un evento è accaduto | Segnale |
| Passare un flusso di dati tra processi imparentati | Pipe |
| Passare dati tra processi non imparentati | FIFO o socket Unix |
| Proteggere una risorsa condivisa | Semaforo o mutex |
| Condividere grandi quantità di dati | Memoria condivisa, con un semaforo |
| Contare eventi in modo affidabile | Segnali real-time o semafori |

## 11. Best practice

1. **Usate `sigaction()`, non `signal()`**: comportamento definito e portabile.
2. **Handler brevi**: impostate un flag `volatile sig_atomic_t` e uscite; il lavoro lo fa il `main`.
3. **Bloccate prima, attendete dopo**: la sequenza `sigprocmask()` → `sigsuspend()` elimina la race condition di `pause()`.
4. **Bloccate il segnale prima di `fork()`** se il figlio deve poterlo ricevere senza perderne.
5. **Salvate e ripristinate `errno`** negli handler che chiamano funzioni di sistema.
6. **Chiamate solo funzioni async-signal-safe** dentro l'handler (`man 7 signal-safety`).
7. **Preferite `SIGTERM` a `SIGKILL`** per terminare un processo: `SIGKILL` non permette la pulizia di file temporanei e risorse.
8. **Controllate sempre il valore di ritorno** di `fork()`, `kill()` e `sigaction()`.
9. **Gestite `EINTR`**: una chiamata di sistema lenta può restituire -1 con `errno == EINTR` se interrotta da un segnale; usate `SA_RESTART` oppure ripetete la chiamata.
10. **Compilate con `-Wall -Wextra`** e provate il programma più volte: le race condition emergono a volte solo con carichi diversi.

### Errori tipici da evitare

- Chiamare `printf()`, `malloc()` o `exit()` dentro un signal handler.
- Usare `signal()` e dare per scontato che l'handler resti installato dopo la prima consegna.
- Controllare un flag e poi chiamare `pause()` senza aver bloccato il segnale.
- Aspettarsi che 10 invii di `SIGUSR1` producano 10 esecuzioni dell'handler.
- Chiamare un solo `waitpid()` nell'handler di `SIGCHLD`, invece di un ciclo con `WNOHANG`.
- Dimenticare che dopo `fork()` il figlio eredita handler e maschera, ma non i segnali pendenti.

## 12. Tip & trick

- `kill -l` mostra tutti i segnali con il loro numero; `man 7 signal` ne descrive il comportamento.
- `strace -e trace=signal ./programma` mostra in tempo reale i segnali inviati e ricevuti.
- `grep Sig /proc/<PID>/status` rivela le maschere di segnali di un processo: `SigBlk` (bloccati), `SigIgn` (ignorati), `SigCgt` (catturati), `SigPnd` (pendenti).
- `kill -0 <PID>` (o `kill(pid, 0)` in C) verifica se un processo esiste senza disturbarlo.
- `Ctrl+\` invia `SIGQUIT`, utile per fermare un programma che ignora `Ctrl+C` e generare un core dump.
- `Ctrl+Z` + `fg`/`bg` usano `SIGTSTP` e `SIGCONT`: è sincronizzazione tramite segnali che usate ogni giorno senza accorgervene.
- `kill -STOP <PID>` e `kill -CONT <PID>` mettono in pausa e riprendono qualunque processo, senza modificarlo.
- In un handler, per stampare un messaggio usate `write(STDOUT_FILENO, msg, strlen(msg))`, non `printf()`.

### Altri comandi utili

- `pgrep -l nome` e `pkill -SEGNALE nome` trovano i processi per nome e inviano loro un segnale.
- `pstree -p` mostra l'albero dei processi con i PID: comodo per vedere chi è padre di chi.
- `timeout 10 ./programma` invia `SIGTERM` dopo 10 secondi; con `timeout -s KILL` invia `SIGKILL`.
- `kill -l 15` converte un numero nel nome del segnale, `kill -l TERM` fa il contrario.
- Lo stato `T` in `ps -o pid,stat,cmd` indica un processo fermato da `SIGSTOP` o `SIGTSTP`; lo stato `Z` uno zombie.

## 13. Esercizi proposti

**Livello base**

1. Scrivete un programma che installi un handler per `SIGINT` e conti quante volte l'utente preme `Ctrl+C`; alla quinta pressione il programma deve terminare stampando il totale.
2. Modificate l'esempio ping-pong usando **due segnali diversi**: `SIGUSR1` per il ping (padre → figlio) e `SIGUSR2` per il pong (figlio → padre).
3. Dimostrate l'esistenza degli zombie: create un figlio che termina subito e un padre che dorme 30 secondi senza `wait()`. Osservate lo stato `Z` con `ps`. Poi risolvete con `SIGCHLD`.

**Livello intermedio**

4. Realizzate un **timeout** con `alarm()`: un programma chiede un numero da tastiera e, se l'utente non risponde entro 5 secondi, stampa un messaggio e termina.
5. Scrivete uno script Bash che lanci tre job in background e attenda che tutti abbiano finito, usando `trap ... CHLD` o `wait`.
6. Un processo "produttore" genera un numero casuale e lo scrive su una pipe; poi avvisa il "consumatore" con `SIGUSR1`. Il consumatore legge dalla pipe solo dopo aver ricevuto il segnale.

**Livello avanzato**

7. Realizzate un **anello di tre processi** (A → B → C → A) che si passano un "testimone" con `SIGUSR1`, stampando il proprio nome a ogni passaggio, per 10 giri.
8. Riproducete la **perdita di segnali**: un padre invia 100 `SIGUSR1` in rapida successione a un figlio che li conta. Quanti ne riceve realmente? Poi riscrivete l'esercizio con `SIGRTMIN` e `sigqueue()` e confrontate i risultati.
9. Implementate un piccolo **demone** che rilegga un file di configurazione quando riceve `SIGHUP` e termini ordinatamente (chiudendo i file aperti) con `SIGTERM`.

### Suggerimenti

- **Esercizio 2**: bloccate entrambi i segnali prima della `fork()`; ciascun processo attende il segnale dell'altro con `sigsuspend()`.
- **Esercizio 4**: se l'handler di `SIGALRM` è installato senza `SA_RESTART`, `read()` restituisce -1 con `errno == EINTR` e il programma può reagire al timeout.
- **Esercizio 7**: ogni processo deve conoscere il PID del successivo; il padre crea i tre figli e comunica loro i PID con una pipe prima di avviare il giro.
- **Esercizio 8**: contate le consegne con una variabile `volatile sig_atomic_t` e stampatele a fine esecuzione; ripetete la prova più volte, perché il numero perso varia.

## 14. Quiz di autovalutazione

**1. Quale tra questi segnali non può essere né catturato né ignorato?**

A) `SIGINT` B) `SIGTERM` C) `SIGKILL` D) `SIGUSR1`

**2. Perché la sequenza `sigprocmask()` + `sigsuspend()` è preferibile a controllare un flag e poi chiamare `pause()`?**

A) Perché `pause()` è deprecata e non esiste più in Linux B) Perché `sigsuspend()` sblocca i segnali e mette il processo in attesa in modo atomico, evitando la perdita del segnale C) Perché `sigsuspend()` permette di ricevere dati insieme al segnale D) Perché `sigsuspend()` rende il segnale non ignorabile

**3. Un processo ha `SIGUSR1` bloccato e ne riceve 3 prima di sbloccarlo. Quante volte verrà eseguito l'handler al momento dello sblocco?**

A) Tre volte B) Zero volte, i segnali bloccati vengono scartati C) Una sola volta, perché i segnali standard non sono accodati D) Dipende dalla priorità del processo

**4. Quale dichiarazione è corretta per una variabile modificata da un signal handler e letta dal `main`?**

A) `int flag;` B) `static char flag;` C) `volatile sig_atomic_t flag;` D) `const int flag;`

**5. Quale segnale riceve un processo padre quando uno dei suoi figli termina?**

A) `SIGTERM` B) `SIGCHLD` C) `SIGCONT` D) `SIGPIPE`

**6. Quale di queste funzioni NON è sicura da chiamare all'interno di un signal handler?**

A) `write()` B) `_exit()` C) `kill()` D) `printf()`

**7. Che cosa fa la chiamata `kill(pid, 0)`?**

A) Termina il processo `pid` B) Invia `SIGHUP` al processo `pid` C) Non invia alcun segnale, ma verifica se il processo esiste ed è raggiungibile D) Sospende il processo `pid`

**8. Quale flag di `sigaction()` fa ripartire automaticamente le chiamate di sistema lente interrotte da un segnale?**

A) `SA_NODEFER` B) `SA_RESTART` C) `SA_RESETHAND` D) `SA_NOCLDSTOP`

**9. Quale funzione attende in modo sincrono un segnale bloccato, senza bisogno di un handler?**

A) `pause()` B) `sigwait()` C) `raise()` D) `alarm()`

**10. Che cosa accade quando un programma scrive tramite un puntatore nullo?**

A) La CPU genera un interrupt hardware che invia `SIGINT` B) La CPU genera un'eccezione e il kernel invia `SIGSEGV` al processo C) Il programma esegue una chiamata di sistema che termina il processo D) Il processo riceve `SIGCHLD` dal kernel

## 15. Risposte al quiz

1. **C) `SIGKILL`**: insieme a `SIGSTOP`, è l'unico segnale che il processo non può catturare, ignorare o bloccare.
2. **B) Perché `sigsuspend()` sblocca i segnali e mette il processo in attesa in modo atomico, evitando la perdita del segnale**: non esiste più la finestra temporale tra il controllo e l'attesa.
3. **C) Una sola volta, perché i segnali standard non sono accodati**: il kernel registra solo che il segnale è pendente, non quante volte è arrivato.
4. **C) `volatile sig_atomic_t flag;`**: `volatile` evita ottimizzazioni del compilatore, `sig_atomic_t` garantisce accessi atomici.
5. **B) `SIGCHLD`**: l'azione di default è ignorarlo, ma si può catturare per chiamare `waitpid()` ed evitare zombie.
6. **D) `printf()`**: usa buffer interni e può causare deadlock o corruzione se interrotta da un handler; `write()`, `_exit()` e `kill()` sono async-signal-safe.
7. **C) Non invia alcun segnale, ma verifica se il processo esiste ed è raggiungibile**: restituisce 0 se esiste, altrimenti -1 con `errno` impostato a `ESRCH` (o `EPERM` se non si hanno i permessi).

8) **B) `SA_RESTART`**: senza questo flag la chiamata interrotta fallisce con `EINTR` e va ripetuta a mano.
9) **B) `sigwait()`**: attende uno dei segnali di un insieme già bloccato e lo restituisce, senza handler né flag.
10) **B) La CPU genera un'eccezione e il kernel invia `SIGSEGV` al processo**: l'eccezione (*page fault*) è sincrona e il kernel la traduce in un segnale.

## 16. Conclusioni

I segnali sono lo strumento di sincronizzazione più semplice offerto da Unix: pochi concetti, poche chiamate di sistema, ma molte insidie. La lezione più importante è che **la sincronizzazione corretta non si improvvisa**: tra il controllo di una condizione e l'attesa si apre sempre una finestra in cui un evento può andare perduto. Bloccare il segnale e attendere con `sigsuspend()` chiude quella finestra.

Usati con criterio (handler minimi, `sigaction()`, maschere dei segnali) i segnali sono ideali per notifiche semplici: terminazione di un figlio, richiesta di ricaricare una configurazione, risveglio di un processo in attesa. Quando servono scambio di dati, conteggi affidabili o sincronizzazione tra molti processi, è il momento di passare a pipe, semafori e memoria condivisa, argomenti dei prossimi articoli.

### Per proseguire

- **Semafori POSIX** (`sem_open()`, `sem_wait()`, `sem_post()`) per la mutua esclusione e la sincronizzazione di più processi.
- **Pipe e FIFO** per scambiare dati insieme alla notifica.
- **Memoria condivisa** (`shm_open()`, `mmap()`) per grandi quantità di dati.
- **`signalfd()`** per leggere i segnali da un descrittore di file e integrarli in un ciclo `select()` o `epoll()`.

---

*Per approfondire: `man 7 signal`, `man 2 sigaction`, `man 2 sigsuspend`, `man 7 signal-safety`.*
