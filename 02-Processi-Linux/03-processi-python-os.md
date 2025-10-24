# Modulo 2 - Processi in Python con il Modulo `os`

## Indice
- [Introduzione al Modulo os](#introduzione-al-modulo-os)
- [os.fork() - Creazione Processi](#osfork---creazione-processi)
- [os.exec*() - Esecuzione Programmi](#osexec---esecuzione-programmi)
- [os.wait() - Attesa Terminazione](#oswait---attesa-terminazione)
- [Gestione Processi Zombie e Orfani](#gestione-processi-zombie-e-orfani)
- [Esempi Pratici Completi](#esempi-pratici-completi)

---

## Introduzione al Modulo os

Il modulo `os` di Python fornisce un'interfaccia per interagire con il sistema operativo, incluse le system call POSIX per la gestione dei processi.

### Import e Verifica Disponibilità

```python
import os

# Verifica se fork() è disponibile
if hasattr(os, 'fork'):
    print("fork() disponibile")
else:
    print("fork() NON disponibile (es. Windows)")

# Sistema operativo
print(f"OS: {os.name}")  # 'posix' su Linux/macOS, 'nt' su Windows
```

### Funzioni Principali

```python
# Gestione processi
os.fork()          # Crea processo figlio
os.exec*()         # Sostituisce immagine processo
os.wait()          # Attende terminazione child
os.waitpid()       # Attende terminazione child specifico
os._exit()         # Terminazione immediata

# Informazioni processi
os.getpid()        # PID corrente
os.getppid()       # PID del parent
os.getuid()        # User ID
os.getgid()        # Group ID

# Segnali
os.kill()          # Invia segnale a processo
os.killpg()        # Invia segnale a process group

# Directory e ambiente
os.getcwd()        # Directory corrente
os.chdir()         # Cambia directory
os.environ         # Variabili d'ambiente
```

---

## os.fork() - Creazione Processi

### Sintassi e Comportamento

```python
import os

pid = os.fork()

if pid < 0:
    # Errore (molto raro)
    print("Fork fallito!")
    
elif pid == 0:
    # Codice CHILD
    print(f"Child: PID={os.getpid()}, PPID={os.getppid()}")
    
else:
    # Codice PARENT
    print(f"Parent: PID={os.getpid()}, Child PID={pid}")
```

### Esempio 1: Fork Semplice

Salva come `esempi/01_fork_base.py`:

```python
#!/usr/bin/env python3
"""Esempio base di fork()"""
import os
import time

print("=== Inizio programma ===")
print(f"PID iniziale: {os.getpid()}")

# Fork del processo
pid = os.fork()

if pid == 0:
    # CHILD PROCESS
    print(f"\nCHILD:")
    print(f"  - Il mio PID: {os.getpid()}")
    print(f"  - PID di mio padre: {os.getppid()}")
    print(f"  - fork() ha ritornato: {pid}")
    print("  - Eseguo lavoro child...")
    time.sleep(2)
    print("  - Child terminato")
    os._exit(0)  # Importante: usa _exit() nel child!
    
else:
    # PARENT PROCESS
    print(f"\nPARENT:")
    print(f"  - Il mio PID: {os.getpid()}")
    print(f"  - PID del mio child: {pid}")
    print(f"  - fork() ha ritornato: {pid}")
    print("  - Aspetto il child...")
    os.wait()  # Attende terminazione child
    print("  - Parent terminato")
```

### Esempio 2: Fork Multipli

Salva come `esempi/02_fork_multipli.py`:

```python
#!/usr/bin/env python3
"""Creazione di multipli processi figli"""
import os
import time

def crea_child(child_id):
    """Crea un processo figlio"""
    pid = os.fork()
    
    if pid == 0:  # Child
        print(f"Child {child_id}: PID={os.getpid()}, PPID={os.getppid()}")
        time.sleep(child_id)  # Dorme per child_id secondi
        print(f"Child {child_id}: terminato dopo {child_id}s")
        os._exit(child_id)  # Exit con codice = child_id
    
    return pid  # Parent ritorna PID del child

# Main
print("Parent: creo 3 processi figli")
children = []

for i in range(1, 4):
    pid = crea_child(i)
    children.append(pid)
    print(f"Parent: creato child {i} con PID {pid}")

print(f"\nParent: aspetto {len(children)} children...")

# Attende tutti i children
while children:
    pid, status = os.wait()
    exit_code = os.WEXITSTATUS(status)
    print(f"Parent: child {pid} terminato con exit code {exit_code}")
    children.remove(pid)

print("Parent: tutti i children terminati")
```

### Esempio 3: Gerarchia di Processi

Salva come `esempi/03_gerarchia.py`:

```python
#!/usr/bin/env python3
"""Creazione di una gerarchia di processi"""
import os
import time

def stampa_info(label):
    """Stampa informazioni sul processo"""
    print(f"{label}: PID={os.getpid()}, PPID={os.getppid()}")

# Processo originale
stampa_info("Originale")

# Prima fork - crea Child 1
pid1 = os.fork()

if pid1 == 0:
    # Child 1
    stampa_info("  Child 1")
    
    # Child 1 fa fork - crea Grandchild
    pid_gc = os.fork()
    
    if pid_gc == 0:
        # Grandchild
        stampa_info("    Grandchild")
        time.sleep(1)
        os._exit(0)
    else:
        # Child 1 aspetta Grandchild
        os.wait()
        print("  Child 1: grandchild terminato")
        os._exit(0)

# Originale fa seconda fork - crea Child 2
pid2 = os.fork()

if pid2 == 0:
    # Child 2
    stampa_info("  Child 2")
    time.sleep(1)
    os._exit(0)

# Originale aspetta entrambi i children
if pid1 > 0 and pid2 > 0:
    os.wait()
    os.wait()
    print("Originale: tutti i children terminati")
```

### Variabili e Memoria dopo fork()

```python
#!/usr/bin/env python3
"""Dimostra la copia della memoria dopo fork()"""
import os

# Variabile globale
counter = 0

print(f"Prima del fork: counter = {counter}")

pid = os.fork()

if pid == 0:
    # Child modifica la sua copia
    counter += 10
    print(f"Child: counter = {counter}")
    os._exit(0)
else:
    # Parent modifica la sua copia
    counter += 100
    os.wait()
    print(f"Parent: counter = {counter}")

# Output:
# Prima del fork: counter = 0
# Child: counter = 10
# Parent: counter = 100
```

---

## os.exec*() - Esecuzione Programmi

### Famiglia di Funzioni exec()

Python fornisce diverse varianti di `exec()`:

```python
# Lista argomenti (l)
os.execl(path, arg0, arg1, ...)
os.execle(path, arg0, arg1, ..., env)
os.execlp(file, arg0, arg1, ...)
os.execlpe(file, arg0, arg1, ..., env)

# Vector argomenti (v)
os.execv(path, args)
os.execve(path, args, env)
os.execvp(file, args)
os.execvpe(file, args, env)
```

**Legenda:**
- `l` = list (argomenti separati)
- `v` = vector (argomenti in lista)
- `p` = path (cerca in `$PATH`)
- `e` = environment (env personalizzato)

### Esempio 4: Esecuzione Comando

Salva come `esempi/04_exec_comando.py`:

```python
#!/usr/bin/env python3
"""Esempio di exec() per eseguire comandi"""
import os

print("Prima di exec()")
print(f"PID: {os.getpid()}")
print(f"Programma: Python\n")

# Sostituisce Python con ls
os.execl("/bin/ls", "ls", "-lh", "/tmp")

# Questo codice NON viene MAI eseguito
print("Questo non verrà mai stampato!")
```

### Esempio 5: Fork + Exec Pattern

Salva come `esempi/05_fork_exec.py`:

```python
#!/usr/bin/env python3
"""Pattern fork() + exec()"""
import os
import sys

def esegui_comando(comando, args):
    """Esegue un comando in un processo separato"""
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            # execvp cerca il comando in $PATH
            os.execvp(comando, [comando] + args)
        except OSError as e:
            print(f"Errore exec: {e}", file=sys.stderr)
            os._exit(1)
    else:  # Parent
        # Aspetta terminazione child
        _, status = os.waitpid(pid, 0)
        
        if os.WIFEXITED(status):
            exit_code = os.WEXITSTATUS(status)
            return exit_code
        else:
            return -1

# Test
print("=== Esecuzione comandi ===\n")

print("1. Comando: ls -l")
esegui_comando("ls", ["-l"])

print("\n2. Comando: echo Hello World")
esegui_comando("echo", ["Hello", "World"])

print("\n3. Comando: python3 --version")
esegui_comando("python3", ["--version"])

print("\n=== Fine ===")
```

### Esempio 6: Exec con Environment Custom

Salva come `esempi/06_exec_env.py`:

```python
#!/usr/bin/env python3
"""Esempio di exec() con environment personalizzato"""
import os

def esegui_con_env(comando, args, env_vars):
    """Esegue comando con environment custom"""
    pid = os.fork()
    
    if pid == 0:  # Child
        # Prepara environment
        new_env = {
            'PATH': '/bin:/usr/bin',
            'HOME': '/tmp',
        }
        new_env.update(env_vars)
        
        try:
            os.execve(comando, [comando] + args, new_env)
        except OSError as e:
            print(f"Errore: {e}")
            os._exit(1)
    else:  # Parent
        os.wait()

# Test
print("Eseguo script Python con MY_VAR=test")

# Crea script temporaneo
with open('/tmp/test_env.py', 'w') as f:
    f.write("""
import os
print(f"MY_VAR = {os.environ.get('MY_VAR', 'NON DEFINITA')}")
print(f"PATH = {os.environ.get('PATH')}")
""")

esegui_con_env(
    '/usr/bin/python3',
    ['/tmp/test_env.py'],
    {'MY_VAR': 'test_value'}
)
```

### Esempio 7: Mini Shell

Salva come `esempi/07_mini_shell.py`:

```python
#!/usr/bin/env python3
"""Mini shell interattiva"""
import os
import sys

def esegui_comando(cmd_line):
    """Esegue una linea di comando"""
    args = cmd_line.strip().split()
    
    if not args:
        return
    
    comando = args[0]
    
    # Comandi built-in
    if comando == "cd":
        try:
            os.chdir(args[1] if len(args) > 1 else os.environ['HOME'])
        except Exception as e:
            print(f"cd: {e}")
        return
    
    elif comando == "pwd":
        print(os.getcwd())
        return
    
    elif comando == "exit":
        sys.exit(0)
    
    # Comandi esterni
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            os.execvp(comando, args)
        except OSError:
            print(f"{comando}: comando non trovato")
            os._exit(127)
    else:  # Parent
        os.waitpid(pid, 0)

def main():
    """Loop principale della shell"""
    print("=== Mini Shell ===")
    print("Comandi: cd, pwd, exit, o qualsiasi comando esterno\n")
    
    while True:
        try:
            # Prompt
            cwd = os.getcwd()
            prompt = f"{cwd}$ "
            cmd_line = input(prompt)
            
            # Esegue comando
            esegui_comando(cmd_line)
            
        except KeyboardInterrupt:
            print("\nUsa 'exit' per uscire")
        except EOFError:
            print("\nBye!")
            break

if __name__ == "__main__":
    main()
```

---

## os.wait() - Attesa Terminazione

### Funzioni di Wait

```python
# Attende qualsiasi child
pid, status = os.wait()

# Attende child specifico
pid, status = os.waitpid(target_pid, options)

# Attende child in process group
pid, status = os.waitpid(-pgid, options)

# Options
os.WNOHANG    # Non blocca se nessun child terminato
os.WUNTRACED  # Riporta anche child fermati (SIGSTOP)
os.WCONTINUED # Riporta child ripresi (SIGCONT)
```

### Interpretare lo Status

```python
def analizza_status(status):
    """Analizza e stampa lo status di uscita"""
    if os.WIFEXITED(status):
        # Terminazione normale
        code = os.WEXITSTATUS(status)
        print(f"Terminato normalmente: exit code = {code}")
        
    elif os.WIFSIGNALED(status):
        # Terminato da segnale
        sig = os.WTERMSIG(status)
        print(f"Terminato da segnale: {sig}")
        
        if os.WCOREDUMP(status):
            print("Core dump generato")
            
    elif os.WIFSTOPPED(status):
        # Fermato (SIGSTOP, SIGTSTP, etc.)
        sig = os.WSTOPSIG(status)
        print(f"Fermato da segnale: {sig}")
        
    elif os.WIFCONTINUED(status):
        # Ripreso (SIGCONT)
        print("Processo ripreso")
```

### Esempio 8: Wait con Analisi Status

Salva come `esempi/08_wait_status.py`:

```python
#!/usr/bin/env python3
"""Esempio di wait() con analisi status"""
import os
import signal
import time

def test_exit_normale():
    """Test terminazione normale"""
    print("\n=== Test 1: Terminazione normale ===")
    
    pid = os.fork()
    if pid == 0:
        print("Child: termino con exit code 42")
        os._exit(42)
    else:
        _, status = os.wait()
        print(f"Parent: child terminato")
        
        if os.WIFEXITED(status):
            code = os.WEXITSTATUS(status)
            print(f"  Exit code: {code}")

def test_segnale():
    """Test terminazione da segnale"""
    print("\n=== Test 2: Terminazione da segnale ===")
    
    pid = os.fork()
    if pid == 0:
        print("Child: aspetto segnale...")
        time.sleep(10)  # Non arriverà mai qui
        os._exit(0)
    else:
        time.sleep(1)
        print(f"Parent: invio SIGTERM a {pid}")
        os.kill(pid, signal.SIGTERM)
        
        _, status = os.wait()
        print(f"Parent: child terminato")
        
        if os.WIFSIGNALED(status):
            sig = os.WTERMSIG(status)
            print(f"  Terminato da segnale: {sig}")

def test_wait_non_bloccante():
    """Test wait non bloccante"""
    print("\n=== Test 3: Wait non bloccante ===")
    
    pid = os.fork()
    if pid == 0:
        time.sleep(3)
        os._exit(0)
    else:
        # Prova subito (child ancora in esecuzione)
        result, status = os.waitpid(pid, os.WNOHANG)
        
        if result == 0:
            print("Parent: child ancora in esecuzione")
            
        # Ora aspetta bloccando
        print("Parent: aspetto bloccante...")
        os.waitpid(pid, 0)
        print("Parent: child terminato")

# Esegue i test
test_exit_normale()
test_segnale()
test_wait_non_bloccante()
```

---

## Gestione Processi Zombie e Orfani

### Processo Zombie

Un processo **zombie** è un processo terminato il cui exit status non è stato ancora letto dal parent.

```
Child termina → Diventa ZOMBIE → Parent fa wait() → Zombie rimosso
```

### Esempio 9: Creazione Zombie

Salva come `esempi/09_zombie.py`:

```python
#!/usr/bin/env python3
"""Dimostrazione processo zombie"""
import os
import time

print("=== Creazione Zombie ===\n")

pid = os.fork()

if pid == 0:
    # Child termina subito
    print(f"Child {os.getpid()}: termino")
    os._exit(0)
else:
    # Parent NON fa wait()
    print(f"Parent: child {pid} diventerà zombie")
    print(f"Parent: verifica con 'ps aux | grep {pid}'")
    
    # Dorme lasciando il child in stato zombie
    print("Parent: aspetto 10 secondi...")
    time.sleep(10)
    
    # Ora rimuove lo zombie
    print("Parent: rimuovo zombie con wait()")
    os.wait()
    print("Parent: zombie rimosso")
```

### Processo Orfano

Un processo **orfano** è un processo il cui parent è terminato. Viene adottato da `init` (PID 1) o systemd.

### Esempio 10: Creazione Orfano

Salva come `esempi/10_orfano.py`:

```python
#!/usr/bin/env python3
"""Dimostrazione processo orfano"""
import os
import time

print("=== Creazione Orfano ===\n")

pid = os.fork()

if pid == 0:
    # Child
    print(f"Child {os.getpid()}: PPID iniziale = {os.getppid()}")
    print("Child: aspetto che parent termini...")
    
    time.sleep(3)
    
    # Ora il parent è terminato
    print(f"Child {os.getpid()}: nuovo PPID = {os.getppid()}")
    print("Child: sono stato adottato da init/systemd!")
    
    time.sleep(2)
    os._exit(0)
else:
    # Parent termina subito
    print(f"Parent {os.getpid()}: termino lasciando child {pid} orfano")
    os._exit(0)
```

### Esempio 11: Prevenzione Zombie con SIGCHLD

Salva come `esempi/11_no_zombie.py`:

```python
#!/usr/bin/env python3
"""Prevenzione zombie con signal handler"""
import os
import signal
import time

def sigchld_handler(signum, frame):
    """Handler per SIGCHLD - raccoglie child terminati"""
    while True:
        try:
            # WNOHANG = non blocca
            pid, status = os.waitpid(-1, os.WNOHANG)
            if pid == 0:
                break
            print(f"Handler: raccolto child {pid}")
        except ChildProcessError:
            break

# Installa handler
signal.signal(signal.SIGCHLD, sigchld_handler)

print("=== Prevenzione Zombie con SIGCHLD ===\n")

# Crea multipli children
for i in range(5):
    pid = os.fork()
    
    if pid == 0:
        # Child dorme e termina
        time.sleep(i + 1)
        print(f"Child {os.getpid()}: termino")
        os._exit(0)
    else:
        print(f"Parent: creato child {pid}")

# Parent continua a lavorare
print("\nParent: continuo a lavorare...")
print("Parent: i child verranno raccolti automaticamente")

time.sleep(10)
print("\nParent: fine")
```

---

## Esempi Pratici Completi

### Esempio 12: Process Manager

Salva come `esempi/12_process_manager.py`:

```python
#!/usr/bin/env python3
"""Gestore semplice di processi"""
import os
import sys
import time
import signal

class ProcessManager:
    """Gestisce un pool di processi worker"""
    
    def __init__(self):
        self.workers = {}  # pid -> info
        self.running = True
    
    def worker_func(self, worker_id):
        """Funzione eseguita dai worker"""
        print(f"Worker {worker_id} (PID {os.getpid()}): avviato")
        
        # Simula lavoro
        for i in range(10):
            if not self.running:
                break
            print(f"Worker {worker_id}: iterazione {i+1}/10")
            time.sleep(2)
        
        print(f"Worker {worker_id}: terminato")
        os._exit(0)
    
    def start_worker(self, worker_id):
        """Avvia un worker"""
        pid = os.fork()
        
        if pid == 0:  # Child
            self.worker_func(worker_id)
        else:  # Parent
            self.workers[pid] = {
                'id': worker_id,
                'start_time': time.time()
            }
            print(f"Manager: avviato worker {worker_id} (PID {pid})")
    
    def stop_worker(self, pid):
        """Ferma un worker specifico"""
        if pid in self.workers:
            try:
                os.kill(pid, signal.SIGTERM)
                print(f"Manager: inviato SIGTERM a {pid}")
            except OSError as e:
                print(f"Manager: errore stop worker: {e}")
    
    def stop_all(self):
        """Ferma tutti i worker"""
        print("\nManager: fermo tutti i worker...")
        self.running = False
        
        for pid in list(self.workers.keys()):
            self.stop_worker(pid)
    
    def wait_workers(self):
        """Attende terminazione worker"""
        while self.workers:
            try:
                pid, status = os.wait()
                
                if pid in self.workers:
                    worker_info = self.workers.pop(pid)
                    duration = time.time() - worker_info['start_time']
                    
                    print(f"Manager: worker {worker_info['id']} "
                          f"(PID {pid}) terminato dopo {duration:.1f}s")
                    
                    if os.WIFEXITED(status):
                        code = os.WEXITSTATUS(status)
                        print(f"  Exit code: {code}")
                    elif os.WIFSIGNALED(status):
                        sig = os.WTERMSIG(status)
                        print(f"  Terminato da segnale: {sig}")
                        
            except ChildProcessError:
                break
    
    def run(self, num_workers=3, duration=10):
        """Esegue il manager"""
        print(f"=== Process Manager ===")
        print(f"Manager PID: {os.getpid()}\n")
        
        # Avvia worker
        for i in range(num_workers):
            self.start_worker(i)
        
        # Lascia lavorare
        print(f"\nManager: lascio lavorare per {duration}s...\n")
        time.sleep(duration)
        
        # Ferma tutti
        self.stop_all()
        
        # Attende terminazione
        self.wait_workers()
        
        print("\nManager: tutti i worker terminati")

# Main
if __name__ == "__main__":
    manager = ProcessManager()
    
    try:
        manager.run(num_workers=3, duration=10)
    except KeyboardInterrupt:
        print("\n\nInterrotto da utente")
        manager.stop_all()
        manager.wait_workers()
```

### Esempio 13: Pipeline di Processi

Salva come `esempi/13_pipeline.py`:

```python
#!/usr/bin/env python3
"""Pipeline di processi comunicanti"""
import os
import sys

def stage1():
    """Primo stadio: genera numeri"""
    print("Stage 1: genero numeri 1-10", file=sys.stderr)
    for i in range(1, 11):
        print(i)
    os._exit(0)

def stage2():
    """Secondo stadio: moltiplica per 2"""
    print("Stage 2: moltiplico per 2", file=sys.stderr)
    for line in sys.stdin:
        num = int(line.strip())
        print(num * 2)
    os._exit(0)

def stage3():
    """Terzo stadio: somma totale"""
    print("Stage 3: calcolo somma", file=sys.stderr)
    total = 0
    for line in sys.stdin:
        total += int(line.strip())
    print(f"\nRisultato finale: {total}", file=sys.stderr)
    os._exit(0)

def create_pipeline():
    """Crea pipeline: stage1 | stage2 | stage3"""
    
    # Pipe 1: stage1 -> stage2
    r1, w1 = os.pipe()
    
    # Pipe 2: stage2 -> stage3
    r2, w2 = os.pipe()
    
    # Fork stage1
    pid1 = os.fork()
    if pid1 == 0:
        # Reindirizza stdout a pipe1
        os.close(r1)
        os.dup2(w1, 1)
        os.close(w1)
        os.close(r2)
        os.close(w2)
        stage1()
    
    # Fork stage2
    pid2 = os.fork()
    if pid2 == 0:
        # Reindirizza stdin da pipe1, stdout a pipe2
        os.close(w1)
        os.dup2(r1, 0)
        os.close(r1)
        os.close(r2)
        os.dup2(w2, 1)
        os.close(w2)
        stage2()
    
    # Fork stage3
    pid3 = os.fork()
    if pid3 == 0:
        # Reindirizza stdin da pipe2
        os.close(r1)
        os.close(w1)
        os.close(w2)
        os.dup2(r2, 0)
        os.close(r2)
        stage3()
    
    # Parent chiude tutte le pipe e aspetta
    os.close(r1)
    os.close(w1)
    os.close(r2)
    os.close(w2)
    
    # Attende tutti gli stage
    os.waitpid(pid1, 0)
    os.waitpid(pid2, 0)
    os.waitpid(pid3, 0)
    
    print("\nPipeline completata", file=sys.stderr)

# Main
if __name__ == "__main__":
    print("=== Pipeline di Processi ===\n", file=sys.stderr)
    create_pipeline()
```

---

## Esercizi

### Esercizio 1
Creare un programma che fork() N processi, ognuno scrive in un file separato, e il parent raccoglie i risultati.

### Esercizio 2
Implementare un sistema di logging distribuito dove multipli processi scrivono log in modo sicuro.

### Esercizio 3
Creare un monitor di sistema che fork() ogni minuto un child per raccogliere statistiche.

### Esercizio 4
Implementare un semplice job scheduler che esegue comandi in background e traccia il loro stato.

---

## Best Practices

1. **Usa sempre `os._exit()` nel child dopo fork()**
   ```python
   if pid == 0:
       # codice child
       os._exit(0)  # NON sys.exit()!
   ```

2. **Gestisci sempre lo status dei child**
   ```python
   pid, status = os.wait()
   if os.WIFEXITED(status):
       code = os.WEXITSTATUS(status)
   ```

3. **Chiudi file descriptor non necessari**
   ```python
   # Dopo fork(), chiudi fd nel processo che non li usa
   if pid == 0:
       os.close(fd_non_necessario)
   ```

4. **Previeni processi zombie**
   - Usa `wait()` o `waitpid()`
   - Oppure installa signal handler per SIGCHLD

5. **Attenzione a risorse condivise**
   - File aperti sono condivisi dopo fork()
   - Lock devono essere gestiti con cura

---

**Precedente:** [02 - System Call](02-system-call.md)  
**Prossimo:** [04 - Modulo multiprocessing](04-multiprocessing.md)
