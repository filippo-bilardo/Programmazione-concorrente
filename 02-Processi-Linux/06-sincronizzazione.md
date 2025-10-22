# Modulo 2 - Sincronizzazione tra Processi

## Indice
- [Introduzione alla Sincronizzazione](#introduzione-alla-sincronizzazione)
- [Lock (Mutex)](#lock-mutex)
- [RLock (Reentrant Lock)](#rlock-reentrant-lock)
- [Semaphore](#semaphore)
- [Event](#event)
- [Condition](#condition)
- [Barrier](#barrier)
- [Esempi Pratici](#esempi-pratici)

---

## Introduzione alla Sincronizzazione

### Perché serve la Sincronizzazione?

Quando multipli processi accedono a risorse condivise, possono verificarsi **race condition**.

```python
# PROBLEMA: Race condition
counter = Value('i', 0)

def increment():
    for _ in range(1000):
        counter.value += 1  # NON atomico!

# 2 processi → risultato imprevedibile
```

### Primitive di Sincronizzazione

```
┌──────────────────────────────────────┐
│    Primitive Sincronizzazione      │
├──────────────────────────────────────┤
│ Lock      ← Mutua esclusione         │
│ RLock     ← Lock rientrante          │
│ Semaphore ← Contatore risorse        │
│ Event     ← Notifica eventi          │
│ Condition ← Wait/Notify condizioni   │
│ Barrier   ← Punto di sincronizzazione│
└──────────────────────────────────────┘
```

---

## Lock (Mutex)

### Concetto

Un **Lock** (o Mutex) garantisce che solo un processo alla volta possa accedere a una risorsa condivisa.

```
Processo 1:  ──┐ LOCK ├────┤ Critical Section ├─┤ UNLOCK ├──
                │      │                        │         │
Processo 2:  ───────────┴─ WAIT ────────────────┴─ LOCK ├───
```

### API

```python
from multiprocessing import Lock

lock = Lock()

# Acquisizione
lock.acquire()        # Blocca fino ad acquisizione
lock.acquire(block=True, timeout=5)  # Con timeout

# Rilascio
lock.release()

# Context manager (RACCOMANDATO)
with lock:
    # sezione critica
    pass
```

### Esempio 30: Lock Base

Salva come `esempi/30_lock_base.py`:

```python
#!/usr/bin/env python3
"""Esempio base di Lock"""
from multiprocessing import Process, Lock, Value
import time

def increment_unsafe(counter, name):
    """Incrementa senza lock (UNSAFE)"""
    for _ in range(1000):
        # Race condition!
        temp = counter.value
        time.sleep(0.0001)  # Simula operazione
        counter.value = temp + 1

def increment_safe(counter, lock, name):
    """Incrementa con lock (SAFE)"""
    for _ in range(1000):
        with lock:
            temp = counter.value
            time.sleep(0.0001)
            counter.value = temp + 1

if __name__ == '__main__':
    # Test UNSAFE
    print("=== Test UNSAFE (race condition) ===")
    counter = Value('i', 0)
    
    p1 = Process(target=increment_unsafe, args=(counter, "P1"))
    p2 = Process(target=increment_unsafe, args=(counter, "P2"))
    
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Atteso: 2000\n")
    
    # Test SAFE
    print("=== Test SAFE (con lock) ===")
    counter = Value('i', 0)
    lock = Lock()
    
    p1 = Process(target=increment_safe, args=(counter, lock, "P1"))
    p2 = Process(target=increment_safe, args=(counter, lock, "P2"))
    
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Atteso: 2000")
```

### Deadlock con Lock

```python
# ATTENZIONE: Deadlock!
lock1 = Lock()
lock2 = Lock()

def process1():
    with lock1:
        time.sleep(0.1)
        with lock2:  # Aspetta lock2
            pass

def process2():
    with lock2:
        time.sleep(0.1)
        with lock1:  # Aspetta lock1 → DEADLOCK!
            pass
```

---

## RLock (Reentrant Lock)

### Concetto

Un **RLock** può essere acquisito multiple volte dallo stesso processo.

```python
from multiprocessing import RLock

rlock = RLock()

# Stesso processo può acquisire multiple volte
with rlock:
    # Prima acquisizione
    with rlock:
        # Seconda acquisizione (OK con RLock)
        pass
```

### Esempio 31: RLock

Salva come `esempi/31_rlock.py`:

```python
#!/usr/bin/env python3
"""RLock - Lock rientrante"""
from multiprocessing import Process, RLock
import time

def funzione_ricorsiva(rlock, depth, max_depth):
    """Funzione che acquisisce lock ricorsivamente"""
    if depth > max_depth:
        return
    
    with rlock:
        print(f"  {'  ' * depth}Depth {depth}: lock acquisito")
        time.sleep(0.1)
        
        # Chiamata ricorsiva (riacquisisce lo stesso lock)
        funzione_ricorsiva(rlock, depth + 1, max_depth)
        
        print(f"  {'  ' * depth}Depth {depth}: lock rilasciato")

if __name__ == '__main__':
    print("=== RLock (Reentrant Lock) ===\n")
    
    rlock = RLock()
    
    p = Process(target=funzione_ricorsiva, args=(rlock, 0, 3))
    p.start()
    p.join()
```

---

## Semaphore

### Concetto

Un **Semaforo** controlla l'accesso a un numero limitato di risorse.

```
Semaphore(3):  [●●●]

P1 acquire → [●●○]  # 2 disponibili
P2 acquire → [●○○]  # 1 disponibile
P3 acquire → [○○○]  # 0 disponibili
P4 acquire → WAIT

P1 release → [●○○]  # 1 disponibile
P4 acquire → [○○○]  # 0 disponibili
```

### API

```python
from multiprocessing import Semaphore

sem = Semaphore(3)  # 3 risorse disponibili

# Acquisizione (decrementa contatore)
sem.acquire()
sem.acquire(block=True, timeout=5)

# Rilascio (incrementa contatore)
sem.release()

# Context manager
with sem:
    # usa risorsa
    pass
```

### Esempio 32: Pool Limitato

Salva come `esempi/32_semaphore.py`:

```python
#!/usr/bin/env python3
"""Semaforo per limitare accesso concorrente"""
from multiprocessing import Process, Semaphore
import time
import random

def usa_risorsa(semaphore, worker_id):
    """Worker che usa risorsa limitata"""
    print(f"Worker {worker_id}: richiedo risorsa...")
    
    with semaphore:
        print(f"Worker {worker_id}: RISORSA ACQUISITA")
        
        # Simula uso risorsa
        duration = random.uniform(1, 3)
        time.sleep(duration)
        
        print(f"Worker {worker_id}: rilascio risorsa dopo {duration:.1f}s")

if __name__ == '__main__':
    print("=== Semaforo (max 3 risorse) ===\n")
    
    # Solo 3 processi possono usare risorsa contemporaneamente
    semaphore = Semaphore(3)
    
    # Crea 10 worker
    workers = [
        Process(target=usa_risorsa, args=(semaphore, i))
        for i in range(10)
    ]
    
    # Avvia tutti
    start = time.time()
    for w in workers:
        w.start()
    
    # Aspetta tutti
    for w in workers:
        w.join()
    
    elapsed = time.time() - start
    print(f"\nTutti i worker completati in {elapsed:.1f}s")
```

### Bounded Semaphore

```python
from multiprocessing import BoundedSemaphore

# Non permette release() oltre il valore iniziale
sem = BoundedSemaphore(3)

sem.acquire()
sem.release()
sem.release()  # ValueError se supera limite
```

---

## Event

### Concetto

Un **Event** è un flag booleano che permette a processi di aspettare eventi.

```
Event: [●] (set)    o  [○] (clear)

Processo 1: event.wait() → blocca se clear
Processo 2: event.set() → sblocca tutti
```

### API

```python
from multiprocessing import Event

event = Event()

# Impostazione
event.set()      # Imposta a True
event.clear()    # Imposta a False

# Attesa
event.wait()              # Blocca fino a set()
event.wait(timeout=5)     # Con timeout

# Stato
event.is_set()   # True se impostato
```

### Esempio 33: Event Sync

Salva come `esempi/33_event.py`:

```python
#!/usr/bin/env python3
"""Sincronizzazione con Event"""
from multiprocessing import Process, Event
import time

def waiter(event, worker_id):
    """Aspetta evento"""
    print(f"Worker {worker_id}: aspetto evento...")
    event.wait()
    print(f"Worker {worker_id}: evento ricevuto, lavoro!")
    time.sleep(1)
    print(f"Worker {worker_id}: completato")

def coordinator(event, workers):
    """Coordina i worker"""
    print("Coordinator: preparo sistema...")
    time.sleep(2)
    
    print("Coordinator: sistema pronto!")
    print(f"Coordinator: sblocco {workers} worker")
    event.set()
    
    print("Coordinator: terminato")

if __name__ == '__main__':
    print("=== Event per Sincronizzazione ===\n")
    
    event = Event()
    n_workers = 5
    
    # Crea worker
    workers = [
        Process(target=waiter, args=(event, i))
        for i in range(n_workers)
    ]
    
    # Crea coordinator
    coord = Process(target=coordinator, args=(event, n_workers))
    
    # Avvia
    for w in workers:
        w.start()
    
    coord.start()
    
    # Aspetta
    coord.join()
    for w in workers:
        w.join()
    
    print("\nFine")
```

---

## Condition

### Concetto

Una **Condition** combina Lock + notifiche per coordinazione complessa.

```python
from multiprocessing import Condition

cond = Condition()

# Producer
with cond:
    # produce data
    cond.notify()     # Sveglia 1 waiter
    cond.notify_all() # Sveglia tutti

# Consumer
with cond:
    while not ready:
        cond.wait()   # Rilascia lock e aspetta
    # consume data
```

### Esempio 34: Producer-Consumer con Condition

Salva come `esempi/34_condition.py`:

```python
#!/usr/bin/env python3
"""Pattern Producer-Consumer con Condition"""
from multiprocessing import Process, Condition, Value
import time
import random

def producer(cond, buffer, buffer_size, prod_id):
    """Produce items"""
    for i in range(5):
        with cond:
            # Aspetta se buffer pieno
            while buffer.value >= buffer_size:
                print(f"Producer {prod_id}: buffer pieno, aspetto")
                cond.wait()
            
            # Produce
            buffer.value += 1
            item = f"P{prod_id}-{i}"
            print(f"Producer {prod_id}: prodotto {item} (buffer={buffer.value})")
            
            # Notifica consumer
            cond.notify()
        
        time.sleep(random.uniform(0.1, 0.5))

def consumer(cond, buffer, cons_id):
    """Consuma items"""
    for _ in range(5):
        with cond:
            # Aspetta se buffer vuoto
            while buffer.value <= 0:
                print(f"  Consumer {cons_id}: buffer vuoto, aspetto")
                cond.wait()
            
            # Consuma
            buffer.value -= 1
            print(f"  Consumer {cons_id}: consumato (buffer={buffer.value})")
            
            # Notifica producer
            cond.notify()
        
        time.sleep(random.uniform(0.1, 0.3))

if __name__ == '__main__':
    print("=== Producer-Consumer con Condition ===\n")
    
    cond = Condition()
    buffer = Value('i', 0)
    buffer_size = 3
    
    # Processi
    producers = [
        Process(target=producer, args=(cond, buffer, buffer_size, i))
        for i in range(2)
    ]
    
    consumers = [
        Process(target=consumer, args=(cond, buffer, i))
        for i in range(2)
    ]
    
    # Avvia
    for p in producers + consumers:
        p.start()
    
    # Aspetta
    for p in producers + consumers:
        p.join()
    
    print("\nFine")
```

---

## Barrier

### Concetto

Una **Barrier** blocca processi fino a quando N processi la raggiungono.

```
3 processi, Barrier(3):

P1: ─────┤ WAIT
P2: ──────┤ WAIT  } Tutti aspettano
P3: ───────┤ WAIT

Quando tutti arrivano:

P1: ──────────── CONTINUE
P2: ──────────── CONTINUE
P3: ──────────── CONTINUE
```

### API

```python
from multiprocessing import Barrier

barrier = Barrier(parties=3)

# Attesa
barrier.wait()  # Blocca fino a N processi
barrier.wait(timeout=10)

# Proprietà
barrier.parties  # Numero processi richiesti
barrier.n_waiting  # Numero processi in attesa
```

### Esempio 35: Barrier Synchronization

Salva come `esempi/35_barrier.py`:

```python
#!/usr/bin/env python3
"""Sincronizzazione con Barrier"""
from multiprocessing import Process, Barrier
import time
import random

def worker(barrier, worker_id, n_phases):
    """Worker con fasi sincronizzate"""
    for phase in range(n_phases):
        # Fase di lavoro
        duration = random.uniform(0.5, 2.0)
        print(f"Worker {worker_id}: fase {phase+1} - lavoro per {duration:.1f}s")
        time.sleep(duration)
        
        # Sincronizzazione
        print(f"Worker {worker_id}: fase {phase+1} completata, aspetto altri")
        barrier.wait()
        
        print(f"Worker {worker_id}: tutti pronti, prossima fase!")
    
    print(f"Worker {worker_id}: COMPLETATO")

if __name__ == '__main__':
    print("=== Barrier Synchronization ===\n")
    
    n_workers = 4
    n_phases = 3
    
    barrier = Barrier(parties=n_workers)
    
    # Crea worker
    workers = [
        Process(target=worker, args=(barrier, i, n_phases))
        for i in range(n_workers)
    ]
    
    # Avvia
    for w in workers:
        w.start()
    
    # Aspetta
    for w in workers:
        w.join()
    
    print("\nTutti i worker completati")
```

---

## Esempi Pratici

### Esempio 36: Database Connection Pool

Salva come `esempi/36_connection_pool.py`:

```python
#!/usr/bin/env python3
"""Connection Pool con Semaphore"""
from multiprocessing import Process, Semaphore
import time
import random

class ConnectionPool:
    """Pool di connessioni database (simulato)"""
    
    def __init__(self, max_connections):
        self.semaphore = Semaphore(max_connections)
        self.max_connections = max_connections
    
    def execute_query(self, worker_id, query):
        """Esegue query acquisendo connessione"""
        print(f"Worker {worker_id}: richiedo connessione per '{query}'")
        
        with self.semaphore:
            print(f"  Worker {worker_id}: CONNESSIONE ACQUISITA")
            
            # Simula esecuzione query
            duration = random.uniform(0.5, 2.0)
            time.sleep(duration)
            
            print(f"  Worker {worker_id}: query completata in {duration:.1f}s")

def worker(pool, worker_id, n_queries):
    """Worker che esegue query"""
    for i in range(n_queries):
        query = f"SELECT * FROM table_{i}"
        pool.execute_query(worker_id, query)
        time.sleep(random.uniform(0.1, 0.5))

if __name__ == '__main__':
    print("=== Database Connection Pool ===")
    print("Max connessioni: 3\n")
    
    pool = ConnectionPool(max_connections=3)
    
    # 10 worker, 2 query ciascuno
    workers = [
        Process(target=worker, args=(pool, i, 2))
        for i in range(10)
    ]
    
    start = time.time()
    
    for w in workers:
        w.start()
    
    for w in workers:
        w.join()
    
    elapsed = time.time() - start
    
    print(f"\nTutte le query completate in {elapsed:.1f}s")
```

### Esempio 37: Pipeline Sincronizzata

Salva come `esempi/37_pipeline_sync.py`:

```python
#!/usr/bin/env python3
"""Pipeline con sincronizzazione Event"""
from multiprocessing import Process, Event, Queue
import time

def stage1(output_queue, ready_event):
    """Stage 1: Genera dati"""
    print("Stage 1: generazione dati...")
    
    for i in range(10):
        data = f"Data-{i}"
        output_queue.put(data)
        print(f"  Stage 1: generato {data}")
        time.sleep(0.2)
    
    output_queue.put(None)  # Sentinel
    ready_event.set()
    print("Stage 1: COMPLETATO")

def stage2(input_queue, output_queue, ready_event):
    """Stage 2: Elabora dati"""
    ready_event.wait()  # Aspetta stage 1
    print("Stage 2: elaborazione dati...")
    
    while True:
        data = input_queue.get()
        if data is None:
            break
        
        # Elabora
        result = data.upper()
        output_queue.put(result)
        print(f"  Stage 2: elaborato {data} → {result}")
        time.sleep(0.1)
    
    output_queue.put(None)  # Sentinel
    print("Stage 2: COMPLETATO")

def stage3(input_queue):
    """Stage 3: Salva risultati"""
    print("Stage 3: salvataggio risultati...")
    
    results = []
    while True:
        result = input_queue.get()
        if result is None:
            break
        
        results.append(result)
        print(f"  Stage 3: salvato {result}")
    
    print(f"Stage 3: COMPLETATO ({len(results)} risultati)")

if __name__ == '__main__':
    print("=== Pipeline Sincronizzata ===\n")
    
    # Code di comunicazione
    queue_1_2 = Queue()
    queue_2_3 = Queue()
    
    # Event per sincronizzazione
    event_1 = Event()
    
    # Stages
    p1 = Process(target=stage1, args=(queue_1_2, event_1))
    p2 = Process(target=stage2, args=(queue_1_2, queue_2_3, event_1))
    p3 = Process(target=stage3, args=(queue_2_3,))
    
    # Avvia
    p1.start()
    p2.start()
    p3.start()
    
    # Aspetta
    p1.join()
    p2.join()
    p3.join()
    
    print("\nPipeline completata")
```

---

## Best Practices

1. **Usa sempre context manager con Lock**
   ```python
   with lock:
       # sezione critica
       pass
   ```

2. **Evita nested locks (deadlock)**
   ```python
   # BAD
   with lock1:
       with lock2:
           pass
   
   # GOOD: acquisizione ordinata
   locks = sorted([lock1, lock2], key=id)
   for lock in locks:
       lock.acquire()
   ```

3. **Timeout per prevenire deadlock**
   ```python
   if lock.acquire(timeout=5):
       try:
           # lavoro
       finally:
           lock.release()
   else:
       print("Timeout!")
   ```

4. **Scegli la primitiva giusta**
   - Lock: Mutua esclusione semplice
   - Semaphore: Risorse limitate
   - Event: Notifiche one-shot
   - Condition: Coordinazione complessa
   - Barrier: Sincronizzazione fasi

---

**Precedente:** [05 - Comunicazione IPC](05-ipc.md)  
**Prossimo:** [Modulo 3 - Thread in Java](../../03-Thread-Java/README.md)
