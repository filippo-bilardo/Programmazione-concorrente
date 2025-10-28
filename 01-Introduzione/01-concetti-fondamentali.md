# Modulo 1 - Concetti Fondamentali della Programmazione Concorrente

## Indice
- [Introduzione](#introduzione)
- [Programmi Sequenziali vs Programmi Concorrenti](#programmi-sequenziali-vs-programmi-concorrenti)
- [Vantaggi della Concorrenza](#vantaggi-della-concorrenza)
- [Sfide della Concorrenza](#sfide-della-concorrenza)
- [Parallelismo vs Concorrenza](#parallelismo-vs-concorrenza)
- [Modelli di Esecuzione](#modelli-di-esecuzione)
- [Multitasking nella Vita Quotidiana](#multitasking-nella-vita-quotidiana)
- [Polling e Interrupt](#polling-e-interrupt)
- [Multitasking nei Microcontrollori](#multitasking-nei-microcontrollori)
- [Esempi Pratici](#esempi-pratici)

---

## Introduzione

La **programmazione concorrente** è un paradigma di programmazione che permette l'esecuzione di più attività in modo apparentemente simultaneo. È fondamentale per:
- Sistemi operativi moderni
- Applicazioni server
- Interfacce utente reattive
- Elaborazione di grandi quantità di dati
- Sistemi embedded e real-time

La **programmazione parallela** è un approccio che sfrutta l'esecuzione simultanea di più processi o thread su architetture multi-core o distribuite, permettendo di dividere compiti complessi in sottocompiti indipendenti che vengono elaborati contemporaneamente. È essenziale per ottimizzare le prestazioni in applicazioni che richiedono elevata potenza di calcolo, come simulazioni scientifiche, elaborazione di immagini o machine learning. 

---

## Programmi Sequenziali vs Programmi Concorrenti

### Programmi Sequenziali

Un programma **sequenziale** esegue le istruzioni una dopo l'altra in un ordine ben definito:

```python
# Esempio: programma sequenziale
def download_file(url):
    print(f"Downloading {url}...")
    time.sleep(2)  # Simula download
    print(f"Downloaded {url}")

# Esecuzione sequenziale
download_file("file1.txt")  # 2 secondi
download_file("file2.txt")  # 2 secondi
download_file("file3.txt")  # 2 secondi
# Tempo totale: 6 secondi
```

**Caratteristiche:**
- ✅ Facile da ragionare e debuggare
- ✅ Comportamento deterministico
- ✅ Nessun problema di sincronizzazione
- ❌ Non sfrutta risorse multiple
- ❌ Tempo di esecuzione più lungo
- ❌ Interfaccia bloccata durante operazioni lunghe

### Programmi Concorrenti

Un programma **concorrente** può eseguire più attività che progrediscono in modo intercalato o parallelo:

```python
# Esempio: programma concorrente
import threading

def download_file(url):
    print(f"Downloading {url}...")
    time.sleep(2)
    print(f"Downloaded {url}")

# Esecuzione concorrente
threads = []
for url in ["file1.txt", "file2.txt", "file3.txt"]:
    t = threading.Thread(target=download_file, args=(url,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()
# Tempo totale: ~2 secondi
```

**Caratteristiche:**
- ✅ Migliore utilizzo delle risorse
- ✅ Maggiore throughput
- ✅ Interfaccia reattiva
- ❌ Più complesso da progettare
- ❌ Possibili race condition
- ❌ Comportamento non deterministico

---

## Vantaggi della Concorrenza

### 1. Migliore Utilizzo delle Risorse

```
CPU Single-core con 2 task:
┌─────────────────────────────────────┐
│ T1 ███  T2 ██  T1 ███  T2 ██  T1 █  │ → Interleaving
└─────────────────────────────────────┘
Tempo di idle minimizzato
```

### 2. Reattività

Un'applicazione può rispondere all'utente mentre esegue operazioni lunghe in background:

```java
// GUI thread rimane reattivo
button.setOnClickListener(() -> {
    // Operazione pesante in thread separato
    new Thread(() -> {
        processData();  // Non blocca la UI
    }).start();
});
```

### 3. Throughput

Server web con thread pool:
```
Request 1 → Thread 1 ───┐
Request 2 → Thread 2 ───┤
Request 3 → Thread 3 ───┼→ Processed requests
Request 4 → Thread 4 ───┤
Request 5 → Thread 5 ───┘
```

### 4. Modularità

Separare logicamente attività indipendenti:
- Thread UI
- Thread networking
- Thread database
- Thread logging

---

## Sfide della Concorrenza

### 1. Race Condition
Due thread accedono alla stessa variabile contemporaneamente:
```python
# Problema: incremento non atomico
counter = 0

def increment():
    global counter
    for _ in range(1000):
        counter += 1  # NON atomico!

# Due thread
t1 = threading.Thread(target=increment)
t2 = threading.Thread(target=increment)
t1.start(); t2.start()
t1.join(); t2.join()
print(counter)  # Potrebbe essere < 2000!
```

### 2. Deadlock
Attesa circolare di risorse:
```
Thread A: lock(R1) → wait(R2)
Thread B: lock(R2) → wait(R1)
Result: DEADLOCK!
```

### 3. Starvation
Un thread non ottiene mai le risorse:
```
High priority threads monopolize CPU
↓
Low priority thread never runs
```

### 4. Comportamento Non Deterministico
L'output può variare tra esecuzioni diverse:
```python
def task1():
    print("A")
    print("B")

def task2():
    print("C")
    print("D")

# Output possibili: ABCD, ACBD, ACDB, CABD, CADB, CDAB
```

---

## Parallelismo vs Concorrenza

### Concorrenza (Concurrency)
**Gestione** di più task che progrediscono in modo intercalato:

```
Single Core - Concorrenza tramite time-slicing:
Time: 0ms   10ms  20ms  30ms  40ms  50ms
      │ T1  │ T2  │ T1  │ T2  │ T1  │
      └─────┴─────┴─────┴─────┴─────┘
```

**Esempio:** Un cuoco che prepara più piatti alternando le attività.

### Parallelismo (Parallelism)
**Esecuzione simultanea** su risorse diverse:

```
Multi Core - Parallelismo reale:
Core 1: │ T1──────────────────────────│
Core 2: │ T2──────────────────────────│
Core 3: │ T3──────────────────────────│
Core 4: │ T4──────────────────────────│
```

**Esempio:** Quattro cuochi che preparano quattro piatti contemporaneamente.

### Relazione tra i due concetti

> "Concurrency is about **dealing with** lots of things at once.  
> Parallelism is about **doing** lots of things at once."  
> — Rob Pike

```
┌──────────────────────────────────────┐
│  Concorrenza SENZA parallelismo:     │
│  • Single-core con context switching │
│  • Async I/O                         │
├──────────────────────────────────────┤
│  Concorrenza CON parallelismo:       │
│  • Multi-core                        │
│  • GPU computing                     │
└──────────────────────────────────────┘
```

---

## Modelli di Esecuzione

### 1. Multitasking

#### Multitasking Cooperativo
I task cedono volontariamente il controllo:
```python
def task1():
    work()
    yield_control()  # Cede il controllo
    more_work()
```
**Problema:** Un task che non cede può bloccare tutto.

#### Multitasking Preemptive
Il sistema operativo interrompe i task forzatamente:
```
OS Scheduler:
├─ Task A (10ms) → preempted
├─ Task B (10ms) → preempted
├─ Task C (10ms) → preempted
└─ Task A (continue...)
```

### 2. Multiprocessing

Processi separati con spazi di memoria indipendenti:

```python
# Python multiprocessing
from multiprocessing import Process

def worker(n):
    print(f"Worker {n}")

processes = [Process(target=worker, args=(i,)) for i in range(4)]
for p in processes:
    p.start()
for p in processes:
    p.join()
```

**Caratteristiche:**
- ✅ Isolamento completo
- ✅ Stabilità (crash di un processo non affetta gli altri)
- ❌ Overhead maggiore
- ❌ Comunicazione più complessa (IPC)

### 3. Multithreading

Thread che condividono lo stesso spazio di memoria:

```java
// Java threads
Thread t1 = new Thread(() -> {
    System.out.println("Thread 1");
});
Thread t2 = new Thread(() -> {
    System.out.println("Thread 2");
});
t1.start();
t2.start();
```

**Caratteristiche:**
- ✅ Overhead ridotto
- ✅ Comunicazione facile (memoria condivisa)
- ❌ Necessità di sincronizzazione
- ❌ Bug più difficili da trovare

### 4. Actor Model

Attori indipendenti che comunicano tramite messaggi:

```
Actor A ──message──> Actor B
   │                    │
   └──message──> Actor C
```

Nessuna memoria condivisa, solo scambio di messaggi.

---

## Multitasking nella Vita Quotidiana

### Esempio: Preparare la Colazione

**Approccio Sequenziale:**
```
1. Bollire acqua (5 min)        │█████│
2. Preparare caffè (2 min)            │██│
3. Tostare pane (3 min)                  │███│
4. Friggere uova (4 min)                     │████│
                                ─────────────────────
Tempo totale: 14 minuti
```

**Approccio Concorrente:**
```
1. Bollire acqua (5 min)        │█████│
   └─ Mentre bolle:
      - Tostare pane (3 min)    │███│
      - Preparare tavola (2 min) │██│
2. Preparare caffè (2 min)            │██│
3. Friggere uova (4 min)        │████│
                                ────────
Tempo totale: 7 minuti (quasi la metà!)
```

**Concetti applicati:**
- Overlapping di task indipendenti
- Utilizzo del tempo di attesa
- Cambio di contesto tra attività

---

## Polling e Interrupt

### Polling

Controllo ripetuto dello stato:

```c
// Polling - inefficiente
while (1) {
    if (data_ready()) {  // Check continuo
        process_data();
    }
    // CPU sempre occupata!
}
```

**Caratteristiche:**
- ❌ Spreco di CPU
- ❌ Latenza variabile
- ✅ Semplice da implementare
- ✅ Comportamento deterministico

### Interrupt

Notifica asincrona dall'hardware:

```c
// Interrupt - efficiente
void interrupt_handler() {
    process_data();
}

void main() {
    setup_interrupt(interrupt_handler);
    while (1) {
        // CPU può fare altro o sleep
        do_other_work();
    }
}
```

**Caratteristiche:**
- ✅ CPU disponibile per altro
- ✅ Risposta immediata
- ❌ Più complesso
- ❌ Possibili race condition

### Confronto

```
Polling:
CPU: ████████████████████████ (100% busy checking)
Event: ────────────█──────────── (evento raro)

Interrupt:
CPU: █──█──█────█──█──█─█────── (idle quando possibile)
Event: ────────────█──────────── (risposta immediata)
```

---

## Multitasking nei Microcontrollori

### Sfide nei Sistemi Embedded

1. **Risorse limitate**
   - RAM ridotta (KB, non MB)
   - CPU più lente
   - Nessun SO completo

2. **Real-time constraints**
   - Tempi di risposta garantiti
   - Priorità dei task critiche

### Approcci Comuni

#### 1. Super Loop con State Machine

```c
void main() {
    init_hardware();
    while (1) {
        task1_state_machine();
        task2_state_machine();
        task3_state_machine();
        // Non bloccanti!
    }
}
```

#### 2. RTOS (Real-Time Operating System)

```c
// FreeRTOS example
void task1(void *params) {
    while (1) {
        do_work();
        vTaskDelay(100);  // Yield
    }
}

void task2(void *params) {
    while (1) {
        do_other_work();
        vTaskDelay(50);
    }
}

int main() {
    xTaskCreate(task1, "Task1", 128, NULL, 1, NULL);
    xTaskCreate(task2, "Task2", 128, NULL, 2, NULL);
    vTaskStartScheduler();
}
```

#### 3. Interrupt-Driven Programming

```c
volatile bool sensor_data_ready = false;

// ISR (Interrupt Service Routine)
void SENSOR_IRQHandler(void) {
    read_sensor_data();
    sensor_data_ready = true;
}

void main() {
    setup_sensor_interrupt();
    while (1) {
        if (sensor_data_ready) {
            process_sensor_data();
            sensor_data_ready = false;
        }
        // Altre attività
    }
}
```

### Best Practices per Microcontrollori

1. **ISR corti e veloci**
   ```c
   // GOOD: ISR veloce
   void ISR() {
       flag = true;  // Solo segnala
   }
   
   // BAD: ISR lento
   void ISR() {
       complex_processing();  // NO!
   }
   ```

2. **Variabili volatili**
   ```c
   volatile uint8_t shared_var;  // Previene ottimizzazioni pericolose
   ```

3. **Disabilitare interrupt per sezioni critiche**
   ```c
   disable_interrupts();
   critical_section();
   enable_interrupts();
   ```

---

## Esempi Pratici

### Esempio 1: Server Web Concorrente (Python)

```python
import socket
import threading

def handle_client(client_socket):
    request = client_socket.recv(1024)
    print(f"Received: {request.decode()}")
    
    response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello!</h1>"
    client_socket.send(response.encode())
    client_socket.close()

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 8080))
    server.listen(5)
    print("Server listening on port 8080")
    
    while True:
        client, address = server.accept()
        print(f"Connection from {address}")
        # Un thread per ogni client
        client_thread = threading.Thread(target=handle_client, args=(client,))
        client_thread.start()

if __name__ == "__main__":
    main()
```

### Esempio 2: Producer-Consumer (Java)

```java
import java.util.concurrent.*;

class ProducerConsumer {
    static BlockingQueue<Integer> queue = new LinkedBlockingQueue<>(10);
    
    static class Producer implements Runnable {
        public void run() {
            try {
                for (int i = 0; i < 20; i++) {
                    queue.put(i);
                    System.out.println("Produced: " + i);
                    Thread.sleep(100);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
    
    static class Consumer implements Runnable {
        public void run() {
            try {
                while (true) {
                    Integer item = queue.take();
                    System.out.println("Consumed: " + item);
                    Thread.sleep(150);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
    
    public static void main(String[] args) {
        new Thread(new Producer()).start();
        new Thread(new Consumer()).start();
    }
}
```

### Esempio 3: Parallel Data Processing (Python)

```python
from multiprocessing import Pool
import time

def process_chunk(data_chunk):
    """Elabora un chunk di dati"""
    result = sum(x * x for x in data_chunk)
    return result

def main():
    # Grandi quantità di dati
    data = list(range(1, 10_000_001))
    
    # Sequenziale
    start = time.time()
    result_seq = sum(x * x for x in data)
    time_seq = time.time() - start
    print(f"Sequential: {time_seq:.2f}s")
    
    # Parallelo
    start = time.time()
    chunk_size = len(data) // 4
    chunks = [data[i:i+chunk_size] for i in range(0, len(data), chunk_size)]
    
    with Pool(processes=4) as pool:
        results = pool.map(process_chunk, chunks)
    result_par = sum(results)
    time_par = time.time() - start
    print(f"Parallel: {time_par:.2f}s")
    print(f"Speedup: {time_seq/time_par:.2f}x")

if __name__ == "__main__":
    main()
```

---

## Esercizi

### Esercizio 1: Download Manager
Creare un programma che scarica 5 file contemporaneamente usando thread.

### Esercizio 2: Task Scheduler
Implementare un semplice scheduler che esegue task con priorità diverse.

### Esercizio 3: Chat Server
Creare un server di chat multi-client usando socket e thread.

---

## Risorse Aggiuntive

- **Libri:**
  - "Operating System Concepts" - Silberschatz
  - "The Art of Multiprocessor Programming" - Herlihy, Shavit
  
- **Online:**
  - [Python threading documentation](https://docs.python.org/3/library/threading.html)
  - [Java Concurrency Tutorial](https://docs.oracle.com/javase/tutorial/essential/concurrency/)

---

**Prossimo:** [02 - Architetture Hardware](02-architetture-hardware.md)
