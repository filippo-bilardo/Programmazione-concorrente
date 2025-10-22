# Corso di Programmazione Concorrente

## Indice del Corso

### Modulo 1: Introduzione alla Programmazione Concorrente

📚 **Guide del Modulo:**
- [01 - Concetti Fondamentali](01-concetti-fondamentali.md)
- [02 - Architetture Hardware](02-architetture-hardware.md)
- [03 - Problematiche della Programmazione Concorrente](03-problematiche.md)

**Contenuti:**

1. **Concetti fondamentali**
   - Programmi sequenziali vs programmi concorrenti
   - Vantaggi e sfide della concorrenza
   - Parallelismo vs Concorrenza
   - Modelli di esecuzione: multitasking, multiprocessing, multithreading
   - Multitasking nella Vita Quotidiana
   - Polling e interrupt
   - Multitasking nei Microcontrollori

2. **Architetture hardware per la concorrenza**
   - Processori single-core e multi-core
   - Hyperthreading e SMT (Simultaneous Multithreading)
   - Memoria condivisa vs memoria distribuita
   - Cache e false sharing
   - NUMA (Non-Uniform Memory Access)
   - GPU e acceleratori

3. **Problematiche della programmazione concorrente**
   - Race condition
   - Deadlock
   - Starvation
   - Livelock
   - Atomicità delle operazioni
   - Priority inversion
   - Problemi classici (Produttore-Consumatore, Lettori-Scrittori, Filosofi a Cena, Barbiere Addormentato)

---

### Modulo 2: Processi nei sistemi Linux con Python

📚 **Guide del Modulo:**
- [01 - Modello Processi](02-Processi-Linux/01-modello-processi.md)
- [02 - System Call](02-Processi-Linux/02-system-call.md)
- [03 - Processi Python os](02-Processi-Linux/03-processi-python-os.md)
- [04 - Modulo multiprocessing](02-Processi-Linux/04-multiprocessing.md)
- [05 - Comunicazione IPC](02-Processi-Linux/05-ipc.md)
- [06 - Sincronizzazione](02-Processi-Linux/06-sincronizzazione.md)

💻 **Esempi pratici:** [esempi/](02-Processi-Linux/esempi/)

**Contenuti:**

1. **Il modello a processi**
   - Definizione e ciclo di vita di un processo
   - Process Control Block (PCB)
   - Stati di un processo
   - Context switching

2. **System call per la gestione dei processi**
   - `fork()` - creazione di processi
   - `exec()` - sostituzione dell'immagine del processo
   - `wait()` e `waitpid()` - sincronizzazione padre-figlio
   - `exit()` - terminazione di un processo
   - `getpid()` e `getppid()` - identificatori di processo

3. **Processi in Python con il modulo `os`**
   - `os.fork()` - creazione di processi figli
   - `os.execv()` e varianti - esecuzione di programmi
   - `os.wait()` - attesa terminazione processi figli
   - Gestione dei processi zombie e orfani
   - Esempi pratici di creazione e gestione processi

4. **Il modulo `multiprocessing` in Python**
   - Classe `Process`
   - Creazione e avvio di processi
   - `join()` e `is_alive()`
   - Pool di processi
   - Passaggio di parametri ai processi

5. **Comunicazione tra processi (IPC)**
   - **Pipe**
     - Pipe anonime con `os.pipe()`
     - `multiprocessing.Pipe()`
   - **Code (Queue)**
     - `multiprocessing.Queue()`
     - Produttore-Consumatore con code
   - **Memoria condivisa**
     - `multiprocessing.Value` e `multiprocessing.Array`
     - `multiprocessing.shared_memory`
   - **Segnali**
     - Modulo `signal` in Python
     - Gestione dei segnali comuni (SIGTERM, SIGINT, SIGUSR1, SIGUSR2)

6. **Sincronizzazione tra processi**
   - `multiprocessing.Lock`
   - `multiprocessing.Semaphore`
   - `multiprocessing.Event`
   - `multiprocessing.Condition`
   - `multiprocessing.Barrier`

---

### Modulo 3: Thread in Java

Programmazione concorrente in Java utilizzando Thread.

**Status:** ✅ Completo

📚 **Guide del Modulo:**
- [01-Introduzione Thread](03-Thread-Java/01-introduzione-thread.md) - Creazione, stati, API base, daemon, priorità
- [02-Sincronizzazione](03-Thread-Java/02-sincronizzazione.md) - `synchronized`, wait/notify, deadlock, volatile, immutabilità
- [03-Memory Model](03-Thread-Java/03-memory-model.md) - JMM, volatile, happens-before, atomicità, safe publication
- [04-Executor Framework](03-Thread-Java/04-executor-framework.md) - Pool, Callable, Future, CompletableFuture, schedulazione
- [05-Concurrent Collections](03-Thread-Java/05-concurrent-collections.md) - ConcurrentHashMap, BlockingQueue, CopyOnWriteArrayList
- [06-Lock e Atomic](03-Thread-Java/06-lock-atomic.md) - ReentrantLock, ReadWriteLock, StampedLock, AtomicInteger, CAS

💻 **Esempi pratici:** [esempi/](03-Thread-Java/esempi/)

**Contenuti:**

1. **Introduzione ai thread**
   - Thread vs Processi (spazio memoria condiviso, context switch veloce)
   - Stati thread: NEW, RUNNABLE, RUNNING, WAITING, BLOCKED, TERMINATED
   - Creazione: estendere `Thread`, implementare `Runnable`, lambda expressions
   - API base: start(), run(), join(), sleep(), interrupt()
   - Thread daemon e priorità
   - Thread groups

2. **Sincronizzazione dei thread**
   - Race condition e problemi di concorrenza
   - **Synchronized**
     - Metodi sincronizzati (su istanza o classe)
     - Blocchi sincronizzati (lock esplicito su oggetti)
     - Monitor object (intrinsic lock)
     - Rientranza (reentrant lock)
   - **Comunicazione tra thread**
     - wait() - rilascia lock e attende
     - notify() - sveglia un thread
     - notifyAll() - sveglia tutti i thread
     - Pattern Producer-Consumer
   - **Deadlock**
     - Cause: ordine lock non deterministico
     - Prevenzione: ordine fisso, tryLock(), timeout
   - **Volatile keyword**
     - Garanzie: visibilità, ordering (no reordering)
     - Limitazioni: NON garantisce atomicità per operazioni composite
   - **Atomic variables**
     - AtomicInteger, AtomicLong, AtomicBoolean, AtomicReference
     - Operazioni atomiche: incrementAndGet(), compareAndSet()
     - Compare-And-Swap (CAS) - operazione hardware lock-free
     - Performance superiori rispetto a synchronized

3. **Java Memory Model (JMM)**
   - Architettura memoria: CPU cache, main memory
   - Happens-Before relationship
   - Regole JMM: program order, monitor lock, volatile variable, thread start/join
   - Reordering e ottimizzazioni compiler/CPU
   - Atomicità operazioni (32-bit atomico, 64-bit NO senza volatile)
   - Safe publication patterns

4. **Executor Framework**
   - Interfacce: Executor, ExecutorService, ScheduledExecutorService
   - Factory methods:
     - newFixedThreadPool(n) - pool fisso
     - newCachedThreadPool() - pool dinamico
     - newSingleThreadExecutor() - esecuzione seriale
     - newScheduledThreadPool(n) - task periodici
     - newWorkStealingPool() - ForkJoinPool con work-stealing
   - Callable e Future - task con ritorno valore
   - invokeAll() e invokeAny() - esecuzione batch
   - ThreadPoolExecutor - configurazione avanzata pool
   - CompletableFuture (Java 8+) - API fluent per composizione asincrona

5. **Concurrent Collections**
   - **ConcurrentHashMap**
     - Lock segmentati (alta concorrenza)
     - Operazioni atomiche: putIfAbsent(), compute(), merge()
     - Bulk operations: forEach(), search(), reduce()
   - **CopyOnWriteArrayList/Set**
     - Copy-on-Write: ogni modifica copia array
     - Ottimizzate per read-heavy workload
     - Iterator snapshot (no ConcurrentModificationException)
   - **BlockingQueue**
     - ArrayBlockingQueue - bounded con array
     - LinkedBlockingQueue - optionally bounded
     - PriorityBlockingQueue - coda con priorità
     - DelayQueue - elementi disponibili dopo delay
     - SynchronousQueue - handoff diretto (no capacità)
   - **ConcurrentSkipListMap/Set** - collezioni ordinate thread-safe

6. **Lock espliciti**
   - **ReentrantLock**
     - tryLock() - acquisizione non bloccante
     - tryLock(timeout) - acquisizione con timeout
     - lockInterruptibly() - lock interrompibile
     - Fair vs unfair lock
   - **ReadWriteLock**
     - Lock separati: readLock() e writeLock()
     - Multipli reader O singolo writer
     - Ottimo per read-heavy workload
   - **StampedLock** (Java 8+)
     - Optimistic read - nessun lock per letture
     - Validation - controllo consistenza
     - Performance massime
   - **Condition variables**
     - Alternative a wait/notify più flessibili
     - Condition multiple per lock singolo

8. **Problemi classici di sincronizzazione**
   - Produttore-Consumatore
   - Lettori-Scrittori
   - Filosofi a cena
   - Barbiere addormentato

---

### Modulo 4: Pattern e Tecniche Avanzate
1. **Pattern di programmazione concorrente**
   - Thread Pool Pattern
   - Future Pattern
   - Producer-Consumer Pattern
   - Pipeline Pattern
   - Fork-Join Pattern
   - Actor Model

2. **Fork-Join Framework in Java**
   - Classe `ForkJoinPool`
   - `RecursiveTask` e `RecursiveAction`
   - Work-stealing algorithm
   - Esempi di parallelizzazione divide-et-impera

3. **Stream paralleli in Java**
   - Conversione stream sequenziali in paralleli
   - `parallel()` e `sequential()`
   - Operazioni parallele su collezioni
   - Prestazioni e considerazioni

4. **Programmazione asincrona**
   - `CompletableFuture` in Java
   - Composizione di operazioni asincrone
   - Gestione degli errori
   - Async/await pattern

---

### Modulo 5: Comunicazione e Sincronizzazione Avanzate
1. **Semafori**
   - Semafori binari e counting
   - Implementazione in Python (`multiprocessing.Semaphore`)
   - Implementazione in Java (`java.util.concurrent.Semaphore`)
   - Applicazioni pratiche

2. **Monitor**
   - Concetto di monitor
   - Monitor in Java (synchronized + wait/notify)
   - Condition variables

3. **Barriere**
   - `multiprocessing.Barrier` in Python
   - `CyclicBarrier` in Java
   - `CountDownLatch` in Java
   - `Phaser` in Java

4. **Message Passing**
   - Code di messaggi
   - Mailbox pattern
   - Request-Reply pattern

5. **Comunicazione tramite socket**
   - Socket TCP/IP per comunicazione tra processi
   - Client-Server concorrenti
   - Thread pool per gestione connessioni

---

### Modulo 6: Programmazione Distribuita
1. **Introduzione ai sistemi distribuiti**
   - Caratteristiche dei sistemi distribuiti
   - Modelli di comunicazione

2. **Remote Procedure Call (RPC)**
   - Concetti base
   - Esempi con gRPC
   - XML-RPC e JSON-RPC in Python

3. **Java RMI (Remote Method Invocation)**
   - Architettura RMI
   - Registry e Naming
   - Serializzazione degli oggetti
   - Esempio pratico di sistema distribuito

4. **Message Queues**
   - RabbitMQ
   - Apache Kafka (cenni)
   - ZeroMQ in Python

---

### Modulo 7: Testing e Debugging
1. **Debugging di programmi concorrenti**
   - Race condition detection
   - Deadlock detection
   - Tools di debugging (gdb, jdb)
   - Thread dump analysis

2. **Testing**
   - Unit testing di codice concorrente
   - Stress testing
   - Tools: JUnit per Java, unittest per Python
   - Coverage analysis

3. **Profiling e ottimizzazione**
   - Profiling di applicazioni multi-thread
   - Identificazione di bottleneck
   - Tools: VisualVM, JProfiler, cProfile

---

### Modulo 8: Best Practices e Anti-pattern
1. **Best practices**
   - Immutabilità
   - Thread-local storage
   - Minimize shared state
   - Lock-free programming (cenni)
   - Documentazione del codice concorrente

2. **Anti-pattern da evitare**
   - Lock troppo granulari o troppo grossolani
   - Nested locks e deadlock
   - Busy waiting
   - Priority inversion

3. **Design patterns per la concorrenza**
   - Immutable objects
   - Thread-safe singleton
   - Double-checked locking
   - Thread pool sizing

---

### Modulo 9: Progetti Pratici
1. **Progetto 1: Web Server Concorrente**
   - Server HTTP multi-threaded in Java
   - Gestione delle richieste con thread pool
   - Static file serving

2. **Progetto 2: Sistema Producer-Consumer**
   - Implementazione con processi in Python
   - Implementazione con thread in Java
   - Confronto prestazioni

3. **Progetto 3: Parallel Data Processing**
   - Elaborazione parallela di file di grandi dimensioni
   - Map-Reduce pattern
   - Fork-Join in Java

4. **Progetto 4: Chat System**
   - Server multi-client
   - Comunicazione tramite socket
   - Sincronizzazione messaggi

5. **Progetto 5: Distributed Task Queue**
   - Sistema distribuito Python-Java
   - Code di task condivise
   - Worker pools

---

### Appendici
**Appendice A: Riferimenti System Call Linux**
- Manual pages delle principali system call
- Codici di errore e gestione

**Appendice B: Java Concurrency API Reference**
- Package `java.util.concurrent`
- Package `java.util.concurrent.atomic`
- Package `java.util.concurrent.locks`

**Appendice C: Python Multiprocessing Reference**
- Modulo `multiprocessing`
- Modulo `threading`
- Modulo `concurrent.futures`

**Appendice D: Glossario**
- Terminologia della programmazione concorrente

---

### Bibliografia e Risorse
- "Java Concurrency in Practice" - Brian Goetz
- "Operating System Concepts" - Silberschatz, Galvin, Gagne
- "The Art of Multiprocessor Programming" - Herlihy, Shavit
- Documentazione ufficiale Python e Java
- Linux man pages

