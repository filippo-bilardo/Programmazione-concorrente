# 2. Sincronizzazione tra Thread in Java

## Perché Sincronizzare?

Quando più thread accedono a **risorse condivise** (variabili, oggetti), possono verificarsi **race condition** e **inconsistenze**.

### Esempio: Race Condition

```java
public class Contatore {
    private int valore = 0;
    
    public void incrementa() {
        valore++;  // NON atomico! È composto da 3 operazioni:
                   // 1. Leggi valore
                   // 2. Incrementa
                   // 3. Scrivi valore
    }
    
    public int getValore() {
        return valore;
    }
}

// Thread 1 e Thread 2 eseguono incrementa() contemporaneamente:
// Thread 1: legge 0
// Thread 2: legge 0
// Thread 1: incrementa a 1
// Thread 2: incrementa a 1
// Thread 1: scrive 1
// Thread 2: scrive 1
// RISULTATO: 1 invece di 2!
```

### Problemi di Concorrenza

1. **Race Condition**: Risultato dipende dall'ordine di esecuzione dei thread
2. **Data Race**: Accesso concorrente non sincronizzato a variabile condivisa (almeno un accesso in scrittura)
3. **Deadlock**: Thread si bloccano a vicenda in attesa di risorse
4. **Starvation**: Thread non riesce mai ad acquisire risorsa
5. **Livelock**: Thread continuano a cambiare stato ma non progrediscono

## La Parola Chiave `synchronized`

Java fornisce `synchronized` per garantire **mutua esclusione**: solo un thread alla volta può eseguire una sezione critica.

### Sincronizzazione di Metodo

```java
public class ContatoreSync {
    private int valore = 0;
    
    // Metodo sincronizzato sull'istanza (this)
    public synchronized void incrementa() {
        valore++;
    }
    
    public synchronized int getValore() {
        return valore;
    }
}
```

**Equivalente a:**

```java
public void incrementa() {
    synchronized(this) {
        valore++;
    }
}
```

### Sincronizzazione di Blocco

```java
public class ContatoreSync {
    private int valore = 0;
    private final Object lock = new Object();
    
    public void incrementa() {
        synchronized(lock) {  // Lock esplicito
            valore++;
        }
    }
    
    public int getValore() {
        synchronized(lock) {
            return valore;
        }
    }
}
```

**Vantaggi blocco sincronizzato:**
- Granularità fine: solo parte del metodo
- Lock multipli per operazioni indipendenti
- Lock su oggetti diversi

### Metodi Statici Sincronizzati

```java
public class Utility {
    private static int counter = 0;
    
    // Sincronizzato sulla classe (Utility.class)
    public static synchronized void incrementaCounter() {
        counter++;
    }
}
```

**Equivalente a:**

```java
public static void incrementaCounter() {
    synchronized(Utility.class) {
        counter++;
    }
}
```

## Monitor e Lock Intrinseci

Ogni oggetto Java ha un **monitor intrinseco** (intrinsic lock):

```
┌───────────────────────────┐
│       OGGETTO             │
│                           │
│  ┌─────────────────────┐  │
│  │   Monitor Lock      │  │
│  │   (1 thread owner)  │  │
│  └─────────────────────┘  │
│                           │
│  ┌─────────────────────┐  │
│  │   Entry Set         │  │
│  │ (thread in attesa   │  │
│  │  del lock)          │  │
│  └─────────────────────┘  │
│                           │
│  ┌─────────────────────┐  │
│  │   Wait Set          │  │
│  │ (thread che hanno   │  │
│  │  chiamato wait())   │  │
│  └─────────────────────┘  │
└───────────────────────────┘
```

### Regole del Monitor

1. **Acquisizione**: Thread acquisisce lock entrando in `synchronized`
2. **Mutua Esclusione**: Solo un thread alla volta possiede il lock
3. **Rilascio**: Lock rilasciato uscendo dal blocco `synchronized`
4. **Rientranza**: Thread può riacquisire lock che già possiede (reentrant lock)

### Esempio Rientranza

```java
public class RientranzaDemo {
    public synchronized void metodoA() {
        System.out.println("In metodoA");
        metodoB();  // Riacquisisce lock (già posseduto)
    }
    
    public synchronized void metodoB() {
        System.out.println("In metodoB");
        // OK: stesso thread, stesso lock
    }
}
```

## Comunicazione tra Thread: wait(), notify(), notifyAll()

I metodi `wait()`, `notify()`, `notifyAll()` permettono ai thread di **comunicare** attraverso il monitor.

### wait()

```java
synchronized(obj) {
    while (!condizione) {
        obj.wait();  // Rilascia lock e attende
    }
    // condizione vera, procedi
}
```

**Effetti:**
1. Thread rilascia il lock
2. Thread passa in **WAITING** (entra nel wait set)
3. Thread aspetta `notify()` o `notifyAll()`
4. Quando svegliato, **riacquisisce il lock** prima di continuare

### notify()

```java
synchronized(obj) {
    // modifica condizione
    obj.notify();  // Sveglia UN thread random dal wait set
}
```

**Effetti:**
- Sveglia un thread casuale dal wait set
- Thread svegliato passa da WAITING a RUNNABLE
- Non rilascia immediatamente il lock

### notifyAll()

```java
synchronized(obj) {
    // modifica condizione
    obj.notifyAll();  // Sveglia TUTTI i thread dal wait set
}
```

**Effetti:**
- Sveglia tutti i thread nel wait set
- Thread competono per acquisire il lock
- **Preferibile** a `notify()` per evitare deadlock

### ⚠️ Regole Importanti

1. **DEVONO essere chiamati dentro `synchronized`**
   ```java
   obj.wait();  // ERRORE: IllegalMonitorStateException
   
   synchronized(obj) {
       obj.wait();  // OK
   }
   ```

2. **Usare SEMPRE in un loop while**
   ```java
   // SBAGLIATO
   if (!condizione) {
       obj.wait();
   }
   
   // CORRETTO (protezione da spurious wakeup)
   while (!condizione) {
       obj.wait();
   }
   ```

3. **Chiamare wait()/notify() sullo STESSO oggetto**
   ```java
   synchronized(objA) {
       objB.wait();  // ERRORE: lock su objA, wait su objB
   }
   ```

## Pattern Producer-Consumer

Uno dei pattern più comuni per la sincronizzazione.

```java
import java.util.LinkedList;
import java.util.Queue;

public class BufferCondiviso {
    private Queue<Integer> buffer = new LinkedList<>();
    private final int MAX_SIZE = 10;
    
    // Producer
    public synchronized void produce(int valore) throws InterruptedException {
        while (buffer.size() == MAX_SIZE) {
            System.out.println("Buffer pieno, producer attende...");
            wait();  // Attende che consumer consumi
        }
        
        buffer.add(valore);
        System.out.println("Prodotto: " + valore + " (buffer size: " + buffer.size() + ")");
        
        notifyAll();  // Notifica consumer
    }
    
    // Consumer
    public synchronized int consume() throws InterruptedException {
        while (buffer.isEmpty()) {
            System.out.println("Buffer vuoto, consumer attende...");
            wait();  // Attende che producer produca
        }
        
        int valore = buffer.remove();
        System.out.println("Consumato: " + valore + " (buffer size: " + buffer.size() + ")");
        
        notifyAll();  // Notifica producer
        return valore;
    }
}

// Uso
class Producer extends Thread {
    private BufferCondiviso buffer;
    
    public Producer(BufferCondiviso buffer) {
        this.buffer = buffer;
    }
    
    @Override
    public void run() {
        try {
            for (int i = 1; i <= 20; i++) {
                buffer.produce(i);
                Thread.sleep(100);  // Simula lavoro
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

class Consumer extends Thread {
    private BufferCondiviso buffer;
    
    public Consumer(BufferCondiviso buffer) {
        this.buffer = buffer;
    }
    
    @Override
    public void run() {
        try {
            for (int i = 1; i <= 20; i++) {
                buffer.consume();
                Thread.sleep(150);  // Simula lavoro
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class ProducerConsumerDemo {
    public static void main(String[] args) {
        BufferCondiviso buffer = new BufferCondiviso();
        
        Producer p = new Producer(buffer);
        Consumer c = new Consumer(buffer);
        
        p.start();
        c.start();
        
        try {
            p.join();
            c.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("Completato!");
    }
}
```

## Deadlock

Un **deadlock** si verifica quando due o più thread si bloccano a vicenda in attesa di risorse.

### Esempio Deadlock

```java
public class DeadlockDemo {
    private final Object lock1 = new Object();
    private final Object lock2 = new Object();
    
    public void metodo1() {
        synchronized(lock1) {
            System.out.println(Thread.currentThread().getName() + ": lock1 acquisito");
            
            try { Thread.sleep(50); } catch (InterruptedException e) {}
            
            System.out.println(Thread.currentThread().getName() + ": attendo lock2...");
            synchronized(lock2) {  // Attende lock2
                System.out.println(Thread.currentThread().getName() + ": lock2 acquisito");
            }
        }
    }
    
    public void metodo2() {
        synchronized(lock2) {
            System.out.println(Thread.currentThread().getName() + ": lock2 acquisito");
            
            try { Thread.sleep(50); } catch (InterruptedException e) {}
            
            System.out.println(Thread.currentThread().getName() + ": attendo lock1...");
            synchronized(lock1) {  // Attende lock1
                System.out.println(Thread.currentThread().getName() + ": lock1 acquisito");
            }
        }
    }
    
    public static void main(String[] args) {
        DeadlockDemo demo = new DeadlockDemo();
        
        Thread t1 = new Thread(() -> demo.metodo1(), "Thread-1");
        Thread t2 = new Thread(() -> demo.metodo2(), "Thread-2");
        
        t1.start();
        t2.start();
        
        // DEADLOCK:
        // Thread-1 ha lock1, attende lock2
        // Thread-2 ha lock2, attende lock1
        // Nessuno può procedere!
    }
}
```

### Condizioni per Deadlock (Coffman Conditions)

Un deadlock si verifica se sono vere TUTTE e 4:

1. **Mutua Esclusione**: Risorsa accessibile a un solo thread
2. **Hold and Wait**: Thread tiene risorse e attende altre
3. **No Preemption**: Risorse non possono essere forzatamente rilasciate
4. **Attesa Circolare**: Ciclo di thread che si attendono a vicenda

### Prevenzione Deadlock

#### 1. Ordine Globale dei Lock

```java
// SOLUZIONE: Sempre acquisire lock nello stesso ordine
public void metodo1() {
    synchronized(lock1) {    // Prima lock1
        synchronized(lock2) { // Poi lock2
            // ...
        }
    }
}

public void metodo2() {
    synchronized(lock1) {    // Prima lock1 (stesso ordine!)
        synchronized(lock2) { // Poi lock2
            // ...
        }
    }
}
```

#### 2. Lock con Timeout (java.util.concurrent.locks)

```java
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

Lock lock1 = new ReentrantLock();
Lock lock2 = new ReentrantLock();

if (lock1.tryLock()) {
    try {
        if (lock2.tryLock()) {
            try {
                // Sezione critica
            } finally {
                lock2.unlock();
            }
        }
    } finally {
        lock1.unlock();
    }
}
```

#### 3. Lock Singolo

```java
// Usa un unico lock per tutte le risorse correlate
private final Object lock = new Object();

public void metodo1() {
    synchronized(lock) {
        // accesso a resource1 e resource2
    }
}

public void metodo2() {
    synchronized(lock) {
        // accesso a resource1 e resource2
    }
}
```

## Volatile

La parola chiave `volatile` garantisce **visibilità** delle modifiche tra thread.

```java
public class VolatileDemo {
    private volatile boolean flag = false;  // volatile!
    
    public void thread1() {
        while (!flag) {
            // attende...
        }
        System.out.println("Flag diventa true!");
    }
    
    public void thread2() {
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {}
        flag = true;  // Modifica visibile a tutti i thread
        System.out.println("Flag impostato a true");
    }
}
```

### Volatile vs Synchronized

| Aspetto | volatile | synchronized |
|---------|----------|--------------|
| **Visibilità** | ✅ Garantita | ✅ Garantita |
| **Atomicità** | ❌ NO (solo per assign/read) | ✅ SI |
| **Lock** | ❌ NO | ✅ SI |
| **Performance** | Veloce | Più lento |
| **Uso** | Flag semplici | Operazioni complesse |

**Quando usare `volatile`:**
- Flag booleani
- Lettura/scrittura atomica (int, long, reference)
- **NON** per `i++` (non atomico)

**Quando usare `synchronized`:**
- Operazioni composite (`i++`, check-then-act)
- Multipli statement atomici
- Modifiche a più variabili correlate

### Esempio: Flag di Terminazione

```java
public class Worker extends Thread {
    private volatile boolean running = true;  // volatile per visibilità
    
    public void run() {
        while (running) {
            // lavoro...
        }
        System.out.println("Thread terminato");
    }
    
    public void stopWorker() {
        running = false;  // Visibile immediatamente ad altri thread
    }
}
```

## Immutabilità: La Sincronizzazione Naturale

Gli oggetti **immutabili** sono automaticamente thread-safe.

```java
public final class PersonaImmutabile {
    private final String nome;
    private final int eta;
    
    public PersonaImmutabile(String nome, int eta) {
        this.nome = nome;
        this.eta = eta;
    }
    
    public String getNome() { return nome; }
    public int getEta() { return eta; }
    
    // Nessun setter! Non può essere modificato dopo la creazione
}
```

**Vantaggi:**
- Thread-safe senza sincronizzazione
- Nessun overhead di lock
- Cacheable
- Prevedibile

**Regole per immutabilità:**
1. Classe `final` (non estendibile)
2. Tutti i campi `final`
3. Tutti i campi privati
4. Nessun setter
5. Inizializzazione completa nel costruttore
6. Nessuna fuga di riferimenti mutabili

## Best Practices

### ✅ DO

1. **Minimizza ambito sincronizzato**
   ```java
   // SBAGLIATO: Sincronizza tutto il metodo
   public synchronized void metodo() {
       // operazioni non critiche
       // sezione critica
       // altre operazioni
   }
   
   // CORRETTO: Sincronizza solo sezione critica
   public void metodo() {
       // operazioni non critiche
       synchronized(this) {
           // sezione critica
       }
       // altre operazioni
   }
   ```

2. **Usa notifyAll() invece di notify()**
   ```java
   synchronized(obj) {
       // modifica stato
       obj.notifyAll();  // Sveglia tutti i thread
   }
   ```

3. **wait() sempre in loop while**
   ```java
   synchronized(obj) {
       while (!condizione) {
           obj.wait();
       }
   }
   ```

4. **Documenta lock order**
   ```java
   // Lock order: lock1 -> lock2 (sempre in quest'ordine)
   synchronized(lock1) {
       synchronized(lock2) {
           // ...
       }
   }
   ```

5. **Preferisci immutabilità**
   ```java
   public final class Config {
       private final int timeout;
       public Config(int timeout) { this.timeout = timeout; }
       public int getTimeout() { return timeout; }
   }
   ```

### ❌ DON'T

1. **Non sincronizzare su oggetti mutabili**
   ```java
   // ERRORE: String può cambiare riferimento
   private String lock = "lock";
   synchronized(lock) { ... }
   
   // CORRETTO: Object finale
   private final Object lock = new Object();
   synchronized(lock) { ... }
   ```

2. **Non usare notify() se non sei certo**
   ```java
   obj.notify();     // Può lasciare thread in attesa
   obj.notifyAll();  // Sicuro: sveglia tutti
   ```

3. **Non acquisire lock in ordine diverso**
   ```java
   // DEADLOCK RISK!
   void metodo1() {
       synchronized(lockA) {
           synchronized(lockB) { ... }
       }
   }
   
   void metodo2() {
       synchronized(lockB) {  // Ordine inverso!
           synchronized(lockA) { ... }
       }
   }
   ```

4. **Non sincronizzare su Boolean/Integer/String**
   ```java
   // ERRORE: Boxing può creare nuovi oggetti
   private Boolean flag = true;
   synchronized(flag) { ... }  // UNSAFE
   ```

## Riepilogo

| Concetto | Scopo | Uso |
|----------|-------|-----|
| **synchronized** | Mutua esclusione | Blocchi/metodi critici |
| **wait()** | Attesa condizione | Producer-consumer |
| **notify()** | Sveglia thread | Segnalazione cambio stato |
| **notifyAll()** | Sveglia tutti | Broadcast eventi |
| **volatile** | Visibilità | Flag semplici |
| **immutabile** | Thread-safety naturale | Oggetti condivisi |

---

**[← Precedente: Introduzione](01-introduzione-thread.md)** | **[Prossimo: Java Memory Model →](03-memory-model.md)**
