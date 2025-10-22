### **1.2 Multitasking con i Microcontrollori**

Il concetto di **multitasking** (o gestione multi-task) nei microcontrollori, come Arduino, è fondamentale per la progettazione di sistemi embedded in grado di eseguire più compiti contemporaneamente. Tuttavia, a differenza dei computer moderni che utilizzano sistemi operativi full-fledged con scheduler dedicati, i microcontrollori hanno risorse limitate e non dispongono di un vero e proprio sistema operativo multitasking. Pertanto, spesso il multitasking deve essere implementato manualmente dal programmatore.

---

### **Importanza del multitasking nei sistemi embedded**

I sistemi embedded sono progettati per operare in contesti real-time e con risorse limitate. Il multitasking, in questo ambito, assume una rilevanza particolare per i seguenti motivi:

- **Reattività**: Permette di gestire eventi esterni e critici nel minor tempo possibile, garantendo risposte immediate quando si verifica un cambiamento nell'ambiente.
- **Efficienza**: Ottimizza l'utilizzo della CPU, evitando che il sistema rimanga inattivo in attesa di un evento, e distribuendo il carico di lavoro in maniera equilibrata.
- **Modularità e Manutenzione**: Favorisce un'architettura del software modulare, in cui ciascun compito è isolato e facilmente gestibile, semplificando così il debug e l’aggiornamento del sistema.

---

### **Caratteristiche dei sistemi embedded**

Prima di approfondire il multitasking, è importante considerare le peculiarità dei sistemi embedded:

- **Risorse Limitate**: A differenza dei sistemi desktop o server, i sistemi embedded operano spesso con processori a bassa potenza, memoria ridotta e limitate capacità di storage.
- **Operatività in Tempo Reale**: Molte applicazioni embedded devono rispondere a eventi esterni entro stretti vincoli temporali, rendendo critica la gestione del tempo di esecuzione.
- **Affidabilità e Stabilità**: Questi sistemi sono frequentemente impiegati in ambiti dove il fallimento non è un’opzione, richiedendo soluzioni software robuste e resiliente agli errori.
- **Interazione con il Mondo Fisico**: La capacità di interfacciarsi con sensori, attuatori e altri dispositivi è fondamentale, richiedendo una gestione precisa degli I/O e dei segnali esterni.

---

### **Caratteristiche del Multitasking nei Microcontrollori** 

1. **Singolo Core**: I microcontrollori tipici (come quelli usati in Arduino) hanno un solo core di elaborazione. Ciò significa che possono eseguire un'istruzione alla volta, ma attraverso tecniche software possiamo simulare l'esecuzione parallela di più task.
   
2. **Non Bloccante**: Il multitasking nei microcontrollori è spesso implementato usando tecniche non bloccanti, come `millis()`, per garantire che nessun task blocchi l'esecuzione degli altri.

3. **Temporizzazione**: La gestione del tempo è cruciale nel multitasking. Ogni task viene eseguito periodicamente o in base a determinati eventi, senza interferire con gli altri.

4. **Priorità e Scadenze**: Nei sistemi embedded, alcuni task possono avere priorità più alta rispetto ad altri. Ad esempio, un sensore di sicurezza potrebbe richiedere una risposta immediata, mentre un display può essere aggiornato con meno frequenza.

---

### **Approcci al Multitasking nei Microcontrollori**

#### **1. Multitasking Cooperativo**
In questo approccio, ogni task si occupa di completare una piccola parte del suo lavoro e poi "cede" il controllo al ciclo principale (`loop()`), permettendo agli altri task di essere eseguiti. Questo tipo di multitasking è detto cooperativo perché ogni task collabora attivamente per mantenere il sistema responsivo.

##### Esempio: [https://wokwi.com/projects/423168989470781441](https://wokwi.com/projects/423168989470781441)
```cpp
unsigned long previousMillisTask1 = 0;
unsigned long previousMillisTask2 = 0;

const long intervalTask1 = 1000; // Intervallo per Task 1 (1 secondo)
const long intervalTask2 = 500;  // Intervallo per Task 2 (0.5 secondi)

void loop() {
  unsigned long currentMillis = millis();

  // Task 1: Accende/spegne un LED ogni 1 secondo
  if (currentMillis - previousMillisTask1 >= intervalTask1) {
    previousMillisTask1 = currentMillis;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle LED
  }

  // Task 2: Stampa un messaggio ogni 0.5 secondi
  if (currentMillis - previousMillisTask2 >= intervalTask2) {
    previousMillisTask2 = currentMillis;
    Serial.println("Task 2 in esecuzione!");
  }
}
```

- **Vantaggi**: Semplice da implementare, efficiente in termini di memoria.
- **Svantaggi**: Se un task richiede troppo tempo, può rallentare l'intero sistema.

---

#### **2. Multitasking con Interrupt**
Gli interrupt consentono di reagire immediatamente a eventi esterni (ad esempio, il premere di un pulsante o il superamento di una soglia da un sensore) senza dover controllare periodicamente lo stato del sistema.

##### Esempio:
```cpp
volatile bool buttonPressed = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, FALLING);
}

void loop() {
  if (buttonPressed) {
    Serial.println("Pulsante premuto!");
    buttonPressed = false;
  }
}

void button_ISR() {
  buttonPressed = true;
}
```

- **Vantaggi**: Risposte immediate agli eventi critici, ideale per interruzioni hardware.
- **Svantaggi**: Complessità maggiore, rischio di overhead se ci sono troppe interruzioni.

---

#### **3. Multitasking con Librerie Specializzate**
Esistono librerie per Arduino che semplificano l'implementazione del multitasking, come `TaskScheduler` o `ArduinoTimer`. Queste librerie forniscono strumenti per pianificare e gestire più task in modo efficiente.

##### Esempio con `TaskScheduler`:
```cpp
#include <TaskScheduler.h>

Scheduler runner;

// Definizione dei task
void task1() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle LED
}

void task2() {
  Serial.println("Task 2 in esecuzione!");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Crea i task
  Task t1(1000, TASK_FOREVER, &task1); // Task 1 ogni 1 secondo
  Task t2(500, TASK_FOREVER, &task2);  // Task 2 ogni 0.5 secondi

  // Aggiunge i task allo scheduler
  runner.addTask(t1);
  runner.addTask(t2);

  t1.enable();
  t2.enable();
}

void loop() {
  runner.execute(); // Esegue i task pianificati
}
```

- **Vantaggi**: Facilità di uso, gestione automatica dei tempi e delle priorità.
- **Svantaggi**: Maggiore consumo di memoria, dipendenza da librerie esterne.

---

### **Limitazioni del Multitasking nei Microcontrollori**

1. **Risorse Limitate**: I microcontrollori hanno CPU lente e memoria ridotta, quindi il numero di task che possono essere gestiti contemporaneamente è limitato.
   
2. **Overhead**: L'implementazione del multitasking introduce un certo overhead, specialmente quando si utilizzano interrupt o librerie complesse.

3. **Complessità**: Gestire molti task simultaneamente può aumentare la complessità del codice, rendendo difficile il debug e la manutenzione.

---

### **Concetti Chiave per il Multitasking nei Microcontrollori**

1. **Non Bloccante**: Usare tecniche non bloccanti (come `millis()`) per evitare che un task blocchi l'esecuzione degli altri.
   
2. **Polling vs Interrupt**: Scegliere tra polling (controllo periodico) e interrupt (risposta immediata) in base alle esigenze del sistema.

3. **Priorità e Scadenze**: Assegnare priorità ai task in base alla loro importanza e definire scadenze per garantire tempi di risposta adeguati.

4. **Efficienza**: Ottimizzare i task per minimizzare il tempo di esecuzione e massimizzare l'utilizzo delle risorse.

---

### **Esempi di Applicazioni del Multitasking**

1. **Controllo di Sensori e Attuatori**: Gestire contemporaneamente più sensori (umidità, temperatura, luminosità) e attuatori (motori, pompe, LED).
   
2. **Interfaccia Utente**: Gestire input utente (pulsanti, touchscreen) mentre si aggiorna uno schermo LCD o si comunica via seriale.

3. **Comunicazione Wireless**: Gestire la comunicazione wireless (Bluetooth, Wi-Fi) mentre si eseguono altre operazioni.

4. **Sistemi di Sicurezza**: Monitorare sensori di movimento, temperature critiche o allarmi mentre si gestisce la comunicazione con un server remoto.

---

### Evoluzione del multitasking nei sistemi embedded

Storicamente, i primi sistemi embedded erano progettati per svolgere un'unica funzione e non richiedevano la complessità del multitasking. Con l’aumentare delle esigenze applicative e l’evoluzione dell’hardware, è diventato necessario gestire più compiti contemporaneamente. Questo ha portato allo sviluppo di:

- **Sistemi Operativi Real-Time (RTOS)**: Software specializzati che garantiscono tempi di risposta prevedibili e gestiscono la concorrenza tra i processi.
- **Tecniche di Scheduling**: Metodi per assegnare priorità e allocare il tempo di CPU in modo efficiente (ad esempio, round-robin, scheduling a priorità, e preemption).

---

### **Conclusione**

Il multitasking nei microcontrollori è una tecnica essenziale per costruire sistemi embedded capaci di gestire più compiti contemporaneamente. Attraverso l'uso di tecniche non bloccanti, interrupt e librerie specializzate, è possibile simulare l'esecuzione parallela di task anche su hardware limitato come Arduino. Tuttavia, è importante tenere sempre presente le limitazioni delle risorse e ottimizzare il codice per garantire prestazioni efficaci e affidabili.