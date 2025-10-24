# Modulo 2 - Comunicazione tra Processi (IPC)

## Indice
- [Introduzione all'IPC](#introduzione-allipc)
- [Pipe](#pipe)
- [Queue](#queue)
- [Memoria Condivisa](#memoria-condivisa)
- [Segnali](#segnali)
- [Socket Unix](#socket-unix)
- [Esempi Pratici](#esempi-pratici)

---

## Introduzione all'IPC

### Cos'è l'IPC?

**IPC (Inter-Process Communication)** è l'insieme di meccanismi che permettono ai processi di scambiarsi dati e coordinarsi.

### Metodi IPC Principali

```
┌────────────────────────────────────────┐
│         Metodi IPC                     │
├────────────────────────────────────────┤
│ 1. PIPE         ← Comunicazione 1-way  │
│ 2. QUEUE        ← Message passing      │
│ 3. SHARED MEM   ← Memoria condivisa    │
│ 4. SIGNALS      ← Notifiche asincrone  │
│ 5. SOCKET       ← Rete locale/remota   │
│ 6. FIFO         ← Named pipes          │
│ 7. MESSAGE Q    ← System V/POSIX       │
│ 8. SEMAPHORE    ← Sincronizzazione     │
└────────────────────────────────────────┘
```

### Confronto Metodi

| Metodo | Bidirezionale | Velocità | Complessità | Uso |
|--------|---------------|----------|-------------|-----|
| Pipe | No (2 pipe) | Alta | Bassa | Parent-Child |
| Queue | Sì | Media | Bassa | Multi-process |
| Shared Mem | Sì | Altissima | Alta | Performance |
| Signal | Sì | Bassa | Bassa | Notifiche |
| Socket | Sì | Media | Media | Network |

---

## Pipe

### Pipe Anonime (os.pipe)

```python
import os

# Crea pipe
r, w = os.pipe()  # r=read end, w=write end

pid = os.fork()

if pid == 0:  # Child
    os.close(r)  # Chiude read end
    os.write(w, b"Hello from child")
    os.close(w)
    os._exit(0)
else:  # Parent
    os.close(w)  # Chiude write end
    data = os.read(r, 1024)
    os.close(r)
    print(f"Parent ricevuto: {data.decode()}")
    os.wait()
```

### Esempio 23: Pipe Base

Salva come `esempi/23_pipe_base.py`:

```python
#!/usr/bin/env python3
"""Comunicazione con pipe anonime"""
import os

def child_writer(write_fd):
    """Child scrive nella pipe"""
    os.close(read_fd)  # Non serve read end
    
    message = "Messaggio dal child process"
    os.write(write_fd, message.encode())
    os.close(write_fd)
    
    print("Child: messaggio inviato")
    os._exit(0)

# Crea pipe
read_fd, write_fd = os.pipe()

print("=== Pipe Anonime ===\n")

pid = os.fork()

if pid == 0:
    # CHILD
    child_writer(write_fd)
else:
    # PARENT
    os.close(write_fd)  # Non serve write end
    
    print("Parent: aspetto messaggio...")
    data = os.read(read_fd, 1024)
    os.close(read_fd)
    
    print(f"Parent: ricevuto '{data.decode()}'")
    os.wait()
```

### multiprocessing.Pipe

```python
from multiprocessing import Process, Pipe

def sender(conn):
    """Invia dati"""
    conn.send({'data': 42})
    conn.close()

def receiver(conn):
    """Riceve dati"""
    msg = conn.recv()
    print(f"Ricevuto: {msg}")
    conn.close()

# Crea pipe bidirezionale
parent_conn, child_conn = Pipe()

p1 = Process(target=sender, args=(child_conn,))
p2 = Process(target=receiver, args=(parent_conn,))
p1.start()
p2.start()
p1.join()
p2.join()
```

### Esempio 24: Pipe Bidirezionale

Salva come `esempi/24_pipe_bidir.py`:

```python
#!/usr/bin/env python3
"""Pipe bidirezionale con multiprocessing"""
from multiprocessing import Process, Pipe
import time

def worker(conn, worker_id):
    """Worker comunica via pipe"""
    # Riceve task
    while True:
        try:
            task = conn.recv()
            
            if task == 'STOP':
                break
            
            # Elabora
            result = task * 2
            print(f"Worker {worker_id}: {task} → {result}")
            
            # Invia risultato
            conn.send(result)
            
        except EOFError:
            break
    
    conn.close()
    print(f"Worker {worker_id}: terminato")

if __name__ == '__main__':
    print("=== Pipe Bidirezionale ===\n")
    
    # Crea pipe
    parent_conn, child_conn = Pipe()
    
    # Avvia worker
    p = Process(target=worker, args=(child_conn, 1))
    p.start()
    
    # Invia task
    tasks = [10, 20, 30, 40, 50]
    for task in tasks:
        parent_conn.send(task)
        result = parent_conn.recv()
        print(f"Main: ricevuto risultato {result}")
        time.sleep(0.5)
    
    # Stop worker
    parent_conn.send('STOP')
    p.join()
    
    print("\nMain: fine")
```

---

## Queue

### multiprocessing.Queue

```python
from multiprocessing import Queue

q = Queue(maxsize=10)  # Optional: max items

# Produttore
q.put(item)
q.put_nowait(item)  # Non blocca, solleva Full

# Consumatore
item = q.get()
item = q.get_nowait()  # Non blocca, solleva Empty

# Utilità
q.empty()
q.full()
q.qsize()  # Approssimativo
q.close()
q.join_thread()
```

### Esempio 25: Queue Multi-Producer Multi-Consumer

Salva come `esempi/25_queue_mpmc.py`:

```python
#!/usr/bin/env python3
"""Multi-Producer Multi-Consumer con Queue"""
from multiprocessing import Process, Queue
import time
import random

def producer(queue, prod_id, n_items):
    """Produce items"""
    for i in range(n_items):
        item = f"P{prod_id}-{i}"
        queue.put(item)
        print(f"[Producer {prod_id}] → {item}")
        time.sleep(random.uniform(0.1, 0.5))
    
    print(f"[Producer {prod_id}] Terminato")

def consumer(queue, cons_id):
    """Consuma items"""
    while True:
        try:
            item = queue.get(timeout=3)
            if item == 'STOP':
                break
            print(f"[Consumer {cons_id}] ← {item}")
            time.sleep(random.uniform(0.1, 0.3))
        except:
            break
    
    print(f"[Consumer {cons_id}] Terminato")

if __name__ == '__main__':
    print("=== Multi-Producer Multi-Consumer ===\n")
    
    queue = Queue()
    
    # 3 Producers
    producers = [
        Process(target=producer, args=(queue, i, 5))
        for i in range(3)
    ]
    
    # 2 Consumers
    consumers = [
        Process(target=consumer, args=(queue, i))
        for i in range(2)
    ]
    
    # Avvia
    for p in producers + consumers:
        p.start()
    
    # Aspetta producers
    for p in producers:
        p.join()
    
    # Stop consumers
    for _ in consumers:
        queue.put('STOP')
    
    # Aspetta consumers
    for c in consumers:
        c.join()
    
    print("\nFine")
```

---

## Memoria Condivisa

### Value e Array

```python
from multiprocessing import Value, Array

# Value - singolo valore condiviso
counter = Value('i', 0)       # int
price = Value('d', 19.99)     # double
flag = Value('b', True)       # bool

# Array - array condiviso
arr = Array('i', [1, 2, 3])  # int array
arr = Array('d', 10)          # 10 double

# Accesso sicuro
with counter.get_lock():
    counter.value += 1
```

### Tipi Disponibili

```python
'c': char
'b': signed char
'B': unsigned char
'h': short
'H': unsigned short
'i': int
'I': unsigned int
'l': long
'L': unsigned long
'f': float
'd': double
```

### Esempio 26: Shared Memory Performance

Salva come `esempi/26_shared_memory.py`:

```python
#!/usr/bin/env python3
"""Memoria condivisa vs Queue - performance"""
from multiprocessing import Process, Value, Array, Queue
import time

def worker_shared(counter, arr, n):
    """Usa memoria condivisa"""
    for _ in range(n):
        with counter.get_lock():
            counter.value += 1
        
        for i in range(len(arr)):
            arr[i] += 1

def worker_queue(queue, n):
    """Usa queue"""
    for _ in range(n):
        queue.put(1)

if __name__ == '__main__':
    N = 10000
    
    # Test 1: Shared Memory
    print("=== Test 1: Shared Memory ===")
    counter = Value('i', 0)
    arr = Array('i', [0] * 10)
    
    start = time.time()
    p = Process(target=worker_shared, args=(counter, arr, N))
    p.start()
    p.join()
    elapsed_shared = time.time() - start
    
    print(f"Counter: {counter.value}")
    print(f"Tempo: {elapsed_shared:.3f}s\n")
    
    # Test 2: Queue
    print("=== Test 2: Queue ===")
    queue = Queue()
    
    start = time.time()
    p = Process(target=worker_queue, args=(queue, N))
    p.start()
    p.join()
    
    count = 0
    while not queue.empty():
        queue.get()
        count += 1
    
    elapsed_queue = time.time() - start
    
    print(f"Counter: {count}")
    print(f"Tempo: {elapsed_queue:.3f}s\n")
    
    print(f"Shared Memory è {elapsed_queue/elapsed_shared:.1f}x più veloce")
```

---

## Segnali

### Signal Basics

```python
import signal
import os

def handler(signum, frame):
    """Signal handler"""
    print(f"Ricevuto segnale {signum}")

# Installa handler
signal.signal(signal.SIGTERM, handler)
signal.signal(signal.SIGUSR1, handler)

# Invia segnale
os.kill(pid, signal.SIGTERM)
```

### Segnali Comuni

```
SIGTERM (15) - Terminazione gentile
SIGKILL (9)  - Terminazione forzata (non catturabile)
SIGINT (2)   - Ctrl+C
SIGUSR1 (10) - User-defined 1
SIGUSR2 (12) - User-defined 2
SIGCHLD (17) - Child terminato
SIGSTOP (19) - Stop process (non catturabile)
SIGCONT (18) - Continua process
```

### Esempio 27: Signal Communication

Salva come `esempi/27_signals.py`:

```python
#!/usr/bin/env python3
"""Comunicazione con segnali"""
import os
import signal
import time

# Variabile globale per comunicazione
message_received = False

def sigusr1_handler(signum, frame):
    """Handler per SIGUSR1"""
    global message_received
    message_received = True
    print(f"Child {os.getpid()}: ricevuto SIGUSR1")

if __name__ == '__main__':
    print("=== Comunicazione con Segnali ===\n")
    
    pid = os.fork()
    
    if pid == 0:
        # CHILD
        # Installa handler
        signal.signal(signal.SIGUSR1, sigusr1_handler)
        
        print(f"Child {os.getpid()}: aspetto segnale...")
        
        # Aspetta segnale
        while not message_received:
            time.sleep(0.1)
        
        print("Child: elaboro messaggio...")
        time.sleep(1)
        
        # Risponde al parent
        print(f"Child: invio SIGUSR1 a parent {os.getppid()}")
        os.kill(os.getppid(), signal.SIGUSR1)
        
        os._exit(0)
    else:
        # PARENT
        # Installa handler
        signal.signal(signal.SIGUSR1, sigusr1_handler)
        
        # Aspetta che child sia pronto
        time.sleep(1)
        
        # Invia segnale a child
        print(f"Parent: invio SIGUSR1 a child {pid}")
        os.kill(pid, signal.SIGUSR1)
        
        # Aspetta risposta
        print("Parent: aspetto risposta...")
        while not message_received:
            time.sleep(0.1)
        
        print("Parent: ricevuto segnale da child")
        
        os.wait()
        print("\nParent: fine")
```

---

## Socket Unix

### Socket UNIX Domain

```python
import socket
import os

# Server
server_address = '/tmp/my_socket'

# Rimuove socket esistente
try:
    os.unlink(server_address)
except OSError:
    pass

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.bind(server_address)
sock.listen(1)

# Client
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.connect(server_address)
```

### Esempio 28: Unix Socket IPC

Salva come `esempi/28_unix_socket.py`:

```python
#!/usr/bin/env python3
"""IPC con Unix Domain Socket"""
import socket
import os
import time
from multiprocessing import Process

SOCKET_PATH = '/tmp/ipc_socket'

def server():
    """Server process"""
    # Cleanup
    try:
        os.unlink(SOCKET_PATH)
    except:
        pass
    
    # Crea socket
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.bind(SOCKET_PATH)
    sock.listen(1)
    
    print("Server: in ascolto...")
    
    conn, addr = sock.accept()
    print("Server: client connesso")
    
    # Riceve dati
    while True:
        data = conn.recv(1024)
        if not data or data == b'QUIT':
            break
        
        print(f"Server ricevuto: {data.decode()}")
        
        # Risponde
        response = f"Echo: {data.decode()}"
        conn.send(response.encode())
    
    conn.close()
    sock.close()
    os.unlink(SOCKET_PATH)
    print("Server: terminato")

def client():
    """Client process"""
    time.sleep(1)  # Aspetta server
    
    # Connette
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(SOCKET_PATH)
    
    print("Client: connesso")
    
    # Invia messaggi
    messages = ['Hello', 'World', 'From', 'Client']
    
    for msg in messages:
        sock.send(msg.encode())
        response = sock.recv(1024)
        print(f"Client ricevuto: {response.decode()}")
        time.sleep(0.5)
    
    sock.send(b'QUIT')
    sock.close()
    print("Client: terminato")

if __name__ == '__main__':
    print("=== Unix Domain Socket ===\n")
    
    # Avvia server e client
    p_server = Process(target=server)
    p_client = Process(target=client)
    
    p_server.start()
    p_client.start()
    
    p_server.join()
    p_client.join()
    
    print("\nFine")
```

---

## Esempi Pratici

### Esempio 29: IPC Benchmark

Salva come `esempi/29_ipc_benchmark.py`:

```python
#!/usr/bin/env python3
"""Benchmark metodi IPC"""
from multiprocessing import Process, Queue, Pipe, Value, Array
import time
import os

N_MESSAGES = 1000

def test_pipe():
    """Test Pipe performance"""
    parent_conn, child_conn = Pipe()
    
    def sender(conn):
        for i in range(N_MESSAGES):
            conn.send(i)
        conn.close()
    
    def receiver(conn):
        for _ in range(N_MESSAGES):
            conn.recv()
        conn.close()
    
    start = time.time()
    
    p1 = Process(target=sender, args=(child_conn,))
    p2 = Process(target=receiver, args=(parent_conn,))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    return time.time() - start

def test_queue():
    """Test Queue performance"""
    queue = Queue()
    
    def sender(q):
        for i in range(N_MESSAGES):
            q.put(i)
    
    def receiver(q):
        for _ in range(N_MESSAGES):
            q.get()
    
    start = time.time()
    
    p1 = Process(target=sender, args=(queue,))
    p2 = Process(target=receiver, args=(queue,))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    return time.time() - start

def test_shared_memory():
    """Test Shared Memory performance"""
    arr = Array('i', N_MESSAGES)
    
    def writer(a):
        for i in range(len(a)):
            a[i] = i
    
    def reader(a):
        total = sum(a)
    
    start = time.time()
    
    p1 = Process(target=writer, args=(arr,))
    p2 = Process(target=reader, args=(arr,))
    p1.start()
    p1.join()  # Writer deve finire prima
    p2.start()
    p2.join()
    
    return time.time() - start

if __name__ == '__main__':
    print("=== IPC Benchmark ===")
    print(f"Messaggi: {N_MESSAGES}\n")
    
    # Test
    time_pipe = test_pipe()
    time_queue = test_queue()
    time_shared = test_shared_memory()
    
    # Risultati
    print(f"Pipe:          {time_pipe:.3f}s")
    print(f"Queue:         {time_queue:.3f}s")
    print(f"Shared Memory: {time_shared:.3f}s")
    
    # Confronto
    fastest = min(time_pipe, time_queue, time_shared)
    print(f"\nPipe:          {time_pipe/fastest:.2f}x")
    print(f"Queue:         {time_queue/fastest:.2f}x")
    print(f"Shared Memory: {time_shared/fastest:.2f}x")
```

---

## Best Practices

1. **Chiudi connessioni inutilizzate**
   ```python
   if pid == 0:
       os.close(read_fd)  # Child non legge
   else:
       os.close(write_fd)  # Parent non scrive
   ```

2. **Usa context manager quando possibile**
   ```python
   with lock:
       shared_data.value += 1
   ```

3. **Gestisci errori di comunicazione**
   ```python
   try:
       data = queue.get(timeout=5)
   except Empty:
       print("Timeout!")
   ```

4. **Scegli il metodo IPC appropriato**
   - Pipe: Parent-Child semplice
   - Queue: Multi-process, message passing
   - Shared Memory: Performance critiche
   - Socket: Flessibilità, network

---

**Precedente:** [04 - Modulo multiprocessing](04-multiprocessing.md)  
**Prossimo:** [06 - Sincronizzazione](06-sincronizzazione.md)
