# 3. Java Memory Model e Visibilità

## Cos'è il Java Memory Model (JMM)?

Il **Java Memory Model** è la specifica che definisce come i thread interagiscono attraverso la memoria. Garantisce:

1. **Visibilità**: Quando un thread vede le modifiche fatte da un altro
2. **Ordering**: L'ordine di esecuzione delle operazioni
3. **Atomicità**: Quali operazioni sono indivisibili

## Architettura Memoria Multi-Thread

```
┌─────────────┐           ┌─────────────┐
│  Thread 1   │           │  Thread 2   │
├─────────────┤           ├─────────────┤
│Working      │           │Working      │
│Memory       │           │Memory       │
│(CPU Cache)  │           │(CPU Cache)  │
└──────┬──────┘           └──────┬──────┘
       │                         │
       │      READ/WRITE         │
       │    ┌─────────────┐      │
       └───→│   MAIN      │←─────┘
            │   MEMORY    │
            │  (Shared)   │
            └─────────────┘
```

### Problemi Senza Sincronizzazione

```java
public class VisibilitaProblem {
    private boolean ready = false;  // NO volatile!
    private int value = 0;
    
    // Thread 1
    public void writer() {
        value = 42;      // Scrittura 1
        ready = true;    // Scrittura 2
    }
    
    // Thread 2
    public void reader() {
        while (!ready) { // Legge da cache locale
            // potrebbe non vedere mai ready=true!
        }
        System.out.println(value);  // Potrebbe stampare 0!
    }
}
```

**Problemi:**
1. Thread 2 potrebbe non vedere mai `ready = true` (cache stale)
2. Thread 2 potrebbe vedere `ready = true` ma `value = 0` (reordering)

## Happens-Before Relationship

Il JMM definisce relazioni **happens-before** che garantiscono l'ordine di esecuzione visibile.

### Regole Happens-Before

1. **Program Order Rule**: Operazioni in un thread hanno ordine sequenziale
   ```java
   int a = 1;  // HB
   int b = 2;  // b happens-after a (nello stesso thread)
   ```

2. **Monitor Lock Rule**: unlock HB acquire dello stesso lock
   ```java
   synchronized(obj) {
       x = 1;  // Scrittura
   }  // unlock HB
   
   synchronized(obj) {  // acquire
       print(x);  // Legge 1 (garantito)
   }
   ```

3. **Volatile Variable Rule**: Scrittura HB lettura della stessa variabile volatile
   ```java
   volatile int v = 0;
   
   // Thread 1
   v = 42;  // Scrittura HB
   
   // Thread 2
   int x = v;  // Legge 42 (garantito)
   ```

4. **Thread Start Rule**: `start()` HB prima istruzione del nuovo thread
   ```java
   int x = 1;
   Thread t = new Thread(() -> {
       System.out.println(x);  // Vede x=1 (garantito)
   });
   t.start();  // HB
   ```

5. **Thread Join Rule**: Ultima istruzione thread HB ritorno da `join()`
   ```java
   Thread t = new Thread(() -> {
       x = 42;  // Scrittura
   });  // HB
   t.start();
   t.join();  // Attende completamento
   System.out.println(x);  // Vede x=42 (garantito)
   ```

6. **Transitività**: Se A HB B e B HB C, allora A HB C

### Esempio Completo

```java
public class HappensBeforeDemo {
    private int a = 0;
    private int b = 0;
    private volatile boolean ready = false;  // volatile!
    
    // Thread 1 (Writer)
    public void writer() {
        a = 1;           // 1. Program order
        b = 2;           // 2. Program order (HB dopo 1)
        ready = true;    // 3. Volatile write (HB dopo 2)
    }
    
    // Thread 2 (Reader)
    public void reader() {
        while (!ready) { // 4. Volatile read
            // attesa
        }
        // Grazie a volatile write HB volatile read:
        // Garantito che vede a=1 e b=2
        System.out.println("a=" + a + ", b=" + b);  // Stampa: a=1, b=2
    }
}
```

**Catena Happens-Before:**
```
a=1 → b=2 → ready=true (volatile write)
                ↓ HB
              ready (volatile read) → print a, b
```

## Volatile in Dettaglio

### Garanzie di `volatile`

1. **Visibilità**: Modifiche immediatamente visibili a tutti i thread
2. **Ordering**: Previene reordering (no istruzioni prima/dopo possono essere riordinate oltre)
3. **NO Atomicità**: `i++` NON è atomico anche con volatile

```java
public class VolatileDemo {
    private volatile int counter = 0;  // Visibilità garantita
    
    // SBAGLIATO: i++ non è atomico
    public void incrementa() {
        counter++;  // Race condition! (read-modify-write)
    }
    
    // CORRETTO: Operazione atomica
    public void set(int value) {
        counter = value;  // OK: semplice assign
    }
    
    public int get() {
        return counter;  // OK: semplice read
    }
}
```

### Volatile vs Synchronized

```java
public class Comparazione {
    private volatile int volatileCounter = 0;
    private int syncCounter = 0;
    
    // Volatile: Visibile ma NON thread-safe per i++
    public void incrementaVolatile() {
        volatileCounter++;  // RACE CONDITION!
    }
    
    // Synchronized: Thread-safe
    public synchronized void incrementaSync() {
        syncCounter++;  // OK: Atomico
    }
}
```

### Idioma: Double-Checked Locking

```java
public class Singleton {
    private static volatile Singleton instance;  // DEVE essere volatile!
    
    public static Singleton getInstance() {
        if (instance == null) {              // 1° check (no lock)
            synchronized(Singleton.class) {
                if (instance == null) {      // 2° check (con lock)
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}
```

**Senza `volatile`:**
1. Thread 1 crea `instance` (costruttore)
2. Thread 2 vede `instance != null` MA oggetto non completamente inizializzato
3. **CRASH!**

**Con `volatile`:**
- Garantisce che `instance` sia completamente inizializzato prima di essere visibile

## Final Fields e Immutabilità

I campi `final` hanno garanzie speciali nel JMM:

```java
public class ImmutablePoint {
    private final int x;  // final
    private final int y;  // final
    
    public ImmutablePoint(int x, int y) {
        this.x = x;
        this.y = y;
    }  // JMM garantisce che x, y siano visibili a tutti i thread dopo costruzione
    
    public int getX() { return x; }
    public int getY() { return y; }
}
```

**Garanzia JMM:**
- Se oggetto correttamente pubblicato (no escape dal costruttore), tutti i campi `final` sono visibili senza sincronizzazione

### ⚠️ Pubblicazione Incorretta

```java
public class PublicazioneScorretta {
    public static ImmutablePoint point;  // Campo statico
    
    public PublicazioneScorretta() {
        point = this;  // ERRORE: escape dal costruttore!
        // Altri thread potrebbero vedere point non completamente inizializzato
    }
}
```

**Soluzione:**

```java
public class PublicazioneCorretta {
    private static ImmutablePoint point;
    
    public PublicazioneCorretta() {
        // Nessun escape
    }
    
    public static void initialize() {
        point = new ImmutablePoint(10, 20);  // OK: dopo costruzione
    }
}
```

## Reordering e Ottimizzazioni

Il compilatore e la CPU possono **riordinare** istruzioni per ottimizzazione, purché non cambino il comportamento in thread singolo.

### Esempio Reordering

```java
public class ReorderingDemo {
    private int a = 0;
    private int b = 0;
    
    // Thread 1
    public void writer() {
        a = 1;  // Può essere riordinato dopo b=2
        b = 2;
    }
    
    // Thread 2
    public void reader() {
        int r1 = b;  // Potrebbe leggere 2
        int r2 = a;  // Ma leggere 0! (se riordinato)
        System.out.println("b=" + r1 + ", a=" + r2);
    }
}
```

**Output possibili:**
- `b=0, a=0` (prima di writer)
- `b=2, a=1` (dopo writer)
- `b=2, a=0` (reordering!)
- `b=0, a=1` (reordering!)

### Prevenzione Reordering

```java
public class NoReordering {
    private int a = 0;
    private volatile int b = 0;  // volatile previene reordering
    
    // Thread 1
    public void writer() {
        a = 1;  // NON può essere riordinato dopo b=2
        b = 2;  // Volatile write
    }
    
    // Thread 2
    public void reader() {
        int r1 = b;  // Volatile read
        int r2 = a;  // NON può essere riordinato prima di b
        // Se r1==2, garantito che r2==1
    }
}
```

## Atomicità e Operazioni Atomiche

### Operazioni Atomiche Naturali

```java
// Atomiche (32-bit)
int i = 42;           // Atomico
boolean b = true;     // Atomico
float f = 3.14f;      // Atomico
Object obj = new Object();  // Atomico (reference)

// NON Atomiche (64-bit senza volatile)
long l = 123456789L;  // NON atomico (2 operazioni 32-bit)
double d = 3.14;      // NON atomico

// Atomiche con volatile
volatile long l = 123456789L;   // Atomico
volatile double d = 3.14;       // Atomico
```

### Operazioni NON Atomiche

```java
i++;               // NON atomico (read-modify-write)
i = i + 1;         // NON atomico
i += 1;            // NON atomico
array[i] = value;  // NON atomico (se i è condiviso)
```

### Esempio: Race Condition

```java
public class RaceCondition {
    private int counter = 0;
    
    public void incrementa() {
        counter++;  // NON atomico!
        // Equivalente a:
        // 1. temp = counter      (read)
        // 2. temp = temp + 1     (modify)
        // 3. counter = temp      (write)
    }
    
    public static void main(String[] args) throws InterruptedException {
        RaceCondition rc = new RaceCondition();
        
        // 1000 thread incrementano 1000 volte ciascuno
        Thread[] threads = new Thread[1000];
        for (int i = 0; i < 1000; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 1000; j++) {
                    rc.incrementa();
                }
            });
            threads[i].start();
        }
        
        for (Thread t : threads) {
            t.join();
        }
        
        System.out.println("Counter: " + rc.counter);
        // Atteso: 1.000.000
        // Reale: ~950.000 (VARIA! Race condition)
    }
}
```

### Soluzione: Synchronized

```java
public class NoRaceCondition {
    private int counter = 0;
    
    public synchronized void incrementa() {
        counter++;  // Atomico grazie a synchronized
    }
}
```

## Safe Publication

Per pubblicare un oggetto in modo sicuro a più thread:

### 1. Inizializzazione Statica

```java
public class SafePublication {
    private static final Object obj = new Object();  // Thread-safe
}
```

### 2. Volatile

```java
public class SafePublication {
    private volatile Object obj;
    
    public void publish() {
        obj = new Object();  // Visibile a tutti
    }
}
```

### 3. Synchronized

```java
public class SafePublication {
    private Object obj;
    
    public synchronized void publish() {
        obj = new Object();
    }
    
    public synchronized Object get() {
        return obj;
    }
}
```

### 4. Final Field

```java
public class SafePublication {
    private final Object obj;
    
    public SafePublication() {
        obj = new Object();  // Safe dopo costruzione
    }
}
```

### 5. Concurrent Collection

```java
import java.util.concurrent.ConcurrentHashMap;

public class SafePublication {
    private static final ConcurrentHashMap<String, Object> map = 
        new ConcurrentHashMap<>();
    
    public void publish(String key, Object value) {
        map.put(key, value);  // Thread-safe
    }
}
```

## Memory Barriers

I **memory barrier** (fence) forzano l'ordinamento e la visibilità.

```
Istruzioni PRIMA del barrier
─────────────────────────────
    MEMORY BARRIER
─────────────────────────────
Istruzioni DOPO il barrier
```

**In Java:**
- `volatile` inserisce barrier
- `synchronized` inserisce barrier
- `final` inserisce barrier
- `Thread.start()` / `join()` inseriscono barrier

## False Sharing e Cache Line

### Problema: False Sharing

```java
public class FalseSharing {
    // Variabili su stessa cache line (64 byte)
    private volatile long var1 = 0;  // Byte 0-7
    private volatile long var2 = 0;  // Byte 8-15
    
    // Thread 1 modifica var1
    // Thread 2 modifica var2
    // Stesso cache line → invalidazione continua → LENTO!
}
```

### Soluzione: Padding

```java
public class NoFalseSharing {
    private volatile long var1 = 0;
    private long p1, p2, p3, p4, p5, p6, p7;  // Padding 56 byte
    private volatile long var2 = 0;
    private long p8, p9, p10, p11, p12, p13, p14;
    
    // var1 e var2 su cache line diverse
}
```

**Java 8+ (più elegante):**

```java
import sun.misc.Contended;

public class NoFalseSharing {
    @Contended
    private volatile long var1 = 0;
    
    @Contended
    private volatile long var2 = 0;
}
// Compilare con: -XX:-RestrictContended
```

## Best Practices

### ✅ DO

1. **Usa volatile per flag semplici**
   ```java
   private volatile boolean running = true;
   ```

2. **Usa synchronized per operazioni composite**
   ```java
   public synchronized void incrementa() {
       counter++;
   }
   ```

3. **Immutabilità quando possibile**
   ```java
   public final class Config {
       private final int value;
       public Config(int value) { this.value = value; }
   }
   ```

4. **Safe publication**
   ```java
   private static final Object INSTANCE = new Object();
   ```

5. **Documenta invarianti di sincronizzazione**
   ```java
   // Guarded by: this
   private int balance = 0;
   ```

### ❌ DON'T

1. **Non usare volatile per operazioni composite**
   ```java
   private volatile int counter = 0;
   counter++;  // RACE CONDITION!
   ```

2. **Non pubblicare `this` dal costruttore**
   ```java
   public class Unsafe {
       public Unsafe() {
           GlobalRegistry.register(this);  // UNSAFE!
       }
   }
   ```

3. **Non assumere atomicità di long/double senza volatile**
   ```java
   private long counter = 0;  // NON atomico!
   ```

4. **Non fare double-checked locking senza volatile**
   ```java
   if (instance == null) {
       synchronized(...) {
           instance = new Singleton();  // DEVE essere volatile!
       }
   }
   ```

## Riepilogo

| Concetto | Scopo | Quando Usare |
|----------|-------|--------------|
| **volatile** | Visibilità + ordering | Flag semplici, double-checked locking |
| **synchronized** | Mutua esclusione + visibilità | Operazioni composite, sezioni critiche |
| **final** | Immutabilità + safe publication | Oggetti immutabili |
| **Happens-Before** | Garanzie di ordinamento | Comprendere visibilità |
| **Memory Barrier** | Forzare ordering | Automatico (volatile, sync) |

---

**[← Precedente: Sincronizzazione](02-sincronizzazione.md)** | **[Prossimo: Executor Framework →](04-executor-framework.md)**
