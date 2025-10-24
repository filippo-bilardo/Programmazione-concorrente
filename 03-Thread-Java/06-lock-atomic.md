# 6. Lock Espliciti e Atomic Variables

## java.util.concurrent.locks

Il package `java.util.concurrent.locks` fornisce meccanismi di lock **espliciti** più flessibili di `synchronized`.

### Perché Lock Espliciti?

`synchronized` ha limitazioni:

1. **Non interrompibile**: Thread bloccato non può essere interrotto
2. **No timeout**: Attesa infinita per il lock
3. **No try-lock**: Non si può tentare acquisizione senza bloccare
4. **Fairness**: Nessuna garanzia FIFO
5. **Un solo lock per blocco**: Non si possono avere lock read/write separati

**Soluzione:** `Lock` interface e implementazioni

## Lock Interface

```java
public interface Lock {
    void lock();                  // Acquisisce lock (blocca)
    void lockInterruptibly();     // Acquisisce lock (interrompibile)
    boolean tryLock();            // Tenta acquisizione (non blocca)
    boolean tryLock(long time, TimeUnit unit);  // Con timeout
    void unlock();                // Rilascia lock
    Condition newCondition();     // Crea condition variable
}
```

## ReentrantLock

Lock **rientrante** (come `synchronized`): thread può riacquisire lock che già possiede.

### Uso Base

```java
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ContatoreLock {
    private int valore = 0;
    private Lock lock = new ReentrantLock();
    
    public void incrementa() {
        lock.lock();  // Acquisisce lock
        try {
            valore++;
        } finally {
            lock.unlock();  // SEMPRE in finally!
        }
    }
    
    public int getValore() {
        lock.lock();
        try {
            return valore;
        } finally {
            lock.unlock();
        }
    }
}
```

**⚠️ Pattern Critico:**
```java
lock.lock();
try {
    // sezione critica
} finally {
    lock.unlock();  // Garantisce rilascio anche con exception
}
```

### tryLock(): Acquisizione Non Bloccante

```java
public class TransferDenaro {
    private final Lock lock = new ReentrantLock();
    private int saldo = 1000;
    
    public boolean trasferisci(TransferDenaro destinazione, int importo) {
        // Tenta acquisire entrambi i lock senza bloccare
        if (this.lock.tryLock()) {
            try {
                if (destinazione.lock.tryLock()) {
                    try {
                        if (this.saldo >= importo) {
                            this.saldo -= importo;
                            destinazione.saldo += importo;
                            return true;
                        }
                    } finally {
                        destinazione.lock.unlock();
                    }
                }
            } finally {
                this.lock.unlock();
            }
        }
        return false;  // Non riuscito ad acquisire lock
    }
}
```

**Vantaggi:**
- **Evita deadlock**: Se non riesce, rilascia e ritenta
- **Responsiveness**: Non blocca indefinitamente

### tryLock(timeout): Acquisizione con Timeout

```java
import java.util.concurrent.TimeUnit;

public class ContoCorrente {
    private final Lock lock = new ReentrantLock();
    private int saldo = 0;
    
    public boolean deposita(int importo, long timeout) {
        try {
            if (lock.tryLock(timeout, TimeUnit.SECONDS)) {
                try {
                    saldo += importo;
                    return true;
                } finally {
                    lock.unlock();
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        return false;  // Timeout scaduto
    }
}
```

### lockInterruptibly(): Lock Interrompibile

```java
public class TaskCancellabile {
    private final Lock lock = new ReentrantLock();
    
    public void eseguiTask() throws InterruptedException {
        lock.lockInterruptibly();  // Lancia InterruptedException se interrotto
        try {
            // Lavoro lungo...
            for (int i = 0; i < 1000000; i++) {
                if (Thread.currentThread().isInterrupted()) {
                    throw new InterruptedException();
                }
                // processo...
            }
        } finally {
            lock.unlock();
        }
    }
}
```

### Fairness

```java
// Lock con fairness: thread acquisiscono in ordine FIFO
Lock fairLock = new ReentrantLock(true);  // Fair

// Lock senza fairness (default): performance migliori ma no garanzie ordine
Lock unfairLock = new ReentrantLock(false);  // Unfair (default)
```

**Trade-off:**
- **Fair**: ✅ Nessuna starvation, ❌ Performance inferiori (~10x più lento)
- **Unfair**: ✅ Performance migliori, ❌ Possibile starvation

### Metodi di Diagnostica

```java
ReentrantLock lock = new ReentrantLock();

lock.lock();
lock.lock();  // Rientrante

System.out.println("Hold count: " + lock.getHoldCount());  // 2
System.out.println("Is locked: " + lock.isLocked());       // true
System.out.println("Is held by me: " + lock.isHeldByCurrentThread());  // true
System.out.println("Queue length: " + lock.getQueueLength());  // Thread in attesa

lock.unlock();
lock.unlock();
```

## ReadWriteLock

Lock con **separazione read/write**: multipli reader O un singolo writer.

```java
public interface ReadWriteLock {
    Lock readLock();   // Lock condiviso (multipli reader)
    Lock writeLock();  // Lock esclusivo (singolo writer)
}
```

### ReentrantReadWriteLock

```java
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Cache {
    private final Map<String, Object> map = new HashMap<>();
    private final ReadWriteLock rwLock = new ReentrantReadWriteLock();
    private final Lock readLock = rwLock.readLock();
    private final Lock writeLock = rwLock.writeLock();
    
    // Read (multipli thread concorrenti)
    public Object get(String key) {
        readLock.lock();
        try {
            return map.get(key);
        } finally {
            readLock.unlock();
        }
    }
    
    // Write (esclusivo)
    public void put(String key, Object value) {
        writeLock.lock();
        try {
            map.put(key, value);
        } finally {
            writeLock.unlock();
        }
    }
    
    // Read-then-Write (upgrade)
    public Object computeIfAbsent(String key, Function<String, Object> mappingFunction) {
        readLock.lock();
        try {
            Object value = map.get(key);
            if (value != null) {
                return value;
            }
        } finally {
            readLock.unlock();
        }
        
        // Upgrade a write lock
        writeLock.lock();
        try {
            // Double-check (altro thread potrebbe aver inserito)
            Object value = map.get(key);
            if (value == null) {
                value = mappingFunction.apply(key);
                map.put(key, value);
            }
            return value;
        } finally {
            writeLock.unlock();
        }
    }
}
```

**Quando Usare:**
- ✅ Letture molto più frequenti di scritture
- ✅ Sezioni critiche lunghe
- ❌ Scritture frequenti (overhead acquisizione write lock)

### StampedLock (Java 8+)

Lock **ottimistico** per performance ancora migliori.

```java
import java.util.concurrent.locks.StampedLock;

public class Point {
    private double x, y;
    private final StampedLock lock = new StampedLock();
    
    // Write
    public void move(double deltaX, double deltaY) {
        long stamp = lock.writeLock();
        try {
            x += deltaX;
            y += deltaY;
        } finally {
            lock.unlockWrite(stamp);
        }
    }
    
    // Optimistic Read
    public double distanceFromOrigin() {
        long stamp = lock.tryOptimisticRead();  // No lock!
        double currentX = x;
        double currentY = y;
        
        if (!lock.validate(stamp)) {  // Verifica se dati validi
            stamp = lock.readLock();  // Fallback a pessimistic read
            try {
                currentX = x;
                currentY = y;
            } finally {
                lock.unlockRead(stamp);
            }
        }
        
        return Math.sqrt(currentX * currentX + currentY * currentY);
    }
}
```

**Vantaggi:**
- **Optimistic read**: Nessun lock per letture (performance massime)
- **Validation**: Controlla se dati letti sono ancora validi
- **Fallback**: Se validation fallisce, acquisisce read lock

**Svantaggi:**
- Non rientrante
- API più complessa

## Condition Variables

Alternative a `wait()`/`notify()` più flessibili.

```java
import java.util.concurrent.locks.Condition;

public class BoundedBuffer<T> {
    private final T[] buffer;
    private int count, putIndex, takeIndex;
    
    private final Lock lock = new ReentrantLock();
    private final Condition notFull = lock.newCondition();   // Buffer non pieno
    private final Condition notEmpty = lock.newCondition();  // Buffer non vuoto
    
    @SuppressWarnings("unchecked")
    public BoundedBuffer(int capacity) {
        buffer = (T[]) new Object[capacity];
    }
    
    public void put(T item) throws InterruptedException {
        lock.lock();
        try {
            while (count == buffer.length) {
                notFull.await();  // Attende che buffer non sia pieno
            }
            buffer[putIndex] = item;
            putIndex = (putIndex + 1) % buffer.length;
            count++;
            notEmpty.signal();  // Notifica che buffer non è vuoto
        } finally {
            lock.unlock();
        }
    }
    
    public T take() throws InterruptedException {
        lock.lock();
        try {
            while (count == 0) {
                notEmpty.await();  // Attende che buffer non sia vuoto
            }
            T item = buffer[takeIndex];
            buffer[takeIndex] = null;
            takeIndex = (takeIndex + 1) % buffer.length;
            count--;
            notFull.signal();  // Notifica che buffer non è pieno
            return item;
        } finally {
            lock.unlock();
        }
    }
}
```

**Vantaggi vs wait()/notify():**
- **Condition multiple**: `notFull` e `notEmpty` separate
- **signal()** vs **signalAll()**: Più controllo su chi svegliare
- **Più chiaro**: Semantica esplicita

## Atomic Variables

Package `java.util.concurrent.atomic` fornisce variabili **atomiche** con operazioni lock-free.

### Classi Principali

```
java.util.concurrent.atomic
├── AtomicBoolean
├── AtomicInteger
├── AtomicLong
├── AtomicReference<V>
├── AtomicIntegerArray
├── AtomicLongArray
├── AtomicReferenceArray<E>
├── AtomicIntegerFieldUpdater<T>
├── AtomicLongFieldUpdater<T>
└── AtomicReferenceFieldUpdater<T,V>
```

### AtomicInteger

```java
import java.util.concurrent.atomic.AtomicInteger;

public class ContatoreAtomic {
    private AtomicInteger valore = new AtomicInteger(0);
    
    public void incrementa() {
        valore.incrementAndGet();  // Atomico, lock-free
    }
    
    public void decrementa() {
        valore.decrementAndGet();
    }
    
    public int get() {
        return valore.get();
    }
    
    // Compare-And-Swap (CAS)
    public boolean incrementaSe(int expected) {
        return valore.compareAndSet(expected, expected + 1);
    }
    
    public static void main(String[] args) throws InterruptedException {
        ContatoreAtomic counter = new ContatoreAtomic();
        
        // 1000 thread incrementano 1000 volte
        Thread[] threads = new Thread[1000];
        for (int i = 0; i < 1000; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 1000; j++) {
                    counter.incrementa();
                }
            });
            threads[i].start();
        }
        
        for (Thread t : threads) {
            t.join();
        }
        
        System.out.println("Valore: " + counter.get());  // 1,000,000 (garantito!)
    }
}
```

### Metodi Comuni

```java
AtomicInteger ai = new AtomicInteger(10);

// Get/Set
int current = ai.get();
ai.set(20);
int old = ai.getAndSet(30);  // Ritorna 20, imposta 30

// Incremento/Decremento
int newValue = ai.incrementAndGet();  // ++i (ritorna nuovo)
int oldValue = ai.getAndIncrement();  // i++ (ritorna vecchio)
ai.decrementAndGet();  // --i
ai.getAndDecrement();  // i--

// Addizione
ai.addAndGet(5);     // i += 5, ritorna nuovo
ai.getAndAdd(5);     // i += 5, ritorna vecchio

// Compare-And-Set (CAS)
boolean success = ai.compareAndSet(30, 40);  // Se valore==30, imposta 40

// Update (Java 8+)
ai.updateAndGet(x -> x * 2);  // Applica funzione atomicamente
ai.getAndUpdate(x -> x * 2);
ai.accumulateAndGet(5, (x, y) -> x + y);  // (x, arg) -> nuovo valore
```

### AtomicReference

Atomic per riferimenti ad oggetti.

```java
import java.util.concurrent.atomic.AtomicReference;

public class Stack<E> {
    private static class Node<E> {
        final E item;
        Node<E> next;
        
        Node(E item, Node<E> next) {
            this.item = item;
            this.next = next;
        }
    }
    
    private AtomicReference<Node<E>> top = new AtomicReference<>();
    
    public void push(E item) {
        Node<E> newHead = new Node<>(item, null);
        Node<E> oldHead;
        do {
            oldHead = top.get();
            newHead.next = oldHead;
        } while (!top.compareAndSet(oldHead, newHead));  // CAS loop
    }
    
    public E pop() {
        Node<E> oldHead;
        Node<E> newHead;
        do {
            oldHead = top.get();
            if (oldHead == null) {
                return null;
            }
            newHead = oldHead.next;
        } while (!top.compareAndSet(oldHead, newHead));  // CAS loop
        return oldHead.item;
    }
}
```

**Pattern CAS Loop:**
1. Leggi valore corrente
2. Calcola nuovo valore
3. Tenta CAS
4. Se fallisce (altro thread ha modificato), riparti da 1

### AtomicIntegerFieldUpdater

Atomic su campi esistenti (senza cambiare tipo).

```java
import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

public class Task {
    private volatile int status = 0;  // DEVE essere volatile!
    
    private static final AtomicIntegerFieldUpdater<Task> statusUpdater =
        AtomicIntegerFieldUpdater.newUpdater(Task.class, "status");
    
    public boolean start() {
        return statusUpdater.compareAndSet(this, 0, 1);  // 0->1 se in stato 0
    }
    
    public boolean complete() {
        return statusUpdater.compareAndSet(this, 1, 2);  // 1->2 se in stato 1
    }
    
    public int getStatus() {
        return status;
    }
}
```

**Vantaggi:**
- Memoria: Nessun overhead per `AtomicInteger` wrapper
- Compatibilità: Campi esistenti senza modificare tipo

## Compare-And-Swap (CAS)

Operazione atomica hardware:

```
boolean CAS(variabile, expected, new) {
    if (variabile == expected) {
        variabile = new;
        return true;
    } else {
        return false;
    }
}
```

**Caratteristiche:**
- **Atomica** a livello hardware (istruzione CMPXCHG su x86)
- **Lock-free**: Non usa lock
- **Wait-free** (teoricamente): Nessuna attesa su lock
- **ABA problem**: Se valore cambia A→B→A, CAS non lo rileva

### Esempio: Incremento Atomico

```java
public class CASDemo {
    private AtomicInteger value = new AtomicInteger(0);
    
    public void increment() {
        int current;
        do {
            current = value.get();
        } while (!value.compareAndSet(current, current + 1));
        
        // Loop fino a CAS successo:
        // - Se altro thread modifica valore, CAS fallisce
        // - Rileggi current e ritenta
    }
}
```

## Performance: Lock vs Atomic

```
Bassa Contesa (pochi thread):
AtomicInteger:     ████████████████████ 100%
ReentrantLock:     ████████████████ 80%
synchronized:      ██████████████ 70%

Alta Contesa (molti thread):
AtomicInteger:     ████████████████ 100%
ReentrantLock:     ████████████ 75%
synchronized:      ████████ 50%
```

**Conclusioni:**
- **Atomic**: Migliori sempre (lock-free)
- **ReentrantLock**: Più flessibile che synchronized
- **synchronized**: Sufficiente per casi semplici

## Best Practices

### ✅ DO

1. **Usa try-finally con lock.unlock()**
   ```java
   lock.lock();
   try {
       // sezione critica
   } finally {
       lock.unlock();
   }
   ```

2. **Preferisci atomic per contatori semplici**
   ```java
   AtomicInteger counter = new AtomicInteger();
   counter.incrementAndGet();
   ```

3. **Usa ReadWriteLock per read-heavy workload**
   ```java
   ReadWriteLock rwLock = new ReentrantReadWriteLock();
   ```

4. **tryLock() per evitare deadlock**
   ```java
   if (lock.tryLock(1, TimeUnit.SECONDS)) {
       try { ... } finally { lock.unlock(); }
   }
   ```

5. **Condition per wait/notify complessi**
   ```java
   Condition cond = lock.newCondition();
   cond.await();
   cond.signal();
   ```

### ❌ DON'T

1. **Non dimenticare unlock()**
   ```java
   lock.lock();
   // codice
   return;  // ERRORE: lock mai rilasciato!
   ```

2. **Non usare ReentrantLock senza motivo**
   ```java
   // Se synchronized basta, usalo
   public synchronized void metodo() { ... }
   ```

3. **Non chiamare unlock() senza lock()**
   ```java
   lock.unlock();  // ERRORE: IllegalMonitorStateException
   ```

4. **Non usare lock fair senza necessità**
   ```java
   // Fair lock è 10x più lento
   Lock lock = new ReentrantLock(true);  // Usa solo se necessario
   ```

## Riepilogo

| Meccanismo | Pro | Contro | Quando Usare |
|------------|-----|--------|--------------|
| **synchronized** | Semplice, familiare | Meno flessibile | Default, casi semplici |
| **ReentrantLock** | Flessibile, timeout, tryLock | Verboso | Necessaria interruption, timeout |
| **ReadWriteLock** | Ottime per read-heavy | Overhead write lock | Molte read, poche write |
| **StampedLock** | Performance massime | Non rientrante, complesso | Ottimizzazione estrema |
| **Atomic** | Lock-free, velocissime | Solo operazioni semplici | Contatori, flag, CAS |

---

**[← Precedente: Concurrent Collections](05-concurrent-collections.md)** | **[Torna al README →](README.md)**
