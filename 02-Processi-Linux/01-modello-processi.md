# Modulo 2 - Il Modello a Processi

## Indice
- [Introduzione](#introduzione)
- [Definizione di Processo](#definizione-di-processo)
- [Ciclo di Vita di un Processo](#ciclo-di-vita-di-un-processo)
- [Process Control Block (PCB)](#process-control-block-pcb)
- [Stati di un Processo](#stati-di-un-processo)
- [Context Switching](#context-switching)
- [Gerarchia dei Processi](#gerarchia-dei-processi)
- [Esempi Pratici](#esempi-pratici)

---

## Introduzione

Un **processo** è un'istanza di un programma in esecuzione. È l'unità fondamentale di esecuzione nei sistemi operativi moderni. Mentre un programma è un'entità statica (file su disco), un processo è un'entità dinamica con risorse allocate e stato di esecuzione.

### Programma vs Processo

```
┌─────────────────────────────────────────┐
│ Programma (file su disco)               │
│ - Codice sorgente o eseguibile          │
│ - Istruzioni statiche                   │
│ - Nessuna esecuzione                    │
└─────────────────────────────────────────┘
                  │
                  │ caricamento in memoria
                  ↓
┌─────────────────────────────────────────┐
│ Processo (in memoria)                   │
│ - Codice in esecuzione                  │
│ - Dati e stack                          │
│ - Risorse (file, memoria, CPU)          │
│ - Stato di esecuzione                   │
└─────────────────────────────────────────┘
```

**Esempio:**
```
Programma: /usr/bin/firefox (file su disco)
Processo: Firefox in esecuzione con PID 1234

Puoi avere:
- 1 programma → molti processi (Firefox aperto 3 volte)
- Stesso programma → processi diversi (ogni finestra è indipendente)
```

---

## Definizione di Processo

### Componenti di un Processo

Un processo è composto da:

```
┌────────────────────────────────────────┐
│         Spazio di Indirizzamento       │
├────────────────────────────────────────┤
│ Text (Codice)                          │ ← Istruzioni del programma
│ - Codice eseguibile                    │
│ - Read-only                            │
├────────────────────────────────────────┤
│ Data                                   │ ← Variabili globali
│ - Variabili globali inizializzate      │
│ - Variabili globali non inizializzate  │
├────────────────────────────────────────┤
│ Heap (crescita →)                      │ ← Memoria dinamica
│ - malloc(), new                        │
│ - Gestita dal programmatore            │
├────────────────────────────────────────┤
│        ...                             │
├────────────────────────────────────────┤
│ Stack (crescita ←)                     │ ← Chiamate funzioni
│ - Variabili locali                     │
│ - Parametri funzioni                   │
│ - Indirizzi di ritorno                 │
└────────────────────────────────────────┘
```

### Attributi di un Processo

```python
# Visualizzare informazioni processo in Python
import os
import psutil

pid = os.getpid()
processo = psutil.Process(pid)

print(f"PID: {processo.pid}")
print(f"Nome: {processo.name()}")
print(f"Stato: {processo.status()}")
print(f"Parent PID: {processo.ppid()}")
print(f"Memoria: {processo.memory_info().rss / 1024**2:.2f} MB")
print(f"CPU: {processo.cpu_percent()}%")
print(f"Thread: {processo.num_threads()}")
print(f"File aperti: {len(processo.open_files())}")
```

---

## Ciclo di Vita di un Processo

### Fasi del Ciclo di Vita

```
┌─────────┐
│  NEW    │  Processo creato ma non ancora schedulato
└────┬────┘
     │ admitted
     ↓
┌─────────┐
│ READY   │  Pronto per eseguire, in attesa CPU
└────┬────┘
     │ dispatch (scheduler)
     ↓
┌─────────┐
│RUNNING  │  In esecuzione sulla CPU
└─┬─┬─┬─┬─┘
  │ │ │ │
  │ │ │ └─→ terminate → [TERMINATED]
  │ │ │
  │ │ └─→ I/O or event wait → [WAITING]
  │ │
  │ └─→ interrupt (timer) → [READY]
  │
  └─→ exit → [TERMINATED]

┌─────────┐
│ WAITING │  In attesa di I/O o evento
└────┬────┘
     │ I/O completion / event
     ↓
   [READY]

┌──────────┐
│TERMINATED│ Processo terminato
└──────────┘
```

### Descrizione degli Stati

#### 1. NEW (Nuovo)
Il processo è appena stato creato ma non ancora pronto per l'esecuzione.

```python
import os

# Fork crea un nuovo processo
pid = os.fork()
# Nel momento del fork, il child è in stato NEW
```

#### 2. READY (Pronto)
Il processo è pronto per eseguire e attende di essere assegnato alla CPU.

```
Ready Queue:
┌───────────────────────────────────┐
│ [P1] → [P3] → [P5] → [P2] → [P7]  │
└───────────────────────────────────┘
        ↓
    Scheduler sceglie il prossimo
```

#### 3. RUNNING (In esecuzione)
Il processo sta eseguendo istruzioni sulla CPU.

```
CPU:
┌─────────────┐
│   P1 ████   │  ← Processo in esecuzione
└─────────────┘
```

Solo un processo per CPU core può essere in RUNNING contemporaneamente.

#### 4. WAITING/BLOCKED (In attesa)
Il processo attende il completamento di un'operazione (I/O, evento, risorsa).

```
Waiting for:
- I/O (disco, rete)
- Input utente
- Segnale
- Lock/Semaforo
- Sleep/Timer

Example:
P1 → read() → WAITING
     ⏳ aspetta I/O disco
     I/O complete → READY
```

#### 5. TERMINATED (Terminato)
Il processo ha completato l'esecuzione o è stato terminato.

```python
# Terminazione normale
sys.exit(0)

# Terminazione con errore
sys.exit(1)

# Terminazione forzata
os.kill(pid, signal.SIGKILL)
```

### Esempio Pratico: Vita di un Processo

```python
import os
import time

print("1. NEW: Processo principale in esecuzione")
print(f"   PID: {os.getpid()}")

# Fork: crea un nuovo processo
pid = os.fork()

if pid == 0:  # Child process
    print("2. READY: Child process pronto")
    print(f"   Child PID: {os.getpid()}")
    
    print("3. RUNNING: Child esegue calcolo")
    result = sum(range(1000000))
    
    print("4. WAITING: Child legge da file")
    time.sleep(2)  # Simula I/O
    
    print("5. READY → RUNNING: I/O completato")
    print(f"   Risultato: {result}")
    
    print("6. TERMINATED: Child termina")
    os._exit(0)
    
else:  # Parent process
    print(f"   Parent ha creato child con PID: {pid}")
    print("   Parent attende child...")
    
    os.waitpid(pid, 0)  # Parent va in WAITING
    print("   Child terminato, parent riprende")
```

---

## Process Control Block (PCB)

### Definizione

Il **PCB** (Process Control Block) è una struttura dati del sistema operativo che contiene tutte le informazioni necessarie per gestire un processo.

```
┌────────────────────────────────────────┐
│     Process Control Block (PCB)        │
├────────────────────────────────────────┤
│ Process ID (PID)              : 1234   │
│ Parent PID (PPID)             : 1000   │
│ User ID (UID)                 : 1001   │
│ Group ID (GID)                : 1001   │
├────────────────────────────────────────┤
│ Stato del Processo            : READY  │
│ Program Counter (PC)          : 0x4000 │
│ Registri CPU                  : {...}  │
│ Stack Pointer                 : 0xFF00 │
├────────────────────────────────────────┤
│ Priorità                      : 20     │
│ Tempo CPU utilizzato          : 150ms  │
│ Quantum rimanente             : 50ms   │
├────────────────────────────────────────┤
│ Memoria allocata              : 4 MB   │
│ Indirizzo base                : 0x1000 │
│ Limite                        : 0x5000 │
├────────────────────────────────────────┤
│ File aperti                   : [0,1,2]│
│ Working directory             : /home  │
│ Variabili ambiente            : {...}  │
└────────────────────────────────────────┘
```

### Informazioni Contenute nel PCB

#### 1. Identificazione del Processo
```bash
# Visualizzare PID
echo $$         # PID della shell corrente
ps -p $$        # Info sul processo

# In Python
import os
print(f"PID: {os.getpid()}")
print(f"PPID: {os.getppid()}")
print(f"UID: {os.getuid()}")
print(f"GID: {os.getgid()}")
```

#### 2. Stato del Processore
- **Program Counter**: prossima istruzione da eseguire
- **Registri**: contenuto di tutti i registri CPU
- **Stack Pointer**: top dello stack

#### 3. Informazioni di Scheduling
- Priorità del processo
- Puntatori alle code di scheduling
- Tempo CPU utilizzato
- Quantum rimanente

#### 4. Gestione della Memoria
- Indirizzo base e limite
- Tabelle delle pagine
- Segmenti allocati

#### 5. Informazioni di I/O
- File aperti (file descriptors)
- Directory corrente
- Dispositivi allocati

### Visualizzare il PCB in Linux

```bash
# /proc filesystem contiene info PCB
cat /proc/self/status        # Info processo corrente
cat /proc/1234/status        # Info processo PID 1234

# Informazioni dettagliate
ls -la /proc/self/           # Tutti i file di info
cat /proc/self/stat          # Statistiche
cat /proc/self/maps          # Mappa memoria
cat /proc/self/fd/           # File descriptors
```

```python
# Python: leggere info PCB
import os

with open(f'/proc/{os.getpid()}/status', 'r') as f:
    print(f.read())
```

---

## Stati di un Processo

### Diagramma degli Stati (Dettagliato)

```
                    ┌──────────────┐
                    │     NEW      │
                    └──────┬───────┘
                           │ fork()
                           ↓
    ┌──────────────────────────────────────┐
    │              READY                   │
    │   (Ready Queue)                      │
    └──────┬─────────────────────────┬─────┘
           │                         ↑
           │ dispatch                │
           │ (scheduler)             │ preempt
           ↓                         │ (timer)
    ┌──────────────────────────────────────┐
    │            RUNNING                   │
    │   (sulla CPU)                        │
    └──┬───────────────────────────────┬───┘
       │                               │
       │ I/O request                   │ exit()
       │ wait()                        │
       │ sleep()                       │
       ↓                               ↓
    ┌──────────────────┐        ┌──────────────┐
    │    WAITING       │        │  TERMINATED  │
    │   (I/O Queue)    │        │   (Zombie)   │
    └──────┬───────────┘        └──────────────┘
           │
           │ I/O complete
           │ signal
           ↓
       [READY]
```

### Transizioni di Stato

| Transizione | Causa | Esempio |
|-------------|-------|---------|
| NEW → READY | Ammissione | fork() completa |
| READY → RUNNING | Dispatch | Scheduler sceglie processo |
| RUNNING → READY | Preemption | Timer interrupt |
| RUNNING → WAITING | Richiesta I/O | read(), sleep() |
| WAITING → READY | I/O completo | Dati disponibili |
| RUNNING → TERMINATED | Uscita | exit(), return |

### Esempio con Codice Python

```python
import os
import time
import signal

def monitor_state(description):
    """Monitora e stampa lo stato del processo"""
    pid = os.getpid()
    with open(f'/proc/{pid}/status', 'r') as f:
        for line in f:
            if line.startswith('State:'):
                print(f"{description}: {line.strip()}")
                break

print("=== Ciclo di vita del processo ===")
monitor_state("1. RUNNING - Inizio")

# Simula WAITING (I/O bound)
print("\n2. Entrando in WAITING (sleep)...")
time.sleep(1)
monitor_state("   Dopo sleep")

# Simula CPU intensive (RUNNING)
print("\n3. RUNNING (CPU intensive)...")
result = sum(i**2 for i in range(1000000))
monitor_state("   Dopo calcolo")

# Fork: crea child (NEW → READY)
print("\n4. Creazione child process...")
pid = os.fork()

if pid == 0:  # Child
    monitor_state("   Child READY/RUNNING")
    time.sleep(2)
    monitor_state("   Child dopo sleep")
    os._exit(0)  # TERMINATED
else:  # Parent
    print(f"   Child PID: {pid}")
    monitor_state("   Parent WAITING (wait)")
    os.waitpid(pid, 0)  # Parent va in WAITING
    monitor_state("   Parent dopo wait")
    print("\n5. TERMINATED - Fine")
```

---

## Context Switching

### Definizione

Il **context switching** è il processo di salvare lo stato di un processo e caricare lo stato di un altro processo sulla CPU.

### Perché è Necessario?

1. **Multitasking**: Permettere a più processi di "condividere" la CPU
2. **Interruzioni**: Gestire eventi hardware/software
3. **Scheduling**: Implementare politiche di scheduling (round-robin, priority, ecc.)

### Fasi del Context Switch

```
Processo A in esecuzione
         │
         │ 1. Interrupt / System Call
         ↓
    ┌─────────────────────────┐
    │ SALVA contesto di A     │
    │ - Registri              │
    │ - Program Counter       │
    │ - Stack Pointer         │
    │ - Stato                 │
    └─────────────────────────┘
         │
         │ 2. Kernel mode
         ↓
    ┌─────────────────────────┐
    │ SCHEDULER               │
    │ - Sceglie processo B    │
    │ - Aggiorna code         │
    └─────────────────────────┘
         │
         │ 3. Cambio contesto
         ↓
    ┌─────────────────────────┐
    │ CARICA contesto di B    │
    │ - Registri              │
    │ - Program Counter       │
    │ - Stack Pointer         │
    │ - Stato                 │
    └─────────────────────────┘
         │
         │ 4. Ritorno user mode
         ↓
Processo B riprende esecuzione
```

### Overhead del Context Switch

Il context switch ha un **costo** in termini di:

```
Tempo context switch tipico: 1-10 microseconds

Componenti del tempo:
┌─────────────────────────────────────┐
│ Salvataggio registri        : 20%   │
│ Aggiornamento PCB           : 15%   │
│ Cambio tabelle memoria      : 25%   │
│ Invalidazione cache         : 30%   │
│ Caricamento nuovo contesto  : 10%   │
└─────────────────────────────────────┘

Cache miss dopo context switch:
CPU senza cache: nessun problema
CPU con cache: 
  - L1 cache invalidata
  - Working set da ricaricare
  - Performance degradation temporaneo
```

### Quando Avviene il Context Switch?

1. **Timer Interrupt**: Quantum scaduto (time-sharing)
2. **I/O Request**: Processo va in WAITING
3. **System Call**: Richiesta al kernel
4. **Segnali**: SIGSTOP, SIGCONT
5. **Priorità**: Processo ad alta priorità diventa ready

### Misurare il Context Switch

```python
import os
import time

def measure_context_switches():
    """Misura i context switch del processo"""
    pid = os.getpid()
    
    def get_switches():
        with open(f'/proc/{pid}/status', 'r') as f:
            for line in f:
                if 'ctxt_switches' in line or 'voluntary_ctxt_switches' in line:
                    print(line.strip())
    
    print("Context switches all'inizio:")
    get_switches()
    
    # Attività che causa molti context switch
    print("\nEsecuzione attività...")
    for _ in range(100):
        time.sleep(0.001)  # Causa context switch (I/O wait)
    
    print("\nContext switches alla fine:")
    get_switches()

measure_context_switches()
```

### Minimizzare il Context Switch

**Best Practices:**

```python
# BAD: Molti context switch
for i in range(1000):
    time.sleep(0.001)  # 1000 context switch!

# GOOD: Un solo context switch
time.sleep(1)  # 1 context switch

# BAD: Molte system call
for i in range(1000):
    os.write(fd, b'x')  # 1000 context switch

# GOOD: Buffering
data = b'x' * 1000
os.write(fd, data)  # 1 context switch
```

---

## Gerarchia dei Processi

### Relazione Padre-Figlio

In Linux, tutti i processi sono organizzati in una **gerarchia ad albero**:

```
init (PID 1) / systemd
    │
    ├── sshd (PID 1234)
    │   ├── bash (PID 1500)
    │   │   ├── python (PID 1600)
    │   │   └── ls (PID 1601)
    │   └── bash (PID 1501)
    │
    ├── apache2 (PID 2000)
    │   ├── worker (PID 2100)
    │   ├── worker (PID 2101)
    │   └── worker (PID 2102)
    │
    └── cron (PID 3000)
        └── backup.sh (PID 3100)
```

### Visualizzare la Gerarchia

```bash
# Albero dei processi
pstree
pstree -p  # Con PID
pstree -u  # Con user

# Visualizzazione gerarchica
ps auxf
ps -eo pid,ppid,cmd --forest

# Parent di un processo
ps -p $$ -o ppid=
```

```python
# Python: visualizzare gerarchia
import os
import psutil

def print_process_tree(pid, indent=0):
    """Stampa albero processi"""
    try:
        p = psutil.Process(pid)
        print("  " * indent + f"├─ {p.name()} (PID: {p.pid})")
        
        for child in p.children():
            print_process_tree(child.pid, indent + 1)
    except:
        pass

print("Albero processi:")
print_process_tree(os.getpid())
```

### Proprietà Ereditate

Quando un processo crea un figlio con `fork()`, il figlio eredita:

```python
import os

# Proprietà ereditate dal parent:
# ✓ User ID (UID) e Group ID (GID)
# ✓ Variabili d'ambiente
# ✓ Directory corrente
# ✓ File descriptors aperti
# ✓ Signal handlers
# ✓ Priorità e nice value

pid = os.fork()
if pid == 0:  # Child
    print(f"Child UID: {os.getuid()}")  # Uguale al parent
    print(f"Child CWD: {os.getcwd()}")  # Uguale al parent
    print(f"Child ENV: {os.environ.get('HOME')}")  # Ereditato
    os._exit(0)
```

### Processi Orfani e Zombie

#### Processo Orfano
Parent termina prima del child:

```python
import os
import time

pid = os.fork()
if pid == 0:  # Child
    print(f"Child: Parent PID = {os.getppid()}")
    time.sleep(2)
    print(f"Child: Nuovo Parent PID = {os.getppid()}")  # Ora è init/systemd
    os._exit(0)
else:  # Parent
    print(f"Parent: termino subito")
    # Parent termina, child diventa orfano
```

**Risultato**: Il processo orfano viene "adottato" da init (PID 1).

#### Processo Zombie
Child termina ma parent non fa `wait()`:

```python
import os
import time

pid = os.fork()
if pid == 0:  # Child
    print("Child: termino")
    os._exit(0)
else:  # Parent
    print(f"Parent: child {pid} è zombie")
    time.sleep(5)  # Non fa wait() → child diventa zombie
    # os.waitpid(pid, 0)  # Questo rimuoverebbe lo zombie
```

**Visualizzare zombie:**
```bash
ps aux | grep Z  # Stato Z = zombie
```

---

## Esempi Pratici

### Esempio 1: Monitorare Stati di un Processo

```python
import os
import time
import psutil

def monitor_process():
    """Monitora gli stati di un processo"""
    pid = os.getpid()
    p = psutil.Process(pid)
    
    print(f"PID: {pid}")
    print(f"Nome: {p.name()}")
    
    # Informazioni iniziali
    print("\n=== Stato iniziale ===")
    print(f"Stato: {p.status()}")
    print(f"CPU%: {p.cpu_percent(interval=0.1)}")
    print(f"Memoria: {p.memory_info().rss / 1024**2:.2f} MB")
    print(f"Thread: {p.num_threads()}")
    print(f"Context switches: {p.num_ctx_switches()}")
    
    # CPU intensive
    print("\n=== CPU intensive ===")
    start = time.time()
    result = sum(i**2 for i in range(10**7))
    print(f"Tempo: {time.time() - start:.2f}s")
    print(f"CPU%: {p.cpu_percent()}")
    
    # I/O intensive
    print("\n=== I/O intensive ===")
    time.sleep(1)
    print(f"Stato: {p.status()}")
    print(f"Context switches: {p.num_ctx_switches()}")

if __name__ == "__main__":
    monitor_process()
```

### Esempio 2: Creare e Gestire Gerarchia

```python
import os
import time
import signal

def child_process(child_num):
    """Funzione eseguita dal child"""
    print(f"Child {child_num}: PID={os.getpid()}, PPID={os.getppid()}")
    time.sleep(5)
    print(f"Child {child_num}: terminato")
    os._exit(0)

def main():
    """Crea gerarchia di processi"""
    print(f"Parent: PID={os.getpid()}")
    
    # Crea 3 child processes
    children = []
    for i in range(3):
        pid = os.fork()
        if pid == 0:  # Child
            child_process(i)
        else:  # Parent
            children.append(pid)
            print(f"Parent: creato child {pid}")
    
    # Parent attende tutti i children
    print("Parent: attendo children...")
    for pid in children:
        os.waitpid(pid, 0)
        print(f"Parent: child {pid} terminato")
    
    print("Parent: tutti i children terminati")

if __name__ == "__main__":
    main()
```

### Esempio 3: Context Switch Measurement

```python
import os
import time

def get_context_switches():
    """Legge il numero di context switch"""
    pid = os.getpid()
    with open(f'/proc/{pid}/status', 'r') as f:
        for line in f:
            if 'voluntary_ctxt_switches' in line:
                vol = int(line.split()[1])
            if 'nonvoluntary_ctxt_switches' in line:
                nonvol = int(line.split()[1])
    return vol, nonvol

def test_cpu_bound():
    """Test CPU-bound (pochi context switch)"""
    print("Test CPU-bound:")
    vol1, nonvol1 = get_context_switches()
    
    # CPU intensive
    result = sum(i**2 for i in range(10**7))
    
    vol2, nonvol2 = get_context_switches()
    print(f"  Voluntary: {vol2 - vol1}")
    print(f"  Non-voluntary: {nonvol2 - nonvol1}")

def test_io_bound():
    """Test I/O-bound (molti context switch)"""
    print("\nTest I/O-bound:")
    vol1, nonvol1 = get_context_switches()
    
    # I/O intensive
    for _ in range(100):
        time.sleep(0.001)
    
    vol2, nonvol2 = get_context_switches()
    print(f"  Voluntary: {vol2 - vol1}")
    print(f"  Non-voluntary: {nonvol2 - nonvol1}")

if __name__ == "__main__":
    test_cpu_bound()
    test_io_bound()
```

---

## Esercizi

### Esercizio 1
Scrivere un programma che crea 5 processi figli e mostra la gerarchia con PID e PPID.

### Esercizio 2
Implementare un monitor che visualizza in tempo reale lo stato di un processo (stato, CPU%, memoria, context switch).

### Esercizio 3
Creare un processo zombie intenzionalmente e poi "pulirlo" con wait().

### Esercizio 4
Misurare il tempo di context switch del sistema creando molti processi che si alternano.

---

## Risorse Aggiuntive

- [Linux Process Management](https://www.kernel.org/doc/html/latest/admin-guide/process-management.html)
- [/proc filesystem documentation](https://www.kernel.org/doc/Documentation/filesystems/proc.txt)
- [psutil documentation](https://psutil.readthedocs.io/)

---

**Precedente:** [03 - Problematiche](03-problematiche.md)  
**Prossimo:** [05 - System Call](05-system-call.md)
