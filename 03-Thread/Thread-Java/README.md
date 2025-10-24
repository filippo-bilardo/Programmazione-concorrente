# Modulo 3: Thread in Java

Questo modulo copre la programmazione multi-thread in Java, dalla creazione base fino alle API avanzate di `java.util.concurrent`.

## 📚 Guide

1. **[Introduzione ai Thread](01-introduzione-thread.md)**
   - Thread vs Processi
   - Stati di un thread (NEW, RUNNABLE, RUNNING, WAITING, BLOCKED, TERMINATED)
   - Creazione thread: estendere `Thread` vs implementare `Runnable`
   - Lambda expressions per thread
   - API `Thread`: start(), run(), join(), sleep(), interrupt()
   - Thread daemon e priorità
   - Thread groups
   - Best practices

2. **[Sincronizzazione tra Thread](02-sincronizzazione.md)**
   - Race condition e problemi di concorrenza
   - Parola chiave `synchronized` (metodi e blocchi)
   - Monitor e lock intrinseci
   - Comunicazione tra thread: wait(), notify(), notifyAll()
   - Pattern Producer-Consumer
   - Deadlock: cause e prevenzione (ordine lock, timeout, lock singolo)
   - Volatile vs Synchronized
   - Immutabilità come sincronizzazione naturale

3. **[Java Memory Model e Visibilità](03-memory-model.md)**
   - Architettura memoria multi-thread (cache, main memory)
   - Happens-Before relationship
   - Regole del Java Memory Model
   - Volatile in dettaglio (garanzie, limitazioni)
   - Final fields e immutabilità
   - Reordering e ottimizzazioni
   - Atomicità delle operazioni
   - Safe publication

4. **[Executor Framework](04-executor-framework.md)**
   - Interfacce: Executor, ExecutorService, ScheduledExecutorService
   - Factory methods: newFixedThreadPool, newCachedThreadPool, newSingleThreadExecutor
   - ScheduledExecutorService: task periodici e schedulati
   - Callable e Future (task con ritorno valore)
   - invokeAll() e invokeAny()
   - ThreadPoolExecutor: configurazione avanzata
   - CompletableFuture (Java 8+): API fluent per composizione asincrona

5. **[Concurrent Collections](05-concurrent-collections.md)**
   - ConcurrentHashMap: map thread-safe con lock segmentati
   - CopyOnWriteArrayList/Set: ottimizzati per read-heavy
   - BlockingQueue: code bloccanti (ArrayBlockingQueue, LinkedBlockingQueue, PriorityBlockingQueue, DelayQueue, SynchronousQueue)
   - ConcurrentSkipListMap/Set: collezioni ordinate thread-safe
   - Confronto performance

6. **[Lock Espliciti e Atomic Variables](06-lock-atomic.md)**
   - Lock interface e ReentrantLock
   - tryLock(), lockInterruptibly(), timeout
   - Fair vs unfair lock
   - ReadWriteLock: lock separati read/write
   - StampedLock: lock ottimistici (Java 8+)
   - Condition variables
   - Atomic variables: AtomicInteger, AtomicLong, AtomicReference
   - Compare-And-Swap (CAS)

## 💻 Esempi Pratici

La cartella [`esempi/`](esempi/) contiene esempi Java funzionanti:

### Creazione Thread
- `01_ThreadBase.java` - Estendere classe Thread
- `RunnableDemo.java` - Implementare Runnable e lambda

### Sincronizzazione
- `RaceCondition.java` - Race condition (problema)
- `SynchronizedCounter.java` - Synchronized (soluzione)
- `ProducerConsumer.java` - Pattern con wait/notify
- `DeadlockDemo.java` - Deadlock e prevenzione

### Executor Framework
- `ExecutorServiceDemo.java` - FixedThreadPool base
- `CallableFutureDemo.java` - Task con ritorno valore

### Concurrent Collections
- `ConcurrentHashMapDemo.java` - Map thread-safe
- `BlockingQueueDemo.java` - Producer-Consumer con BlockingQueue

### Lock e Atomic
- `AtomicIntegerDemo.java` - Contatore lock-free
- `ReentrantLockDemo.java` - Lock esplicito
- `ReadWriteLockDemo.java` - Separazione read/write

## 🚀 Come Compilare ed Eseguire

### Prerequisiti

```bash
# Java Development Kit 8+
java -version
javac -version
```

### Compilazione

```bash
# Compila singolo file
javac esempi/RunnableDemo.java

# Compila tutti i file
javac esempi/*.java
```

### Esecuzione

```bash
# Esegui esempio
cd esempi
java RunnableDemo

# Oppure dalla directory radice
java -cp esempi RunnableDemo
```

### Esecuzione con Package

Se gli esempi usano package:

```bash
javac -d bin esempi/*.java
java -cp bin NomeClasse
```

## 📊 Concetti Chiave

### Stati Thread

```
NEW → start() → RUNNABLE ⇄ RUNNING
                   ↓          ↓
              BLOCKED    WAITING/TIMED_WAITING
                   ↓          ↓
                → TERMINATED ←
```

### Sincronizzazione

```java
// Metodo sincronizzato
public synchronized void metodo() { ... }

// Blocco sincronizzato
synchronized(obj) {
    // sezione critica
}
```

### Producer-Consumer

```java
synchronized(buffer) {
    while (buffer.isEmpty()) {
        buffer.wait();
    }
    item = buffer.remove();
    buffer.notifyAll();
}
```

### Executor Pattern

```java
ExecutorService executor = Executors.newFixedThreadPool(10);
executor.submit(() -> { /* task */ });
executor.shutdown();
```

## 🔧 Best Practices

### Thread Safety

1. **Immutabilità quando possibile**
   ```java
   public final class ImmutablePoint {
       private final int x, y;
       // nessun setter
   }
   ```

2. **Synchronized per sezioni critiche**
   ```java
   public synchronized void incrementa() {
       counter++;
   }
   ```

3. **Usa concurrent collections**
   ```java
   Map<K,V> map = new ConcurrentHashMap<>();
   BlockingQueue<E> queue = new LinkedBlockingQueue<>();
   ```

4. **Preferisci Runnable a Thread**
   ```java
   Thread t = new Thread(() -> { /* task */ });
   ```

5. **Usa ExecutorService per pool**
   ```java
   ExecutorService executor = Executors.newFixedThreadPool(cores);
   ```

### Evitare Problemi

1. **Sempre wait() in loop**
   ```java
   while (!condizione) {
       obj.wait();
   }
   ```

2. **Ordine fisso acquisizione lock**
   ```java
   synchronized(lockA) {
       synchronized(lockB) { ... }
   }
   ```

3. **Finally con unlock()**
   ```java
   lock.lock();
   try { ... } finally { lock.unlock(); }
   ```

4. **Volatile per flag semplici**
   ```java
   private volatile boolean running = true;
   ```

## 📖 Confronti

### Thread Creation

| Approccio | Pro | Contro |
|-----------|-----|--------|
| Estendere Thread | Semplice | No ereditarietà multipla |
| Implementare Runnable | Flessibile | Più verboso |
| Lambda | Conciso | Solo Java 8+ |

### Sincronizzazione

| Meccanismo | Pro | Contro | Uso |
|------------|-----|--------|-----|
| synchronized | Semplice | Meno flessibile | Default |
| ReentrantLock | Flessibile, tryLock | Verboso | Timeout, interruption |
| ReadWriteLock | Performance read | Overhead | Read-heavy |
| Atomic | Lock-free | Solo operazioni semplici | Contatori |

### Executor Types

| Tipo | Thread | Queue | Uso |
|------|--------|-------|-----|
| FixedThreadPool | n fisso | Unbounded | Carico costante |
| CachedThreadPool | Dinamico | SynchronousQueue | Burst |
| SingleThreadExecutor | 1 | Unbounded | Seriale |
| ScheduledThreadPool | n fisso | DelayQueue | Periodico |

## 🎯 Esercizi Proposti

1. **Thread Monitor**: Crea un monitor che stampa statistiche su thread attivi ogni secondo

2. **Parallel Sorter**: Implementa un merge sort parallelo usando ExecutorService

3. **Connection Pool**: Crea un pool di connessioni thread-safe con Semaphore

4. **Cache LRU**: Implementa cache LRU thread-safe con ReentrantReadWriteLock

5. **Rate Limiter**: Implementa un rate limiter con AtomicInteger e timestamps

6. **Fork-Join Sum**: Usa ForkJoinPool per sommare array di milioni di elementi

## ⚠️ Problemi Comuni

### Race Condition
```java
// PROBLEMA
counter++;  // NON atomico!

// SOLUZIONE
synchronized(this) { counter++; }
// oppure
AtomicInteger counter;
counter.incrementAndGet();
```

### Deadlock
```java
// PROBLEMA
Thread1: lock(A) → lock(B)
Thread2: lock(B) → lock(A)

// SOLUZIONE: Ordine fisso
Thread1: lock(A) → lock(B)
Thread2: lock(A) → lock(B)
```

### Memory Visibility
```java
// PROBLEMA
boolean flag = false;  // Non visibile tra thread

// SOLUZIONE
volatile boolean flag = false;
```

### Dimenticare Shutdown
```java
// PROBLEMA
executor.submit(task);  // JVM non termina!

// SOLUZIONE
executor.shutdown();
executor.awaitTermination(60, TimeUnit.SECONDS);
```

## 📚 Risorse Aggiuntive

### Documentazione Oracle
- [Java Concurrency Tutorial](https://docs.oracle.com/javase/tutorial/essential/concurrency/)
- [java.util.concurrent API](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/package-summary.html)

### Libri
- "Java Concurrency in Practice" - Brian Goetz
- "Effective Java" (Item 66-84) - Joshua Bloch
- "The Art of Multiprocessor Programming" - Maurice Herlihy

### Tool
- VisualVM - Thread monitoring
- JConsole - JMX monitoring
- Thread Dump Analyzer - Deadlock detection

## 🔗 Collegamenti Rapidi

- **[← Torna al Corso Completo](../README.md)**
- **[Modulo 2: Processi Linux →](../02-Processi-Linux/README.md)**

---

**Nota**: Gli esempi richiedono Java 8 o superiore. Per funzionalità avanzate (CompletableFuture, StampedLock) si consiglia Java 11+.
