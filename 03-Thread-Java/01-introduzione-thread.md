# 1. Introduzione ai Thread in Java

## Cos'è un Thread?

Un **thread** (filo di esecuzione) è la più piccola unità di esecuzione gestibile dal sistema operativo. A differenza dei processi, i thread condividono lo stesso spazio di memoria del processo che li contiene.

### Thread vs Processi

```
PROCESSO                           THREAD
┌─────────────────────┐           ┌─────────────────────┐
│  Spazio Indirizzi   │           │  Stack Thread 1     │
│  Proprio            │           ├─────────────────────┤
│                     │           │  Stack Thread 2     │
├─────────────────────┤           ├─────────────────────┤
│  File Descriptor    │           │  Stack Thread N     │
│  Tabella            │           ├─────────────────────┤
├─────────────────────┤           │  HEAP Condiviso     │
│  Codice             │           ├─────────────────────┤
└─────────────────────┘           │  Codice Condiviso   │
Isolato, pesante                  └─────────────────────┘
Comunicazione: IPC                Leggeri, condivisione
                                  Comunicazione: memoria
```

### Caratteristiche Thread

| Aspetto | Descrizione |
|---------|-------------|
| **Spazio Memoria** | Condiviso con altri thread dello stesso processo |
| **Stack** | Privato per ogni thread |
| **Heap** | Condiviso tra tutti i thread |
| **Variabili Locali** | Private (sullo stack) |
| **Variabili Istanza/Classe** | Condivise |
| **Creazione** | Molto veloce (nessun fork) |
| **Context Switch** | Veloce (stesso spazio indirizzi) |
| **Comunicazione** | Diretta via memoria condivisa |

## Thread in Java

Java fornisce supporto nativo per la programmazione multi-thread attraverso:

1. **Classe `Thread`** - rappresenta un thread di esecuzione
2. **Interfaccia `Runnable`** - definisce il codice da eseguire
3. **Package `java.util.concurrent`** - API avanzate per concorrenza

### Stati di un Thread Java

```
        ┌──────────┐
        │   NEW    │ thread creato ma non avviato
        └────┬─────┘
             │ start()
             ↓
        ┌──────────┐
   ┌───→│ RUNNABLE │←───┐ pronto per esecuzione
   │    └────┬─────┘    │
   │         │           │
   │         ↓           │
   │    ┌──────────┐    │
   │    │ RUNNING  │────┘ in esecuzione
   │    └────┬─────┘
   │         │
   │         ├──────────────→ wait() ────→ ┌─────────┐
   │         │                              │ WAITING │
   │         │                notify() ←────└─────────┘
   │         │
   │         ├──────────────→ sleep() ───→ ┌──────────────┐
   │         │                             │ TIMED_WAITING│
   │         │              timeout ←──────└──────────────┘
   │         │
   │         ↓
   │    ┌──────────┐
   └────│ BLOCKED  │ in attesa di un lock
        └────┬─────┘
             │
             ↓
        ┌──────────┐
        │TERMINATED│ esecuzione completata
        └──────────┘
```

### Ciclo di Vita Thread

```java
Thread t = new Thread();  // NEW
t.start();                // RUNNABLE → RUNNING
// esecuzione...
t.sleep(1000);           // TIMED_WAITING
// dopo 1s...
synchronized(obj) {      // BLOCKED se lock occupato
    obj.wait();          // WAITING
    obj.notify();        // torna RUNNABLE
}
// termina run()
// → TERMINATED
```

## Creazione Thread: Due Approcci

### 1. Estendere la Classe Thread

```java
class MioThread extends Thread {
    @Override
    public void run() {
        // codice da eseguire nel thread
        System.out.println("Thread: " + getName());
    }
}

// Uso
MioThread t = new MioThread();
t.start();  // NON chiamare run() direttamente!
```

**Vantaggi:**
- Sintassi semplice
- Accesso diretto ai metodi della classe Thread

**Svantaggi:**
- Non si può estendere un'altra classe (Java singola ereditarietà)
- Accoppiamento stretto

### 2. Implementare Interfaccia Runnable (CONSIGLIATO)

```java
class MioRunnable implements Runnable {
    @Override
    public void run() {
        // codice da eseguire nel thread
        System.out.println("Thread: " + Thread.currentThread().getName());
    }
}

// Uso
Thread t = new Thread(new MioRunnable());
t.start();
```

**Vantaggi:**
- Può estendere altre classi
- Separazione tra codice (Runnable) e meccanismo di esecuzione (Thread)
- Può essere usato con Executor framework
- Design più flessibile

**Svantaggi:**
- Leggermente più verboso

### 3. Lambda Expression (Java 8+)

```java
Thread t = new Thread(() -> {
    System.out.println("Thread con lambda!");
});
t.start();
```

**Vantaggi:**
- Sintassi concisa
- Ideale per task semplici

## API Fondamentali della Classe Thread

### Costruttori

```java
Thread()                          // Nome automatico (Thread-0, Thread-1...)
Thread(String name)               // Nome personalizzato
Thread(Runnable target)           // Con oggetto Runnable
Thread(Runnable target, String name)
Thread(ThreadGroup group, Runnable target, String name)
```

### Metodi di Istanza

```java
void start()              // Avvia il thread (chiama run() in nuovo thread)
void run()               // Codice eseguito nel thread (da sovrascrivere)
String getName()         // Restituisce il nome del thread
void setName(String)     // Imposta il nome
long getId()             // Restituisce ID univoco
State getState()         // Restituisce lo stato corrente
boolean isAlive()        // true se thread è ancora in esecuzione
void join()              // Attende terminazione del thread
void join(long millis)   // Attende max millis millisecondi
int getPriority()        // Restituisce priorità (1-10)
void setPriority(int)    // Imposta priorità
ThreadGroup getThreadGroup() // Restituisce gruppo di appartenenza
```

### Metodi Statici

```java
static Thread currentThread()    // Riferimento al thread corrente
static void sleep(long millis)   // Sospende esecuzione per millis ms
static void yield()              // Suggerisce scheduler di cedere CPU
static boolean interrupted()     // Verifica e azzera flag interruzione
```

### Metodi Deprecati (NON USARE)

```java
@Deprecated void stop()       // Termina forzatamente (UNSAFE)
@Deprecated void suspend()    // Sospende (può causare deadlock)
@Deprecated void resume()     // Riprende (UNSAFE)
```

## Esempio Completo: Confronto Approcci

```java
public class ThreadApprocci {
    
    // Approccio 1: Estendere Thread
    static class ContatoreThread extends Thread {
        private String nome;
        
        public ContatoreThread(String nome) {
            this.nome = nome;
        }
        
        @Override
        public void run() {
            for (int i = 1; i <= 5; i++) {
                System.out.println(nome + ": " + i);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    System.out.println(nome + " interrotto");
                }
            }
        }
    }
    
    // Approccio 2: Implementare Runnable
    static class ContatoreRunnable implements Runnable {
        private String nome;
        
        public ContatoreRunnable(String nome) {
            this.nome = nome;
        }
        
        @Override
        public void run() {
            for (int i = 1; i <= 5; i++) {
                System.out.println(nome + ": " + i);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    System.out.println(nome + " interrotto");
                }
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Approccio 1: Estendere Thread ===");
        ContatoreThread t1 = new ContatoreThread("ThreadA");
        ContatoreThread t2 = new ContatoreThread("ThreadB");
        t1.start();
        t2.start();
        
        try {
            t1.join();  // Attende terminazione t1
            t2.join();  // Attende terminazione t2
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("\n=== Approccio 2: Implementare Runnable ===");
        Thread t3 = new Thread(new ContatoreRunnable("RunnableA"));
        Thread t4 = new Thread(new ContatoreRunnable("RunnableB"));
        t3.start();
        t4.start();
        
        try {
            t3.join();
            t4.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("\n=== Approccio 3: Lambda ===");
        Thread t5 = new Thread(() -> {
            for (int i = 1; i <= 5; i++) {
                System.out.println("LambdaThread: " + i);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    System.out.println("Lambda interrotto");
                }
            }
        });
        t5.start();
        
        try {
            t5.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("\nTutti i thread completati!");
    }
}
```

### Output Atteso (ordine può variare)

```
=== Approccio 1: Estendere Thread ===
ThreadA: 1
ThreadB: 1
ThreadA: 2
ThreadB: 2
ThreadA: 3
ThreadB: 3
ThreadA: 4
ThreadB: 4
ThreadA: 5
ThreadB: 5

=== Approccio 2: Implementare Runnable ===
RunnableA: 1
RunnableB: 1
RunnableA: 2
RunnableB: 2
...
```

## Thread Daemon

I **daemon thread** sono thread di "servizio" che non impediscono la terminazione della JVM.

```java
Thread daemon = new Thread(() -> {
    while (true) {
        System.out.println("Daemon in esecuzione...");
        Thread.sleep(1000);
    }
});
daemon.setDaemon(true);  // DEVE essere chiamato PRIMA di start()
daemon.start();

// Quando tutti i thread non-daemon terminano,
// la JVM termina anche se daemon è ancora in esecuzione
```

**Usi comuni:**
- Garbage Collector
- Finalizer
- Monitor di sistema
- Background jobs

## Priorità Thread

Java assegna priorità da 1 (MIN_PRIORITY) a 10 (MAX_PRIORITY), default 5 (NORM_PRIORITY).

```java
Thread t1 = new Thread(() -> System.out.println("Task 1"));
Thread t2 = new Thread(() -> System.out.println("Task 2"));

t1.setPriority(Thread.MAX_PRIORITY);  // 10
t2.setPriority(Thread.MIN_PRIORITY);  // 1

t1.start();
t2.start();
```

**⚠️ ATTENZIONE:**
- La priorità è solo un **suggerimento** allo scheduler
- Il comportamento dipende dal sistema operativo
- Non garantisce ordine di esecuzione
- Non fare affidamento per logica critica

## Thread Groups

I thread possono essere organizzati in gruppi gerarchici:

```java
ThreadGroup gruppo = new ThreadGroup("MioGruppo");
Thread t1 = new Thread(gruppo, () -> System.out.println("T1"));
Thread t2 = new Thread(gruppo, () -> System.out.println("T2"));

t1.start();
t2.start();

// Operazioni di gruppo
System.out.println("Thread attivi: " + gruppo.activeCount());
gruppo.list();  // Stampa info su tutti i thread
```

**Usi:**
- Organizzazione logica
- Operazioni batch (interrupt, setMaxPriority)
- Gestione sicurezza (SecurityManager)

## Metodo sleep() vs yield()

### sleep(long millis)

```java
try {
    Thread.sleep(1000);  // Sospende per 1 secondo
} catch (InterruptedException e) {
    // Gestione interruzione
}
```

- Thread passa in stato **TIMED_WAITING**
- Rilascia CPU ma **NON rilascia lock**
- Può essere interrotto con `interrupt()`

### yield()

```java
Thread.yield();  // Suggerisce di cedere la CPU
```

- Thread rimane in stato **RUNNABLE**
- Suggerimento allo scheduler (può essere ignorato)
- Non rilascia lock
- Uso raro nelle applicazioni moderne

## Best Practices

### ✅ DO

1. **Usa Runnable invece di estendere Thread**
   ```java
   Thread t = new Thread(() -> { /* task */ });
   ```

2. **Dai nomi significativi ai thread**
   ```java
   Thread t = new Thread(task, "DatabaseWorker");
   ```

3. **Gestisci sempre InterruptedException**
   ```java
   try {
       Thread.sleep(1000);
   } catch (InterruptedException e) {
       Thread.currentThread().interrupt(); // Ripristina flag
       // cleanup
   }
   ```

4. **Usa join() per attendere terminazione**
   ```java
   thread.start();
   thread.join();  // Attende completamento
   ```

5. **Preferisci Executor Framework per gestione avanzata**
   ```java
   ExecutorService executor = Executors.newFixedThreadPool(5);
   executor.submit(() -> { /* task */ });
   ```

### ❌ DON'T

1. **Non chiamare run() direttamente**
   ```java
   thread.run();   // ERRORE: esegue nel thread corrente!
   thread.start(); // CORRETTO: crea nuovo thread
   ```

2. **Non usare metodi deprecati**
   ```java
   thread.stop();    // UNSAFE - non usare
   thread.suspend(); // UNSAFE - non usare
   ```

3. **Non fare affidamento sulla priorità per logica critica**
   ```java
   // ERRORE: comportamento non garantito
   if (thread.getPriority() == Thread.MAX_PRIORITY) { ... }
   ```

4. **Non ignorare InterruptedException**
   ```java
   try {
       Thread.sleep(1000);
   } catch (InterruptedException e) {
       // ERRORE: non lasciare vuoto!
   }
   ```

## Riepilogo

| Concetto | Descrizione |
|----------|-------------|
| **Thread** | Unità di esecuzione leggera che condivide memoria con altri thread |
| **Runnable** | Interfaccia funzionale che definisce il task (`run()`) |
| **start()** | Avvia nuovo thread (chiama `run()` internamente) |
| **run()** | Metodo contenente il codice da eseguire nel thread |
| **join()** | Attende terminazione di un thread |
| **sleep()** | Sospende thread per un tempo specificato |
| **interrupt()** | Segnala a un thread di interrompere l'esecuzione |
| **Daemon** | Thread di background che non impedisce terminazione JVM |

## Prossimi Passi

Nei prossimi capitoli approfondiremo:

1. **Sincronizzazione** - `synchronized`, lock, problemi di concorrenza
2. **Comunicazione tra Thread** - `wait()`, `notify()`, `notifyAll()`
3. **Java Memory Model** - visibilità, `volatile`, ordering
4. **Executor Framework** - gestione avanzata pool di thread
5. **Concurrent Collections** - strutture dati thread-safe

---

**[← Torna al README Modulo 3](README.md)** | **[Prossimo: Sincronizzazione →](02-sincronizzazione.md)**
