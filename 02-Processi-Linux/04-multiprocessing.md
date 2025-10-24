# Modulo 2 - Il Modulo `multiprocessing` in Python

## Indice
- [Introduzione a multiprocessing](#introduzione-a-multiprocessing)
- [Classe Process](#classe-process)
- [Passaggio Parametri](#passaggio-parametri)
- [Pool di Processi](#pool-di-processi)
- [Sincronizzazione](#sincronizzazione)
- [Comunicazione: Queue e Pipe](#comunicazione-queue-e-pipe)
- [Memoria Condivisa](#memoria-condivisa)
- [Esempi Pratici](#esempi-pratici)

---

## Introduzione a multiprocessing

Il modulo `multiprocessing` fornisce un'API di alto livello per la programmazione con processi, simile al modulo `threading` ma con processi reali invece di thread.

### Vantaggi rispetto a os.fork()

```python
✓ API più semplice e pythonic
✓ Gestione automatica delle risorse
✓ Comunicazione facilitata (Queue, Pipe)
✓ Pool di processi con load balancing
✓ Sincronizzazione integrata (Lock, Semaphore)
✓ Funziona anche su Windows
✗ Overhead leggermente maggiore
```

### Import

```python
import multiprocessing as mp
from multiprocessing import Process, Queue, Pool, Lock
```

---

## Classe Process

### Creazione Base

```python
from multiprocessing import Process

def worker(name):
    print(f"Worker {name}: PID={os.getpid()}")

# Crea processo
p = Process(target=worker, args=('A',))

# Avvia
p.start()

# Attende terminazione
p.join()

print("Fine")
```

### Esempio 14: Process Base

Salva come `esempi/14_process_base.py`:

```python
#!/usr/bin/env python3
"""Esempio base con multiprocessing.Process"""
import os
import time
from multiprocessing import Process

def task(task_id, duration):
    """Task eseguito dal processo"""
    pid = os.getpid()
    print(f"Task {task_id}: avviato (PID={pid})")
    time.sleep(duration)
    print(f"Task {task_id}: completato dopo {duration}s")

if __name__ == '__main__':
    print("=== multiprocessing.Process ===\n")
    
    # Crea processi
    processes = []
    for i in range(3):
        p = Process(target=task, args=(i, i+1))
        processes.append(p)
        p.start()
        print(f"Main: avviato processo {i} (PID={p.pid})")
    
    # Attende tutti
    print("\nMain: aspetto terminazione...\n")
    for p in processes:
        p.join()
    
    print("\nMain: tutti i processi terminati")
```

### Metodi Principali

```python
p = Process(target=func, args=(...), kwargs={...}, name='MyProcess')

# Controllo
p.start()           # Avvia il processo
p.join(timeout)     # Attende terminazione
p.terminate()       # Invia SIGTERM
p.kill()            # Invia SIGKILL

# Proprietà
p.pid              # Process ID
p.name             # Nome
p.is_alive()       # True se in esecuzione
p.exitcode         # Exit code (None se ancora running)
p.daemon           # True se è daemon
```

### Esempio 15: Estendere Process

Salva come `esempi/15_process_class.py`:

```python
#!/usr/bin/env python3
"""Estendere la classe Process"""
from multiprocessing import Process
import time
import os

class WorkerProcess(Process):
    """Custom Process class"""
    
    def __init__(self, task_id, iterations):
        super().__init__()
        self.task_id = task_id
        self.iterations = iterations
    
    def run(self):
        """Override del metodo run()"""
        print(f"Worker {self.task_id}: avviato (PID={os.getpid()})")
        
        for i in range(self.iterations):
            print(f"Worker {self.task_id}: iterazione {i+1}/{self.iterations}")
            time.sleep(1)
        
        print(f"Worker {self.task_id}: terminato")

if __name__ == '__main__':
    print("=== Custom Process Class ===\n")
    
    # Crea workers
    workers = [WorkerProcess(i, 3) for i in range(3)]
    
    # Avvia tutti
    for w in workers:
        w.start()
    
    # Aspetta tutti
    for w in workers:
        w.join()
    
    print("\nMain: fine")
```

---

## Passaggio Parametri

### Args e Kwargs

```python
def worker(a, b, c=10):
    print(f"a={a}, b={b}, c={c}")

# Con args
p1 = Process(target=worker, args=(1, 2))

# Con kwargs
p2 = Process(target=worker, args=(1, 2), kwargs={'c': 20})
```

### Esempio 16: Passaggio Dati

Salva come `esempi/16_parametri.py`:

```python
#!/usr/bin/env python3
"""Passaggio parametri ai processi"""
from multiprocessing import Process
import os

def calcola(operazione, a, b, risultato_queue=None):
    """Esegue operazione matematica"""
    pid = os.getpid()
    
    if operazione == 'add':
        res = a + b
    elif operazione == 'mul':
        res = a * b
    elif operazione == 'pow':
        res = a ** b
    else:
        res = None
    
    print(f"Processo {pid}: {operazione}({a}, {b}) = {res}")
    
    if risultato_queue:
        risultato_queue.put((operazione, res))

if __name__ == '__main__':
    from multiprocessing import Queue
    
    # Queue per risultati
    q = Queue()
    
    # Operazioni
    operazioni = [
        ('add', 10, 5),
        ('mul', 10, 5),
        ('pow', 2, 10)
    ]
    
    processes = []
    for op, a, b in operazioni:
        p = Process(target=calcola, args=(op, a, b, q))
        processes.append(p)
        p.start()
    
    # Attende
    for p in processes:
        p.join()
    
    # Legge risultati
    print("\nRisultati:")
    while not q.empty():
        op, res = q.get()
        print(f"  {op}: {res}")
```

---

## Pool di Processi

Il Pool gestisce automaticamente un insieme di processi worker.

### Creazione Pool

```python
from multiprocessing import Pool

# Crea pool (di default: cpu_count() worker)
pool = Pool()

# Pool con N worker
pool = Pool(processes=4)

# Usa il pool
results = pool.map(func, data)

# Chiudi pool
pool.close()
pool.join()
```

### Esempio 17: Pool.map()

Salva come `esempi/17_pool_map.py`:

```python
#!/usr/bin/env python3
"""Pool di processi con map()"""
from multiprocessing import Pool
import time
import os

def elabora(n):
    """Elabora un numero"""
    pid = os.getpid()
    print(f"Worker {pid}: elaboro {n}")
    time.sleep(0.5)
    return n * n

if __name__ == '__main__':
    print("=== Pool.map() ===\n")
    
    # Dati da elaborare
    numeri = list(range(1, 11))
    
    # Crea pool con 3 worker
    with Pool(processes=3) as pool:
        print(f"Pool con {pool._processes} worker\n")
        
        # Map distribuisce automaticamente il lavoro
        risultati = pool.map(elabora, numeri)
    
    print(f"\nRisultati: {risultati}")
    print(f"Somma: {sum(risultati)}")
```

### Metodi Pool

```python
# map() - blocking, ritorna risultati ordinati
results = pool.map(func, iterable)

# map_async() - non-blocking
async_result = pool.map_async(func, iterable)
results = async_result.get(timeout=10)

# apply() - singolo task, blocking
result = pool.apply(func, args=(arg1, arg2))

# apply_async() - singolo task, non-blocking
async_result = pool.apply_async(func, args=(arg1, arg2))
result = async_result.get()

# starmap() - multiple args per elemento
results = pool.starmap(func, [(1,2), (3,4), (5,6)])

# imap() - lazy iterator
for result in pool.imap(func, iterable):
    print(result)
```

### Esempio 18: Pool Completo

Salva come `esempi/18_pool_completo.py`:

```python
#!/usr/bin/env python3
"""Esempio completo con Pool"""
from multiprocessing import Pool, cpu_count
import time
import os

def task_pesante(n):
    """Simula task computazionalmente intensivo"""
    pid = os.getpid()
    print(f"Worker {pid}: inizio task {n}")
    
    # Simula calcolo pesante
    total = sum(i*i for i in range(n * 100000))
    
    time.sleep(0.1)
    print(f"Worker {pid}: completato task {n}")
    return (n, total)

if __name__ == '__main__':
    print(f"=== Pool di Processi ===")
    print(f"CPU disponibili: {cpu_count()}\n")
    
    tasks = [1, 2, 3, 4, 5, 6, 7, 8]
    
    # Sequenziale (per confronto)
    print("Esecuzione SEQUENZIALE:")
    start = time.time()
    results_seq = [task_pesante(t) for t in tasks]
    time_seq = time.time() - start
    print(f"Tempo: {time_seq:.2f}s\n")
    
    # Parallelo con Pool
    print("Esecuzione PARALLELA:")
    start = time.time()
    with Pool(processes=4) as pool:
        results_par = pool.map(task_pesante, tasks)
    time_par = time.time() - start
    print(f"Tempo: {time_par:.2f}s\n")
    
    print(f"Speedup: {time_seq/time_par:.2f}x")
```

---

## Sincronizzazione

### Lock

```python
from multiprocessing import Process, Lock

def safe_print(lock, msg):
    with lock:
        print(msg)

lock = Lock()
p1 = Process(target=safe_print, args=(lock, "Messaggio 1"))
p2 = Process(target=safe_print, args=(lock, "Messaggio 2"))
p1.start()
p2.start()
```

### Esempio 19: Lock

Salva come `esempi/19_lock.py`:

```python
#!/usr/bin/env python3
"""Sincronizzazione con Lock"""
from multiprocessing import Process, Lock
import time

def conta_senza_lock(worker_id):
    """Senza protezione (può avere race condition)"""
    for i in range(3):
        print(f"Worker {worker_id}: {i}")
        time.sleep(0.1)

def conta_con_lock(worker_id, lock):
    """Con Lock - accesso mutuamente esclusivo"""
    with lock:
        for i in range(3):
            print(f"Worker {worker_id}: {i}")
            time.sleep(0.1)

if __name__ == '__main__':
    # Test senza Lock
    print("=== SENZA Lock ===")
    p1 = Process(target=conta_senza_lock, args=(1,))
    p2 = Process(target=conta_senza_lock, args=(2,))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    time.sleep(1)
    
    # Test con Lock
    print("\n=== CON Lock ===")
    lock = Lock()
    p1 = Process(target=conta_con_lock, args=(1, lock))
    p2 = Process(target=conta_con_lock, args=(2, lock))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
```

---

## Comunicazione: Queue e Pipe

### Queue

```python
from multiprocessing import Queue

q = Queue()
q.put(item)          # Inserisce
item = q.get()       # Estrae (blocking)
item = q.get_nowait()  # Estrae (non-blocking)
q.empty()            # True se vuota
q.qsize()            # Numero elementi (approssimativo)
```

### Esempio 20: Producer-Consumer

Salva come `esempi/20_producer_consumer.py`:

```python
#!/usr/bin/env python3
"""Pattern Producer-Consumer con Queue"""
from multiprocessing import Process, Queue
import time
import random

def producer(queue, producer_id, n_items):
    """Produce items e li mette nella queue"""
    for i in range(n_items):
        item = f"Item-{producer_id}-{i}"
        print(f"Producer {producer_id}: prodotto {item}")
        queue.put(item)
        time.sleep(random.uniform(0.1, 0.5))
    
    print(f"Producer {producer_id}: terminato")

def consumer(queue, consumer_id):
    """Consuma items dalla queue"""
    while True:
        try:
            item = queue.get(timeout=2)
            print(f"  Consumer {consumer_id}: consumato {item}")
            time.sleep(random.uniform(0.1, 0.3))
        except:
            break
    
    print(f"  Consumer {consumer_id}: terminato")

if __name__ == '__main__':
    print("=== Producer-Consumer ===\n")
    
    queue = Queue()
    
    # Crea producers
    producers = [
        Process(target=producer, args=(queue, i, 5))
        for i in range(2)
    ]
    
    # Crea consumers
    consumers = [
        Process(target=consumer, args=(queue, i))
        for i in range(3)
    ]
    
    # Avvia tutti
    for p in producers + consumers:
        p.start()
    
    # Aspetta producers
    for p in producers:
        p.join()
    
    # Aspetta consumers (con timeout)
    for c in consumers:
        c.join(timeout=3)
        if c.is_alive():
            c.terminate()
    
    print("\nFine")
```

### Pipe

```python
from multiprocessing import Pipe

# Crea pipe bidirezionale
parent_conn, child_conn = Pipe()

# Invia
parent_conn.send(data)

# Ricevi
data = child_conn.recv()
```

---

## Memoria Condivisa

### Value e Array

```python
from multiprocessing import Value, Array

# Shared integer
counter = Value('i', 0)  # 'i' = signed int

# Shared array
arr = Array('d', [1.0, 2.0, 3.0])  # 'd' = double

# Accesso
with counter.get_lock():
    counter.value += 1
```

### Esempio 21: Shared Memory

Salva come `esempi/21_shared_memory.py`:

```python
#!/usr/bin/env python3
"""Memoria condivisa con Value e Array"""
from multiprocessing import Process, Value, Array
import time

def incrementa(counter, arr, worker_id):
    """Incrementa counter e array condivisi"""
    for i in range(100):
        # Counter condiviso
        with counter.get_lock():
            counter.value += 1
        
        # Array condiviso
        arr[worker_id] += 1
        
        time.sleep(0.01)

if __name__ == '__main__':
    # Counter condiviso
    counter = Value('i', 0)
    
    # Array condiviso
    arr = Array('i', [0] * 3)
    
    # Crea processi
    processes = [
        Process(target=incrementa, args=(counter, arr, i))
        for i in range(3)
    ]
    
    # Avvia
    for p in processes:
        p.start()
    
    # Aspetta
    for p in processes:
        p.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Array finale: {list(arr)}")
    print(f"Somma array: {sum(arr)}")
```

---

## Esempi Pratici

### Esempio 22: Elaborazione Parallela File

Salva come `esempi/22_parallel_file_processing.py`:

```python
#!/usr/bin/env python3
"""Elaborazione parallela di file"""
from multiprocessing import Pool
import os
import time

def conta_parole(filepath):
    """Conta le parole in un file"""
    try:
        with open(filepath, 'r') as f:
            content = f.read()
            words = len(content.split())
            return (filepath, words)
    except:
        return (filepath, 0)

def trova_file_txt(directory):
    """Trova tutti i file .txt in una directory"""
    txt_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.txt') or file.endswith('.md'):
                txt_files.append(os.path.join(root, file))
    return txt_files

if __name__ == '__main__':
    # Directory da analizzare
    directory = '/home/git-projects/TPSIT_2_MY'
    
    print("=== Elaborazione Parallela File ===\n")
    print(f"Directory: {directory}\n")
    
    # Trova file
    print("Ricerca file...")
    files = trova_file_txt(directory)
    print(f"Trovati {len(files)} file\n")
    
    if not files:
        print("Nessun file trovato")
        exit()
    
    # Elabora con Pool
    print("Elaborazione in corso...\n")
    start = time.time()
    
    with Pool(processes=4) as pool:
        results = pool.map(conta_parole, files)
    
    elapsed = time.time() - start
    
    # Risultati
    total_words = sum(words for _, words in results)
    
    print("Top 10 file per numero parole:")
    results_sorted = sorted(results, key=lambda x: x[1], reverse=True)
    for path, words in results_sorted[:10]:
        filename = os.path.basename(path)
        print(f"  {words:>6} parole - {filename}")
    
    print(f"\nTotale: {total_words} parole in {len(files)} file")
    print(f"Tempo: {elapsed:.2f}s")
```

---

## Best Practices

1. **Usa sempre `if __name__ == '__main__':`**
   ```python
   if __name__ == '__main__':
       # codice multiprocessing
   ```

2. **Chiudi esplicitamente Pool e Queue**
   ```python
   with Pool() as pool:
       results = pool.map(func, data)
   ```

3. **Evita condivisione eccessiva di dati**
   - Usa Queue per comunicazione
   - Memoria condivisa solo se necessario

4. **Gestisci correttamente la terminazione**
   ```python
   try:
       process.join(timeout=10)
   finally:
       if process.is_alive():
           process.terminate()
   ```

---

**Precedente:** [03 - Processi Python os](03-processi-python-os.md)  
**Prossimo:** [05 - Comunicazione IPC](05-ipc.md)
