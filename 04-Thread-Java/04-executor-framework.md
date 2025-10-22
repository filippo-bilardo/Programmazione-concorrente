# 4. Executor Framework

## Perché Executor Framework?

Creare thread manualmente con `new Thread()` ha svantaggi:

1. **Overhead**: Creazione/distruzione thread è costosa
2. **Scalabilità**: Troppi thread degradano performance
3. **Gestione**: Difficile controllare lifecycle e risorse
4. **Riutilizzo**: Thread non vengono riutilizzati

**Soluzione:** **Executor Framework** (package `java.util.concurrent`)

## Architettura Executor Framework

```
┌─────────────────────────────────────────┐
│         java.util.concurrent            │
├─────────────────────────────────────────┤
│  Executor (interfaccia)                 │
│    └─→ void execute(Runnable)           │
│                                         │
│  ExecutorService (interfaccia)          │
│    ├─→ Future<?> submit(Runnable)       │
│    ├─→ <T> Future<T> submit(Callable<T>)│
│    ├─→ shutdown()                       │
│    └─→ shutdownNow()                    │
│                                         │
│  AbstractExecutorService (abstract)     │
│                                         │
│  ThreadPoolExecutor (concreta)          │
│    └─→ Pool di thread riutilizzabili    │
│                                         │
│  ScheduledExecutorService (interfaccia) │
│    └─→ Esecuzione schedulata/periodica  │
└─────────────────────────────────────────┘
```

## Interfaccia Executor

La più semplice: esegue task in modo asincrono.

```java
public interface Executor {
    void execute(Runnable command);
}
```

### Esempio Base

```java
import java.util.concurrent.Executor;

public class ExecutorDemo {
    public static void main(String[] args) {
        Executor executor = new Executor() {
            @Override
            public void execute(Runnable command) {
                new Thread(command).start();  // Crea nuovo thread
            }
        };
        
        executor.execute(() -> System.out.println("Task eseguito!"));
    }
}
```

## ExecutorService

Estende `Executor` con metodi per gestione lifecycle e task che ritornano risultati.

### Metodi Principali

```java
public interface ExecutorService extends Executor {
    // Sottomissione task
    Future<?> submit(Runnable task);
    <T> Future<T> submit(Runnable task, T result);
    <T> Future<T> submit(Callable<T> task);
    
    // Esecuzione batch
    <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks);
    <T> T invokeAny(Collection<? extends Callable<T>> tasks);
    
    // Lifecycle
    void shutdown();                    // Termina dopo task correnti
    List<Runnable> shutdownNow();      // Interrompe task
    boolean isShutdown();
    boolean isTerminated();
    boolean awaitTermination(long timeout, TimeUnit unit);
}
```

## Executors Factory

La classe `Executors` fornisce metodi factory per creare pool comuni.

### 1. newFixedThreadPool(int n)

Pool con **numero fisso** di thread.

```java
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

ExecutorService executor = Executors.newFixedThreadPool(5);

for (int i = 0; i < 10; i++) {
    final int taskId = i;
    executor.submit(() -> {
        System.out.println("Task " + taskId + " eseguito da " + 
                          Thread.currentThread().getName());
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    });
}

executor.shutdown();  // Non accetta nuovi task
```

**Caratteristiche:**
- Pool con `n` thread
- Se tutti occupati, task in coda
- Thread riutilizzati per task successivi
- **Uso:** Task di durata simile, carico costante

### 2. newCachedThreadPool()

Pool che **cresce dinamicamente**.

```java
ExecutorService executor = Executors.newCachedThreadPool();

for (int i = 0; i < 100; i++) {
    executor.submit(() -> {
        System.out.println("Task da " + Thread.currentThread().getName());
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    });
}

executor.shutdown();
```

**Caratteristiche:**
- Crea nuovi thread al bisogno
- Riusa thread idle (timeout 60s)
- Nessun limite al numero di thread
- **Uso:** Task brevi, burst di richieste
- **⚠️ Attenzione:** Può creare troppi thread se non controllato

### 3. newSingleThreadExecutor()

Pool con **un solo thread**.

```java
ExecutorService executor = Executors.newSingleThreadExecutor();

for (int i = 0; i < 5; i++) {
    final int taskId = i;
    executor.submit(() -> {
        System.out.println("Task " + taskId + " (sequenziale)");
    });
}

executor.shutdown();
```

**Caratteristiche:**
- Esecuzione **seriale** (un task alla volta)
- Task in coda FIFO
- Se thread muore, viene ricreato
- **Uso:** Task sequenziali, accesso a risorsa condivisa

### 4. newScheduledThreadPool(int n)

Pool per task **schedulati** o **periodici**.

```java
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(2);

// Esecuzione ritardata (una volta)
scheduler.schedule(() -> {
    System.out.println("Eseguito dopo 3 secondi");
}, 3, TimeUnit.SECONDS);

// Esecuzione periodica (fixed rate)
scheduler.scheduleAtFixedRate(() -> {
    System.out.println("Ogni 2 secondi: " + System.currentTimeMillis());
}, 0, 2, TimeUnit.SECONDS);

// Esecuzione periodica (fixed delay)
scheduler.scheduleWithFixedDelay(() -> {
    System.out.println("2s dopo termine precedente");
}, 0, 2, TimeUnit.SECONDS);

// Shutdown dopo 10 secondi
scheduler.schedule(() -> {
    scheduler.shutdown();
}, 10, TimeUnit.SECONDS);
```

**Differenza:**
- **`scheduleAtFixedRate`**: Esecuzioni ogni N secondi (periodo fisso)
- **`scheduleWithFixedDelay`**: N secondi **dopo** terminazione precedente

```
scheduleAtFixedRate (period=2s):
|────Task────|       |────Task────|       |────Task────|
0s          1s      2s          3s      4s          5s

scheduleWithFixedDelay (delay=2s):
|────Task────|──2s──|────Task────|──2s──|────Task────|
0s          1s     3s          4s     6s          7s
```

### 5. newWorkStealingPool() (Java 8+)

Pool basato su **ForkJoinPool** con work-stealing.

```java
ExecutorService executor = Executors.newWorkStealingPool();
// Usa parallelismo = numero core CPU

// Oppure specifica parallelismo
ExecutorService executor2 = Executors.newWorkStealingPool(4);
```

**Caratteristiche:**
- Thread possono "rubare" task da code di altri thread
- Ottimo per task ricorsivi (divide-and-conquer)
- **Uso:** Task che generano sottotask

## Callable e Future

`Callable` è come `Runnable` ma può **ritornare un valore** e **lanciare eccezioni**.

### Callable<V>

```java
@FunctionalInterface
public interface Callable<V> {
    V call() throws Exception;
}
```

### Future<V>

Rappresenta il **risultato futuro** di un'operazione asincrona.

```java
public interface Future<V> {
    boolean cancel(boolean mayInterruptIfRunning);
    boolean isCancelled();
    boolean isDone();
    V get() throws InterruptedException, ExecutionException;
    V get(long timeout, TimeUnit unit) throws TimeoutException;
}
```

### Esempio Completo

```java
import java.util.concurrent.*;

public class CallableFutureDemo {
    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(3);
        
        // Task che calcola somma
        Callable<Integer> task1 = () -> {
            System.out.println("Task 1: calcolo somma...");
            Thread.sleep(2000);
            return 100 + 200;
        };
        
        // Task che calcola prodotto
        Callable<Integer> task2 = () -> {
            System.out.println("Task 2: calcolo prodotto...");
            Thread.sleep(1500);
            return 10 * 20;
        };
        
        // Sottometti task
        Future<Integer> future1 = executor.submit(task1);
        Future<Integer> future2 = executor.submit(task2);
        
        try {
            // Continua con altro lavoro...
            System.out.println("Main thread: lavoro in corso...");
            
            // Recupera risultati (blocca se non pronti)
            Integer risultato1 = future1.get();  // Attende max infinito
            Integer risultato2 = future2.get(3, TimeUnit.SECONDS);  // Timeout 3s
            
            System.out.println("Somma: " + risultato1);
            System.out.println("Prodotto: " + risultato2);
            
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            e.printStackTrace();
        } catch (ExecutionException e) {
            System.err.println("Eccezione nel task: " + e.getCause());
        } catch (TimeoutException e) {
            System.err.println("Timeout raggiunto!");
            future2.cancel(true);  // Interrompi task
        } finally {
            executor.shutdown();
        }
    }
}
```

### invokeAll() e invokeAny()

#### invokeAll()

Esegue **tutti** i task e aspetta terminazione di tutti.

```java
ExecutorService executor = Executors.newFixedThreadPool(3);

List<Callable<Integer>> tasks = Arrays.asList(
    () -> { Thread.sleep(1000); return 1; },
    () -> { Thread.sleep(2000); return 2; },
    () -> { Thread.sleep(1500); return 3; }
);

try {
    List<Future<Integer>> results = executor.invokeAll(tasks);
    
    // Tutti i task sono completati
    for (Future<Integer> future : results) {
        System.out.println("Risultato: " + future.get());
    }
} catch (InterruptedException | ExecutionException e) {
    e.printStackTrace();
} finally {
    executor.shutdown();
}
```

#### invokeAny()

Ritorna il risultato del **primo task completato** con successo, cancella gli altri.

```java
ExecutorService executor = Executors.newFixedThreadPool(3);

List<Callable<String>> tasks = Arrays.asList(
    () -> { Thread.sleep(3000); return "Task 1"; },
    () -> { Thread.sleep(1000); return "Task 2"; },  // Vince!
    () -> { Thread.sleep(2000); return "Task 3"; }
);

try {
    String result = executor.invokeAny(tasks);
    System.out.println("Primo completato: " + result);  // "Task 2"
} catch (InterruptedException | ExecutionException e) {
    e.printStackTrace();
} finally {
    executor.shutdown();
}
```

## ThreadPoolExecutor: Configurazione Avanzata

Per controllo fine sul pool:

```java
import java.util.concurrent.*;

ThreadPoolExecutor executor = new ThreadPoolExecutor(
    2,                          // corePoolSize: thread minimo
    4,                          // maximumPoolSize: thread massimo
    60L,                        // keepAliveTime: tempo idle prima rimozione
    TimeUnit.SECONDS,           // unità keepAliveTime
    new LinkedBlockingQueue<>(100),  // workQueue: coda task
    new ThreadPoolExecutor.CallerRunsPolicy()  // rejectedExecutionHandler
);
```

### Parametri

| Parametro | Descrizione |
|-----------|-------------|
| **corePoolSize** | Numero minimo di thread mantenuti vivi |
| **maximumPoolSize** | Numero massimo di thread creabili |
| **keepAliveTime** | Tempo idle prima rimozione thread (se > corePoolSize) |
| **workQueue** | Coda per task in attesa (es. `LinkedBlockingQueue`) |
| **threadFactory** | Factory per creare thread custom |
| **rejectedExecutionHandler** | Strategia quando coda piena |

### RejectedExecutionHandler

Quando pool saturo (coda piena + max thread):

1. **AbortPolicy** (default): Lancia `RejectedExecutionException`
2. **CallerRunsPolicy**: Esegue task nel thread chiamante
3. **DiscardPolicy**: Scarta task silenziosamente
4. **DiscardOldestPolicy**: Scarta task più vecchio, accoda nuovo

```java
executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
```

## CompletableFuture (Java 8+)

`Future` avanzato con API fluent per composizione asincrona.

### Creazione

```java
import java.util.concurrent.CompletableFuture;

// Async task
CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {
    try {
        Thread.sleep(1000);
    } catch (InterruptedException e) {
        throw new RuntimeException(e);
    }
    return "Hello";
});

// Sync (completato immediatamente)
CompletableFuture<String> completed = CompletableFuture.completedFuture("World");
```

### Composizione

```java
CompletableFuture.supplyAsync(() -> {
    return "Hello";
})
.thenApply(s -> s + " World")           // Trasforma risultato
.thenApply(String::toUpperCase)         // Altra trasformazione
.thenAccept(s -> System.out.println(s)) // Consuma (no return)
.thenRun(() -> System.out.println("Done"));  // Esegue azione
```

### Combinazione

```java
CompletableFuture<String> future1 = CompletableFuture.supplyAsync(() -> "Hello");
CompletableFuture<String> future2 = CompletableFuture.supplyAsync(() -> "World");

// Combina risultati
CompletableFuture<String> combined = future1.thenCombine(future2, (s1, s2) -> s1 + " " + s2);
System.out.println(combined.get());  // "Hello World"

// Esegui entrambi, poi azione
CompletableFuture<Void> both = future1.thenAcceptBoth(future2, (s1, s2) -> {
    System.out.println(s1 + " and " + s2);
});
```

### Gestione Errori

```java
CompletableFuture.supplyAsync(() -> {
    if (Math.random() > 0.5) {
        throw new RuntimeException("Errore!");
    }
    return "Successo";
})
.exceptionally(ex -> {
    System.err.println("Errore gestito: " + ex.getMessage());
    return "Valore di default";
})
.thenAccept(System.out::println);
```

### anyOf() e allOf()

```java
CompletableFuture<String> f1 = CompletableFuture.supplyAsync(() -> {
    sleep(3000);
    return "Task 1";
});

CompletableFuture<String> f2 = CompletableFuture.supplyAsync(() -> {
    sleep(1000);
    return "Task 2";
});

// Primo che completa
CompletableFuture<Object> any = CompletableFuture.anyOf(f1, f2);
System.out.println("Primo: " + any.get());  // "Task 2"

// Tutti completati
CompletableFuture<Void> all = CompletableFuture.allOf(f1, f2);
all.get();  // Attende tutti
System.out.println("Tutti completati");
```

## Best Practices

### ✅ DO

1. **Usa pool invece di creare thread manualmente**
   ```java
   ExecutorService executor = Executors.newFixedThreadPool(10);
   executor.submit(task);
   ```

2. **Shutdown dell'executor**
   ```java
   executor.shutdown();
   try {
       if (!executor.awaitTermination(60, TimeUnit.SECONDS)) {
           executor.shutdownNow();
       }
   } catch (InterruptedException e) {
       executor.shutdownNow();
   }
   ```

3. **Gestisci eccezioni nei task**
   ```java
   Future<Integer> future = executor.submit(() -> {
       try {
           // task
           return result;
       } catch (Exception e) {
           log.error("Errore nel task", e);
           throw e;
       }
   });
   ```

4. **Dimensiona pool correttamente**
   ```java
   // CPU-bound: n = numero core
   int cores = Runtime.getRuntime().availableProcessors();
   ExecutorService executor = Executors.newFixedThreadPool(cores);
   
   // I/O-bound: n > numero core
   ExecutorService executor = Executors.newFixedThreadPool(cores * 2);
   ```

5. **Usa CompletableFuture per composizione**
   ```java
   CompletableFuture.supplyAsync(() -> fetchData())
       .thenApply(this::process)
       .thenAccept(this::save);
   ```

### ❌ DON'T

1. **Non dimenticare shutdown**
   ```java
   executor.submit(task);
   // ERRORE: executor mai chiuso → JVM non termina
   ```

2. **Non usare newCachedThreadPool per task lunghi**
   ```java
   // ERRORE: può creare migliaia di thread
   ExecutorService executor = Executors.newCachedThreadPool();
   for (int i = 0; i < 10000; i++) {
       executor.submit(() -> heavyTask());
   }
   ```

3. **Non bloccare thread del pool**
   ```java
   executor.submit(() -> {
       future.get();  // ERRORE: blocca thread del pool
   });
   ```

4. **Non ignorare eccezioni in Future**
   ```java
   Future<?> future = executor.submit(task);
   // ERRORE: eccezioni silenziate se non chiami get()
   ```

## Riepilogo

| Tipo Pool | Quando Usare |
|-----------|--------------|
| **FixedThreadPool** | Carico costante, task simili |
| **CachedThreadPool** | Burst, task brevi |
| **SingleThreadExecutor** | Esecuzione seriale |
| **ScheduledThreadPool** | Task periodici/schedulati |
| **WorkStealingPool** | Task ricorsivi, divide-conquer |

---

**[← Precedente: Memory Model](03-memory-model.md)** | **[Prossimo: Concurrent Collections →](05-concurrent-collections.md)**
