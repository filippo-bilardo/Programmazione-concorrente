# Modulo 2 - System Call per la Gestione dei Processi

## Indice
- [Introduzione alle System Call](#introduzione-alle-system-call)
- [fork() - Creazione di Processi](#fork---creazione-di-processi)
- [exec() - Sostituzione dell'Immagine](#exec---sostituzione-dellimmagine)
- [wait() e waitpid() - Sincronizzazione](#wait-e-waitpid---sincronizzazione)
- [exit() - Terminazione](#exit---terminazione)
- [getpid() e getppid() - Identificatori](#getpid-e-getppid---identificatori)
- [Esempi Pratici Completi](#esempi-pratici-completi)

---

## Introduzione alle System Call

### Cos'è una System Call?

Una **system call** è un'interfaccia programmatica che permette ai programmi in user space di richiedere servizi al kernel del sistema operativo.

```
┌──────────────────────────────────┐
│     User Space (Ring 3)          │
│  ┌────────────────────────────┐  │
│  │   Applicazione             │  │
│  │   (Python, C, Java, ...)   │  │
│  └────────────┬───────────────┘  │
│               │ system call      │
│               ↓                  │
├──────────────────────────────────┤
│     Kernel Space (Ring 0)        │
│  ┌────────────────────────────┐  │
│  │   Sistema Operativo        │  │
│  │   (Linux Kernel)           │  │
│  └────────────────────────────┘  │
│               │                  │
│               ↓                  │
├──────────────────────────────────┤
│        Hardware                  │
└──────────────────────────────────┘
```

### Come Funziona una System Call

```
1. Applicazione chiama funzione (es. fork())
   ↓
2. Libreria C prepara parametri
   ↓
3. Interrupt software (trap) al kernel
   ↓
4. Context switch: user mode → kernel mode
   ↓
5. Kernel esegue la system call
   ↓
6. Kernel ritorna risultato
   ↓
7. Context switch: kernel mode → user mode
   ↓
8. Applicazione riceve risultato
```

### Overhead delle System Call

```
Operazione              Tempo
─────────────────────────────────
Funzione normale        ~1 ns
System call semplice    ~100 ns
Context switch          ~1-10 μs
```

---

## fork() - Creazione di Processi

### Definizione

`fork()` crea un nuovo processo (child) duplicando il processo chiamante (parent). Il child è una copia quasi esatta del parent.

### Signature

```c
// C
#include <unistd.h>
pid_t fork(void);

// Ritorna:
// - PID del child nel parent (> 0)
// - 0 nel child
// - -1 in caso di errore
```

```python
# Python
import os
pid = os.fork()
```

### Come Funziona fork()

```
Prima del fork():
┌────────────────────┐
│   Parent Process   │
│   PID = 1234       │
│   Code + Data      │
└────────────────────┘

Dopo fork():
┌────────────────────┐     ┌────────────────────┐
│   Parent Process   │     │   Child Process    │
│   PID = 1234       │     │   PID = 1235       │
│   PPID = 1000      │     │   PPID = 1234      │
│   fork() = 1235    │     │   fork() = 0       │
│   Code + Data      │     │   Code + Data      │
│   (originale)      │     │   (copia)          │
└────────────────────┘     └────────────────────┘
```

### Cosa viene Copiato?

**Copiato (Copy-on-Write):**
- ✓ Segmento codice (condiviso in read-only)
- ✓ Segmento dati (copiato solo se modificato)
- ✓ Heap
- ✓ Stack
- ✓ File descriptors aperti
- ✓ Variabili d'ambiente
- ✓ Directory corrente
- ✓ Signal handlers

**Non Copiato:**
- ✗ PID (il child ha un PID diverso)
- ✗ PPID (il child ha come parent il chiamante)
- ✗ Lock del parent
- ✗ Timer pending
- ✗ Signal pending

### Copy-on-Write (COW)

Ottimizzazione di `fork()`:

```
Al momento del fork():
┌─────────────────────────────────────┐
│ Parent e Child condividono pagine   │
│ di memoria (read-only)              │
└─────────────────────────────────────┘

Quando Parent/Child scrive:
┌──────────────┐   ┌──────────────┐
│   Parent     │   │    Child     │
│  (pagina     │   │  (pagina     │
│   privata)   │   │   privata)   │
└──────────────┘   └──────────────┘
```

Questo rende `fork()` molto efficiente!

### Esempio Base

```python
import os

print("Prima del fork")
print(f"PID corrente: {os.getpid()}")

pid = os.fork()

if pid < 0:
    print("Errore nella fork!")
elif pid == 0:
    # Codice del CHILD
    print(f"CHILD: Il mio PID è {os.getpid()}")
    print(f"CHILD: Il PID di mio padre è {os.getppid()}")
    print(f"CHILD: fork() ha ritornato {pid}")
else:
    # Codice del PARENT
    print(f"PARENT: Il mio PID è {os.getpid()}")
    print(f"PARENT: Ho creato un figlio con PID {pid}")
    print(f"PARENT: fork() ha ritornato {pid}")
```

**Output:**
```
Prima del fork
PID corrente: 1234
PARENT: Il mio PID è 1234
PARENT: Ho creato un figlio con PID 1235
PARENT: fork() ha ritornato 1235
CHILD: Il mio PID è 1235
CHILD: Il PID di mio padre è 1234
CHILD: fork() ha ritornato 0
```

### Esempio: Fork Multipli

```python
import os

print("Creazione di una gerarchia")

# Prima fork
pid1 = os.fork()
if pid1 == 0:
    print(f"  Child 1: PID={os.getpid()}")
    os._exit(0)

# Seconda fork (solo il parent la esegue)
pid2 = os.fork()
if pid2 == 0:
    print(f"  Child 2: PID={os.getpid()}")
    os._exit(0)

# Solo il parent arriva qui
print(f"Parent: PID={os.getpid()}, Children=[{pid1}, {pid2}]")

# Attende i figli
os.waitpid(pid1, 0)
os.waitpid(pid2, 0)
print("Parent: tutti i figli terminati")
```

### Fork Bomb (⚠️ NON ESEGUIRE!)

```python
# ATTENZIONE: Questo blocca il sistema!
import os
while True:
    os.fork()  # Crea infiniti processi
```

**Protezione:**
```bash
# Limitare numero processi per utente
ulimit -u 100
```

---

## exec() - Sostituzione dell'Immagine

### Definizione

La famiglia di funzioni `exec()` **sostituisce** l'immagine del processo corrente con un nuovo programma. Il PID rimane lo stesso, ma il codice cambia completamente.

### Famiglia exec()

```python
# Python fornisce diverse varianti:
os.execl(path, arg0, arg1, ...)
os.execle(path, arg0, arg1, ..., env)
os.execlp(file, arg0, arg1, ...)
os.execlpe(file, arg0, arg1, ..., env)
os.execv(path, args)
os.execve(path, args, env)
os.execvp(file, args)
os.execvpe(file, args, env)

# Differenze:
# l = list (argomenti separati)
# v = vector (argomenti in lista)
# p = path (cerca in $PATH)
# e = environment (variabili d'ambiente custom)
```

### Come Funziona exec()

```
Prima di exec():
┌────────────────────────┐
│ Process PID=1234       │
│ ┌────────────────────┐ │
│ │ Programma A        │ │
│ │ - codice           │ │
│ │ - dati             │ │
│ │ - stack            │ │
│ └────────────────────┘ │
└────────────────────────┘
         │
         │ exec("/bin/ls")
         ↓
Dopo exec():
┌────────────────────────┐
│ Process PID=1234       │  ← Stesso PID!
│ ┌────────────────────┐ │
│ │ Programma B (/bin/ls)│
│ │ - nuovo codice     │ │
│ │ - nuovi dati       │ │
│ │ - nuovo stack      │ │
│ └────────────────────┘ │
└────────────────────────┘
```

**Importante:** Se `exec()` ha successo, **NON ritorna mai** al codice originale!

### Esempio Base

```python
import os

print("Prima di exec()")
print(f"PID: {os.getpid()}")

# Sostituisce il programma corrente con 'ls'
os.execl("/bin/ls", "ls", "-l", "/tmp")

# Questo codice NON viene MAI eseguito!
print("Questa riga non sarà mai stampata")
```

### Esempio: fork() + exec()

Il pattern più comune è `fork()` seguito da `exec()`:

```python
import os

pid = os.fork()

if pid == 0:  # Child
    print("Child: eseguo 'ls -l'")
    os.execl("/bin/ls", "ls", "-l")
    # Se arriviamo qui, exec() è fallito
    print("ERRORE: exec() fallito!")
    os._exit(1)
else:  # Parent
    print(f"Parent: aspetto child {pid}")
    os.waitpid(pid, 0)
    print("Parent: child terminato")
```

### Esempio: exec() con Argomenti

```python
import os

def execute_command(command, args):
    """Esegue un comando esterno"""
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            # execvp cerca il comando in $PATH
            os.execvp(command, [command] + args)
        except OSError as e:
            print(f"Errore: {e}")
            os._exit(1)
    else:  # Parent
        os.waitpid(pid, 0)

# Esempi
execute_command("echo", ["Hello", "World"])
execute_command("python3", ["--version"])
execute_command("ls", ["-lh", "/home"])
```

### Esempio: exec() con Environment

```python
import os

pid = os.fork()

if pid == 0:  # Child
    # Ambiente custom
    env = {
        'PATH': '/bin:/usr/bin',
        'MY_VAR': 'custom_value',
        'HOME': '/tmp'
    }
    
    # execve con ambiente personalizzato
    os.execve(
        "/usr/bin/env",
        ["env"],  # Comando che stampa l'ambiente
        env
    )
else:  # Parent
    os.waitpid(pid, 0)
```

### Shell Semplice

```python
import os

def simple_shell():
    """Shell minimale"""
    while True:
        try:
            # Prompt
            command = input("$ ").strip()
            
            if not command:
                continue
            
            if command == "exit":
                break
            
            # Parsing
            args = command.split()
            
            # Fork + exec
            pid = os.fork()
            
            if pid == 0:  # Child
                try:
                    os.execvp(args[0], args)
                except OSError:
                    print(f"Comando non trovato: {args[0]}")
                    os._exit(1)
            else:  # Parent
                os.waitpid(pid, 0)
        
        except KeyboardInterrupt:
            print("\nUsa 'exit' per uscire")
        except EOFError:
            break

if __name__ == "__main__":
    simple_shell()
```

---

## wait() e waitpid() - Sincronizzazione

### Definizione

Le system call `wait()` e `waitpid()` permettono al parent di attendere la terminazione di un child e recuperare il suo stato di uscita.

### Perché Servono?

1. **Sincronizzazione**: Parent attende il completamento del child
2. **Pulizia risorse**: Rimuove processi zombie
3. **Stato di uscita**: Recupera il valore di ritorno del child

### Signature

```python
# wait() - attende qualsiasi child
pid, status = os.wait()

# waitpid() - attende un child specifico
pid, status = os.waitpid(pid, options)

# Options:
# 0           = bloccante
# os.WNOHANG  = non bloccante
# os.WUNTRACED = riporta anche child fermati
```

### Comportamento di wait()

```
Parent esegue wait():
┌────────────────────────────────┐
│ Parent RUNNING                 │
│   ↓                            │
│ wait()                         │
│   ↓                            │
│ Parent WAITING ⏳              │ ← Bloccato
│   │                            │
│   │ (child termina)            │
│   ↓                            │
│ Parent READY → RUNNING         │ ← Risvegliato
└────────────────────────────────┘
```

### Esempio Base: wait()

```python
import os
import time

pid = os.fork()

if pid == 0:  # Child
    print("Child: inizio")
    time.sleep(2)
    print("Child: fine")
    os._exit(42)  # Exit con codice 42
else:  # Parent
    print(f"Parent: aspetto child {pid}")
    
    # wait() blocca fino a terminazione child
    terminated_pid, status = os.wait()
    
    print(f"Parent: child {terminated_pid} terminato")
    print(f"Parent: stato = {status}")
    
    # Estrarre exit code
    if os.WIFEXITED(status):
        exit_code = os.WEXITSTATUS(status)
        print(f"Parent: exit code = {exit_code}")
```

### Esempio: waitpid() con Opzioni

```python
import os
import time

pid = os.fork()

if pid == 0:  # Child
    time.sleep(3)
    os._exit(0)
else:  # Parent
    print("Parent: controllo se child è terminato (non bloccante)")
    
    # WNOHANG = ritorna immediatamente
    result, status = os.waitpid(pid, os.WNOHANG)
    
    if result == 0:
        print("Parent: child ancora in esecuzione")
        
        # Ora aspetto bloccando
        print("Parent: aspetto con waitpid bloccante")
        result, status = os.waitpid(pid, 0)
        print(f"Parent: child {result} terminato")
```

### Gestire Multipli Children

```python
import os
import time
import random

def create_children(n):
    """Crea n processi figli"""
    children = []
    
    for i in range(n):
        pid = os.fork()
        
        if pid == 0:  # Child
            # Ogni child dorme un tempo casuale
            sleep_time = random.randint(1, 5)
            print(f"Child {os.getpid()}: dormo {sleep_time}s")
            time.sleep(sleep_time)
            print(f"Child {os.getpid()}: fine")
            os._exit(i)  # Exit con codice i
        else:  # Parent
            children.append(pid)
    
    return children

def wait_all_children(children):
    """Attende tutti i children"""
    print(f"\nParent: aspetto {len(children)} children...")
    
    while children:
        # wait() attende QUALSIASI child
        pid, status = os.wait()
        
        if os.WIFEXITED(status):
            exit_code = os.WEXITSTATUS(status)
            print(f"Parent: child {pid} terminato con codice {exit_code}")
        
        children.remove(pid)
    
    print("Parent: tutti i children terminati")

# Main
children = create_children(3)
wait_all_children(children)
```

### Interpretare lo Status

```python
import os

def analyze_status(status):
    """Analizza lo status di uscita"""
    if os.WIFEXITED(status):
        # Terminazione normale
        exit_code = os.WEXITSTATUS(status)
        print(f"  Terminato normalmente con codice {exit_code}")
    
    elif os.WIFSIGNALED(status):
        # Terminato da segnale
        signal_num = os.WTERMSIG(status)
        print(f"  Terminato da segnale {signal_num}")
        
        if os.WCOREDUMP(status):
            print(f"  Core dump generato")
    
    elif os.WIFSTOPPED(status):
        # Fermato (SIGSTOP)
        signal_num = os.WSTOPSIG(status)
        print(f"  Fermato da segnale {signal_num}")

# Esempio
pid = os.fork()
if pid == 0:
    os._exit(5)
else:
    _, status = os.wait()
    analyze_status(status)
```

### Processo Zombie

Se il parent NON fa `wait()`, il child diventa **zombie**:

```python
import os
import time

pid = os.fork()

if pid == 0:  # Child
    print("Child: termino")
    os._exit(0)
else:  # Parent
    print(f"Parent: child {pid} diventerà zombie")
    print("Parent: non faccio wait(), vado a dormire")
    
    time.sleep(10)  # Child è zombie per 10 secondi
    
    # Verifica con: ps aux | grep Z
    
    # Pulizia
    os.wait()
    print("Parent: zombie rimosso")
```

---

## exit() - Terminazione

### Definizione

`exit()` e `_exit()` terminano il processo corrente.

### Differenze

```python
# os._exit(status)
# - Terminazione IMMEDIATA
# - NON esegue cleanup
# - NON chiama atexit handlers
# - NON flush dei buffer
# - Usato nei child dopo fork()

# sys.exit(status)  
# - Solleva SystemExit exception
# - Esegue cleanup (finally, atexit)
# - Flush dei buffer
# - Usato nel codice normale
```

### Esempio: _exit() vs exit()

```python
import os
import sys
import atexit

# Registra handler di cleanup
@atexit.register
def cleanup():
    print("Cleanup eseguito!")

print("Inizio programma")

pid = os.fork()

if pid == 0:  # Child
    print("Child: uso os._exit()")
    os._exit(0)  # NO cleanup, NO print
else:  # Parent
    os.wait()
    print("Parent: uso sys.exit()")
    sys.exit(0)  # Esegue cleanup, stampa "Cleanup eseguito!"
```

**Output:**
```
Inizio programma
Child: uso os._exit()
Parent: uso sys.exit()
Cleanup eseguito!
```

### Exit Status Codes

```python
# Convenzioni Unix
os._exit(0)    # Successo
os._exit(1)    # Errore generico
os._exit(2)    # Errore sintassi comando
os._exit(126)  # Comando non eseguibile
os._exit(127)  # Comando non trovato
os._exit(128+N)  # Terminato da segnale N

# Esempi
import sys

if not file_exists:
    sys.exit(1)  # Errore

if success:
    sys.exit(0)  # Successo
```

---

## getpid() e getppid() - Identificatori

### Definizione

```python
# os.getpid() - Process ID corrente
pid = os.getpid()

# os.getppid() - Parent Process ID
ppid = os.getppid()
```

### Esempio: Identificatori

```python
import os

print("=== Identificatori di Processo ===")
print(f"PID: {os.getpid()}")
print(f"PPID: {os.getppid()}")
print(f"UID: {os.getuid()}")
print(f"GID: {os.getgid()}")
print(f"EUID: {os.geteuid()}")
print(f"EGID: {os.getegid()}")

pid = os.fork()

if pid == 0:  # Child
    print("\n=== Child ===")
    print(f"PID: {os.getpid()}")
    print(f"PPID: {os.getppid()}")  # PID del parent
    os._exit(0)
else:  # Parent
    os.wait()
```

### Tracciare la Gerarchia

```python
import os

def print_hierarchy():
    """Stampa la gerarchia dei processi"""
    pid = os.getpid()
    ppid = os.getppid()
    
    print(f"Processo corrente: PID={pid}")
    print(f"Processo parent: PID={ppid}")
    
    # Legge info dal /proc
    try:
        with open(f'/proc/{ppid}/status', 'r') as f:
            for line in f:
                if line.startswith('Name:'):
                    print(f"Nome parent: {line.split()[1]}")
                    break
    except:
        pass

print_hierarchy()
```

---

## Esempi Pratici Completi

### Esempio 1: Sistema di Job Execution

```python
import os
import sys
import time

def execute_job(job_id, command, args):
    """Esegue un job in un processo separato"""
    print(f"Job {job_id}: avvio comando '{command}'")
    
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            # Reindirizza output a file
            log_file = f"job_{job_id}.log"
            fd = os.open(log_file, os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o644)
            os.dup2(fd, 1)  # stdout
            os.dup2(fd, 2)  # stderr
            os.close(fd)
            
            # Esegue comando
            os.execvp(command, [command] + args)
        except Exception as e:
            print(f"Errore: {e}", file=sys.stderr)
            os._exit(1)
    else:  # Parent
        return pid

def main():
    """Job scheduler semplice"""
    jobs = [
        ("ls", ["-l", "/tmp"]),
        ("sleep", ["2"]),
        ("echo", ["Hello", "World"]),
    ]
    
    running_jobs = {}
    
    # Avvia tutti i job
    for i, (cmd, args) in enumerate(jobs):
        pid = execute_job(i, cmd, args)
        running_jobs[pid] = i
    
    # Attende completamento
    print(f"\nAttendo {len(running_jobs)} job...")
    
    while running_jobs:
        pid, status = os.wait()
        job_id = running_jobs.pop(pid)
        
        if os.WIFEXITED(status):
            exit_code = os.WEXITSTATUS(status)
            print(f"Job {job_id} (PID {pid}): completato (exit={exit_code})")
        else:
            print(f"Job {job_id} (PID {pid}): terminato anomalamente")
    
    print("\nTutti i job completati")

if __name__ == "__main__":
    main()
```

### Esempio 2: Process Pool

```python
import os
import time
import random

class ProcessPool:
    """Pool semplice di processi worker"""
    
    def __init__(self, num_workers):
        self.num_workers = num_workers
        self.workers = []
    
    def worker(self, worker_id, tasks_queue):
        """Funzione eseguita da ogni worker"""
        while True:
            # Simula ricezione task (in realtà dovrebbe usare IPC)
            task_id = random.randint(1, 10)
            
            if task_id == 0:  # Segnale di terminazione
                break
            
            print(f"Worker {worker_id}: eseguo task {task_id}")
            time.sleep(random.uniform(0.5, 2.0))
            print(f"Worker {worker_id}: task {task_id} completato")
    
    def start(self):
        """Avvia tutti i worker"""
        for i in range(self.num_workers):
            pid = os.fork()
            
            if pid == 0:  # Child (worker)
                self.worker(i, None)
                os._exit(0)
            else:  # Parent
                self.workers.append(pid)
                print(f"Avviato worker {i} con PID {pid}")
    
    def stop(self):
        """Ferma tutti i worker"""
        print("\nFermo tutti i worker...")
        
        # Invia SIGTERM a tutti
        for pid in self.workers:
            try:
                os.kill(pid, 15)  # SIGTERM
            except:
                pass
        
        # Attende terminazione
        for pid in self.workers:
            try:
                os.waitpid(pid, 0)
                print(f"Worker {pid} terminato")
            except:
                pass

# Test
if __name__ == "__main__":
    pool = ProcessPool(num_workers=3)
    pool.start()
    
    time.sleep(5)  # Lascia lavorare
    
    pool.stop()
```

### Esempio 3: Daemon Process

```python
import os
import sys
import time

def daemonize():
    """Trasforma il processo in un daemon"""
    
    # Fork 1: separa dal parent
    pid = os.fork()
    if pid > 0:
        sys.exit(0)  # Parent esce
    
    # Diventa session leader
    os.setsid()
    
    # Fork 2: previene acquisizione controlling terminal
    pid = os.fork()
    if pid > 0:
        sys.exit(0)
    
    # Cambia directory
    os.chdir('/')
    
    # Chiudi file descriptors standard
    sys.stdout.flush()
    sys.stderr.flush()
    
    with open('/dev/null', 'r') as f:
        os.dup2(f.fileno(), sys.stdin.fileno())
    with open('/dev/null', 'a') as f:
        os.dup2(f.fileno(), sys.stdout.fileno())
        os.dup2(f.fileno(), sys.stderr.fileno())

def daemon_main():
    """Funzione principale del daemon"""
    with open('/tmp/daemon.log', 'a') as log:
        while True:
            log.write(f"Daemon alive: {time.ctime()}\n")
            log.flush()
            time.sleep(5)

if __name__ == "__main__":
    print("Avvio daemon...")
    daemonize()
    daemon_main()
```

---

## Esercizi

### Esercizio 1
Implementare una shell semplice che supporta pipe (`|`) tra comandi.

### Esercizio 2
Creare un process pool che distribuisce task tra worker processes usando code di comunicazione.

### Esercizio 3
Implementare un sistema di monitoraggio che fork() un child ogni minuto per raccogliere statistiche di sistema.

### Esercizio 4
Scrivere un programma che gestisce processi zombie e orfani in modo appropriato.

---

## Risorse Aggiuntive

- [fork() man page](https://man7.org/linux/man-pages/man2/fork.2.html)
- [exec() man page](https://man7.org/linux/man-pages/man3/exec.3.html)
- [wait() man page](https://man7.org/linux/man-pages/man2/wait.2.html)

---

**Precedente:** [04 - Modello Processi](04-modello-processi.md)  
**Prossimo:** [06 - Processi Python os](06-processi-python-os.md)
