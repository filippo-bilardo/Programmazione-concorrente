# Modulo 1 - Problematiche della Programmazione Concorrente

## Indice
- [Introduzione](#introduzione)
- [Race Condition](#race-condition)
- [Deadlock](#deadlock)
- [Starvation](#starvation)
- [Livelock](#livelock)
- [Atomicità delle Operazioni](#atomicità-delle-operazioni)
- [Priority Inversion](#priority-inversion)
- [Problemi Classici](#problemi-classici)
- [Tecniche di Prevenzione](#tecniche-di-prevenzione)

---

## Introduzione

La programmazione concorrente introduce una serie di problematiche che non esistono nei programmi sequenziali. Questi problemi derivano principalmente da:
- Accesso condiviso a risorse
- Timing non deterministico
- Interleaving imprevedibile
- Mancanza di sincronizzazione

Comprendere questi problemi è essenziale per scrivere programmi concorrenti corretti.

---

## Race Condition

### Definizione

Una **race condition** (condizione di corsa) si verifica quando il comportamento di un programma dipende dalla sequenza temporale o dall'interleaving di thread/processi multipli che accedono a risorse condivise. Il risultato finale dipende da quale thread "vince la corsa" nell'accedere alla risorsa.

### Perché si chiamano "Race Condition"?

Il termine deriva dal fatto che più thread/processi "gareggiano" (race) per accedere alla stessa risorsa, e il risultato dipende da chi arriva primo. È come una corsa dove l'ordine di arrivo non è predicibile e cambia ogni volta.

### Caratteristiche delle Race Condition

1. **Non Determinismo**: Esecuzioni diverse producono risultati diversi
2. **Difficile da Riprodurre**: Il bug può apparire raramente e casualmente
3. **Dipendenza dal Timing**: Influenzato da carico CPU, scheduling, velocità I/O
4. **Heisenbug**: Il bug scompare quando si cerca di debuggare (a causa di breakpoint che alterano il timing)

### Esempio Classico: Incremento Non Atomico

```python
# Codice Python con race condition
import threading

counter = 0

def increment():
    global counter
    for _ in range(100000):
        counter += 1  # NON è atomico!

# Due thread
t1 = threading.Thread(target=increment)
t2 = threading.Thread(target=increment)

t1.start()
t2.start()
t1.join()
t2.join()

print(f"Counter: {counter}")
# Risultato atteso: 200000
# Risultato reale: spesso < 200000 (es. 137529)
```

### Perché Succede?

L'operazione `counter += 1` appare semplice nel codice, ma a livello di istruzioni macchina si traduce in **tre operazioni distinte**:

```assembly
1. LOAD counter into register    ; Legge valore dalla memoria
2. ADD 1 to register             ; Incrementa nel registro CPU
3. STORE register to counter     ; Scrive valore in memoria
```

Questo è il problema: tra il LOAD e lo STORE, il valore in memoria può essere modificato da un altro thread!

**Interleaving problematico (scenario A):**
```
Tempo  Thread 1          Thread 2          counter (memoria)
  0    LOAD (0)                            0
  1                      LOAD (0)          0
  2    ADD (1)                             0
  3                      ADD (1)           0
  4    STORE (1)                           1
  5                      STORE (1)         1  ← Dovrebbe essere 2!
```

In questo scenario, entrambi i thread leggono 0, incrementano a 1, e scrivono 1. Un incremento è "perso".

**Interleaving problematico (scenario B - ancora peggio):**
```
Tempo  Thread 1          Thread 2          counter
  0    LOAD (0)                            0
  1    ADD (1)                             0
  2                      LOAD (0)          0
  3    STORE (1)                           1
  4                      ADD (1)           1
  5                      STORE (1)         1  ← Ancora 1!
```

**Interleaving corretto (con fortuna):**
```
Tempo  Thread 1          Thread 2          counter
  0    LOAD (0)                            0
  1    ADD (1)                             0
  2    STORE (1)                           1
  3                      LOAD (1)          1  ← Legge valore aggiornato
  4                      ADD (1)           1
  5                      STORE (2)         2  ✓ Corretto!
```

Il problema è che **non possiamo controllare quale scenario si verifica**. Dipende completamente dallo scheduler del sistema operativo.

### Visualizzazione

```
Tempo →
T1: ─LOAD(0)─ADD─STORE(1)────────────────
T2: ────────────────LOAD(0)─ADD─STORE(1)─
                                  ↑
                           Valore perso!
```

### Race Condition nei Banking System

```python
# Esempio realistico: trasferimento bancario
class BankAccount:
    def __init__(self, balance):
        self.balance = balance
    
    def transfer(self, amount, to_account):
        # RACE CONDITION!
        if self.balance >= amount:
            self.balance -= amount
            to_account.balance += amount
            return True
        return False

# Due thread trasferiscono contemporaneamente
account_a = BankAccount(100)
account_b = BankAccount(50)

def transfer_money():
    account_a.transfer(80, account_b)

t1 = threading.Thread(target=transfer_money)
t2 = threading.Thread(target=transfer_money)
# Entrambi potrebbero passare il check del balance!
```

### Soluzioni

#### 1. Lock (Mutex)

```python
import threading

counter = 0
lock = threading.Lock()

def increment():
    global counter
    for _ in range(100000):
        with lock:  # Sezione critica
            counter += 1

# Ora è corretto!
```

#### 2. Operazioni Atomiche

```python
from threading import Lock
import threading

class AtomicCounter:
    def __init__(self):
        self.value = 0
        self._lock = Lock()
    
    def increment(self):
        with self._lock:
            self.value += 1
            return self.value

counter = AtomicCounter()

def worker():
    for _ in range(100000):
        counter.increment()
```

#### 3. Atomic Types (Java)

```java
import java.util.concurrent.atomic.AtomicInteger;

class Counter {
    private AtomicInteger count = new AtomicInteger(0);
    
    public void increment() {
        count.incrementAndGet();  // Atomico!
    }
    
    public int getCount() {
        return count.get();
    }
}
```

---

## Deadlock

### Definizione

Un **deadlock** (stallo critico) si verifica quando due o più processi/thread sono bloccati indefinitamente, ognuno in attesa che l'altro rilasci una risorsa. Nessuno può procedere, creando un blocco permanente del sistema.

### Analogia della Vita Reale

Immagina un incrocio stradale a 4 vie dove:
- 4 auto arrivano contemporaneamente da 4 direzioni diverse
- Ogni auto occupa metà strada (la sua risorsa)
- Ogni auto deve attraversare l'altra metà (la risorsa dell'auto successiva)
- Nessuna auto può procedere senza che un'altra si muova
- **Risultato: Deadlock!** Tutte le auto sono bloccate per sempre.

```
        │ ↓ │
    ────┼───┼────
  →     │ ● │     ←
    ────┼───┼────
        │ ↑ │
    Tutte bloccate!
```

### Perché è un Problema Grave?

1. **Blocco Permanente**: A differenza di un rallentamento, il sistema non si riprende mai da solo
2. **Sprechi di Risorse**: Le risorse bloccate non possono essere usate da altri
3. **Cascading Effects**: Un deadlock può propagarsi e bloccare altri processi
4. **Difficile da Rilevare**: Può non essere immediatamente evidente che c'è un deadlock
5. **Recovery Complesso**: Richiede intervento esterno (terminare processi, restart)

### Esempio Classico: Filosofi a Cena

```
     F1
    /  \
  c1    c5
  |      |
 F5      F2
  |      |
  c4    c2
    \  /
     F3
```

Ogni filosofo ha bisogno di due bacchette per mangiare.

```python
import threading
import time

# 5 bacchette (lock)
chopsticks = [threading.Lock() for _ in range(5)]

def philosopher(id):
    left = id
    right = (id + 1) % 5
    
    while True:
        # Pensa
        time.sleep(0.1)
        
        # Prende bacchetta sinistra
        chopsticks[left].acquire()
        print(f"Philosopher {id} got left chopstick")
        
        # Prende bacchetta destra
        chopsticks[right].acquire()
        print(f"Philosopher {id} got right chopstick")
        
        # Mangia
        print(f"Philosopher {id} is eating")
        time.sleep(0.1)
        
        # Rilascia
        chopsticks[right].release()
        chopsticks[left].release()

# Se tutti prendono la bacchetta sinistra contemporaneamente → DEADLOCK!
```

### Condizioni di Coffman

Il teorema di Coffman (1971) stabilisce che un deadlock può verificarsi **SOLO se sono presenti TUTTE E 4 le seguenti condizioni contemporaneamente**:

#### 1. Mutua Esclusione (Mutual Exclusion)
**Definizione**: Almeno una risorsa deve essere non condivisibile - può essere usata da un solo processo alla volta.

**Esempio**: 
```
Stampante: può stampare un documento alla volta
Lock/Mutex: può essere acquisito da un solo thread
File in scrittura: solo un processo può scrivere
```

**Non è mutua esclusione**:
```
File in lettura: più processi possono leggere simultaneamente
Memoria read-only: infiniti lettori
```

#### 2. Hold and Wait (Possesso e Attesa)
**Definizione**: Un processo che già possiede almeno una risorsa sta aspettando di acquisire altre risorse possedute da altri processi.

**Esempio**:
```
Thread A: 
  - Possiede: Lock1 ✓
  - Aspetta: Lock2 (posseduto da Thread B)

Thread B:
  - Possiede: Lock2 ✓
  - Aspetta: Lock1 (posseduto da Thread A)
```

**Visualizzazione**:
```
Thread A: [█ Lock1] ──waiting──> Lock2
Thread B: [█ Lock2] ──waiting──> Lock1
```

#### 3. No Preemption (Non Prelazione)
**Definizione**: Le risorse non possono essere forzatamente rimosse da un processo che le possiede. Devono essere rilasciate volontariamente.

**Esempio**:
```python
# Il sistema operativo NON può fare:
force_release(thread_a, lock1)  # Non permesso!

# Il thread deve rilasciare volontariamente:
lock1.release()  # Solo il possessore può farlo
```

**Perché questa condizione?**
- Rilasciare forzatamente può lasciare dati in stato inconsistente
- Il processo potrebbe essere nel mezzo di un'operazione critica
- Esempio: se interrompo durante una transazione bancaria, i conti potrebbero risultare sbilanciati

#### 4. Attesa Circolare (Circular Wait)
**Definizione**: Esiste un insieme di processi {P0, P1, ..., Pn} dove:
- P0 aspetta una risorsa posseduta da P1
- P1 aspetta una risorsa posseduta da P2
- ...
- Pn aspetta una risorsa posseduta da P0

**Visualizzazione del ciclo**:
```
    P1 ──holds──> R1 ──needed by──> P2
    ↑                                 │
    │                                 │
needed by                          holds
    │                                 ↓
    P4 <──needed by── R3 <──holds── P3
    │                                 ↑
    └──holds──> R4 ──needed by───────┘

Ciclo: P1 → P2 → P3 → P4 → P1
```

**Esempio concreto (database)**:
```
Transaction T1: Lock(Account_A) → waiting for Lock(Account_B)
Transaction T2: Lock(Account_B) → waiting for Lock(Account_C)
Transaction T3: Lock(Account_C) → waiting for Lock(Account_A)

Ciclo completo: T1 → T2 → T3 → T1 (DEADLOCK!)
```

### Importanza delle 4 Condizioni

**Rompere ANCHE SOLO UNA delle 4 condizioni previene il deadlock!**

Questo è fondamentale per le strategie di prevenzione:
- Elimina mutua esclusione → usa risorse condivisibili quando possibile
- Elimina hold-and-wait → acquisisci tutte le risorse in una volta
- Permetti preemption → forza il rilascio con timeout
- Elimina attesa circolare → imponi ordinamento nell'acquisizione

```
┌─────────────────────────────────────┐
│ Tutte e 4 condizioni presenti       │
│ ↓                                   │
│ DEADLOCK POSSIBILE                  │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ Almeno 1 condizione assente         │
│ ↓                                   │
│ DEADLOCK IMPOSSIBILE ✓              │
└─────────────────────────────────────┘
```

### Esempio: Lock Classico

```python
import threading

lock_a = threading.Lock()
lock_b = threading.Lock()

def thread1():
    with lock_a:
        print("Thread 1: acquired lock_a")
        time.sleep(0.1)  # Simula lavoro
        with lock_b:
            print("Thread 1: acquired lock_b")

def thread2():
    with lock_b:  # Ordine inverso!
        print("Thread 2: acquired lock_b")
        time.sleep(0.1)
        with lock_a:
            print("Thread 2: acquired lock_a")

# DEADLOCK!
t1 = threading.Thread(target=thread1)
t2 = threading.Thread(target=thread2)
t1.start()
t2.start()
```

**Diagramma del deadlock:**
```
Thread 1: ─┬─ lock_a ─┬─ waiting for lock_b ─┐
           │          │                       │
Thread 2: ─┴─ lock_b ─┴─ waiting for lock_a ←┘
           ↑           ↑
        T=0ms       T=100ms
```

### Strategie di Prevenzione

#### 1. Lock Ordering

Imporre un ordine globale nell'acquisizione dei lock:

```python
def thread1():
    # Acquisisce sempre in ordine: lock_a poi lock_b
    with lock_a:
        with lock_b:
            do_work()

def thread2():
    # Stesso ordine!
    with lock_a:
        with lock_b:
            do_work()
```

#### 2. Try-Lock con Timeout

```python
def thread1():
    while True:
        if lock_a.acquire(timeout=1):
            try:
                if lock_b.acquire(timeout=1):
                    try:
                        do_work()
                        break
                    finally:
                        lock_b.release()
                else:
                    # Timeout su lock_b, rilascia lock_a
                    lock_a.release()
                    time.sleep(random.random())  # Backoff
            except:
                lock_a.release()
                raise
```

#### 3. Lock Hierarchy

```python
class Account:
    _id_counter = 0
    
    def __init__(self, balance):
        self.id = Account._id_counter
        Account._id_counter += 1
        self.balance = balance
        self.lock = threading.Lock()

def transfer(from_account, to_account, amount):
    # Acquisisce sempre in ordine di ID
    first = from_account if from_account.id < to_account.id else to_account
    second = to_account if from_account.id < to_account.id else from_account
    
    with first.lock:
        with second.lock:
            from_account.balance -= amount
            to_account.balance += amount
```

#### 4. Deadlock Detection

```bash
# Linux - rilevamento deadlock thread
gdb -p <pid>
(gdb) info threads
(gdb) thread apply all bt

# Java - thread dump
jstack <pid>
# Cerca "Found one Java-level deadlock"
```

---

## Starvation

### Definizione

**Starvation** (inedia/denutrizione) si verifica quando un thread/processo non riesce mai (o per un tempo indefinitamente lungo) ad ottenere le risorse necessarie per progredire, nonostante le risorse siano disponibili e assegnate ad altri processi.

### Differenza tra Deadlock e Starvation

È importante distinguere questi due concetti:

**Deadlock:**
```
Processo A: bloccato per sempre ⚫
Processo B: bloccato per sempre ⚫
Nessuno può progredire
Sistema completamente fermo
```

**Starvation:**
```
Processo A: esegue ████████████
Processo B: esegue ████████████
Processo C: non esegue mai ⚫ (starved)
Il sistema funziona, ma C è "dimenticato"
```

**Tabella comparativa:**

| Aspetto | Deadlock | Starvation |
|---------|----------|------------|
| Processi bloccati | Tutti nel ciclo | Solo alcuni |
| Sistema funziona? | No, fermo | Sì, ma ingiusto |
| Risorse disponibili? | No, tutte bloccate | Sì, ma date ad altri |
| Causa | Attesa circolare | Scheduling iniquo |
| Durata | Permanente | Indefinita (ma teoricamente risolvibile) |

### Cause Comuni di Starvation

#### 1. Priority-Based Scheduling
Processi ad alta priorità monopolizzano la CPU:

```
Priority Queue:
┌─────────────────────────────┐
│ P1 (priority 10) ████████   │  ← Sempre eseguiti
│ P2 (priority 10) ████████   │
│ P3 (priority 9)  ████████   │
│ ...                         │
│ P99 (priority 1) ⚫ STARVED │  ← Mai eseguito
└─────────────────────────────┘
```

#### 2. Lock Contention
Thread "sfortunati" che non riescono mai ad acquisire un lock:

```
Lock acquisito da:
Thread A ████  Thread B ████  Thread A ████  Thread B ████
Thread C continua ad aspettare... ⏳⏳⏳⏳⏳ (STARVED)
```

#### 3. Readers-Writers Unfairness
Flusso continuo di lettori blocca gli scrittori:

```
Time:  0ms     1000ms    2000ms    3000ms
Readers: ██████████████████████████████
Writer:  ⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳ (waiting forever)
```

#### 4. Resource Allocation
Risorse sempre assegnate agli stessi processi:

```
CPU allocation:
P1: 50% ████████████████████████
P2: 50% ████████████████████████
P3:  0% ⚫ (STARVED - nessuna CPU time)
```

### Esempio: Priority Starvation

```python
import threading
import queue
import time

class PriorityTask:
    def __init__(self, priority, task):
        self.priority = priority
        self.task = task
    
    def __lt__(self, other):
        return self.priority > other.priority  # Higher priority first

task_queue = queue.PriorityQueue()

def worker():
    while True:
        task = task_queue.get()
        print(f"Executing task with priority {task.priority}")
        task.task()
        task_queue.task_done()

# Worker thread
threading.Thread(target=worker, daemon=True).start()

# Aggiungi task ad alta priorità continuamente
for i in range(100):
    task_queue.put(PriorityTask(priority=10, task=lambda: time.sleep(0.1)))
    
# Questo task a bassa priorità potrebbe non essere mai eseguito!
task_queue.put(PriorityTask(priority=1, task=lambda: print("LOW PRIORITY")))
```

### Readers-Writers Starvation

```python
import threading

class ReadersWriters:
    def __init__(self):
        self.readers = 0
        self.lock = threading.Lock()
        self.write_lock = threading.Lock()
    
    def start_read(self):
        with self.lock:
            self.readers += 1
            if self.readers == 1:
                self.write_lock.acquire()  # Block writers
    
    def end_read(self):
        with self.lock:
            self.readers -= 1
            if self.readers == 0:
                self.write_lock.release()
    
    def start_write(self):
        self.write_lock.acquire()  # Se lettori continui → STARVATION!
    
    def end_write(self):
        self.write_lock.release()
```

**Problema:** Se ci sono sempre lettori, gli scrittori non riescono mai a scrivere.

### Soluzioni

#### 1. Fair Scheduling

```python
import threading

class FairLock:
    def __init__(self):
        self.lock = threading.Lock()
        self.waiting = []
        self.current = None
    
    def acquire(self):
        thread_id = threading.current_thread().ident
        with self.lock:
            self.waiting.append(thread_id)
        
        # Attende il proprio turno
        while self.current != thread_id:
            time.sleep(0.001)
    
    def release(self):
        with self.lock:
            if self.waiting:
                self.current = self.waiting.pop(0)
```

#### 2. Aging

Aumentare la priorità di thread che aspettano troppo:

```python
class Task:
    def __init__(self, priority):
        self.base_priority = priority
        self.wait_time = 0
    
    def get_effective_priority(self):
        # Priorità aumenta con il tempo di attesa
        return self.base_priority + self.wait_time * 0.1
```

---

## Livelock

### Definizione

**Livelock** è una situazione simile al deadlock, ma con una differenza cruciale: i processi/thread **continuano ad eseguire** e cambiare stato in risposta agli altri, ma **senza fare progressi reali** verso il completamento. È come essere "bloccati nella cortesia".

### Analogia: Le Persone Cortesi

Immagina due persone che si incontrano in un corridoio stretto:

```
Corridoio:
┌──────────────────────┐
│  👤A          👤B    │
└──────────────────────┘

Tempo 0: A va a sinistra, B va a destra
┌──────────────────────┐
│     👤A    👤B       │
└──────────────────────┘

Tempo 1: Entrambi vedono che vanno a sbattere
        A va a destra, B va a sinistra (per cortesia)
┌──────────────────────┐
│  👤A          👤B    │  ← Stessa situazione!
└──────────────────────┘

Tempo 2: Di nuovo si evitano
        A va a sinistra, B va a destra
┌──────────────────────┐
│     👤A    👤B       │
└──────────────────────┘

... questo continua all'infinito! (LIVELOCK)
```

Entrambi si muovono attivamente, ma nessuno passa mai!

### Differenze tra Deadlock, Starvation e Livelock

**Tabella comparativa:**

| Caratteristica | Deadlock | Starvation | Livelock |
|----------------|----------|------------|----------|
| **Stato dei processi** | Bloccati (waiting) | In attesa | Attivi (running) |
| **Cambiano stato?** | No | No | Sì, continuamente |
| **Fanno progressi?** | No | Eventualmente sì | No |
| **Utilizzo CPU** | 0% | 0-100% (altri usano) | 100% (sprecata!) |
| **Facile da rilevare?** | Moderato | Difficile | Molto difficile |
| **Esempio vita reale** | Incrocio bloccato | Fila infinita | Balletto cortese |

### Visualizzazione degli Stati

```
Deadlock:
Thread A: ⚫──────────────────  (bloccato, non cambia stato)
Thread B: ⚫──────────────────  (bloccato, non cambia stato)
CPU:      ░░░░░░░░░░░░░░░░░░  (idle)

Starvation:
Thread A: ████████████████████  (esegue)
Thread B: ⚫──────────────────  (aspetta, non esegue mai)
CPU:      ████████████████████  (usata da A)

Livelock:
Thread A: ◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑  (cambia stato continuamente)
Thread B: ◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐  (cambia stato continuamente)
CPU:      ████████████████████  (100% usata MA senza progressi!)
```

### Perché è Difficile da Rilevare?

1. **I thread sono attivi**: Sembrano funzionare normalmente
2. **CPU al 100%**: Il sistema sembra occupato
3. **Nessun errore**: Non ci sono eccezioni o timeout
4. **Cambiamenti di stato**: I log mostrano attività
5. **Solo il risultato finale manca**: Il lavoro non si completa mai

### Esempio: Persone Cortesi

```python
import threading
import time

class Person:
    def __init__(self, name):
        self.name = name
        self.side = "left"
    
    def pass_through(self, other):
        while self.side == other.side:
            print(f"{self.name}: stepping to {'right' if self.side == 'left' else 'left'}")
            self.side = "right" if self.side == "left" else "left"
            time.sleep(0.1)
        
        print(f"{self.name}: passed!")

# Due persone che si spostano continuamente
# ma sempre dalla stessa parte → LIVELOCK
```

### Esempio: Retry Simultaneo

```python
import threading
import time

def task_with_retry(id, resource):
    while True:
        if resource.try_acquire():
            try:
                do_work()
                break
            finally:
                resource.release()
        else:
            # Entrambi i thread rilasciano e riprovano simultaneamente
            time.sleep(0.1)  # Stesso tempo di attesa → LIVELOCK
```

### Soluzione: Randomized Backoff

```python
import random
import time

def task_with_random_retry(id, resource):
    while True:
        if resource.try_acquire():
            try:
                do_work()
                break
            finally:
                resource.release()
        else:
            # Backoff randomico previene livelock
            time.sleep(random.uniform(0.1, 0.5))
```

---

## Atomicità delle Operazioni

### Definizione

Un'operazione è **atomica** (dal greco "atomos" = indivisibile) se appare come un'unica operazione indivisibile dal punto di vista degli altri thread: viene eseguita **completamente** o **non viene eseguita affatto**, senza stati intermedi osservabili.

### Analogia: Transazione Bancaria

Considera un trasferimento di denaro:

```
NON Atomico (MALE):
┌────────────────────────────────────┐
│ 1. Leggi saldo account A: 1000€   │ ← Visibile
│ 2. Sottrai 100€ da A: 900€        │ ← Visibile  
│ 3. Leggi saldo account B: 500€    │ ← Visibile
│ 4. Aggiungi 100€ a B: 600€        │ ← Visibile
└────────────────────────────────────┘
Problema: tra step 2 e 4, i 100€ "scompaiono"!

Atomico (BENE):
┌────────────────────────────────────┐
│ transfer(A, B, 100€)               │ ← Singola operazione
│ [internamente: A-=100, B+=100]     │
└────────────────────────────────────┘
Dall'esterno: o succede tutto o niente
```

### Proprietà delle Operazioni Atomiche

#### 1. All-or-Nothing (Tutto o Niente)
```
Atomica:     ████ (successo completo)
          O
             ⚫ (fallimento completo)

Non Atomica: ██░░ (stato intermedio) ← PROBLEMA!
```

#### 2. Indivisibilità
Nessun altro thread può osservare uno stato intermedio:

```
Thread A: [Operazione Atomica]
Thread B: Vede solo ──before── o ──after──
          Non può vedere stati intermedi
```

#### 3. Isolamento
L'operazione non può essere interrotta nel mezzo:

```
Atomica:
Thread A: [████████████] ← Non interrompibile
Thread B:                  ████████

Non Atomica:
Thread A: [███░        ]
Thread B:     ████ ← Interferenza!
Thread A:        ░███]
```

### Perché Servono Operazioni Atomiche?

Consideriamo l'operazione `x++` in dettaglio:

```c
// Codice C: x++
x = x + 1;

// Assembly (semplificato):
LOAD R1, [x]      ; R1 = valore di x
ADD R1, 1         ; R1 = R1 + 1
STORE [x], R1     ; x = R1

// Sono 3 istruzioni separate!
```

**Problema con 2 thread:**

```
Scenario 1 (corretto per fortuna):
T1: LOAD(10) → ADD(11) → STORE(11)
T2:                                    LOAD(11) → ADD(12) → STORE(12)
Risultato: 12 ✓

Scenario 2 (race condition):
T1: LOAD(10) → ADD(11) → STORE(11)
T2: LOAD(10) → ADD(11) → STORE(11)
Risultato: 11 ✗ (dovrebbe essere 12!)

Scenario 3 (ancora peggio):
T1: LOAD(10)
T2:          LOAD(10) → ADD(11) → STORE(11)
T1:                                           ADD(11) → STORE(11)
Risultato: 11 ✗ (il lavoro di T2 è sovrascritto!)
```

### Livelli di Atomicità

#### 1. Hardware Level (CPU)
Le CPU forniscono istruzioni atomiche native:

```assembly
; x86/x64
LOCK INC [memory]     ; Incremento atomico
LOCK ADD [mem], value ; Addizione atomica
CMPXCHG              ; Compare-and-Exchange atomico
XCHG                 ; Exchange atomico

; ARM
LDREX / STREX        ; Load/Store Exclusive
```

Il prefisso `LOCK` blocca il bus di memoria, garantendo atomicità.

#### 2. Software Level (Librerie)
Linguaggi forniscono astrazioni:

```java
// Java
AtomicInteger counter = new AtomicInteger(0);
counter.incrementAndGet();  // Atomico

// C++ 
std::atomic<int> counter{0};
counter++;  // Atomico

// Python (con lock interno)
import threading
lock = threading.Lock()
with lock:  # "Pseudo-atomico"
    counter += 1
```

#### 3. Database Level (Transazioni)
```sql
BEGIN TRANSACTION;
  UPDATE accounts SET balance = balance - 100 WHERE id = 1;
  UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;  -- Atomico: tutto o niente
```

### Operazioni Non Atomiche

```python
# NON atomico in Python
counter += 1

# NON atomico in Java
count++;

# NON atomico in C
x = x + 1;
```

### Operazioni Atomiche Hardware

Le CPU forniscono istruzioni atomiche:

- **CAS** (Compare-And-Swap)
- **TAS** (Test-And-Set)
- **FAA** (Fetch-And-Add)
- **LL/SC** (Load-Linked/Store-Conditional)

#### Compare-And-Swap (CAS)

```c
bool compare_and_swap(int *ptr, int expected, int new_value) {
    // Atomico!
    if (*ptr == expected) {
        *ptr = new_value;
        return true;
    }
    return false;
}
```

**Uso per lock-free increment:**
```c
void atomic_increment(int *counter) {
    int old_value, new_value;
    do {
        old_value = *counter;
        new_value = old_value + 1;
    } while (!compare_and_swap(counter, old_value, new_value));
}
```

### Atomic Types

#### Java

```java
import java.util.concurrent.atomic.*;

AtomicInteger counter = new AtomicInteger(0);
counter.incrementAndGet();  // Atomico
counter.compareAndSet(0, 1);  // CAS

AtomicReference<String> ref = new AtomicReference<>("initial");
ref.compareAndSet("initial", "updated");
```

#### Python (con lock implicito)

```python
import threading

class AtomicInteger:
    def __init__(self, value=0):
        self._value = value
        self._lock = threading.Lock()
    
    def increment(self):
        with self._lock:
            self._value += 1
            return self._value
    
    def get(self):
        with self._lock:
            return self._value
```

#### C++ (std::atomic)

```cpp
#include <atomic>

std::atomic<int> counter(0);
counter++;  // Atomico
counter.fetch_add(1);  // Atomico

// CAS
int expected = 0;
counter.compare_exchange_strong(expected, 1);
```

---

## Priority Inversion

### Definizione

**Priority inversion** (inversione di priorità) è una situazione paradossale nei sistemi real-time dove un task ad **alta priorità** è costretto ad aspettare un task a **bassa priorità**, invertendo di fatto il sistema di priorità progettato nel sistema.

### Il Problema Fondamentale

In un sistema con priorità, ci aspettiamo:
```
✓ Alta priorità esegue PRIMA della bassa priorità
✗ MA con priority inversion: alta priorità BLOCCATA da bassa!
```

### Analogia: L'Autostrada

Immagina una autostrada a 3 corsie:

```
Corsia Veloce (High):    🚗💨  (Ferrari - priorità 10)
Corsia Normale (Medium): 🚙    (Auto normale - priorità 5)  
Corsia Lenta (Low):      🚛    (Camion - priorità 1)

Scenario normale:
Ferrari supera tutti ✓

Scenario con Priority Inversion:
🚛 ha un carico che la 🚗 deve ritirare
🚗 deve aspettare 🚛
Ma 🚛 è bloccato nel traffico da 🚙
Risultato: 🚗 (priorità 10) aspetta 🚙 (priorità 5)!
```

### Caso Reale: Mars Pathfinder (1997)

**Storia vera**: La missione Mars Pathfinder della NASA ha subito reset improvvisi a causa di priority inversion!

```
Sistema Pathfinder:
┌─────────────────────────────────────────┐
│ High Priority: Bus Management Task      │ ← Critico per il sistema
│ Medium Priority: Comunicazione          │
│ Low Priority: Meteorological Data       │ ← Raccolta dati meteo
└─────────────────────────────────────────┘

Problema:
1. Low priority acquisisce un mutex condiviso
2. High priority cerca lo stesso mutex → BLOCCATO
3. Medium priority esegue (non serve il mutex)
4. Low priority è preempted da Medium
5. High priority aspetta Low che non può eseguire!
6. Watchdog rileva timeout → SYSTEM RESET
```

**Soluzione applicata**: Priority inheritance (aggiornamento software inviato dalla Terra!)

### Scenario Classico Dettagliato

```
Task Priorità:
H (High):   priorità 10 - Critical
M (Medium): priorità 5  - Normal  
L (Low):    priorità 1  - Background
```

**Timeline completa dell'inversione:**

```
Time: 0ms      50ms    100ms   150ms   200ms   250ms
─────────────────────────────────────────────────────
L:    ─[LOCK]─ ■■■■■■■■■■■■■■■■■■■■■■■■ [UNLOCK]─
                ↑                        ↑
                │ Preempted              │ Finalmente!
                │                        │
M:              └──[RUNNING]─────────────┘
                   ████████████████████
                   
H:              ──[BLOCKED waiting for lock]──[RUN]
                   ⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳⏳
                   ↑
                   Priority Inversion Zone!
                   H aspetta L, ma L non può eseguire
```

**Dettaglio degli eventi:**

```
T=0ms:   L acquisisce il lock
         L inizia elaborazione

T=50ms:  H si risveglia (evento critico)
         H cerca di acquisire il lock
         H va in WAIT (lock posseduto da L)

T=51ms:  M si risveglia  
         M ha priorità > L → PREEMPTS L
         M inizia a eseguire

T=50-200ms: ⚠️ PRIORITY INVERSION
         H (priorità 10) è bloccato
         M (priorità 5) sta eseguendo  
         L (priorità 1) è preempted
         
         Dovrebbe essere: H > M > L
         Realtà:          M > (H waiting) > (L preempted)

T=200ms: M finisce
         L riprende esecuzione

T=250ms: L rilascia lock
         H finalmente può eseguire!
```

### Effetti dell'Inversione

```
┌─────────────────────────────────────────────────┐
│ Tempo di Risposta del Task H                    │
├─────────────────────────────────────────────────┤
│ Ideale:      [trigger]─5ms─[done]              │
│                                                  │
│ Con Inversion: [trigger]─200ms─[done]          │
│                   └── Inaccettabile!             │
└─────────────────────────────────────────────────┘

Bounded Priority Inversion:
  H aspetta al massimo il tempo di esecuzione di L

Unbounded Priority Inversion:
  H aspetta L + tutti i task a media priorità
  Può durare indefinitamente!
```

### Esempio

```python
import threading
import time

lock = threading.Lock()

def low_priority():
    with lock:
        print("Low: acquired lock")
        time.sleep(2)  # Lungo lavoro
        print("Low: released lock")

def medium_priority():
    time.sleep(0.1)
    print("Medium: running (preempts low)")
    time.sleep(1.5)
    print("Medium: done")

def high_priority():
    time.sleep(0.2)
    print("High: trying to acquire lock")
    with lock:  # Bloccato da Low, che è preempted da Medium!
        print("High: acquired lock")

# High deve attendere sia Low che Medium
```

### Soluzioni

#### 1. Priority Inheritance

Il task che detiene il lock eredita temporaneamente la priorità del task in attesa:

```
L holds lock, priority = 1
H waits for lock, priority = 10
→ L inherits priority 10 until it releases lock
```

#### 2. Priority Ceiling

Ogni lock ha una priorità massima; chi acquisisce il lock ottiene quella priorità:

```python
class PriorityCeilingLock:
    def __init__(self, ceiling_priority):
        self.lock = threading.Lock()
        self.ceiling = ceiling_priority
    
    def acquire(self):
        # Eleva la priorità al ceiling
        old_priority = get_current_priority()
        set_priority(self.ceiling)
        self.lock.acquire()
        return old_priority
    
    def release(self, old_priority):
        self.lock.release()
        set_priority(old_priority)
```

---

## Problemi Classici

### 1. Produttore-Consumatore

**Problema:** Sincronizzare produttore e consumatore con buffer limitato.

```python
import threading
import queue
import time
import random

buffer = queue.Queue(maxsize=5)

def producer():
    for i in range(10):
        item = f"item-{i}"
        buffer.put(item)  # Blocca se pieno
        print(f"Produced: {item}")
        time.sleep(random.random())

def consumer():
    while True:
        item = buffer.get()  # Blocca se vuoto
        print(f"Consumed: {item}")
        time.sleep(random.random())
        buffer.task_done()

threading.Thread(target=producer).start()
threading.Thread(target=consumer, daemon=True).start()
```

### 2. Lettori-Scrittori

**Problema:** Più lettori simultanei OK, ma scrittori in mutua esclusione.

```python
import threading

class ReadWriteLock:
    def __init__(self):
        self.readers = 0
        self.lock = threading.Lock()
        self.write_lock = threading.Lock()
    
    def acquire_read(self):
        with self.lock:
            self.readers += 1
            if self.readers == 1:
                self.write_lock.acquire()
    
    def release_read(self):
        with self.lock:
            self.readers -= 1
            if self.readers == 0:
                self.write_lock.release()
    
    def acquire_write(self):
        self.write_lock.acquire()
    
    def release_write(self):
        self.write_lock.release()
```

### 3. Barbiere Addormentato

**Problema:** Sincronizzare barbiere e clienti con sala d'attesa limitata.

```python
import threading
import time
import random

waiting_room = threading.Semaphore(3)  # 3 sedie
barber_ready = threading.Semaphore(0)
customer_ready = threading.Semaphore(0)
mutex = threading.Lock()
waiting_customers = 0

def barber():
    while True:
        customer_ready.acquire()  # Attende cliente
        with mutex:
            global waiting_customers
            waiting_customers -= 1
        barber_ready.release()  # Segnala pronto
        print("Barber: cutting hair")
        time.sleep(random.uniform(0.5, 1.5))

def customer(id):
    global waiting_customers
    with mutex:
        if waiting_customers < 3:
            waiting_customers += 1
            print(f"Customer {id}: waiting")
            customer_ready.release()
        else:
            print(f"Customer {id}: leaving (full)")
            return
    
    barber_ready.acquire()  # Attende barbiere
    print(f"Customer {id}: getting haircut")
```

---

## Tecniche di Prevenzione

### 1. Evitare Stato Condiviso

```python
# BAD: stato condiviso
global_counter = 0

def worker():
    global global_counter
    global_counter += 1

# GOOD: nessuno stato condiviso
def worker(counter_queue):
    result = do_work()
    counter_queue.put(result)
```

### 2. Immutabilità

```python
# Oggetti immutabili sono inherently thread-safe
from collections import namedtuple

Point = namedtuple('Point', ['x', 'y'])
p = Point(10, 20)
# p.x = 30  # Errore!
```

### 3. Thread-Local Storage

```python
import threading

# Ogni thread ha la propria copia
thread_local = threading.local()

def worker():
    thread_local.value = get_thread_id()
    # Nessuna interferenza tra thread
```

### 4. Lock-Free Data Structures

```python
# Esempio: queue lock-free
from queue import Queue

q = Queue()  # Implementazione lock-free in CPython
q.put(item)  # Thread-safe senza lock espliciti
```

---

## Esercizi

### Esercizio 1: Race Condition Detection
Scrivere un programma che dimostra una race condition e poi correggerla.

### Esercizio 2: Deadlock Prevention
Implementare il problema dei filosofi a cena usando lock ordering.

### Esercizio 3: Readers-Writers
Implementare una soluzione al problema readers-writers che previene starvation degli scrittori.

### Esercizio 4: Atomic Counter
Creare un contatore thread-safe usando solo operazioni atomiche (senza lock).

---

## Tool per Debugging

### 1. Thread Sanitizer (C/C++)

```bash
gcc -fsanitize=thread -g program.c
./a.out
```

### 2. Helgrind (Valgrind)

```bash
valgrind --tool=helgrind ./program
```

### 3. Java Thread Dump

```bash
jstack <pid> > thread_dump.txt
```

### 4. Python Threading Debug

```python
import sys
sys.settrace(threading._trace_hook)
```

---

## Best Practices

1. **Minimizzare le sezioni critiche**
   ```python
   # BAD: lock troppo ampio
   with lock:
       data = fetch_data()
       result = process(data)
       save(result)
   
   # GOOD: lock minimo
   data = fetch_data()
   result = process(data)
   with lock:
       save(result)
   ```

2. **Evitare nested locks**
   ```python
   # BAD
   with lock_a:
       with lock_b:  # Possibile deadlock
           do_work()
   
   # GOOD: usare un singolo lock
   with combined_lock:
       do_work()
   ```

3. **Timeout sui lock**
   ```python
   if lock.acquire(timeout=5):
       try:
           do_work()
       finally:
           lock.release()
   else:
       handle_timeout()
   ```

4. **Documentare l'ordine dei lock**
   ```python
   # Lock order: lock_a → lock_b → lock_c
   # ALWAYS acquire in this order!
   ```

---

## Risorse Aggiuntive

- **Libri:**
  - "The Art of Multiprocessor Programming"
  - "Java Concurrency in Practice"
  - "Operating System Concepts"

- **Tool:**
  - ThreadSanitizer
  - Valgrind/Helgrind
  - Intel Inspector
  - JProfiler

---

**Precedente:** [02 - Architetture Hardware](02-architetture-hardware.md)  
**Prossimo:** [Modulo 2 - Processi in Linux](../Modulo2/README.md)
