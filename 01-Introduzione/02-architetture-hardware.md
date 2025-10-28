# Modulo 1 - Architetture Hardware per la Concorrenza

## Indice
- [Introduzione](#introduzione)
- [Processori Single-Core e Multi-Core](#processori-single-core-e-multi-core)
- [Hyperthreading e SMT](#hyperthreading-e-smt)
- [Memoria Condivisa vs Memoria Distribuita](#memoria-condivisa-vs-memoria-distribuita)
- [Gerarchia della Memoria e Cache](#gerarchia-della-memoria-e-cache)
- [Modelli di Consistenza della Memoria](#modelli-di-consistenza-della-memoria)
- [NUMA - Non-Uniform Memory Access](#numa---non-uniform-memory-access)
- [GPU e Acceleratori](#gpu-e-acceleratori)
- [Esempi Pratici](#esempi-pratici)

---

## Introduzione

L'architettura hardware è fondamentale per comprendere come i programmi concorrenti vengono eseguiti. Le caratteristiche del processore, della memoria e del sistema di interconnessione influenzano direttamente:
- Le prestazioni dei programmi concorrenti
- I problemi di sincronizzazione
- Le strategie di ottimizzazione

---

## Processori Single-Core e Multi-Core

### Processori Single-Core

```
┌─────────────────────────┐
│   Single Core CPU       │
│  ┌──────────────────┐   │
│  │  Execution Unit  │   │
│  │  (ALU, FPU, etc) │   │
│  └──────────────────┘   │
│  ┌──────────────────┐   │
│  │   L1 Cache       │   │
│  └──────────────────┘   │
│  ┌──────────────────┐   │
│  │   L2 Cache       │   │
│  └──────────────────┘   │
└─────────────────────────┘
```

**Caratteristiche:**
- ✅ Semplice da programmare
- ✅ Nessun problema di sincronizzazione hardware
- ❌ Concorrenza solo tramite time-slicing
- ❌ Impossibile parallelismo reale

**Esempio di esecuzione:**
```
Time:     0ms    10ms   20ms   30ms   40ms
Thread A: ████          ████          ████
Thread B:     ████          ████
```

### Processori Multi-Core

```
┌─────────────────────────────────────────┐
│         Multi-Core CPU                  │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  │
│  │ Core 0  │  │ Core 1  │  │ Core 2  │  │
│  │ L1 Cache│  │ L1 Cache│  │ L1 Cache│  │
│  └────┬────┘  └────┬────┘  └────┬────┘  │
│       │            │            │       │
│  ┌────┴────────────┴────────────┴────┐  │
│  │         Shared L3 Cache           │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

**Caratteristiche:**
- ✅ Parallelismo reale
- ✅ Migliore throughput
- ✅ Scalabilità
- ❌ Problemi di cache coherence
- ❌ Necessità di sincronizzazione

**Esempio di esecuzione:**
```
Time:     0ms────────────────→40ms
Core 0:   █████████████████████████
Core 1:   █████████████████████████
Core 2:   █████████████████████████
```

### Legge di Amdahl

La legge di Amdahl descrive il limite teorico dello speedup:

```
Speedup = 1 / ((1 - P) + P/N)

P = porzione parallelizzabile del codice
N = numero di processori
```

**Esempio:**
- Se il 75% del codice è parallelizzabile (P=0.75)
- Con 4 core (N=4):
```
Speedup = 1 / ((1 - 0.75) + 0.75/4)
        = 1 / (0.25 + 0.1875)
        = 1 / 0.4375
        = 2.29x
```

**Grafico dello Speedup:**
```
Speedup
  4│              ╱─── P=100%
   │            ╱
  3│          ╱
   │        ╱  ╱─── P=75%
  2│      ╱  ╱
   │    ╱  ╱─── P=50%
  1│  ╱──────── P=25%
   └────────────────→ Cores
    1  2  3  4  5  6
```

### Verificare il Numero di Core

```python
# Python
import multiprocessing
print(f"CPU count: {multiprocessing.cpu_count()}")
```

```java
// Java
int cores = Runtime.getRuntime().availableProcessors();
System.out.println("Available processors: " + cores);
```

```bash
# Linux
lscpu
nproc
cat /proc/cpuinfo | grep processor | wc -l
```

---

## Hyperthreading e SMT

### SMT - Simultaneous Multithreading

SMT (noto come Hyper-Threading in Intel) permette a un core fisico di eseguire più thread logici:

```
┌────────────────────────────────┐
│    Physical Core               │
│  ┌──────────────────────────┐  │
│  │  Execution Units         │  │
│  │  (Shared)                │  │
│  └──────────────────────────┘  │
│  ┌───────────┐  ┌───────────┐  │
│  │ Thread 0  │  │ Thread 1  │  │
│  │ Context   │  │ Context   │  │
│  │ (Regs)    │  │ (Regs)    │  │
│  └───────────┘  └───────────┘  │
└────────────────────────────────┘
```

**Come funziona:**
- Ogni thread ha il proprio set di registri
- Le unità di esecuzione sono condivise
- Quando un thread è in stallo (es. attesa memoria), l'altro può eseguire

**Esempio:**
```
Senza SMT (1 core):
Thread A: ████ idle ████ idle ████
Utilizzo: ~60%

Con SMT (1 core, 2 thread logici):
Thread A: ████      ████      ████
Thread B:      ████      ████
Utilizzo: ~90%
```

**Benefici:**
- ✅ Migliore utilizzo delle risorse del core
- ✅ Throughput aumentato del 20-30%
- ✅ Nessun costo hardware aggiuntivo significativo

**Limitazioni:**
- ❌ Non raddoppia le prestazioni
- ❌ Risorse condivise possono causare contesa
- ❌ Non è parallelismo "vero"

### Esempio Pratico

```python
import os
import psutil

# CPU fisiche
physical_cores = psutil.cpu_count(logical=False)
# CPU logiche (include hyperthreading)
logical_cores = psutil.cpu_count(logical=True)

print(f"Physical cores: {physical_cores}")
print(f"Logical cores: {logical_cores}")
print(f"Hyperthreading: {'Yes' if logical_cores > physical_cores else 'No'}")

# Output tipico su Intel i7:
# Physical cores: 4
# Logical cores: 8
# Hyperthreading: Yes
```

---

## Memoria Condivisa vs Memoria Distribuita

### Architettura a Memoria Condivisa (SMP - Symmetric Multiprocessing)

```
┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐
│ CPU0 │  │ CPU1 │  │ CPU2 │  │ CPU3 │
└───┬──┘  └───┬──┘  └───┬──┘  └───┬──┘
    │         │         │         │
    └─────────┴─────────┴─────────┘
              │
    ┌─────────┴─────────┐
    │  Shared Memory    │
    │  (RAM)            │
    └───────────────────┘
```

**Caratteristiche:**
- ✅ Comunicazione veloce (stessa memoria)
- ✅ Facile condivisione dati
- ❌ Necessità di sincronizzazione
- ❌ Scalabilità limitata
- ❌ Cache coherence overhead

**Esempio - Thread in Java:**
```java
// Memoria condivisa tra thread
class SharedCounter {
    private int count = 0;  // Condivisa!
    
    public synchronized void increment() {
        count++;
    }
    
    public synchronized int getCount() {
        return count;
    }
}
```

### Architettura a Memoria Distribuita

```
┌──────────────┐      ┌──────────────┐
│   Node 0     │      │   Node 1     │
│ ┌──────┐     │      │     ┌──────┐ │
│ │ CPU  │     │◄────►│     │ CPU  │ │
│ └──────┘     │Network│    └──────┘ │
│ ┌──────┐     │      │     ┌──────┐ │
│ │Memory│     │      │     │Memory│ │
│ └──────┘     │      │     └──────┘ │
└──────────────┘      └──────────────┘
```

**Caratteristiche:**
- ✅ Scalabilità eccellente
- ✅ Nessun problema di cache coherence
- ✅ Fault tolerance
- ❌ Comunicazione più lenta (rete)
- ❌ Programmazione più complessa

**Esempio - MPI (Message Passing Interface):**
```python
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    data = {'a': 7, 'b': 3.14}
    comm.send(data, dest=1, tag=11)
    print(f"Rank 0 sent data")
elif rank == 1:
    data = comm.recv(source=0, tag=11)
    print(f"Rank 1 received: {data}")
```

### Confronto

| Aspetto | Memoria Condivisa | Memoria Distribuita |
|---------|-------------------|---------------------|
| Latenza comunicazione | Bassa (ns) | Alta (µs-ms) |
| Scalabilità | Limitata (< 100 core) | Eccellente (migliaia) |
| Programmazione | Più semplice | Più complessa |
| Costo | Moderato | Alto |
| Sincronizzazione | Lock, semaphores | Message passing |
| Esempi | Multi-threading | Cluster, MPI |

---

## Gerarchia della Memoria e Cache

### Gerarchia della Memoria

```
┌──────────────────────────────────────┐
│ Registers (< 1ns)                    │ ← Fastest
├──────────────────────────────────────┤
│ L1 Cache (~1ns)     32-64 KB         │
├──────────────────────────────────────┤
│ L2 Cache (~3ns)     256-512 KB       │
├──────────────────────────────────────┤
│ L3 Cache (~12ns)    4-32 MB          │
├──────────────────────────────────────┤
│ RAM (~100ns)        8-128 GB         │
├──────────────────────────────────────┤
│ SSD (~100µs)        256GB-4TB        │
├──────────────────────────────────────┤
│ HDD (~10ms)         1-20 TB          │ ← Slowest
└──────────────────────────────────────┘
```

### Cache Line e False Sharing

Una **cache line** è l'unità minima di dati trasferita tra memoria e cache (tipicamente 64 byte):

```
Cache Line (64 bytes):
┌────────────────────────────────────────┐
│ Byte 0-63                              │
└────────────────────────────────────────┘
```

**Problema del False Sharing:**

```c
struct Data {
    int counter_a;  // Byte 0-3
    int counter_b;  // Byte 4-7
} data;

// Thread 1 modifica counter_a
// Thread 2 modifica counter_b
// Entrambi sono sulla stessa cache line!
```

**Effetto:**
```
Core 0:  Scrive counter_a → Invalida cache line su Core 1
Core 1:  Scrive counter_b → Invalida cache line su Core 0
Core 0:  Scrive counter_a → Invalida cache line su Core 1
...
Performance degradation!
```

**Soluzione - Cache Line Padding:**

```c
struct Data {
    int counter_a;
    char padding[60];  // Forza su cache line diverse
    int counter_b;
};
```

oppure in Java:

```java
class Counter {
    volatile long value;
    // JVM padding automatico con @Contended (Java 8+)
}
```

### Cache Coherence

Protocollo per mantenere consistenza tra cache multiple (es. MESI):

```
M - Modified:   Cache line modificata (esclusiva)
E - Exclusive:  Cache line non modificata (esclusiva)
S - Shared:     Cache line condivisa (read-only)
I - Invalid:    Cache line invalida
```

**Esempio:**
```
Core 0 legge X:   [S] X=5
Core 1 legge X:   [S] X=5  (entrambi shared)
Core 0 scrive X:  [M] X=10 (Core 0 modified)
                  [I] X     (Core 1 invalid)
Core 1 legge X:   [S] X=10 (ricarica da Core 0)
```

---

## Modelli di Consistenza della Memoria

### Sequential Consistency

Le operazioni appaiono come se eseguite in un ordine sequenziale:

```
Thread 1: Write A, Write B
Thread 2: Read B, Read A

Se Thread 2 legge B (nuovo valore), 
DEVE leggere anche A (nuovo valore)
```

### Weak/Relaxed Consistency

Le operazioni possono essere riordinate per ottimizzazione:

```java
// Possibile riordino!
int a = 0;
int b = 0;

// Thread 1
a = 1;
b = 1;

// Thread 2 potrebbe vedere:
// b=1, a=0  ← Riordino!
```

**Soluzione - Memory Barriers:**

```java
class Example {
    private volatile int value;  // Java volatile previene riordino
    
    // In C/C++
    // std::atomic<int> value;
    // value.store(42, std::memory_order_release);
}
```

---

## NUMA - Non-Uniform Memory Access

In sistemi con molti core, l'accesso alla memoria non è uniforme:

```
┌───────────────┐         ┌───────────────┐
│   Socket 0    │         │   Socket 1    │
│  ┌─────────┐  │         │  ┌─────────┐  │
│  │ 8 cores │  │◄───────►│  │ 8 cores │  │
│  └─────────┘  │  QPI/   │  └─────────┘  │
│  ┌─────────┐  │  UPI    │  ┌─────────┐  │
│  │ Local   │  │         │  │ Local   │  │
│  │ Memory  │  │         │  │ Memory  │  │
│  │ 32GB    │  │         │  │ 32GB    │  │
│  └─────────┘  │         │  └─────────┘  │
└───────────────┘         └───────────────┘
     Fast access ↑         ↑ Slow access
```

**Implicazioni:**
- Accesso a memoria locale: ~50ns
- Accesso a memoria remota: ~100ns+

**Best Practice:**
```bash
# Linux - binding processi a NUMA node
numactl --cpunodebind=0 --membind=0 ./myprogram
```

---

## GPU e Acceleratori

### Architettura GPU

```
┌────────────────────────────────────┐
│           GPU                      │
│  ┌──────┐ ┌──────┐ ┌──────┐       │
│  │ SM 0 │ │ SM 1 │ │ SM N │  ...  │
│  │(many │ │(many │ │(many │       │
│  │cores)│ │cores)│ │cores)│       │
│  └──────┘ └──────┘ └──────┘       │
│  ┌────────────────────────────┐   │
│  │    GPU Memory (VRAM)       │   │
│  │    (8-80 GB)               │   │
│  └────────────────────────────┘   │
└────────────────────────────────────┘
```

**Caratteristiche:**
- Migliaia di core semplici
- Ottimizzata per throughput, non latenza
- SIMD (Single Instruction Multiple Data)

**Esempio - CUDA:**
```cuda
__global__ void vectorAdd(float *a, float *b, float *c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

// Launch con 256 thread per blocco
vectorAdd<<<(n+255)/256, 256>>>(d_a, d_b, d_c, n);
```

---

## Esempi Pratici

### Esempio 1: Cache Line Padding in Python

```python
import threading
import time

class Counter:
    def __init__(self):
        self.value = 0
        self.padding = [0] * 15  # Force different cache lines

def worker_no_padding(counter, iterations):
    for _ in range(iterations):
        counter.value += 1

def benchmark():
    iterations = 10_000_000
    
    # Due contatori vicini (stesso cache line)
    counter1 = Counter()
    counter2 = Counter()
    
    start = time.time()
    t1 = threading.Thread(target=worker_no_padding, args=(counter1, iterations))
    t2 = threading.Thread(target=worker_no_padding, args=(counter2, iterations))
    t1.start(); t2.start()
    t1.join(); t2.join()
    
    print(f"Time: {time.time() - start:.2f}s")
```

### Esempio 2: NUMA Awareness

```python
import os

def get_numa_info():
    """Ottiene informazioni NUMA del sistema"""
    try:
        with open('/sys/devices/system/node/online', 'r') as f:
            nodes = f.read().strip()
            print(f"NUMA nodes: {nodes}")
        
        for node in range(2):  # Assumendo 2 nodi
            with open(f'/sys/devices/system/node/node{node}/cpulist', 'r') as f:
                cpus = f.read().strip()
                print(f"Node {node} CPUs: {cpus}")
    except FileNotFoundError:
        print("NUMA information not available")

get_numa_info()
```

### Esempio 3: Thread Affinity

```python
import os
import threading

def worker(cpu_id):
    # Imposta affinity (solo Linux)
    os.sched_setaffinity(0, {cpu_id})
    
    # Lavoro CPU-intensive
    result = sum(i*i for i in range(10_000_000))
    print(f"Thread on CPU {cpu_id}: {result}")

# Crea thread su CPU specifici
threads = []
for cpu in range(4):
    t = threading.Thread(target=worker, args=(cpu,))
    threads.append(t)
    t.start()

for t in threads:
    t.join()
```

---

## Esercizi

### Esercizio 1: Benchmarking Multi-Core
Scrivere un programma che misura lo speedup effettivo aumentando il numero di thread da 1 a N (numero di core).

### Esercizio 2: False Sharing Demo
Creare due versioni di un programma (con e senza padding) e misurare la differenza di prestazioni.

### Esercizio 3: NUMA Optimization
Scrivere un programma che alloca memoria e thread sullo stesso NUMA node e confrontare con allocazione casuale.

---

## Risorse Aggiuntive

- **Strumenti:**
  - `lscpu` - informazioni CPU
  - `numactl` - controllo NUMA
  - `perf` - profiling prestazioni
  - `likwid` - hardware performance monitoring

- **Documentazione:**
  - Intel Architecture Optimization Manual
  - AMD Optimization Guide
  - NUMA API documentation

---

**Precedente:** [01 - Concetti Fondamentali](01-concetti-fondamentali.md)  
**Prossimo:** [03 - Problematiche](03-problematiche.md)
