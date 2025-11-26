# Comunicazione tra Processi: I Segnali

## Indice
1. [Introduzione ai Segnali](#introduzione-ai-segnali)
2. [Tipi di Segnali](#tipi-di-segnali)
3. [Gestione dei Segnali](#gestione-dei-segnali)
4. [Signal Handler](#signal-handler)
5. [Segnali e Fork](#segnali-e-fork)
6. [Segnali e Exec](#segnali-e-exec)
7. [Invio di Segnali](#invio-di-segnali)
8. [Esempi Pratici](#esempi-pratici)

---

## Introduzione ai Segnali

I **segnali** sono un meccanismo di comunicazione tra processi (IPC - Inter-Process Communication) utilizzato nei sistemi Unix/Linux. Un segnale è una notifica asincrona inviata a un processo per informarlo che si è verificato un evento particolare.

### Caratteristiche principali
- **Asincroni**: possono arrivare in qualsiasi momento durante l'esecuzione del processo
- **Leggeri**: meccanismo semplice e veloce
- **Limitati**: possono trasmettere solo un tipo di evento, senza dati aggiuntivi

### Utilizzi comuni
- Notifica di eventi (es. divisione per zero, accesso a memoria non valida)
- Comunicazione tra processi
- Gestione della terminazione dei processi
- Implementazione di timeout e allarmi
- Controllo da parte dell'utente (es. CTRL-C)

---

## Tipi di Segnali

### Segnali Standard POSIX

| Segnale | Numero | Descrizione | Azione di Default |
|---------|--------|-------------|-------------------|
| SIGHUP | 1 | Hangup - terminazione del terminale di controllo | Termina |
| SIGINT | 2 | Interrupt - CTRL-C | Termina |
| SIGQUIT | 3 | Quit - CTRL-\ | Termina + core dump |
| SIGILL | 4 | Istruzione illegale | Termina + core dump |
| SIGTRAP | 5 | Trace/breakpoint trap | Termina + core dump |
| SIGABRT | 6 | Abort | Termina + core dump |
| SIGFPE | 8 | Errore aritmetico (divisione per zero) | Termina + core dump |
| SIGKILL | 9 | Kill - terminazione forzata | Termina (non intercettabile) |
| SIGSEGV | 11 | Violazione di segmento | Termina + core dump |
| SIGPIPE | 13 | Scrittura su pipe senza lettori | Termina |
| SIGALRM | 14 | Timer alarm | Termina |
| SIGTERM | 15 | Terminazione (kill standard) | Termina |
| SIGUSR1 | 10 | Segnale utente 1 | Termina |
| SIGUSR2 | 12 | Segnale utente 2 | Termina |
| SIGCHLD | 17 | Terminazione processo figlio | Ignora |
| SIGCONT | 18 | Continua se fermato | Continua |
| SIGSTOP | 19 | Stop - ferma il processo | Stop (non intercettabile) |
| SIGTSTP | 20 | Stop da terminale - CTRL-Z | Stop |

### Categorie di Segnali

1. **Segnali di Errore del Programma**
   - SIGFPE, SIGILL, SIGSEGV, SIGBUS
   - Generati dal kernel quando il programma esegue operazioni non valide

2. **Segnali di Terminazione**
   - SIGTERM, SIGINT, SIGQUIT, SIGKILL
   - Utilizzati per terminare i processi

3. **Segnali di Controllo Job**
   - SIGSTOP, SIGTSTP, SIGCONT
   - Gestiscono la sospensione e ripresa dei processi

4. **Segnali Definiti dall'Utente**
   - SIGUSR1, SIGUSR2
   - Possono essere utilizzati per qualsiasi scopo

5. **Segnali di Notifica**
   - SIGCHLD, SIGALRM, SIGPIPE
   - Notificano eventi specifici

---

## Gestione dei Segnali

### Comportamenti possibili

Un processo può gestire un segnale in tre modi:

1. **Azione di Default** (SIG_DFL)
   - Comportamento predefinito del sistema
   - Varia a seconda del segnale

2. **Ignorare il Segnale** (SIG_IGN)
   - Il segnale viene ricevuto ma ignorato
   - Non tutti i segnali possono essere ignorati (es. SIGKILL, SIGSTOP)

3. **Handler Personalizzato**
   - Funzione definita dall'utente che viene eseguita alla ricezione del segnale

### La funzione `signal()`

```c
#include <signal.h>

void (*signal(int signum, void (*handler)(int)))(int);
```

**Parametri:**
- `signum`: numero del segnale da gestire
- `handler`: puntatore alla funzione handler o SIG_DFL o SIG_IGN

**Ritorna:**
- Puntatore alla precedente funzione handler
- SIG_ERR in caso di errore

#### Sintassi semplificata in Python

```python
import signal

signal.signal(signal.SIGINT, handler_function)
```

---

## Signal Handler

### Caratteristiche degli Handler

Un signal handler è una funzione che:
- Viene chiamata automaticamente quando arriva il segnale
- Deve essere **breve** e **veloce**
- Deve essere **rientrante** (thread-safe)
- Non dovrebbe chiamare funzioni non signal-safe

### Prototipo dell'Handler in C

```c
void handler(int signum) {
    // Codice da eseguire
}
```

### Prototipo dell'Handler in Python

```python
def handler(signum, frame):
    # Codice da eseguire
    pass
```

### Esempio Base - Gestione SIGINT (CTRL-C)

**C:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void gestore_ctrl_c(int signum) {
    printf("\nRicevuto SIGINT. Terminazione...\n");
    exit(0);
}

int main() {
    signal(SIGINT, gestore_ctrl_c);
    
    while(1) {
        printf("In esecuzione...\n");
        sleep(1);
    }
    return 0;
}
```

**Python:**
```python
import signal
import time
import sys

def gestore_ctrl_c(signum, frame):
    print("\nRicevuto SIGINT. Terminazione...")
    sys.exit(0)

signal.signal(signal.SIGINT, gestore_ctrl_c)

while True:
    print("In esecuzione...")
    time.sleep(1)
```

### Ripristino del Comportamento di Default

**C:**
```c
signal(SIGINT, SIG_DFL);  // Ripristina comportamento default
```

**Python:**
```python
signal.signal(signal.SIGINT, signal.SIG_DFL)
```

### Ignorare un Segnale

**C:**
```c
signal(SIGINT, SIG_IGN);  // Ignora il segnale
```

**Python:**
```python
signal.signal(signal.SIGINT, signal.SIG_IGN)
```

---

## Segnali e Fork

### Ereditarietà dei Segnali

Quando un processo esegue `fork()`:
- Il processo figlio **eredita** gli handler dei segnali del padre
- Le disposizioni dei segnali (default, ignore, handler) vengono copiate
- Padre e figlio possono modificare i loro handler indipendentemente

### Esempio

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int sig) {
    printf("Processo %d ricevuto segnale %d\n", getpid(), sig);
}

int main() {
    signal(SIGINT, handler);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Processo figlio - eredita l'handler
        while(1) {
            printf("Figlio %d in esecuzione\n", getpid());
            sleep(2);
        }
    } else {
        // Processo padre
        while(1) {
            printf("Padre %d in esecuzione\n", getpid());
            sleep(2);
        }
    }
    return 0;
}
```

---

## Segnali e Exec

### Comportamento dopo exec()

Quando un processo esegue una delle funzioni `exec()`:

1. **Segnali con handler personalizzati** → tornano al comportamento di default
2. **Segnali ignorati (SIG_IGN)** → rimangono ignorati
3. **Segnali con comportamento default** → rimangono al default

**Motivo:** il codice dell'handler non esiste più dopo exec(), quindi non può essere mantenuto.

### Esempio

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Handler chiamato\n");
}

int main() {
    signal(SIGINT, handler);
    printf("Handler impostato. Premere CTRL-C...\n");
    sleep(5);
    
    printf("Esecuzione di exec...\n");
    execl("/bin/sleep", "sleep", "10", NULL);
    
    return 0;
}
```

---

## Invio di Segnali

### La funzione kill()

Nonostante il nome, `kill()` serve per **inviare** segnali a processi, non solo per terminarli.

**C:**
```c
#include <signal.h>
#include <sys/types.h>

int kill(pid_t pid, int sig);
```

**Python:**
```python
import os
import signal

os.kill(pid, signal.SIGUSR1)
```

### Parametri di kill()

- `pid > 0`: invia il segnale al processo con quel PID
- `pid = 0`: invia il segnale a tutti i processi del gruppo
- `pid = -1`: invia il segnale a tutti i processi (con permessi)
- `pid < -1`: invia il segnale al gruppo di processi |pid|

**Ritorna:**
- 0 in caso di successo
- -1 in caso di errore (errno impostato)

### Esempio di Comunicazione tra Processi

**C:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int contatore = 0;

void handler(int sig) {
    contatore++;
    printf("Ricevuto segnale. Totale: %d\n", contatore);
    if (contatore >= 5) {
        printf("Terminazione figlio\n");
        exit(0);
    }
}

int main() {
    signal(SIGUSR1, handler);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Figlio - riceve segnali
        while(1) {
            printf("Figlio in attesa...\n");
            sleep(2);
        }
    } else {
        // Padre - invia segnali
        for(int i = 0; i < 10; i++) {
            printf("Padre invia segnale %d\n", i+1);
            kill(pid, SIGUSR1);
            sleep(1);
        }
    }
    return 0;
}
```

### Altri Metodi per Inviare Segnali

1. **Da riga di comando:**
   ```bash
   kill -SIGTERM 1234
   kill -15 1234
   kill 1234  # default: SIGTERM
   ```

2. **Da tastiera:**
   - CTRL-C → SIGINT
   - CTRL-Z → SIGTSTP
   - CTRL-\ → SIGQUIT

---

## Funzioni Correlate ai Segnali

### alarm() - Impostare un Timer

**C:**
```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

Invia SIGALRM al processo dopo `seconds` secondi.

**Esempio:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void alarm_handler(int sig) {
    printf("Tempo scaduto!\n");
    exit(0);
}

int main() {
    signal(SIGALRM, alarm_handler);
    alarm(5);  // Allarme dopo 5 secondi
    
    printf("Timer impostato. Attesa...\n");
    while(1) {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    return 0;
}
```

**Python:**
```python
import signal
import time

def alarm_handler(signum, frame):
    print("Tempo scaduto!")
    exit(0)

signal.signal(signal.SIGALRM, alarm_handler)
signal.alarm(5)

print("Timer impostato. Attesa...")
while True:
    time.sleep(1)
    print(".", end="", flush=True)
```

### pause() - Sospendere fino a un Segnale

**C:**
```c
#include <unistd.h>

int pause(void);
```

Sospende il processo fino alla ricezione di un segnale.

**Esempio:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Ricevuto segnale %d\n", sig);
}

int main() {
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);
    
    printf("In pausa. Inviare un segnale per continuare.\n");
    pause();
    
    printf("Ripresa esecuzione.\n");
    return 0;
}
```

**Python:**
```python
import signal

def handler(signum, frame):
    print(f"Ricevuto segnale {signum}")

signal.signal(signal.SIGINT, handler)
signal.signal(signal.SIGUSR1, handler)

print("In pausa. Inviare un segnale per continuare.")
signal.pause()

print("Ripresa esecuzione.")
```

### sleep() e Segnali

`sleep()` può essere interrotto dalla ricezione di un segnale. In C, restituisce il numero di secondi rimanenti.

**C:**
```c
int rimasti = sleep(10);
if (rimasti > 0) {
    printf("Sleep interrotto, rimanevano %d secondi\n", rimasti);
}
```

---

## SIGCHLD - Gestione Terminazione Figli

### Caratteristiche di SIGCHLD

- Inviato al processo padre quando un figlio termina
- Comportamento di default: ignorato
- Utilizzato per evitare processi zombie

### Esempio Base

**C:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void sigchld_handler(int sig) {
    int status;
    pid_t pid = wait(&status);
    printf("Figlio %d terminato con stato %d\n", pid, WEXITSTATUS(status));
}

int main() {
    signal(SIGCHLD, sigchld_handler);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Figlio
        printf("Figlio in esecuzione\n");
        sleep(3);
        exit(42);
    } else {
        // Padre
        printf("Padre in attesa\n");
        while(1) {
            sleep(1);
            printf("Padre attivo\n");
        }
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

def sigchld_handler(signum, frame):
    pid, status = os.wait()
    exit_code = status >> 8
    print(f"Figlio {pid} terminato con stato {exit_code}")

signal.signal(signal.SIGCHLD, sigchld_handler)

pid = os.fork()

if pid == 0:
    # Figlio
    print("Figlio in esecuzione")
    time.sleep(3)
    sys.exit(42)
else:
    # Padre
    print("Padre in attesa")
    while True:
        time.sleep(1)
        print("Padre attivo")
```

### Gestione Multipli Figli

```c
void sigchld_handler(int sig) {
    int status;
    pid_t pid;
    
    // Gestisce tutti i figli terminati
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Figlio %d terminato\n", pid);
    }
}
```

---

## Esempi Pratici

### 1. Handler Unico per Multipli Segnali

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler_universale(int sig) {
    switch(sig) {
        case SIGINT:
            printf("Ricevuto SIGINT\n");
            break;
        case SIGUSR1:
            printf("Ricevuto SIGUSR1 - LED Rosso\n");
            break;
        case SIGUSR2:
            printf("Ricevuto SIGUSR2 - LED Verde\n");
            break;
        default:
            printf("Segnale sconosciuto: %d\n", sig);
    }
}

int main() {
    signal(SIGINT, handler_universale);
    signal(SIGUSR1, handler_universale);
    signal(SIGUSR2, handler_universale);
    
    printf("PID: %d\n", getpid());
    printf("Invia segnali con: kill -SIGUSR1 %d\n", getpid());
    
    while(1) {
        sleep(1);
    }
    return 0;
}
```

### 2. Timeout su Operazioni

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

jmp_buf env;

void timeout_handler(int sig) {
    printf("\nTimeout!\n");
    longjmp(env, 1);
}

int main() {
    signal(SIGALRM, timeout_handler);
    
    if (setjmp(env) == 0) {
        alarm(5);  // Timeout di 5 secondi
        printf("Hai 5 secondi per premere ENTER: ");
        getchar();
        alarm(0);  // Annulla alarm
        printf("OK, in tempo!\n");
    } else {
        printf("Troppo tardi!\n");
    }
    
    return 0;
}
```

### 3. Shell Semplice con Gestione Segnali

```python
import os
import signal
import sys

def sigint_handler(signum, frame):
    print("\nUsa 'exit' per uscire")

signal.signal(signal.SIGINT, sigint_handler)

while True:
    try:
        cmd = input("$ ")
        if cmd == "exit":
            break
        
        pid = os.fork()
        if pid == 0:
            # Figlio - esegue comando
            args = cmd.split()
            try:
                os.execvp(args[0], args)
            except:
                print(f"Comando non trovato: {args[0]}")
                sys.exit(1)
        else:
            # Padre - attende
            os.wait()
    except EOFError:
        break

print("Uscita dalla shell")
```

---

## Best Practices

### 1. Handler Signal-Safe

Evitare in un handler:
- `printf()` (usare `write()` invece)
- `malloc()` / `free()`
- Funzioni non rientranti

**Corretto:**
```c
void handler(int sig) {
    const char msg[] = "Segnale ricevuto\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}
```

### 2. Salvare e Ripristinare errno

```c
void handler(int sig) {
    int saved_errno = errno;
    // ... codice dell'handler ...
    errno = saved_errno;
}
```

### 3. Usare sigaction() invece di signal()

`sigaction()` è più portabile e affidabile:

```c
struct sigaction sa;
sa.sa_handler = handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = 0;
sigaction(SIGINT, &sa, NULL);
```

### 4. Gestire Race Conditions

Usare atomic operations o maschere di segnali quando necessario.

---

## Segnali Non Intercettabili

Due segnali **non possono** essere gestiti o ignorati:
- **SIGKILL** (9): terminazione immediata
- **SIGSTOP** (19): stop immediato

Questi garantiscono che il sistema possa sempre controllare i processi.

---

## Debugging

### Visualizzare Segnali Pendenti

```bash
cat /proc/<PID>/status | grep Sig
```

### Tracciare Segnali

```bash
strace -e signal ./programma
```

---

## Conclusioni

I segnali sono un meccanismo potente ma delicato:
- **Vantaggi**: leggeri, veloci, asincroni
- **Svantaggi**: limitati, possono creare race conditions

Per comunicazione più complessa tra processi, considerare:
- Pipe
- Socket
- Shared memory
- Message queues

---

## Riferimenti

- `man 7 signal` - Panoramica completa dei segnali
- `man 2 kill` - Invio di segnali
- `man 2 sigaction` - Gestione avanzata dei segnali
- `man 3 signal` - Gestione base dei segnali

---

[Torna all'Indice Principale](../README.md)
