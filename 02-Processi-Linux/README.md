# Modulo 2: Processi nei sistemi Linux con Python

Questo modulo copre la programmazione con processi in Linux utilizzando Python, dalle system call di basso livello fino alle API ad alto livello del modulo `multiprocessing`.

## 📚 Guide

1. **[Modello Processi](01-modello-processi.md)**
   - Definizione e ciclo di vita di un processo
   - Process Control Block (PCB)
   - Stati di un processo (NEW, READY, RUNNING, WAITING, TERMINATED)
   - Context switching e overhead
   - Gerarchia di processi (parent-child)
   - Processi zombie e orfani

2. **[System Call](02-system-call.md)**
   - Introduzione alle system call
   - `fork()` - creazione processi con copy-on-write
   - `exec()` - famiglia di funzioni per esecuzione programmi
   - `wait()` / `waitpid()` - sincronizzazione parent-child
   - `exit()` / `_exit()` - terminazione processi
   - `getpid()` / `getppid()` - identificatori processo
   - Pattern fork + exec
   - Esempi: job scheduler, process pool, daemon

3. **[Processi Python con modulo os](03-processi-python-os.md)**
   - `os.fork()` - creazione processi in Python
   - `os.exec*()` - esecuzione comandi esterni
   - `os.wait()` - gestione terminazione children
   - Gestione processi zombie e orfani
   - Esempi: mini shell, process manager, pipeline

4. **[Modulo multiprocessing](04-multiprocessing.md)**
   - Classe `Process` - API ad alto livello
   - Passaggio parametri ai processi
   - Pool di processi - parallelizzazione automatica
   - Lock e RLock per sincronizzazione
   - Queue per comunicazione
   - Memoria condivisa (Value, Array)
   - Esempi: elaborazione parallela file, producer-consumer

5. **[Comunicazione tra Processi (IPC)](05-ipc.md)**
   - Pipe anonime (`os.pipe()`, `multiprocessing.Pipe()`)
   - Queue (`multiprocessing.Queue()`)
   - Memoria condivisa (`Value`, `Array`, `shared_memory`)
   - Segnali (modulo `signal`)
   - Socket Unix Domain
   - Confronto performance metodi IPC
   - Esempi: MPMC, pipeline, IPC benchmark

6. **[Sincronizzazione tra Processi](06-sincronizzazione.md)**
   - Lock (Mutex) - mutua esclusione
   - RLock - lock rientrante
   - Semaphore - controllo risorse limitate
   - Event - notifiche tra processi
   - Condition - coordinazione complessa
   - Barrier - sincronizzazione di fase
   - Esempi: connection pool, pipeline sincronizzata

## 💻 Esempi Pratici

La cartella [`esempi/`](esempi/) contiene oltre 37 esempi funzionanti:

### Categoria: fork() e exec()
- `01_fork_base.py` - Fork basilare con parent/child
- `02_fork_multipli.py` - Creazione multipli children
- `03_gerarchia.py` - Gerarchia multi-livello
- `04_exec_comando.py` - Sostituzione con exec()
- `05_fork_exec.py` - Pattern fork + exec
- `06_exec_env.py` - Exec con environment custom
- `07_mini_shell.py` - Shell interattiva minimale

### Categoria: wait() e gestione status
- `08_wait_status.py` - Interpretazione exit status
- `09_zombie.py` - Creazione e gestione zombie
- `10_orfano.py` - Processo orfano adottato da init
- `11_no_zombie.py` - Prevenzione zombie con SIGCHLD

### Categoria: Process Management
- `12_process_manager.py` - Gestore pool di worker
- `13_pipeline.py` - Pipeline di processi comunicanti

### Categoria: multiprocessing.Process
- `14_process_base.py` - Uso base di Process
- `15_process_class.py` - Estendere classe Process
- `16_parametri.py` - Passaggio parametri

### Categoria: Pool
- `17_pool_map.py` - Pool.map() base
- `18_pool_completo.py` - Confronto sequenziale vs parallelo
- `22_parallel_file_processing.py` - Elaborazione parallela file

### Categoria: Sincronizzazione
- `19_lock.py` - Lock per race condition
- `30_lock_base.py` - Confronto con/senza lock
- `31_rlock.py` - RLock rientrante
- `32_semaphore.py` - Semaforo per risorse limitate
- `33_event.py` - Event per notifiche
- `34_condition.py` - Producer-Consumer con Condition
- `35_barrier.py` - Barrier per sincronizzazione fasi
- `36_connection_pool.py` - Connection pool con Semaphore
- `37_pipeline_sync.py` - Pipeline con Event

### Categoria: IPC
- `20_producer_consumer.py` - Pattern con Queue
- `21_shared_memory.py` - Value e Array condivisi
- `23_pipe_base.py` - Pipe anonime
- `24_pipe_bidir.py` - Pipe bidirezionale
- `25_queue_mpmc.py` - Multi-Producer Multi-Consumer
- `26_shared_memory.py` - Performance comparison
- `27_signals.py` - Comunicazione con segnali
- `28_unix_socket.py` - Unix Domain Socket
- `29_ipc_benchmark.py` - Benchmark metodi IPC

## 🚀 Come Eseguire gli Esempi

### Prerequisiti

```bash
# Installa psutil (usato in alcuni esempi)
pip3 install psutil
```

### Esecuzione

Tutti gli script sono eseguibili direttamente:

```bash
# Esempio 1: Fork base
python3 esempi/01_fork_base.py

# Esempio 17: Pool map
python3 esempi/17_pool_map.py

# Esempio 36: Connection pool
python3 esempi/36_connection_pool.py
```

Oppure rendendoli eseguibili:

```bash
chmod +x esempi/*.py
./esempi/01_fork_base.py
```

## 📊 Concetti Chiave

### Process Lifecycle
```
NEW → READY → RUNNING → WAITING → TERMINATED
      ↑_________|         |
                          ↓
                      READY (preemption)
```

### Fork() Semantics
```
Parent Process (PID=1234)
         |
         | fork()
         |
         ├─→ Parent (fork returns child PID)
         └─→ Child (fork returns 0)
```

### Copy-on-Write
```
Al fork():     Parent e Child condividono memoria (read-only)
Alla scrittura: Pagina copiata solo se modificata
```

### IPC Methods Performance
```
Velocità: Shared Memory > Pipe > Queue > Socket
```

## 🔧 Best Practices

1. **Usa `os._exit()` nei child dopo fork()**
   ```python
   if pid == 0:
       # codice child
       os._exit(0)  # NON sys.exit()
   ```

2. **Sempre `if __name__ == '__main__':` con multiprocessing**
   ```python
   if __name__ == '__main__':
       p = Process(target=func)
       p.start()
   ```

3. **Chiudi file descriptor non necessari dopo fork()**
   ```python
   if pid == 0:
       os.close(read_fd)  # Child non legge
   ```

4. **Gestisci sempre lo status dei child con wait()**
   ```python
   pid, status = os.wait()
   if os.WIFEXITED(status):
       code = os.WEXITSTATUS(status)
   ```

5. **Usa context manager con Lock**
   ```python
   with lock:
       # sezione critica
       pass
   ```

## 📖 Risorse Aggiuntive

### Man Pages
- `man 2 fork` - System call fork()
- `man 2 execve` - Famiglia exec()
- `man 2 wait` - Wait e waitpid()
- `man 7 signal` - Segnali disponibili

### Documentazione Python
- [multiprocessing](https://docs.python.org/3/library/multiprocessing.html)
- [os](https://docs.python.org/3/library/os.html)
- [signal](https://docs.python.org/3/library/signal.html)

### Libri
- "Advanced Programming in the UNIX Environment" - W. Richard Stevens
- "The Linux Programming Interface" - Michael Kerrisk
- "Operating System Concepts" - Silberschatz, Galvin, Gagne

## 🎯 Esercizi Proposti

1. **Process Monitor**: Crea un monitor che fork() ogni minuto un child per raccogliere statistiche CPU/memoria del sistema

2. **Job Scheduler**: Implementa uno scheduler che esegue comandi in background e traccia il loro stato (running/completed/failed)

3. **Web Crawler Parallelo**: Usa Pool per scaricare e analizzare multipli URL contemporaneamente

4. **Log Aggregator**: Multipli processi scrivono log; un processo centrale aggrega e ordina cronologicamente

5. **MapReduce Semplice**: Implementa un framework MapReduce basilare usando Process e Queue

## ⚠️ Note di Compatibilità

- **Linux/macOS**: Tutti gli esempi funzionano
- **Windows**: `os.fork()` NON disponibile; usa `multiprocessing.Process`
- Python 3.6+ richiesto per alcuni esempi

---

**[← Torna al Corso Completo](../README.md)**
