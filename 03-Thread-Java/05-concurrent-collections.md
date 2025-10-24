# 5. Concurrent Collections

## Perché Concurrent Collections?

Le collezioni standard (`ArrayList`, `HashMap`, `HashSet`) **NON sono thread-safe**. Accesso concorrente può causare:

1. **ConcurrentModificationException**
2. **Dati corrotti**
3. **Loop infiniti** (es. HashMap in Java 7)
4. **Perdita di dati**

### Soluzioni

1. **Synchronized Wrappers** (vecchio approccio)
   ```java
   List<String> syncList = Collections.synchronizedList(new ArrayList<>());
   Map<String, Integer> syncMap = Collections.synchronizedMap(new HashMap<>());
   ```
   **Problema:** Lock globale su intera collezione → basse performance

2. **Concurrent Collections** (MODERNO) ✅
   - Lock più granulari
   - Algoritmi lock-free
   - Performance superiori

## Package java.util.concurrent

```
java.util.concurrent
├── ConcurrentHashMap<K,V>      // Map thread-safe
├── ConcurrentSkipListMap<K,V>  // SortedMap thread-safe
├── ConcurrentSkipListSet<E>    // SortedSet thread-safe
├── CopyOnWriteArrayList<E>     // List thread-safe (read-heavy)
├── CopyOnWriteArraySet<E>      // Set thread-safe (read-heavy)
├── BlockingQueue<E>            // Interfaccia code bloccanti
│   ├── ArrayBlockingQueue<E>   // Coda bounded
│   ├── LinkedBlockingQueue<E>  // Coda optionally bounded
│   ├── PriorityBlockingQueue<E>// Coda con priorità
│   ├── DelayQueue<E>           // Coda con delay
│   └── SynchronousQueue<E>     // Coda senza capacità
├── BlockingDeque<E>            // Deque bloccante
│   └── LinkedBlockingDeque<E>
└── TransferQueue<E>            // Code con handoff
    └── LinkedTransferQueue<E>
```

## ConcurrentHashMap

Map thread-safe con **lock segmentati** (alta concorrenza).

### Caratteristiche

- **Segmentazione**: Map divisa in segmenti, lock per segmento
- **Letture lock-free** (quasi sempre)
- **Scritture lock segmento** (non intera map)
- **Nessun lock globale**
- **Iterator weakly consistent** (riflettono stato al momento creazione, no exception)

### API Base

```java
import java.util.concurrent.ConcurrentHashMap;

ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();

// Operazioni base (thread-safe)
map.put("A", 1);
map.put("B", 2);
Integer value = map.get("A");
map.remove("B");

// Size (approssimato, può cambiare durante chiamata)
int size = map.size();

// Iterazione (snapshot)
for (Map.Entry<String, Integer> entry : map.entrySet()) {
    System.out.println(entry.getKey() + " = " + entry.getValue());
}
```

### Operazioni Atomiche (Java 8+)

```java
ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();

// putIfAbsent: inserisce solo se chiave assente
Integer oldValue = map.putIfAbsent("counter", 1);  // Ritorna null se inserito

// compute: calcola nuovo valore atomicamente
map.put("counter", 5);
map.compute("counter", (k, v) -> v == null ? 1 : v + 1);  // counter = 6

// computeIfAbsent: calcola se assente
map.computeIfAbsent("key", k -> k.length());

// computeIfPresent: calcola se presente
map.computeIfPresent("counter", (k, v) -> v * 2);  // counter = 12

// merge: combina valori
map.merge("counter", 10, (oldVal, newVal) -> oldVal + newVal);  // counter = 22

// replace: sostituisce solo se chiave presente
boolean replaced = map.replace("counter", 22, 100);  // true se sostituito
```

### Esempio: Contatore Concorrente

```java
public class ContatoreConcorrente {
    private ConcurrentHashMap<String, Integer> contatori = new ConcurrentHashMap<>();
    
    public void incrementa(String chiave) {
        contatori.merge(chiave, 1, Integer::sum);
        // Equivalente atomico a:
        // Integer old = map.get(chiave);
        // map.put(chiave, old == null ? 1 : old + 1);
    }
    
    public int get(String chiave) {
        return contatori.getOrDefault(chiave, 0);
    }
    
    public static void main(String[] args) throws InterruptedException {
        ContatoreConcorrente cc = new ContatoreConcorrente();
        
        // 1000 thread incrementano contatori
        Thread[] threads = new Thread[1000];
        for (int i = 0; i < 1000; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 100; j++) {
                    cc.incrementa("A");
                    cc.incrementa("B");
                }
            });
            threads[i].start();
        }
        
        for (Thread t : threads) {
            t.join();
        }
        
        System.out.println("A: " + cc.get("A"));  // 100,000
        System.out.println("B: " + cc.get("B"));  // 100,000
    }
}
```

### Operazioni Bulk (forEach, search, reduce)

```java
ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();
map.put("A", 10);
map.put("B", 20);
map.put("C", 30);

// forEach: applica azione a ogni entry
map.forEach((k, v) -> System.out.println(k + " = " + v));

// search: trova primo match
String result = map.search(1, (k, v) -> v > 15 ? k : null);
System.out.println("Prima chiave con v>15: " + result);  // "B" o "C"

// reduce: riduce a singolo valore
Integer sum = map.reduce(1, (k, v) -> v, Integer::sum);
System.out.println("Somma valori: " + sum);  // 60
```

**Parametro parallelismThreshold:**
- Se map.size() >= threshold → esecuzione parallela
- Usa `1` per sempre parallelo
- Usa `Long.MAX_VALUE` per sempre sequenziale

## CopyOnWriteArrayList

List thread-safe ottimizzata per **molte letture, poche scritture**.

### Caratteristiche

- **Copy-on-Write**: Ogni modifica crea copia dell'array sottostante
- **Letture lock-free** (velocissime)
- **Scritture lente** (O(n) per copia)
- **Iterator snapshot** (mai ConcurrentModificationException)

### Quando Usare

- ✅ Molte letture, poche scritture (es. listener list)
- ✅ Iterazioni frequenti
- ❌ Scritture frequenti

### Esempio

```java
import java.util.concurrent.CopyOnWriteArrayList;

CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<>();

// Thread 1: Scrittura (lenta)
new Thread(() -> {
    for (int i = 0; i < 10; i++) {
        list.add("Item " + i);
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}).start();

// Thread 2: Lettura (veloce, nessun lock)
new Thread(() -> {
    for (int i = 0; i < 20; i++) {
        for (String item : list) {  // Iterator snapshot
            System.out.println(item);
        }
        try {
            Thread.sleep(50);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}).start();
```

### Esempio: Event Listeners

```java
public class EventDispatcher {
    private CopyOnWriteArrayList<EventListener> listeners = new CopyOnWriteArrayList<>();
    
    public void addListener(EventListener listener) {
        listeners.add(listener);  // Scrittura rara
    }
    
    public void removeListener(EventListener listener) {
        listeners.remove(listener);  // Scrittura rara
    }
    
    public void fireEvent(String event) {
        // Lettura frequente, nessun lock
        for (EventListener listener : listeners) {
            listener.onEvent(event);
        }
    }
}

interface EventListener {
    void onEvent(String event);
}
```

## BlockingQueue

Code **bloccanti**: operazioni bloccano se coda piena (put) o vuota (take).

### Interfaccia

```java
public interface BlockingQueue<E> extends Queue<E> {
    // Inserimento
    boolean add(E e);          // Lancia exception se piena
    boolean offer(E e);        // Ritorna false se piena
    void put(E e);             // BLOCCA se piena
    boolean offer(E e, long timeout, TimeUnit unit);  // Timeout
    
    // Rimozione
    E remove();                // Lancia exception se vuota
    E poll();                  // Ritorna null se vuota
    E take();                  // BLOCCA se vuota
    E poll(long timeout, TimeUnit unit);  // Timeout
    
    // Ispezione
    E element();               // Lancia exception se vuota
    E peek();                  // Ritorna null se vuota
}
```

### Operazioni Confrontate

| Operazione | Lancia Exception | Ritorna Speciale | Blocca | Timeout |
|------------|------------------|------------------|--------|---------|
| **Insert** | `add(e)` | `offer(e)` | `put(e)` | `offer(e, time, unit)` |
| **Remove** | `remove()` | `poll()` | `take()` | `poll(time, unit)` |
| **Examine** | `element()` | `peek()` | N/A | N/A |

### ArrayBlockingQueue

Coda **bounded** con array circolare.

```java
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

BlockingQueue<String> queue = new ArrayBlockingQueue<>(10);  // Capacità 10

// Producer
new Thread(() -> {
    try {
        for (int i = 0; i < 20; i++) {
            queue.put("Item " + i);  // Blocca se piena
            System.out.println("Prodotto: Item " + i);
        }
    } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
    }
}).start();

// Consumer
new Thread(() -> {
    try {
        while (true) {
            String item = queue.take();  // Blocca se vuota
            System.out.println("Consumato: " + item);
            Thread.sleep(100);
        }
    } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
    }
}).start();
```

### LinkedBlockingQueue

Coda **optionally bounded** con linked nodes.

```java
// Unbounded (capacità infinita)
BlockingQueue<String> unbounded = new LinkedBlockingQueue<>();

// Bounded
BlockingQueue<String> bounded = new LinkedBlockingQueue<>(100);
```

**Confronto:**

| Aspetto | ArrayBlockingQueue | LinkedBlockingQueue |
|---------|-------------------|---------------------|
| Struttura | Array circolare | Linked nodes |
| Bounded | SI | Opzionale |
| Lock | Singolo | Due (put/take separati) |
| Performance | Migliore se bounded | Migliore throughput |

### PriorityBlockingQueue

Coda **unbounded** con priorità.

```java
import java.util.concurrent.PriorityBlockingQueue;

PriorityBlockingQueue<Task> queue = new PriorityBlockingQueue<>();

class Task implements Comparable<Task> {
    String name;
    int priority;
    
    public Task(String name, int priority) {
        this.name = name;
        this.priority = priority;
    }
    
    @Override
    public int compareTo(Task other) {
        return Integer.compare(other.priority, this.priority);  // Priorità alta prima
    }
    
    @Override
    public String toString() {
        return name + " (pri=" + priority + ")";
    }
}

// Inserimento
queue.put(new Task("Low", 1));
queue.put(new Task("High", 10));
queue.put(new Task("Medium", 5));

// Rimozione (ordine priorità)
System.out.println(queue.take());  // High (pri=10)
System.out.println(queue.take());  // Medium (pri=5)
System.out.println(queue.take());  // Low (pri=1)
```

### DelayQueue

Coda dove elementi diventano disponibili solo dopo un **delay**.

```java
import java.util.concurrent.DelayQueue;
import java.util.concurrent.Delayed;
import java.util.concurrent.TimeUnit;

class DelayedTask implements Delayed {
    private String name;
    private long startTime;
    
    public DelayedTask(String name, long delayMs) {
        this.name = name;
        this.startTime = System.currentTimeMillis() + delayMs;
    }
    
    @Override
    public long getDelay(TimeUnit unit) {
        long diff = startTime - System.currentTimeMillis();
        return unit.convert(diff, TimeUnit.MILLISECONDS);
    }
    
    @Override
    public int compareTo(Delayed o) {
        return Long.compare(this.startTime, ((DelayedTask)o).startTime);
    }
    
    @Override
    public String toString() {
        return name;
    }
}

DelayQueue<DelayedTask> queue = new DelayQueue<>();

// Inserimento con delay
queue.put(new DelayedTask("Task 3s", 3000));
queue.put(new DelayedTask("Task 1s", 1000));
queue.put(new DelayedTask("Task 2s", 2000));

// Consumazione (ordine delay)
while (!queue.isEmpty()) {
    DelayedTask task = queue.take();  // Blocca fino a delay scaduto
    System.out.println("Eseguito: " + task);
}
// Output:
// Eseguito: Task 1s (dopo ~1s)
// Eseguito: Task 2s (dopo ~2s totali)
// Eseguito: Task 3s (dopo ~3s totali)
```

### SynchronousQueue

Coda **senza capacità**: ogni `put()` attende corrispondente `take()`.

```java
import java.util.concurrent.SynchronousQueue;

SynchronousQueue<String> queue = new SynchronousQueue<>();

// Producer
new Thread(() -> {
    try {
        System.out.println("Producer: attendo consumer...");
        queue.put("Data");  // BLOCCA fino a take()
        System.out.println("Producer: dato consegnato!");
    } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
    }
}).start();

Thread.sleep(2000);  // Simula lavoro

// Consumer
new Thread(() -> {
    try {
        System.out.println("Consumer: ricevo dato...");
        String data = queue.take();  // Sblocca producer
        System.out.println("Consumer: ricevuto " + data);
    } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
    }
}).start();
```

**Uso:** Handoff diretto tra thread (es. pool worker)

## ConcurrentSkipListMap e ConcurrentSkipListSet

Map/Set **ordinati** thread-safe.

```java
import java.util.concurrent.ConcurrentSkipListMap;
import java.util.concurrent.ConcurrentSkipListSet;

ConcurrentSkipListMap<Integer, String> map = new ConcurrentSkipListMap<>();
map.put(3, "tre");
map.put(1, "uno");
map.put(2, "due");

// Iterazione ordinata
for (Map.Entry<Integer, String> entry : map.entrySet()) {
    System.out.println(entry.getKey() + " = " + entry.getValue());
}
// Output: 1=uno, 2=due, 3=tre

ConcurrentSkipListSet<Integer> set = new ConcurrentSkipListSet<>();
set.add(5);
set.add(1);
set.add(3);
System.out.println(set);  // [1, 3, 5]
```

**Complessità:** O(log n) per operazioni base (vs O(1) per ConcurrentHashMap)

**Uso:** Quando serve ordinamento + thread-safety

## Confronto Performance

### Scrittura Singola

```
ConcurrentHashMap: ████████████████ 100%
SynchronizedMap:   ████████ 50%
```

### Lettura Concorrente (4 thread)

```
ConcurrentHashMap:     ████████████████████████████████ 100%
SynchronizedMap:       ████████ 25%
CopyOnWriteArrayList:  ████████████████████████████████ 100% (read-only)
```

### Scrittura Concorrente (4 thread)

```
ConcurrentHashMap:     ████████████████████ 100%
SynchronizedMap:       ██████ 30%
CopyOnWriteArrayList:  ██ 10% (copy overhead)
```

## Best Practices

### ✅ DO

1. **Usa ConcurrentHashMap per map condivise**
   ```java
   Map<String, Object> cache = new ConcurrentHashMap<>();
   ```

2. **Usa BlockingQueue per producer-consumer**
   ```java
   BlockingQueue<Task> queue = new LinkedBlockingQueue<>(100);
   ```

3. **Usa CopyOnWriteArrayList per listener list**
   ```java
   List<Listener> listeners = new CopyOnWriteArrayList<>();
   ```

4. **Usa operazioni atomiche**
   ```java
   map.computeIfAbsent(key, k -> computeValue());
   ```

5. **Preferisci concurrent collection a synchronized wrapper**
   ```java
   // VECCHIO
   Map<K,V> map = Collections.synchronizedMap(new HashMap<>());
   
   // MODERNO
   Map<K,V> map = new ConcurrentHashMap<>();
   ```

### ❌ DON'T

1. **Non iterare synchronized collection senza lock esterno**
   ```java
   List<String> list = Collections.synchronizedList(new ArrayList<>());
   
   // ERRORE
   for (String s : list) { ... }
   
   // CORRETTO
   synchronized(list) {
       for (String s : list) { ... }
   }
   ```

2. **Non usare CopyOnWriteArrayList per scritture frequenti**
   ```java
   // ERRORE: ogni add() copia intero array
   for (int i = 0; i < 10000; i++) {
       cowList.add(i);
   }
   ```

3. **Non assumere size() preciso su ConcurrentHashMap**
   ```java
   int size = map.size();  // Può essere stale immediatamente
   ```

4. **Non usare null in ConcurrentHashMap**
   ```java
   map.put(null, value);   // ERRORE: NullPointerException
   map.put(key, null);     // ERRORE: NullPointerException
   ```

## Riepilogo

| Collection | Thread-Safe | Ordering | Blocking | Uso Ideale |
|------------|-------------|----------|----------|------------|
| **ConcurrentHashMap** | ✅ | No | No | Map general-purpose |
| **CopyOnWriteArrayList** | ✅ | Insertion | No | Read-heavy list |
| **ConcurrentSkipListMap** | ✅ | Sorted | No | Sorted map |
| **ArrayBlockingQueue** | ✅ | FIFO | ✅ | Bounded producer-consumer |
| **LinkedBlockingQueue** | ✅ | FIFO | ✅ | Unbounded producer-consumer |
| **PriorityBlockingQueue** | ✅ | Priority | ✅ | Task scheduling |
| **DelayQueue** | ✅ | Delay | ✅ | Delayed execution |
| **SynchronousQueue** | ✅ | N/A | ✅ | Handoff diretto |

---

**[← Precedente: Executor Framework](04-executor-framework.md)** | **[Prossimo: Lock e Atomic →](06-lock-atomic.md)**
