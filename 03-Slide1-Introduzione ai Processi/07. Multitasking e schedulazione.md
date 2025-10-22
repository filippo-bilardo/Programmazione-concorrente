### 1.5 Multitasking e Schedulazione

**Multitasking** e **schedulazione** sono concetti fondamentali nei sistemi operativi moderni, inclusi quelli basati su Linux. Consentono al sistema di eseguire più processi o thread contemporaneamente, migliorando l'efficienza nell'uso delle risorse della CPU.

#### **Multitasking**

Il **multitasking** è la capacità di un sistema operativo di gestire più processi nello stesso tempo. Sebbene i processori eseguano una singola istruzione alla volta, il sistema operativo utilizza tecniche per far sembrare che più processi o thread vengano eseguiti simultaneamente.

Ci sono due tipi principali di multitasking:

1. **Multitasking cooperativo**  
   In questo modello, ogni processo decide autonomamente quando cedere il controllo della CPU, permettendo ad altri processi di essere eseguiti. Questo approccio è meno comune nei sistemi operativi moderni perché può portare a problemi di efficienza e stabilità. Se un processo non cede mai la CPU, gli altri processi possono essere bloccati.

2. **Multitasking preemptive (preemptivo)**  
   In questo modello, il sistema operativo ha il pieno controllo su quando e per quanto tempo un processo può usare la CPU. Ogni processo riceve una porzione di tempo, chiamata **time slice** o **quantum**, per eseguire le sue operazioni. Quando il time slice scade, il sistema operativo può sospendere il processo in esecuzione e passare a un altro processo. Questo è il modello usato dai moderni sistemi operativi, inclusi Linux, Windows e macOS.

#### **Schedulazione**

La **schedulazione** è il meccanismo utilizzato dal sistema operativo per decidere quale processo o thread deve essere eseguito dalla CPU in un dato momento. L'algoritmo di scheduling determina la priorità dei processi, come vengono allocati i tempi di esecuzione e quando un processo viene sospeso o ripreso.

I principali obiettivi di un buon algoritmo di scheduling sono:
- **Utilizzo della CPU**: Massimizzare il tempo in cui la CPU è attivamente utilizzata per l'elaborazione.
- **Throughput**: Aumentare il numero di processi completati in un dato periodo di tempo.
- **Turnaround time**: Ridurre il tempo totale che un processo impiega dalla sua creazione alla sua terminazione.
- **Tempo di attesa**: Minimizzare il tempo che un processo trascorre in stato di attesa.
- **Tempo di risposta**: Ridurre il tempo che intercorre tra la sottomissione di un processo interattivo e l'inizio della sua risposta.

#### **Tipi di algoritmi di schedulazione:**

1. **First-Come, First-Served (FCFS)**  
   - Esegue i processi nell'ordine in cui sono arrivati.
   - Non è preemptivo: un processo in esecuzione continua fino alla sua terminazione.
   - È semplice da implementare, ma può causare lunghi tempi di attesa per i processi più recenti, specialmente se un processo lungo viene eseguito per primo (problema del "convoglio").

2. **Shortest Job Next (SJN) o Shortest Job First (SJF)**  
   - Prioritizza i processi che richiedono il minor tempo di CPU.
   - Può essere non preemptivo o preemptivo (in tal caso viene chiamato **Shortest Remaining Time First**).
   - Minimizza il tempo di attesa medio, ma richiede una stima accurata della durata dei processi, cosa che spesso è difficile.

3. **Round Robin (RR)**  
   - Ogni processo riceve un **time slice** fisso per eseguire. Se il processo non termina entro il suo time slice, viene sospeso e messo in fondo alla coda, e il processo successivo viene eseguito.
   - È preemptivo e garantisce che tutti i processi ottengano una porzione equa della CPU.
   - L'efficienza dipende dalla lunghezza del time slice: se troppo breve, il sistema passa troppo tempo a cambiare contesto tra i processi; se troppo lungo, il sistema diventa meno reattivo.

4. **Priority Scheduling**  
   - Ogni processo ha una priorità associata. La CPU viene assegnata al processo con la priorità più alta.
   - Può essere preemptivo o non preemptivo.
   - Può soffrire di **starvation**, dove processi a bassa priorità non vengono mai eseguiti. Questo problema può essere mitigato utilizzando l'**invecchiamento** (aging), dove la priorità di un processo aumenta nel tempo.

5. **Multilevel Queue Scheduling**  
   - I processi vengono divisi in diverse code in base alla priorità o alla tipologia (ad esempio, processi interattivi vs batch). Ogni coda può avere un proprio algoritmo di scheduling.
   - Ad esempio, i processi interattivi possono essere eseguiti con Round Robin, mentre i processi batch possono essere gestiti con FCFS.

6. **Multilevel Feedback Queue Scheduling**  
   - È una variante avanzata del Multilevel Queue. I processi possono spostarsi tra le code in base al loro comportamento e al tempo di esecuzione. Ad esempio, un processo che consuma molta CPU può essere spostato in una coda a bassa priorità, mentre un processo che attende frequentemente I/O può ricevere una priorità più alta.

#### **Schedulazione in Linux:**

Linux utilizza un **scheduler preemptivo** con un approccio basato su priorità, ottimizzato per bilanciare carichi di lavoro interattivi e di background. L'algoritmo di scheduling attuale è il **Completely Fair Scheduler (CFS)**, introdotto con il kernel 2.6.23. CFS assegna a ciascun processo una porzione equa della CPU, cercando di mantenere una distribuzione "giusta" tra i processi, tenendo conto delle loro priorità.

- **CFS** non si basa su time slice fissi come il Round Robin, ma utilizza un modello proporzionale. Ogni processo ha un "tempo virtuale" che viene confrontato con quello degli altri processi per determinare chi ha ricevuto meno tempo CPU e dovrebbe quindi essere eseguito per recuperare.
- Processi interattivi ricevono più tempo CPU, poiché tendono a utilizzare meno risorse e ad attendere eventi I/O.

#### **Conclusione:**

Multitasking e schedulazione sono essenziali per garantire che i processi in esecuzione condividano equamente le risorse della CPU e che il sistema risponda in modo efficiente alle richieste degli utenti e dei processi. Un buon algoritmo di scheduling ottimizza l'uso della CPU, minimizza il tempo di attesa e migliora le prestazioni complessive del sistema.

---
[INDICE](README.md)