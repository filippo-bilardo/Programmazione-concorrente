# I segnali in Unix e Linux

## Comunicazione e sincronizzazione tra processi in C

**Ambiente degli esempi:** Linux, C11 e interfacce POSIX; processi con un solo thread.

Questa dispensa sviluppa gli argomenti delle slide [«Sincronizzazione tra processi in Unix: i segnali»](https://github.com/filippo-bilardo/Programmazione-concorrente/blob/main/02-Processi-Linux/Slide3-Comunicazione%20tra%20processi%2C%20i%20segnali/01-6-SegnaliUnix.pdf), attribuite ad Anna Ciampolini nei metadati del PDF. Il testo è riorganizzato per argomenti e gli esempi sono riscritti per chiarire il funzionamento dei segnali e correggere le criticità dei frammenti originali.

**Prerequisiti:** funzioni e puntatori a funzione in C; PID; `fork()`; famiglia `exec()`; distinzione tra processo padre e processo figlio.

Al termine del percorso saprai:

- distinguere generazione, attesa e consegna di un segnale;
- scegliere tra azione predefinita, segnale ignorato e handler;
- usare `sigaction()`, `kill()` e le maschere dei segnali;
- attendere eventi senza consumare inutilmente CPU e senza perdere risvegli;
- gestire la terminazione dei figli con `SIGCHLD` e `waitpid()`;
- interpretare correttamente `EINTR` e i limiti dei segnali standard.

## Indice

- [I segnali in Unix e Linux](#i-segnali-in-unix-e-linux)
  - [Comunicazione e sincronizzazione tra processi in C](#comunicazione-e-sincronizzazione-tra-processi-in-c)
  - [Indice](#indice)
  - [1. Il problema della sincronizzazione](#1-il-problema-della-sincronizzazione)
  - [2. Che cosa sono i segnali](#2-che-cosa-sono-i-segnali)
    - [2.1 Da dove provengono](#21-da-dove-provengono)
    - [2.2 Generazione, pendenza e consegna](#22-generazione-pendenza-e-consegna)
  - [3. Come reagisce un processo](#3-come-reagisce-un-processo)
  - [4. I segnali principali](#4-i-segnali-principali)
  - [5. Inviare segnali con kill](#5-inviare-segnali-con-kill)
    - [5.1 Il comando della shell](#51-il-comando-della-shell)
    - [5.2 La funzione C](#52-la-funzione-c)
  - [6. Da signal a sigaction](#6-da-signal-a-sigaction)
    - [6.1 L'interfaccia signal](#61-linterfaccia-signal)
    - [6.2 L'interfaccia sigaction](#62-linterfaccia-sigaction)
  - [7. Scrivere correttamente un handler](#7-scrivere-correttamente-un-handler)
  - [8. Maschere, segnali pendenti e accorpamento](#8-maschere-segnali-pendenti-e-accorpamento)
    - [8.1 I segnali standard non sono un contatore](#81-i-segnali-standard-non-sono-un-contatore)
  - [9. Attendere un evento senza perdere il risveglio](#9-attendere-un-evento-senza-perdere-il-risveglio)
    - [9.1 La soluzione con sigsuspend](#91-la-soluzione-con-sigsuspend)
    - [9.2 La soluzione con sigwait](#92-la-soluzione-con-sigwait)
  - [10. sleep, alarm e pause](#10-sleep-alarm-e-pause)
  - [11. Segnali, fork ed exec](#11-segnali-fork-ed-exec)
    - [11.1 Dopo fork](#111-dopo-fork)
    - [11.2 Dopo exec](#112-dopo-exec)
  - [12. SIGCHLD e raccolta dei processi figli](#12-sigchld-e-raccolta-dei-processi-figli)
  - [13. Chiamate interrotte ed EINTR](#13-chiamate-interrotte-ed-eintr)
  - [14. Cinque programmi completi](#14-cinque-programmi-completi)
    - [Preparazione del laboratorio](#preparazione-del-laboratorio)
    - [Programma A - Ricevere SIGUSR1 e SIGUSR2](#programma-a---ricevere-sigusr1-e-sigusr2)
    - [Programma B - Attendere un timer senza race condition](#programma-b---attendere-un-timer-senza-race-condition)
    - [Programma C - Raccogliere tutti i figli con SIGCHLD](#programma-c---raccogliere-tutti-i-figli-con-sigchld)
    - [Programma D - Alternanza padre-figlio con sigwait](#programma-d---alternanza-padre-figlio-con-sigwait)
    - [Programma E - Un segnale ignorato attraversa exec](#programma-e---un-segnale-ignorato-attraversa-exec)
    - [Verifica degli esempi](#verifica-degli-esempi)
  - [15. Errori frequenti ed esercitazioni](#15-errori-frequenti-ed-esercitazioni)
    - [15.1 Errori da riconoscere](#151-errori-da-riconoscere)
    - [15.2 Esercitazioni di laboratorio](#152-esercitazioni-di-laboratorio)
    - [15.3 Domande di ripasso](#153-domande-di-ripasso)
  - [16. Riferimenti tecnici](#16-riferimenti-tecnici)

## 1. Il problema della sincronizzazione

Due processi possono essere eseguiti in ordine diverso a ogni avvio. La successione effettiva dipende dallo scheduler, dalla disponibilità della CPU e dalle operazioni di attesa.

Se un processo padre deve eseguire un'operazione soltanto dopo che il figlio ha terminato un'attività, occorre un meccanismo di **sincronizzazione**: un modo per imporre un vincolo sull'ordine delle operazioni.

Un segnale può notificare che si è verificato un evento: «è scaduto un timer», «un figlio ha cambiato stato», «è stata richiesta la terminazione».

Per trasferire sequenze di dati o messaggi strutturati si impiegano altri strumenti IPC, come pipe, socket e memoria condivisa. I segnali standard sono particolarmente adatti a notifiche semplici; per usarli come protocollo di sincronizzazione occorre controllare anche il modo in cui vengono attesi e consegnati.

## 2. Che cosa sono i segnali

Un **segnale** è una notifica software indirizzata a un processo o a uno specifico thread. In questa dispensa consideriamo processi con un solo thread.

Il destinatario identifica il tipo di evento attraverso un nome simbolico dichiarato in `<signal.h>`, per esempio `SIGINT`, `SIGTERM` o `SIGUSR1`.

### 2.1 Da dove provengono

| Origine | Esempio |
| --- | --- |
| Terminale | `Ctrl+C` richiede normalmente l'invio di `SIGINT` al gruppo di processi in primo piano. |
| Eccezione durante l'esecuzione | Un accesso a memoria non valido può provocare `SIGSEGV`. |
| Evento gestito dal kernel | La scadenza di `alarm()` genera `SIGALRM`. |
| Altro processo | Un processo invia `SIGUSR1` tramite `kill()`. |
| Processo stesso | Un programma può generare un segnale per sé, per esempio con `raise()`. |

Molti segnali arrivano **in modo asincrono** rispetto al flusso del programma. Altri sono conseguenza diretta dell'istruzione eseguita: per esempio un errore di accesso alla memoria. Questa seconda situazione viene descritta come generazione sincrona del segnale. [Manuale GNU: generazione dei segnali][gnu-generation]

### 2.2 Generazione, pendenza e consegna

Tre momenti distinti aiutano a capire il comportamento dei segnali:

1. **Generazione:** si verifica l'evento che produce il segnale.
2. **Pendenza:** il segnale è in attesa di essere consegnato, per esempio perché è bloccato.
3. **Consegna:** viene applicata l'azione prevista per quel segnale.

Queste fasi sono descritte anche nel [manuale GNU sulla consegna dei segnali][gnu-delivery].

Il successo di `kill()` non certifica che il destinatario abbia già completato il proprio handler.

## 3. Come reagisce un processo

La **disposizione** di un segnale stabilisce la reazione del processo alla sua consegna.

| Disposizione | Significato |
| --- | --- |
| `SIG_DFL` | Applica l'azione predefinita per quel tipo di segnale. |
| `SIG_IGN` | Ignora il segnale. |
| Funzione handler | Esegue una funzione definita dal programma. |

L'azione predefinita può terminare il processo, terminarlo con possibile produzione di un *core dump*, arrestarlo temporaneamente, farlo proseguire oppure ignorare la notifica. [Manuale GNU: consegna e azioni][gnu-delivery]

Quando viene eseguito un handler, il kernel predispone il suo avvio nel contesto del destinatario. **Non viene creato un nuovo processo o un nuovo thread.** Se l'handler ritorna normalmente, l'esecuzione interrotta riprende; se il segnale termina o arresta il processo, questo schema di ritorno immediato non si applica. [signal(7)][signal7]

Due segnali hanno un vincolo speciale: **`SIGKILL` e `SIGSTOP` non possono essere intercettati, ignorati o bloccati**.

## 4. I segnali principali

| Segnale | Significato comune | Azione predefinita in Linux |
| --- | --- | --- |
| `SIGHUP` | Disconnessione del terminale di controllo; spesso riutilizzato dalle applicazioni per ricaricare la configurazione | Terminazione |
| `SIGINT` | Richiesta di interruzione, normalmente con `Ctrl+C` | Terminazione |
| `SIGQUIT` | Richiesta di uscita, normalmente con `Ctrl+\` | Terminazione con core dump, se abilitato |
| `SIGILL` | Istruzione non valida | Terminazione con core dump, se abilitato |
| `SIGFPE` | Errore aritmetico rilevato durante l'esecuzione | Terminazione con core dump, se abilitato |
| `SIGSEGV` | Accesso a memoria non valido | Terminazione con core dump, se abilitato |
| `SIGPIPE` | Scrittura su pipe senza lettori o su connessione non più utilizzabile in scrittura | Terminazione |
| `SIGALRM` | Scadenza di un timer `alarm()` | Terminazione |
| `SIGTERM` | Richiesta di terminazione gestibile dal programma | Terminazione |
| `SIGKILL` | Terminazione forzata | Terminazione |
| `SIGUSR1`, `SIGUSR2` | Eventi definiti dall'applicazione | Terminazione |
| `SIGCHLD` | Cambiamento di stato di un figlio | Notifica ignorata; vedere la sezione 12 per gli zombie |
| `SIGSTOP` | Arresto non intercettabile | Arresto |
| `SIGTSTP` | Arresto richiesto dal terminale, normalmente con `Ctrl+Z` | Arresto |
| `SIGCONT` | Ripresa di un processo arrestato | Continuazione |

Approfondimenti sulla tabella: [errori di programma][gnu-errors], [terminazione][gnu-termination], [controllo dei job][gnu-jobs], [segnali applicativi][gnu-misc] ed [errori operativi][gnu-operation].

**Nomi e numeri.** I numeri possono cambiare tra sistemi e architetture: nel codice si usano i nomi simbolici. [Manuale GNU: nomi dei segnali][gnu-standard]

Nei Linux attuali sono presenti sia segnali standard sia segnali real-time; la descrizione «Linux ha 32 segnali» è incompleta. Per i real-time si impiegano `SIGRTMIN` e `SIGRTMAX`, evitando numeri fissati nel sorgente. [signal(7)][signal7]

La produzione effettiva di un core dump dipende anche dai limiti e dalla configurazione del sistema. `SIGCONT` ha l'effetto di far riprendere un processo arrestato anche quando la sua notifica viene ignorata o bloccata.

## 5. Inviare segnali con kill

### 5.1 Il comando della shell

Il comando `kill` invia un segnale. Il suo nome deriva dall'uso frequente per richiedere la terminazione, ma può inviare anche altri tipi di notifica.

Esempi con un PID illustrativo, da sostituire con quello del proprio programma:

```bash
kill -USR1 12345
kill -USR2 12345
kill -TERM 12345
kill -STOP 12345
kill -CONT 12345
```

Senza opzioni che selezionino il segnale, `kill PID` invia normalmente `SIGTERM`. Per elencare i segnali riconosciuti dalla shell:

```bash
kill -l
```

La richiesta `SIGTERM` permette al programma di gestire la chiusura; `SIGKILL` impedisce l'esecuzione di handler e procedure di pulizia del processo.

### 5.2 La funzione C

```c
#include <signal.h>

int kill(pid_t pid, int sig);
```

`pid_t` è il tipo usato dalle interfacce POSIX per gli identificatori di processo.

| Valore di `pid` | Destinatari |
| --- | --- |
| `pid > 0` | Il processo con quel PID. |
| `pid == 0` | I processi del gruppo del chiamante per i quali l'invio è consentito. |
| `pid < -1` | I processi del gruppo con identificatore `-pid`, nei limiti dei permessi. |
| `pid == -1` | In Linux, tutti i processi segnalabili dal chiamante, con esclusione di PID 1 e del chiamante stesso. |

POSIX specifica il significato generale di `pid == -1`, lasciando alcune esclusioni dipendenti dall'implementazione: non è un valore dal comportamento completamente non specificato.

La funzione restituisce `0` in caso di successo oppure `-1` impostando `errno`. Tra gli errori: `ESRCH` per destinatario non trovato, `EPERM` per permessi insufficienti, `EINVAL` per segnale non valido. Con `sig == 0` esegue controlli di esistenza e permessi senza inviare un segnale; il successo non dimostra che il processo stia ancora svolgendo lavoro utile. [kill(2)][kill2]

## 6. Da signal a sigaction

### 6.1 L'interfaccia signal

La funzione storica `signal()` associa una disposizione a un segnale:

```c
void (*signal(int signum, void (*handler)(int)))(int);
```

La dichiarazione si legge così: riceve il numero del segnale e il puntatore a un handler; restituisce il puntatore al gestore precedente. Accetta anche `SIG_DFL` e `SIG_IGN`, e segnala un errore restituendo `SIG_ERR`.

L'handler ha questa forma:

```c
void gestore(int signum);
```

Le differenze storiche tra implementazioni di `signal()` riguardano anche la permanenza del gestore e il riavvio delle chiamate interrotte. Per installare handler in programmi POSIX è preferibile **`sigaction()`**, che rende queste scelte esplicite. [signal(2)][signal2]

### 6.2 L'interfaccia sigaction

```c
#include <signal.h>

int sigaction(int signum,
              const struct sigaction *act,
              struct sigaction *oldact);
```

| Elemento | Ruolo |
| --- | --- |
| `signum` | Segnale da configurare. |
| `act` | Nuova configurazione; `NULL` permette di interrogare quella esistente. |
| `oldact` | Se diverso da `NULL`, riceve la configurazione precedente. |
| `sa_handler` | Handler semplice, `SIG_IGN` oppure `SIG_DFL`. |
| `sa_mask` | Segnali aggiuntivi da bloccare mentre viene eseguito l'handler. |
| `sa_flags` | Opzioni che modificano il comportamento. |

Restituisce `0` in caso di successo, `-1` in caso di errore.

Con `sa_flags = 0`, il gestore rimane installato e il segnale che lo ha attivato viene bloccato durante la sua esecuzione. Tra le opzioni disponibili: `SA_RESTART` per alcune chiamate interrotte, `SA_NOCLDSTOP` per evitare notifiche di arresto/ripresa dei figli, `SA_RESETHAND` per ripristinare l'azione predefinita, `SA_NODEFER` per consentire nuove consegne dello stesso segnale durante l'handler, salvo altri blocchi. [sigaction(2)][sigaction2]

Nei programmi completi useremo l'handler semplice. L'opzione `SA_SIGINFO` permette invece un gestore con tre parametri, configurato tramite `sa_sigaction`, per ottenere informazioni aggiuntive sull'evento.

## 7. Scrivere correttamente un handler

Un handler può interrompere il programma mentre una funzione sta aggiornando strutture interne. Richiamare da quel punto una funzione non rientrante può corromperne lo stato.

Per questo motivo **`printf()`, `fprintf()`, `perror()`, `malloc()` e `free()` non vanno usate in un normale handler asincrono**. Tra le funzioni POSIX async-signal-safe vi sono `write()`, `_exit()`, `kill()` e `waitpid()`. La proprietà async-signal-safe non significa che una funzione sia sempre istantanea o che non possa bloccarsi. [signal-safety(7)][safety7]

Due tecniche sono particolarmente utili:

- eseguire nell'handler una piccola operazione consentita, per esempio una `write()` di un messaggio fisso;
- impostare un indicatore e lasciare l'elaborazione al flusso principale.

Frammento per la seconda tecnica:

```c
static volatile sig_atomic_t richiesta_uscita = 0;

static void gestore_term(int signum)
{
    (void)signum;
    richiesta_uscita = 1;
}
```

`sig_atomic_t` consente accessi elementari non interrotti dai segnali; `volatile` rende visibili al compilatore gli accessi richiesti dal programma. Il tipo è adatto a un flag semplice: non rende automaticamente indivisibili operazioni composte come `contatore++` e non sostituisce le primitive di sincronizzazione tra thread.

Se l'handler chiama una funzione che può modificare `errno`, deve salvarne il valore all'ingresso e ripristinarlo prima del ritorno. Un handler che assegna soltanto un flag non ha questa necessità. [signal-safety(7)][safety7]

## 8. Maschere, segnali pendenti e accorpamento

La **maschera dei segnali** indica quali segnali sono temporaneamente bloccati. Negli esempi con un solo thread si modifica con `sigprocmask()`.

| Operazione | Effetto |
| --- | --- |
| `sigemptyset(&insieme)` | Crea un insieme vuoto. |
| `sigaddset(&insieme, SIGUSR1)` | Aggiunge `SIGUSR1` all'insieme. |
| `sigprocmask(SIG_BLOCK, &insieme, &precedente)` | Aggiunge l'insieme alla maschera, conservando quella precedente. |
| `sigprocmask(SIG_UNBLOCK, &insieme, NULL)` | Rimuove quei segnali dalla maschera. |
| `sigprocmask(SIG_SETMASK, &precedente, NULL)` | Ripristina esattamente una maschera salvata. |

**Bloccare** un segnale ne rinvia la consegna; **ignorarlo** ne configura invece la disposizione. Le due operazioni non sono equivalenti. Un segnale ignorato può essere scartato anche se è bloccato. [Manuale GNU: blocco][gnu-blocking] e [consegna][gnu-delivery]

Nei programmi multithread la maschera è propria di ciascun thread e si usa `pthread_sigmask()`. [sigprocmask(2)][sigprocmask2]

### 8.1 I segnali standard non sono un contatore

Se `SIGUSR1` è bloccato e viene generato dieci volte prima di essere consegnato, non si ottengono necessariamente dieci esecuzioni dell'handler: rimane una sola istanza pendente per quel tipo di segnale.

Per questo motivo un ciclo che invia migliaia di `SIGUSR1` non permette di contare in modo affidabile migliaia di eventi. Anche leggere un flag e poi azzerarlo, senza bloccare il segnale durante queste operazioni, può sovrascrivere una nuova notifica.

I segnali real-time possono accodare più istanze, entro i limiti di risorse del sistema. Per contare eventi distinti si possono scegliere segnali real-time o strumenti IPC adatti al protocollo. [signal(7)][signal7]

## 9. Attendere un evento senza perdere il risveglio

Un ciclo vuoto come `for (;;);` esegue una **attesa attiva**: occupa CPU senza svolgere lavoro utile. `pause()` permette di sospendere l'esecuzione, ma la sua posizione nel programma conta.

Questo frammento è **errato come schema di sincronizzazione** se l'handler può impostare `pronto` in qualsiasi momento:

```c
/* Frammento con race condition. */
while (!pronto)
    pause();
```

| Passaggio | Evento |
| --- | --- |
| 1 | Il programma legge `pronto`, che vale ancora `0`. |
| 2 | Prima di `pause()`, arriva il segnale. |
| 3 | L'handler imposta `pronto = 1` e ritorna. |
| 4 | Il programma chiama `pause()` e può attendere indefinitamente un altro segnale. |

Il problema nasce dalla separazione tra il controllo della condizione e l'inizio dell'attesa. Aggiungere una `sleep()` può cambiare la probabilità del problema, senza eliminarlo.

### 9.1 La soluzione con sigsuspend

La sequenza corretta è:

1. Bloccare il segnale interessato e salvare la maschera precedente.
2. Installare l'handler e predisporre la condizione.
3. Controllare la condizione mentre il segnale è ancora bloccato.
4. Se occorre attendere, chiamare `sigsuspend()` con una maschera che consenta la consegna del segnale.
5. Ripetere il controllo della condizione dopo il ritorno dell'handler.

`sigsuspend()` collega il cambio temporaneo di maschera all'attesa, eliminando la finestra critica. Se il segnale è già pendente, viene gestito durante questa attesa. Al ritorno viene ripristinata la maschera in vigore prima della chiamata. Il ritorno `-1` con `errno == EINTR`, dopo l'handler, è il comportamento previsto. Il programma B applica questo schema a un timer. [sigsuspend(2)][sigsuspend2]

### 9.2 La soluzione con sigwait

Quando il programma deve attendere esplicitamente un segnale, si può usare `sigwait()`: il segnale resta bloccato per la consegna asincrona e viene accettato dalla funzione. L'elaborazione avviene nel normale flusso del programma, senza handler. Il programma D usa questa tecnica.

`sigwait()` restituisce `0` in caso di successo; in caso di errore restituisce direttamente il codice dell'errore. Il numero del segnale viene scritto nel parametro di uscita. [sigwait(3)][sigwait3]

## 10. sleep, alarm e pause

Le tre funzioni rispondono a esigenze diverse e sono dichiarate in `<unistd.h>`:

| Funzione | Scopo | Ritorno principale |
| --- | --- | --- |
| `sleep(secondi)` | Sospende per un intervallo, salvo interruzione da un handler. | Secondi rimasti da attendere; `0` se l'attesa è completata. |
| `alarm(secondi)` | Programma un futuro `SIGALRM`, senza sospendere il chiamante. | Secondi residui di un eventuale allarme precedente. |
| `pause()` | Sospende fino a un segnale che attivi un handler oppure termini il processo. | Se l'handler ritorna, `-1` con `errno == EINTR`. |

Un segnale ignorato o ancora bloccato non fa ritornare `pause()` come farebbe un handler. [pause(2)][pause2]

`sleep()` non garantisce che il programma torni a eseguire esattamente allo scadere dei secondi richiesti: può anche dover attendere la CPU. Se serve completare un intervallo dopo un'interruzione, si gestisce il tempo residuo; `nanosleep()` offre una risoluzione più fine e restituisce il residuo tramite una struttura. [sleep(3)][sleep3], [nanosleep(2)][nanosleep2]

Per `alarm()` valgono queste regole:

- `alarm(0)` annulla un allarme programmato;
- una nuova `alarm()` sostituisce la precedente;
- senza handler, `SIGALRM` termina il processo;
- `alarm()` non è un timer periodico: una nuova scadenza richiede un'altra programmazione.

L'eventuale interazione tra `alarm()` e l'implementazione di `sleep()` va considerata quando si scrive codice portabile. Nei nostri esempi il timer è atteso con `sigsuspend()`. [alarm(2)][alarm2]

## 11. Segnali, fork ed exec

### 11.1 Dopo fork

`fork()` crea un figlio con un proprio spazio di indirizzamento. Il figlio eredita copie delle disposizioni e della maschera dei segnali del padre. Il suo insieme di segnali pendenti parte vuoto.

Le successive modifiche effettuate dal figlio non cambiano la configurazione del padre. Anche una variabile globale, come un contatore degli eventi, diventa una variabile distinta nei due processi: un incremento nel figlio non aggiorna il contatore del padre. [fork(2)][fork2]

### 11.2 Dopo exec

Una funzione della famiglia `exec()` sostituisce il programma eseguito dal processo, mantenendone il PID. I vecchi handler appartenevano al codice sostituito: le relative disposizioni vengono riportate all'azione predefinita. I segnali ignorati restano ignorati in Linux. [execve(2)][execve2]

| Proprietà | Nel figlio dopo `fork()` | Dopo `exec()` in Linux |
| --- | --- | --- |
| Disposizione con handler | Copiata dal padre | Riportata a `SIG_DFL` |
| Disposizione `SIG_IGN` | Copiata dal padre | Conservata |
| Disposizione `SIG_DFL` | Copiata dal padre | Conservata |
| Maschera dei segnali | Copiata dal padre | Conservata |
| Segnali pendenti | Insieme inizialmente vuoto | Conservati |
| Timer impostato con `alarm()` | Non ereditato | Conservato |

Il programma E imposta `SIGINT` come ignorato e poi esegue `/bin/sleep`: l'ignoramento rimane attivo anche dopo la sostituzione del programma. [sigprocmask(2)][sigprocmask2], [alarm(2)][alarm2]

## 12. SIGCHLD e raccolta dei processi figli

Il padre può ricevere `SIGCHLD` quando un figlio termina, viene arrestato o riprende. Con `SA_NOCLDSTOP` si escludono le notifiche di arresto e ripresa.

Un figlio terminato può restare nello stato **zombie** finché il padre non ne raccoglie lo stato con una funzione di attesa. La notifica `SIGCHLD` e la raccolta dello stato sono due operazioni distinte: ricevere il segnale non elimina automaticamente lo zombie.

```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

Con `waitpid(-1, &stato, WNOHANG)` il padre controlla qualsiasi figlio senza bloccarsi:

| Ritorno | Interpretazione |
| --- | --- |
| PID positivo | È stato raccolto lo stato di un figlio. |
| `0` | Esistono figli interessati dalla chiamata, ma nessuno ha uno stato disponibile. |
| `-1`, `errno == ECHILD` | Non ci sono figli su cui attendere. |
| `-1`, altro `errno` | Si è verificato un errore da gestire. |

Per interpretare lo stato si usano le macro previste dall'interfaccia:

- `WIFEXITED(stato)`: terminazione tramite `exit()`, `_exit()` o ritorno da `main()`;
- `WEXITSTATUS(stato)`: codice di uscita, da leggere solo se `WIFEXITED()` è vero;
- `WIFSIGNALED(stato)`: terminazione causata da un segnale;
- `WTERMSIG(stato)`: segnale responsabile, da leggere solo se `WIFSIGNALED()` è vero.

L'espressione `stato >> 8` presente nelle slide va sostituita con le macro appropriate. Un solo `SIGCHLD` può corrispondere a più figli terminati: occorre raccogliere **tutti** gli stati disponibili, come nel programma C. [wait(2)][wait2]

**Particolarità di Linux/POSIX moderno:** lasciare la disposizione predefinita di `SIGCHLD`, la cui notifica è ignorata, non equivale a impostare esplicitamente `SIG_IGN`. L'ignoramento esplicito o `SA_NOCLDWAIT` evita la conservazione degli zombie e impedisce di raccogliere normalmente quegli stati. Quando il codice di uscita serve al padre, si mantiene il meccanismo di attesa. [wait(2)][wait2]

## 13. Chiamate interrotte ed EINTR

Se un handler viene eseguito mentre il processo è bloccato in una chiamata, il comportamento dipende dall'interfaccia e dalle opzioni usate per installare il gestore.

Una chiamata può essere riavviata automaticamente, fallire con `errno == EINTR` oppure restituire un risultato parziale. Per esempio, una lettura interrotta prima di trasferire dati può fallire con `EINTR`; se alcuni dati sono già stati letti, può restituire il numero di byte effettivamente trasferiti. [read(2)][read2]

Frammento per ritentare una lettura che il programma desidera comunque completare:

```c
/* All'interno di una funzione; richiede errno.h e unistd.h. */
char buffer[128];
ssize_t letti;

do {
    letti = read(STDIN_FILENO, buffer, sizeof(buffer));
} while (letti == -1 && errno == EINTR);
```

Questo schema ritenta la singola lettura interrotta; non garantisce di riempire tutto il buffer. Se il segnale richiede la chiusura del programma, il ciclo va adattato per rispettare quella richiesta.

**`SA_RESTART` non riavvia ogni chiamata.** In Linux, per esempio, può riavviare determinate letture da terminali, pipe o socket; `pause()`, `sigsuspend()` e `nanosleep()` non vengono riavviate in questo modo. La gestione delle interruzioni va progettata in base all'operazione attesa. [signal(7)][signal7]

## 14. Cinque programmi completi

### Preparazione del laboratorio

Salva ciascun blocco con il nome indicato. La macro `_POSIX_C_SOURCE 200809L`, definita prima degli header, rende disponibili le dichiarazioni POSIX necessarie anche usando la compilazione C11 in modalità rigorosa.

```bash
cc -std=c11 -Wall -Wextra -Wpedantic 01-ricevitore.c -o ricevitore
cc -std=c11 -Wall -Wextra -Wpedantic 02-timer.c -o timer
cc -std=c11 -Wall -Wextra -Wpedantic 03-figli.c -o figli
cc -std=c11 -Wall -Wextra -Wpedantic 04-ping-pong.c -o ping-pong
cc -std=c11 -Wall -Wextra -Wpedantic 05-exec-ignora.c -o exec-ignora
```

I programmi sono indipendenti. I frammenti delle sezioni precedenti servono invece a illustrare dichiarazioni o singoli concetti.

### Programma A - Ricevere SIGUSR1 e SIGUSR2

**File: `01-ricevitore.c`**

Il programma installa lo stesso gestore per due segnali. L'handler usa `write()` per messaggi fissi e conserva `errno`. Il processo resta sospeso con `pause()`, senza attesa attiva.

```c
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void gestore(int signum)
{
    int errno_salvato = errno;
    const char msg1[] = "Ricevuto SIGUSR1\n";
    const char msg2[] = "Ricevuto SIGUSR2\n";

    if (signum == SIGUSR1)
        (void)write(STDOUT_FILENO, msg1, sizeof(msg1) - 1);
    else if (signum == SIGUSR2)
        (void)write(STDOUT_FILENO, msg2, sizeof(msg2) - 1);

    errno = errno_salvato;
}

int main(void)
{
    struct sigaction azione = {0};
    azione.sa_handler = gestore;
    azione.sa_flags = 0;

    if (sigemptyset(&azione.sa_mask) == -1 ||
        sigaddset(&azione.sa_mask, SIGUSR1) == -1 ||
        sigaddset(&azione.sa_mask, SIGUSR2) == -1 ||
        sigaction(SIGUSR1, &azione, NULL) == -1 ||
        sigaction(SIGUSR2, &azione, NULL) == -1) {
        perror("configurazione segnali");
        return EXIT_FAILURE;
    }

    printf("Ricevitore pronto. PID=%ld\n", (long)getpid());
    fflush(stdout);

    for (;;)
        pause();
}
```

**Prova in due terminali.** Nel primo esegui:

```bash
./ricevitore
```

Attendi il messaggio «Ricevitore pronto» e copia il PID mostrato. Nel secondo terminale sostituisci `12345` con quel valore e invia i comandi uno alla volta:

```bash
kill -USR1 12345
kill -USR2 12345
kill -TERM 12345
```

I primi due comandi producono rispettivamente `Ricevuto SIGUSR1` e `Ricevuto SIGUSR2`. Il terzo termina il programma con l'azione predefinita di `SIGTERM`.

Qui il ciclo incondizionato con `pause()` è adatto: ogni notifica viene elaborata direttamente dal gestore e non c'è un controllo di flag separato dall'attesa. Il ricevitore non conta tutte le occorrenze di un segnale inviato a raffica; inoltre la scrittura nell'handler è una dimostrazione minima, che non gestisce errori di output.

### Programma B - Attendere un timer senza race condition

**File: `02-timer.c`**

`SIGALRM` viene bloccato prima di predisporre l'attesa. Il gestore imposta soltanto un flag; la stampa avviene nel flusso principale.

```c
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t scaduto = 0;

static void gestore_alarm(int signum)
{
    (void)signum;
    scaduto = 1;
}

int main(void)
{
    sigset_t bloccati, precedente, attesa;
    struct sigaction azione = {0};
    azione.sa_handler = gestore_alarm;

    if (sigemptyset(&bloccati) == -1 ||
        sigaddset(&bloccati, SIGALRM) == -1 ||
        sigprocmask(SIG_BLOCK, &bloccati, &precedente) == -1 ||
        sigemptyset(&azione.sa_mask) == -1 ||
        sigaction(SIGALRM, &azione, NULL) == -1) {
        perror("configurazione timer");
        return EXIT_FAILURE;
    }

    attesa = precedente;
    if (sigdelset(&attesa, SIGALRM) == -1) {
        perror("sigdelset");
        return EXIT_FAILURE;
    }

    puts("Timer avviato: attendo 2 secondi.");
    fflush(stdout);
    alarm(2);

    while (!scaduto) {
        if (sigsuspend(&attesa) == -1 && errno != EINTR) {
            perror("sigsuspend");
            return EXIT_FAILURE;
        }
    }

    if (sigprocmask(SIG_SETMASK, &precedente, NULL) == -1) {
        perror("ripristino maschera");
        return EXIT_FAILURE;
    }

    puts("Timer scaduto: il flusso principale riprende.");
    return EXIT_SUCCESS;
}
```

Esegui `./timer`. Dopo circa due secondi vedrai:

```text
Timer avviato: attendo 2 secondi.
Timer scaduto: il flusso principale riprende.
```

Anche se il processo venisse sospeso dallo scheduler prima di chiamare `sigsuspend()`, la scadenza rimarrebbe pendente: il segnale verrebbe consegnato quando la maschera temporanea lo consente. Il controllo di `scaduto` avviene sempre con `SIGALRM` bloccato.

### Programma C - Raccogliere tutti i figli con SIGCHLD

**File: `03-figli.c`**

Il padre configura il gestore e blocca `SIGCHLD` **prima della prima `fork()`**. I tre figli terminano con codici differenti. Il padre raccoglie tutti gli stati disponibili e si sospende soltanto quando esistono ancora figli, ma nessuno è raccoglibile.

```c
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void gestore_chld(int signum)
{
    (void)signum; /* Basta far ritornare sigsuspend(). */
}

int main(void)
{
    sigset_t bloccati, precedente, attesa;
    struct sigaction azione = {0};
    int esito = EXIT_SUCCESS;

    azione.sa_handler = gestore_chld;
    azione.sa_flags = SA_NOCLDSTOP;

    if (sigemptyset(&bloccati) == -1 ||
        sigaddset(&bloccati, SIGCHLD) == -1 ||
        sigprocmask(SIG_BLOCK, &bloccati, &precedente) == -1 ||
        sigemptyset(&azione.sa_mask) == -1 ||
        sigaction(SIGCHLD, &azione, NULL) == -1) {
        perror("configurazione SIGCHLD");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            esito = EXIT_FAILURE;
            break; /* Raccoglie comunque i figli gia' creati. */
        }
        if (pid == 0) {
            if (sigprocmask(SIG_SETMASK, &precedente, NULL) == -1)
                _exit(125);
            _exit(20 + i);
        }
    }

    attesa = precedente;
    if (sigdelset(&attesa, SIGCHLD) == -1) {
        perror("sigdelset");
        return EXIT_FAILURE;
    }

    for (;;) {
        int stato;
        pid_t terminato = waitpid(-1, &stato, WNOHANG);

        if (terminato > 0) {
            if (WIFEXITED(stato))
                printf("Figlio %ld: exit=%d\n",
                       (long)terminato, WEXITSTATUS(stato));
            else if (WIFSIGNALED(stato))
                printf("Figlio %ld: segnale=%d\n",
                       (long)terminato, WTERMSIG(stato));
            continue; /* Controlla subito anche gli altri figli. */
        }

        if (terminato == 0) {
            if (sigsuspend(&attesa) == -1 && errno != EINTR) {
                perror("sigsuspend");
                esito = EXIT_FAILURE;
                break;
            }
            continue;
        }

        if (errno == EINTR)
            continue;
        if (errno == ECHILD)
            break; /* Non restano figli da raccogliere. */
        perror("waitpid");
        esito = EXIT_FAILURE;
        break;
    }

    if (sigprocmask(SIG_SETMASK, &precedente, NULL) == -1) {
        perror("ripristino maschera");
        return EXIT_FAILURE;
    }
    return esito;
}
```

Un possibile output di `./figli` è:

```text
Figlio 21001: exit=20
Figlio 21003: exit=22
Figlio 21002: exit=21
```

PID e ordine possono cambiare. L'obiettivo è raccogliere una volta ciascuno i tre stati, anche se le notifiche `SIGCHLD` si accorpano.

Il gestore non stampa e non interpreta gli stati: serve a far ritornare `sigsuspend()`. Le operazioni di `waitpid()` e `printf()` avvengono nel normale flusso del padre.

### Programma D - Alternanza padre-figlio con sigwait

**File: `04-ping-pong.c`**

Il padre e il figlio eseguono cinque turni alternati. `SIGUSR1` è bloccato prima della `fork()`, quindi anche il figlio eredita il blocco. Entrambi accettano il segnale tramite `sigwait()`.

| Fase di un turno | Padre | Figlio |
| --- | --- | --- |
| Avvio | Stampa `PADRE n`, poi invia `SIGUSR1`. | Attende il segnale. |
| Lavoro del figlio | Attende la conferma. | Stampa `FIGLIO n`, poi invia `SIGUSR1` al padre. |
| Chiusura | Riceve la conferma e può iniziare il turno seguente. | Si prepara ad attendere il turno seguente. |

```c
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int attendi_turno(const sigset_t *insieme)
{
    int signum;
    int errore = sigwait(insieme, &signum);
    if (errore != 0) {
        errno = errore;
        perror("sigwait");
        return -1;
    }
    return 0;
}

int main(void)
{
    sigset_t insieme, precedente;
    pid_t padre = getpid();

    if (sigemptyset(&insieme) == -1 ||
        sigaddset(&insieme, SIGUSR1) == -1 ||
        sigprocmask(SIG_BLOCK, &insieme, &precedente) == -1) {
        perror("blocco SIGUSR1");
        return EXIT_FAILURE;
    }

    pid_t figlio = fork();
    if (figlio == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (figlio == 0) {
        for (int turno = 1; turno <= 5; ++turno) {
            if (attendi_turno(&insieme) == -1)
                _exit(EXIT_FAILURE);
            printf("FIGLIO %d\n", turno);
            fflush(stdout);
            if (kill(padre, SIGUSR1) == -1) {
                perror("kill padre");
                _exit(EXIT_FAILURE);
            }
        }
        _exit(EXIT_SUCCESS);
    }

    int esito = EXIT_SUCCESS;
    for (int turno = 1; turno <= 5; ++turno) {
        printf("PADRE %d\n", turno);
        fflush(stdout);
        if (kill(figlio, SIGUSR1) == -1) {
            perror("kill figlio");
            esito = EXIT_FAILURE;
            break;
        }
        if (attendi_turno(&insieme) == -1) {
            esito = EXIT_FAILURE;
            break;
        }
    }

    if (esito != EXIT_SUCCESS)
        (void)kill(figlio, SIGTERM);

    int stato;
    pid_t raccolto;
    do {
        raccolto = waitpid(figlio, &stato, 0);
    } while (raccolto == -1 && errno == EINTR);

    if (raccolto == -1) {
        perror("waitpid");
        esito = EXIT_FAILURE;
    } else if (!WIFEXITED(stato) || WEXITSTATUS(stato) != EXIT_SUCCESS) {
        esito = EXIT_FAILURE;
    }

    if (sigprocmask(SIG_SETMASK, &precedente, NULL) == -1) {
        perror("ripristino maschera");
        return EXIT_FAILURE;
    }
    return esito;
}
```

Output di `./ping-pong`:

```text
PADRE 1
FIGLIO 1
PADRE 2
FIGLIO 2
PADRE 3
FIGLIO 3
PADRE 4
FIGLIO 4
PADRE 5
FIGLIO 5
```

Non servono ritardi artificiali. Se il segnale arriva prima della `sigwait()`, resta pendente e può essere accettato dalla chiamata. Il protocollo mantiene al massimo una notifica non ancora accettata per ciascuna direzione, evitando di usare `SIGUSR1` come una coda di eventi.

Questo protocollo dimostra l'alternanza quando entrambi i processi partecipano fino alla fine. Come estensione, aggiungi un timeout con `sigtimedwait()` o la gestione della terminazione anticipata del figlio: il padre non deve restare in attesa di una conferma che non arriverà.

### Programma E - Un segnale ignorato attraversa exec

**File: `05-exec-ignora.c`**

Il programma ignora `SIGINT`, mostra il PID e sostituisce il proprio codice con `/bin/sleep`. Per una prova manuale comoda puoi portare la durata da `5` a `30` secondi.

```c
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    struct sigaction azione = {0};
    azione.sa_handler = SIG_IGN;

    if (sigemptyset(&azione.sa_mask) == -1 ||
        sigaction(SIGINT, &azione, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("SIGINT ignorato. PID=%ld\n", (long)getpid());
    fflush(stdout);
    execl("/bin/sleep", "sleep", "5", (char *)NULL);

    perror("execl"); /* Raggiungibile soltanto se exec fallisce. */
    return EXIT_FAILURE;
}
```

Durante l'esecuzione di `sleep`, `kill -INT PID` non termina quel processo; `kill -TERM PID` può terminarlo. Con `ps -p PID -o pid,comm` puoi osservare che il programma è diventato `sleep` mantenendo il PID.

Un errore di `execl()` fa proseguire il vecchio codice e viene segnalato da `perror()`. Se invece `execl()` riesce, non ritorna al programma precedente.

### Verifica degli esempi

I cinque sorgenti sono stati compilati in Linux con GCC 13.3.0 e opzioni `-std=c11 -Wall -Wextra -Wpedantic -Werror`, senza avvisi.

Sono stati verificati la ricezione dei due segnali utente, l'accorpamento di segnali standard pendenti, la scadenza del timer e il comportamento dopo `exec()`. I programmi C e D sono stati eseguiti 50 volte ciascuno: tutti i figli sono stati raccolti e l'alternanza è rimasta quella indicata.

## 15. Errori frequenti ed esercitazioni

### 15.1 Errori da riconoscere

| Errore | Perché è un problema | Correzione |
| --- | --- | --- |
| `for (;;);` per aspettare un segnale | Consuma CPU senza svolgere lavoro. | Usa un'attesa bloccante adatta al protocollo. |
| `printf()` nell'handler | Non è async-signal-safe. | Usa un flag o una piccola operazione consentita. |
| `if (!pronto) pause();` | Il segnale può arrivare tra controllo e attesa. | Blocca il segnale e usa `sigsuspend()`, oppure accettalo con `sigwait()`. |
| `sleep(1)` per imporre l'ordine padre-figlio | Il ritardo non crea una garanzia di sincronizzazione. | Usa una conferma esplicita tra i processi. |
| Una `wait()` per ogni `SIGCHLD` | Le notifiche possono accorparsi. | Raccogli in ciclo tutti gli stati disponibili. |
| Installare il gestore `SIGCHLD` solo dopo `fork()` | Il figlio può terminare prima della configurazione. | Predisponi la gestione prima di creare i figli. |
| `status >> 8` per il codice di uscita | Presuppone una codifica dello stato. | Usa `WIFEXITED()` e `WEXITSTATUS()`. |
| Usare numeri per identificare i segnali nel sorgente | Riduce la portabilità. | Usa le costanti simboliche. |
| Trattare `SIGUSR1` come un contatore | Più occorrenze pendenti possono diventare una sola. | Definisci un protocollo con conferme o scegli un'altra IPC. |
| Ritentare ogni operazione su `EINTR` senza valutare l'evento | Può ignorare una richiesta di chiusura o altre decisioni del programma. | Decidi se continuare in base al significato del segnale. |

### 15.2 Esercitazioni di laboratorio

**1. Ricevitore e controllo del processo.** Avvia il programma A, invia separatamente `SIGUSR1` e `SIGUSR2`, poi arrestalo con `SIGSTOP`. Invia più `SIGUSR1` mentre è fermo e riprendilo con `SIGCONT`.

**Da osservare:** gli handler non vengono eseguiti mentre il processo è arrestato; più occorrenze standard dello stesso tipo, rimaste pendenti, non costituiscono un contatore.

**2. Uscita controllata.** Modifica il programma A per trattare `SIGTERM` come una richiesta di chiusura. L'handler deve impostare un flag; il flusso principale deve stampare «Chiusura completata» e terminare.

**Vincolo:** elimina la finestra tra il controllo del flag e l'attesa usando la strategia del programma B. Anche il test e l'eventuale azzeramento di un flag devono rispettare il blocco del segnale interessato.

**3. Raccolta di più figli.** Porta a dieci il numero di figli del programma C. Assegna a ciascuno un codice distinto e verifica che il padre raccolga tutti gli stati, indipendentemente dall'ordine di terminazione.

**Da consegnare:** sorgente, output e spiegazione del motivo per cui il numero di handler attivati non è un conteggio affidabile dei figli terminati.

**4. Protocollo a turni.** Modifica il programma D per eseguire venti turni. Poi introduci un timeout nell'attesa della conferma.

**Da verificare:** alternanza corretta, gestione della mancata risposta, terminazione del figlio e raccolta finale del suo stato.

**5. Ereditarietà attraverso exec.** Esegui il programma E, annota il PID e verifica che `SIGINT` rimanga ignorato. Poi sostituisci `SIG_IGN` con un handler semplice e ripeti la prova.

**Risultato atteso:** il vecchio handler non sopravvive alla sostituzione del programma. Con `sleep` avviato in primo piano, la consegna successiva di `SIGINT` applica normalmente l'azione predefinita.

### 15.3 Domande di ripasso

1. Che differenza c'è tra bloccare e ignorare un segnale?
2. Perché il successo di `kill()` non è una conferma di lavoro completato?
3. Perché `SIGKILL` non permette una chiusura gestita dal programma?
4. Quale problema elimina `sigsuspend()` rispetto a un controllo seguito da `pause()`?
5. Che cosa accade se lo stesso segnale standard viene generato più volte mentre è pendente?
6. Perché ricevere `SIGCHLD` non basta a raccogliere lo stato di un figlio?
7. Quali impostazioni dei segnali vengono conservate da `exec()`?
8. Perché `SA_RESTART` non elimina la necessità di capire il comportamento delle chiamate interrotte?

## 16. Riferimenti tecnici

La dispensa mantiene i temi delle slide e aggiorna le spiegazioni usando la documentazione del progetto Linux man-pages e il manuale della GNU C Library. Le pagine di manuale permettono di approfondire opzioni, casi particolari e differenze tra sistemi.

- [signal(7)][signal7]: panoramica, azioni predefinite, segnali standard e real-time, interruzione delle chiamate.
- [signal(2)][signal2]: interfaccia storica e problemi di portabilità.
- [sigaction(2)][sigaction2]: installazione e configurazione dei gestori.
- [signal-safety(7)][safety7]: funzioni async-signal-safe e conservazione di `errno`.
- [kill(2)][kill2]: invio, destinatari, controlli dei permessi ed errori.
- [sigprocmask(2)][sigprocmask2]: maschera dei segnali.
- [sigsuspend(2)][sigsuspend2]: cambio temporaneo della maschera e attesa.
- [sigwait(3)][sigwait3]: accettazione sincrona di segnali.
- [wait(2)][wait2]: `waitpid()`, macro dello stato e processi zombie.
- [fork(2)][fork2] e [execve(2)][execve2]: ereditarietà e sostituzione del programma.
- [sleep(3)][sleep3], [alarm(2)][alarm2] e [pause(2)][pause2]: attese e timer.
- [read(2)][read2] e [nanosleep(2)][nanosleep2]: risultati parziali, interruzioni e tempo residuo.
- **GNU C Library:** [generazione][gnu-generation], [consegna][gnu-delivery], [nomi dei segnali][gnu-standard] e [blocco][gnu-blocking]; descrizioni di [errori di programma][gnu-errors], [terminazione][gnu-termination], [controllo dei job][gnu-jobs], [segnali applicativi][gnu-misc] ed [errori operativi][gnu-operation].

[signal7]: https://man7.org/linux/man-pages/man7/signal.7.html
[signal2]: https://man7.org/linux/man-pages/man2/signal.2.html
[sigaction2]: https://man7.org/linux/man-pages/man2/sigaction.2.html
[safety7]: https://man7.org/linux/man-pages/man7/signal-safety.7.html
[kill2]: https://man7.org/linux/man-pages/man2/kill.2.html
[sigprocmask2]: https://man7.org/linux/man-pages/man2/sigprocmask.2.html
[sigsuspend2]: https://man7.org/linux/man-pages/man2/sigsuspend.2.html
[sigwait3]: https://man7.org/linux/man-pages/man3/sigwait.3.html
[wait2]: https://man7.org/linux/man-pages/man2/wait.2.html
[fork2]: https://man7.org/linux/man-pages/man2/fork.2.html
[execve2]: https://man7.org/linux/man-pages/man2/execve.2.html
[sleep3]: https://man7.org/linux/man-pages/man3/sleep.3.html
[alarm2]: https://man7.org/linux/man-pages/man2/alarm.2.html
[pause2]: https://man7.org/linux/man-pages/man2/pause.2.html
[read2]: https://man7.org/linux/man-pages/man2/read.2.html
[nanosleep2]: https://man7.org/linux/man-pages/man2/nanosleep.2.html
[gnu-generation]: https://sourceware.org/glibc/manual/latest/html_node/Signal-Generation.html
[gnu-delivery]: https://sourceware.org/glibc/manual/latest/html_node/Delivery-of-Signal.html
[gnu-standard]: https://sourceware.org/glibc/manual/latest/html_node/Standard-Signals.html
[gnu-blocking]: https://sourceware.org/glibc/manual/latest/html_node/Blocking-Signals.html
[gnu-errors]: https://sourceware.org/glibc/manual/latest/html_node/Program-Error-Signals.html
[gnu-termination]: https://sourceware.org/glibc/manual/latest/html_node/Termination-Signals.html
[gnu-jobs]: https://sourceware.org/glibc/manual/latest/html_node/Job-Control-Signals.html
[gnu-misc]: https://sourceware.org/glibc/manual/latest/html_node/Miscellaneous-Signals.html
[gnu-operation]: https://sourceware.org/glibc/manual/latest/html_node/Operation-Error-Signals.html
