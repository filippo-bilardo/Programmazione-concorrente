# Esercizi sui Segnali Unix/Linux

## Indice
1. [Esercizi Base](#esercizi-base)
2. [Esercizi Intermedi](#esercizi-intermedi)
3. [Esercizi Avanzati](#esercizi-avanzati)
4. [Progetti Completi](#progetti-completi)
5. [Soluzioni](#soluzioni)

---

## Esercizi Base

### Esercizio 1.1 - Primo Handler
**Obiettivo**: Comprendere il meccanismo base dei signal handler.

Scrivi un programma che:
- Gestisce il segnale SIGINT (CTRL-C)
- Quando riceve SIGINT, stampa "Hai premuto CTRL-C!" invece di terminare
- Dopo 3 CTRL-C, il programma termina mostrando "Arrivederci!"

**Suggerimenti:**
- Usa una variabile globale per contare i CTRL-C ricevuti
- Ricorda di includere `<signal.h>`

---

### Esercizio 1.2 - Ignorare un Segnale
**Obiettivo**: Imparare a ignorare segnali.

Scrivi un programma che:
- Ignora completamente SIGINT per i primi 10 secondi
- Dopo 10 secondi, ripristina il comportamento di default
- Stampa messaggi informativi per l'utente

**Esempio output:**
```
SIGINT ignorato per 10 secondi. Prova a premere CTRL-C...
[utente preme CTRL-C]
SIGINT ricevuto ma ignorato!
...
[dopo 10 secondi]
Comportamento default ripristinato. CTRL-C ora terminerà il programma.
```

---

### Esercizio 1.3 - Signal Handler Multipli
**Obiettivo**: Gestire diversi tipi di segnali.

Scrivi un programma che:
- Gestisce SIGUSR1, SIGUSR2 e SIGINT
- Per SIGUSR1: stampa "Segnale 1 ricevuto - LED VERDE acceso"
- Per SIGUSR2: stampa "Segnale 2 ricevuto - LED ROSSO acceso"
- Per SIGINT: termina con messaggio "Terminazione richiesta dall'utente"
- Il programma resta in loop infinito aspettando segnali

**Test:**
```bash
# Terminale 1
./esercizio &
# Annotare il PID

# Terminale 2
kill -SIGUSR1 <PID>
kill -SIGUSR2 <PID>
kill -SIGINT <PID>
```

---

### Esercizio 1.4 - Alarm Timer
**Obiettivo**: Utilizzare alarm() per creare un timeout.

Scrivi un programma che:
- Chiede all'utente di inserire una password entro 5 secondi
- Usa `alarm(5)` per impostare il timeout
- Se l'utente inserisce la password in tempo, verifica se è corretta ("segreto123")
- Se scade il tempo, stampa "Tempo scaduto!" e termina

---

### Esercizio 1.5 - Pause e Resume
**Obiettivo**: Comprendere l'uso di pause().

Scrivi un programma che:
- Stampa "Programma in attesa di segnale..."
- Si mette in pausa con `pause()`
- Quando riceve un segnale (qualsiasi), stampa quale segnale ha ricevuto
- Continua l'esecuzione e termina normalmente

---

## Esercizi Intermedi

### Esercizio 2.1 - Comunicazione Padre-Figlio
**Obiettivo**: Usare segnali per comunicazione tra processi.

Scrivi un programma dove:
- Il padre crea un figlio con `fork()`
- Il padre invia al figlio SIGUSR1 ogni 2 secondi per 5 volte
- Il figlio conta i segnali ricevuti e stampa "Ricevuto segnale n. X"
- Dopo 5 segnali, il figlio termina con exit code 0
- Il padre usa `wait()` per attendere la terminazione del figlio

**Schema:**
```
[Padre] PID: 1234
  └─> Crea figlio
  └─> Invia SIGUSR1 ogni 2 sec
      [Figlio] PID: 1235
        └─> Handler conta segnali
        └─> Termina dopo 5 segnali
  └─> Wait sul figlio
  └─> Stampa "Figlio terminato"
```

---

### Esercizio 2.2 - SIGCHLD Handler
**Obiettivo**: Gestire la terminazione dei processi figli.

Scrivi un programma dove:
- Il padre crea 3 processi figli
- Ogni figlio attende un numero diverso di secondi (3, 5, 7) e poi termina con exit code diverso (1, 2, 3)
- Il padre ha un handler per SIGCHLD che stampa quale figlio è terminato e con quale exit code
- Il padre resta in loop infinito dopo aver creato i figli

**Output atteso:**
```
Creato figlio 1 (PID: 1001)
Creato figlio 2 (PID: 1002)
Creato figlio 3 (PID: 1003)
Padre in attesa...
[dopo 3 sec] Figlio 1001 terminato con exit code 1
Padre in attesa...
[dopo 5 sec] Figlio 1002 terminato con exit code 2
Padre in attesa...
[dopo 7 sec] Figlio 1003 terminato con exit code 3
Tutti i figli terminati
```

---

### Esercizio 2.3 - Semaforo con Segnali
**Obiettivo**: Simulare un semaforo stradale.

Scrivi un programma che:
- Simula un semaforo con 3 stati: VERDE, GIALLO, ROSSO
- Inizia in stato VERDE
- SIGUSR1 → passa allo stato successivo (VERDE→GIALLO→ROSSO→VERDE)
- SIGUSR2 → torna allo stato VERDE (reset)
- SIGINT → termina mostrando statistiche (quante volte ogni stato è stato attivo)
- Stampa lo stato corrente ogni volta che cambia

---

### Esercizio 2.4 - Sleep Interrompibile
**Obiettivo**: Gestire l'interruzione di sleep().

Scrivi un programma che:
- Esegue `sleep(30)`
- Se l'utente preme CTRL-C durante lo sleep, l'handler stampa quanti secondi rimanevano
- Chiede all'utente se vuole continuare (s/n)
- Se sì, riprende il sleep per il tempo rimanente
- Se no, termina

**Esempio:**
```
Sleep di 30 secondi avviato...
[utente preme CTRL-C dopo 10 sec]
Sleep interrotto! Rimanevano 20 secondi.
Continuare? (s/n): s
Ripresa sleep per 20 secondi...
```

---

### Esercizio 2.5 - Exec e Segnali
**Obiettivo**: Comprendere il comportamento dei segnali dopo exec().

Scrivi due programmi:

**Programma A (sender):**
- Ignora SIGINT
- Ha un handler per SIGUSR1 che stampa "Handler A: SIGUSR1"
- Esegue per 5 secondi stampando "Programma A attivo"
- Dopo 5 secondi, fa exec() del Programma B

**Programma B (receiver):**
- Ha un handler per SIGUSR1 che stampa "Handler B: SIGUSR1"
- Esegue in loop infinito stampando "Programma B attivo"

**Domanda**: Cosa succede se invii SIGINT e SIGUSR1 prima e dopo l'exec()?

---

## Esercizi Avanzati

### Esercizio 3.1 - Shell Personalizzata
**Obiettivo**: Creare una mini shell con gestione segnali.

Scrivi una shell che:
- Legge comandi dall'utente in un loop
- Esegue i comandi in processi figli con fork() + exec()
- SIGINT (CTRL-C): termina il comando corrente, non la shell
- SIGTSTP (CTRL-Z): mette in background il comando corrente
- Comando "jobs": mostra i processi in background
- Comando "fg": porta in foreground l'ultimo processo in background
- Comando "exit": termina la shell

**Funzionalità base:**
```bash
myshell$ ls -la
[output di ls]
myshell$ sleep 100
^C
Comando terminato
myshell$ exit
```

---

### Esercizio 3.2 - Watchdog Timer
**Obiettivo**: Implementare un sistema di controllo timeout.

Scrivi un programma che:
- Crea un processo figlio che esegue un'operazione lunga (es. sleep 30)
- Il padre imposta un timeout di 10 secondi
- Se il figlio non termina entro 10 secondi, il padre lo termina con SIGKILL
- Il padre gestisce SIGCHLD per sapere se il figlio è terminato normalmente o è stato killato
- Stampa statistiche finali

**Output:**
```
Avvio operazione con timeout di 10 secondi...
[se termina in tempo]
Operazione completata con successo in 7 secondi

[se timeout]
TIMEOUT! Processo terminato forzatamente dopo 10 secondi
```

---

### Esercizio 3.3 - Sistema di Notifiche
**Obiettivo**: Implementare un sistema produttore-consumatore con segnali.

Scrivi un programma con 3 processi:
1. **Produttore**: genera numeri casuali ogni 2 secondi, li salva in un file, invia SIGUSR1 al consumatore
2. **Consumatore**: quando riceve SIGUSR1, legge il numero dal file, lo elabora (es. calcola il quadrato), invia SIGUSR2 al logger
3. **Logger**: quando riceve SIGUSR2, registra l'operazione su un file di log

Il processo principale:
- Crea i 3 processi
- Gestisce SIGCHLD per sapere quando un processo termina
- Dopo 20 secondi, invia SIGTERM a tutti e termina

---

### Esercizio 3.4 - Race Condition con Segnali
**Obiettivo**: Comprendere e risolvere race conditions.

**Parte A - Problema:**
Scrivi un programma che:
- Ha una variabile globale `contatore = 0`
- Handler per SIGUSR1 che incrementa il contatore
- Il main invia SIGUSR1 a se stesso 1000 volte velocemente
- Stampa il valore finale del contatore

**Domanda**: Il contatore sarà sempre 1000? Perché?

**Parte B - Soluzione:**
Modifica il programma usando:
- Maschere di segnali (`sigprocmask()`)
- Sezioni critiche
- Implementa una soluzione che garantisca il conteggio corretto

---

### Esercizio 3.5 - Sistema Multi-Client
**Obiettivo**: Gestire multiple connessioni con segnali.

Scrivi un server che:
- Crea N processi figli (es. 3) all'avvio
- Ogni figlio gestisce richieste "simulate" (stampa ogni 3 secondi "Processo X: richiesta elaborata")
- Il padre monitora i figli con SIGCHLD
- Se un figlio muore, il padre ne crea uno nuovo
- SIGINT al padre: termina tutti i figli e il padre
- SIGUSR1 al padre: stampa statistiche (numero richieste per figlio)

**Test:**
```bash
./server &
# Osserva i processi figli
ps aux | grep server
# Killa un figlio manualmente
kill -9 <PID_figlio>
# Verifica che ne venga creato uno nuovo
# Invia segnali al padre
kill -SIGUSR1 <PID_padre>
kill -SIGINT <PID_padre>
```

---

## Progetti Completi

### Progetto 1 - Sistema di Controllo LED RGB
**Descrizione**: Simula il controllo di un LED RGB tramite segnali.

**Requisiti:**
- 3 processi: Controller, LED_Handler, Logger
- Controller: legge comandi da tastiera
  - `r` → invia SIGUSR1 (rosso)
  - `g` → invia SIGUSR2 (verde)  
  - `b` → invia SIGRTMIN (blu)
  - `o` → invia SIGTERM (spegni)
- LED_Handler: gestisce i segnali e "accende" il LED corrispondente
- Logger: riceve notifiche via SIGCHLD e registra ogni cambio di stato
- Implementa anche il lampeggio: se ricevi lo stesso segnale 2 volte consecutive, lampeggia

**Funzionalità avanzate:**
- Mix di colori (es. rosso + verde = giallo)
- Transizioni graduali
- Salvataggio/ripristino stato

---

### Progetto 2 - Cron Semplificato
**Descrizione**: Implementa un sistema simile a cron per eseguire task programmati.

**Requisiti:**
- Legge da un file di configurazione le operazioni da eseguire
- Ogni riga: `<secondi> <comando>`
  - Esempio: `10 /usr/bin/notify-send "10 secondi passati"`
- Usa `alarm()` e SIGALRM per schedulare i task
- SIGHUP: ricarica il file di configurazione
- SIGUSR1: stampa la lista dei task e il prossimo in esecuzione
- SIGINT: termina dopo aver completato il task corrente

**File di configurazione (tasks.conf):**
```
5 echo "Task ogni 5 secondi"
10 date >> /tmp/log.txt
15 notify-send "Promemoria"
```

---

### Progetto 3 - Sistema di Backup Automatico
**Descrizione**: Sistema che esegue backup periodici con controllo via segnali.

**Componenti:**
1. **Daemon di backup**: esegue backup ogni N secondi (configurabile)
2. **Controller**: invia comandi al daemon
3. **Monitor**: visualizza lo stato

**Segnali:**
- SIGUSR1 → esegui backup immediato
- SIGUSR2 → pausa/riprendi backup automatici
- SIGHUP → ricarica configurazione
- SIGINT → termina dopo aver completato il backup corrente
- SIGCHLD → notifica quando un backup è completato

**Funzionalità:**
- Gestione errori (directory non esistente, disco pieno)
- Log dettagliato
- Statistiche (numero backup, dimensione totale, tempo medio)
- File di configurazione per directory, intervallo, retention

---

## Domande di Teoria

### Sezione 1 - Concetti Base
1. Cos'è un segnale in Unix/Linux?
2. Quali sono i tre modi in cui un processo può gestire un segnale?
3. Cosa restituisce la system call `signal()` in caso di successo?
4. Qual è la differenza tra SIGTERM e SIGKILL?
5. Perché alcuni segnali non possono essere intercettati?

### Sezione 2 - Comportamento
6. Cosa succede agli handler di segnali dopo una `fork()`?
7. Cosa succede agli handler di segnali dopo una `exec()`?
8. Perché è importante che un signal handler sia breve e veloce?
9. Cosa sono le funzioni "signal-safe"? Fai 3 esempi.
10. Cosa succede se arriva un segnale mentre il processo sta eseguendo l'handler di un altro segnale?

### Sezione 3 - Problematiche
11. Cos'è una race condition nel contesto dei segnali?
12. Perché non si dovrebbe usare `printf()` in un signal handler?
13. Come si può evitare che i processi figli diventino zombie?
14. Qual è il problema nell'uso di variabili globali negli handler?
15. Come si gestiscono correttamente multiple istanze dello stesso segnale?

---

## Quiz a Risposta Multipla

### Q1. Quale segnale NON può essere intercettato?
- A) SIGTERM
- B) SIGKILL
- C) SIGINT
- D) SIGUSR1

### Q2. Cosa restituisce `fork()` nel processo figlio quando riceve SIGUSR1 se il padre aveva un handler per SIGUSR1?
- A) Il figlio non eredita l'handler
- B) Il figlio eredita l'handler
- C) Il figlio riceve il segnale ma lo ignora
- D) Dipende dal sistema operativo

### Q3. Dopo `execl("/bin/ls", "ls", NULL)`, un handler per SIGINT:
- A) Viene mantenuto
- B) Viene rimosso (torna a default)
- C) Causa un errore
- D) Dipende dalle opzioni di compilazione

### Q4. Quale funzione è signal-safe?
- A) printf()
- B) malloc()
- C) write()
- D) fopen()

### Q5. Un processo padre deve gestire la terminazione di N figli. La soluzione corretta è:
- A) Chiamare wait() N volte nel main
- B) Usare un handler SIGCHLD che chiama wait() una volta
- C) Usare un handler SIGCHLD che chiama waitpid() in loop con WNOHANG
- D) Non serve gestire SIGCHLD, il kernel lo fa automaticamente

---

## Esercizi di Debugging

### Debug 1 - Trova l'errore
```c
#include <stdio.h>
#include <signal.h>

int counter = 0;

void handler(int sig) {
    counter++;
    printf("Segnale numero: %d\n", counter);
    malloc(100);  // Alloca memoria
}

int main() {
    signal(SIGUSR1, handler);
    while(1);
    return 0;
}
```
**Domanda**: Quali sono i problemi in questo codice?

---

### Debug 2 - Race Condition
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int ready = 0;

void handler(int sig) {
    ready = 1;
}

int main() {
    signal(SIGUSR1, handler);
    
    if (fork() == 0) {
        // Figlio
        sleep(1);
        kill(getppid(), SIGUSR1);
    } else {
        // Padre
        while (!ready) {
            // Busy wait
        }
        printf("Segnale ricevuto!\n");
    }
    return 0;
}
```
**Domanda**: Perché questo codice può non funzionare correttamente?

---

### Debug 3 - Memory Leak
```c
void sigchld_handler(int sig) {
    int status;
    pid_t pid = wait(&status);
    char *msg = malloc(100);
    sprintf(msg, "Figlio %d terminato\n", pid);
    printf("%s", msg);
    // Dimenticato free(msg)!
}
```
**Domanda**: Oltre al memory leak, quali altri problemi ci sono?

---

## Soluzioni

### Soluzione Esercizio 1.1

**C:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int counter = 0;

void sigint_handler(int sig) {
    counter++;
    printf("\nHai premuto CTRL-C! (n. %d)\n", counter);
    
    if (counter >= 3) {
        printf("Arrivederci!\n");
        exit(0);
    }
}

int main() {
    signal(SIGINT, sigint_handler);
    
    printf("Premi CTRL-C 3 volte per uscire\n");
    while(1) {
        printf("Programma in esecuzione...\n");
        sleep(2);
    }
    
    return 0;
}
```

**Python:**
```python
import signal
import time
import sys

counter = 0

def sigint_handler(signum, frame):
    global counter
    counter += 1
    print(f"\nHai premuto CTRL-C! (n. {counter})")
    
    if counter >= 3:
        print("Arrivederci!")
        sys.exit(0)

signal.signal(signal.SIGINT, sigint_handler)

print("Premi CTRL-C 3 volte per uscire")
while True:
    print("Programma in esecuzione...")
    time.sleep(2)
```

---

### Soluzione Esercizio 2.1

**C:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int signal_count = 0;

void sigusr1_handler(int sig) {
    signal_count++;
    printf("[Figlio %d] Ricevuto segnale n. %d\n", getpid(), signal_count);
    
    if (signal_count >= 5) {
        printf("[Figlio] Ricevuti 5 segnali, termino\n");
        exit(0);
    }
}

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Processo figlio
        signal(SIGUSR1, sigusr1_handler);
        printf("[Figlio %d] In attesa di segnali...\n", getpid());
        
        while(1) {
            pause();  // Attende segnali
        }
    } else {
        // Processo padre
        printf("[Padre %d] Figlio creato: %d\n", getpid(), pid);
        
        for(int i = 1; i <= 5; i++) {
            sleep(2);
            printf("[Padre] Invio segnale %d\n", i);
            kill(pid, SIGUSR1);
        }
        
        int status;
        wait(&status);
        printf("[Padre] Figlio terminato\n");
    }
    
    return 0;
}
```

**Python:**
```python
import signal
import os
import sys
import time

signal_count = 0

def sigusr1_handler(signum, frame):
    global signal_count
    signal_count += 1
    print(f"[Figlio {os.getpid()}] Ricevuto segnale n. {signal_count}")
    
    if signal_count >= 5:
        print("[Figlio] Ricevuti 5 segnali, termino")
        sys.exit(0)

pid = os.fork()

if pid == 0:
    # Processo figlio
    signal.signal(signal.SIGUSR1, sigusr1_handler)
    print(f"[Figlio {os.getpid()}] In attesa di segnali...")
    
    while True:
        signal.pause()  # Attende segnali
else:
    # Processo padre
    print(f"[Padre {os.getpid()}] Figlio creato: {pid}")
    
    for i in range(1, 6):
        time.sleep(2)
        print(f"[Padre] Invio segnale {i}")
        os.kill(pid, signal.SIGUSR1)
    
    os.wait()
    print("[Padre] Figlio terminato")
```

---

### Risposte Quiz

**Q1**: B (SIGKILL)  
**Q2**: B (Il figlio eredita l'handler)  
**Q3**: B (Viene rimosso, torna a default)  
**Q4**: C (write())  
**Q5**: C (Handler con waitpid() in loop con WNOHANG)

---

### Soluzioni Debug

**Debug 1 - Problemi:**
1. `printf()` non è signal-safe
2. `malloc()` non è signal-safe
3. Memory leak (memoria allocata non liberata)
4. Possibili race conditions sulla variabile `counter`

**Soluzione corretta:**
```c
void handler(int sig) {
    static volatile sig_atomic_t counter = 0;
    counter++;
    const char msg[] = "Segnale ricevuto\n";
    write(STDOUT_FILENO, msg, sizeof(msg)-1);
}
```

**Debug 2 - Problemi:**
1. Busy waiting consuma CPU
2. Race condition: il segnale potrebbe arrivare prima che il padre inizi il loop
3. La variabile `ready` dovrebbe essere `volatile sig_atomic_t`

**Debug 3 - Problemi:**
1. Memory leak (malloc senza free)
2. `malloc()` non è signal-safe
3. `sprintf()` non è signal-safe
4. `printf()` non è signal-safe

---

## Criteri di Valutazione

### Esercizi Base (1.1-1.5)
- **Sufficiente**: Programma funziona con gestione base dei segnali
- **Buono**: Gestione errori + commenti nel codice
- **Ottimo**: Codice pulito + gestione edge cases + output user-friendly

### Esercizi Intermedi (2.1-2.5)
- **Sufficiente**: Implementazione corretta della logica richiesta
- **Buono**: + Gestione errori + sincronizzazione corretta
- **Ottimo**: + Codice modulare + documentazione + test

### Esercizi Avanzati (3.1-3.5)
- **Sufficiente**: Funzionalità base implementate
- **Buono**: + Gestione completa degli errori + robustezza
- **Ottimo**: + Design pulito + estensibilità + performance

### Progetti Completi
- **Sufficiente**: Tutti i requisiti base soddisfatti
- **Buono**: + Funzionalità avanzate + documentazione
- **Ottimo**: + Architettura solida + testing + README dettagliato

---

## Risorse Aggiuntive

- [Guida Completa ai Segnali](03-Guida-Completa-Segnali.md)
- `man 7 signal` - Documentazione completa
- `man 2 sigaction` - Gestione avanzata
- Test online: https://www.tutorialspoint.com/unix/unix-signals-traps.htm

---

**Nota per gli studenti**: Prima di iniziare gli esercizi, assicurati di aver letto e compreso la [Guida Completa ai Segnali](03-Guida-Completa-Segnali.md). Inizia dagli esercizi base e procedi gradualmente verso quelli più complessi.

---

[Torna alla Guida Segnali](03-Guida-Completa-Segnali.md) | [Torna all'Indice Principale](../README.md)
