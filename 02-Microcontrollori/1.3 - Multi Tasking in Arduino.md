### Estensione del Parallelismo tra Cuoco e Scrittura di Software con Arduino

La preparazione di un pasto da parte di un cuoco può essere facilmente paragonata alla scrittura di software per un microcontrollore come Arduino. Entrambi i processi richiedono la gestione di più compiti contemporaneamente, l'uso di interrupt o polling per reagire a eventi, e una pianificazione accurata delle risorse.

---

### Multi Tasking in Arduino

In Arduino, il **multi tasking** non è nativo come nei sistemi operativi moderni, ma può essere simulato attraverso tecniche come il **temporizzatore** (usando `millis()`), lo **stato attuale** di variabili, o librerie specializzate come `TaskScheduler`. L'idea è quella di dividere il tempo di esecuzione del programma tra diverse funzioni o routine.

**Esempio:**
Immagina un progetto Arduino che deve:
1. Leggere i valori di un sensore di temperatura ogni 5 secondi.
2. Controllare se un pulsante è stato premuto per accendere/spegnere un LED.
3. Invocare un segnale sonoro ogni volta che la temperatura supera una certa soglia.

Un approccio multi-tasking potrebbe essere strutturato come segue:

```cpp
unsigned long previousMillis = 0; // Variabile per memorizzare il tempo passato
const long interval = 5000;       // Intervallo di lettura del sensore (in millisecondi)

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);   // Configura il pin del LED
}

void loop() {
  unsigned long currentMillis = millis();

  // Task 1: Leggi il sensore di temperatura ogni 5 secondi
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float temperature = readTemperature(); // Funzione ipotetica per leggere il sensore
    Serial.println("Temperatura letta: " + String(temperature));

    // Task 3: Se la temperatura è troppo alta, emetti un segnale sonoro
    if (temperature > 30) {
      tone(8, 1000, 500); // Emette un suono per 500 ms
    }
  }

  // Task 2: Controlla se il pulsante è stato premuto
  if (digitalRead(BUTTON_PIN) == HIGH) { // Se il pulsante è premuto
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Accendi/spegni il LED
  }
}
```

Qui, il codice simula il multi tasking gestendo tre attività principali:
1. Lettura periodica del sensore di temperatura.
2. Controllo dello stato del pulsante.
3. Azione condizionale basata sulla temperatura.

Come nel caso del cuoco, il programma divide il proprio tempo tra queste attività senza bloccarsi completamente su una sola.

---

### Interrupt in Arduino

Gli **interrupt** in Arduino sono utilizzati per eseguire una determinata azione quando si verifica un evento specifico, senza dover controllare manualmente lo stato del sistema periodicamente. Sono particolarmente utili quando un evento richiede una risposta immediata.

**Esempio:**
Supponiamo che vogliamo accendere un LED ogni volta che un pulsante viene premuto, indipendentemente da ciò che sta facendo il programma principale. Possiamo usare un interrupt per questo scopo.

```cpp
volatile bool buttonPressed = false; // Variabile globale per tenere traccia dello stato del pulsante

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Configura il pin del LED
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, RISING); // Attiva l'interrupt
}

void loop() {
  if (buttonPressed) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Accendi/spegni il LED
    buttonPressed = false; // Resetta lo stato del pulsante
  }
}

// Funzione di callback per l'interrupt
void button_ISR() {
  buttonPressed = true; // Imposta la variabile quando il pulsante è premuto
}
```

In questo caso, l'interrupt (`attachInterrupt`) permette al programma di reagire immediatamente al premere del pulsante, senza dover controllare periodicamente lo stato del pulsante nel ciclo principale (`loop()`). Questo è simile al timer del forno che avvisa il cuoco quando il pane è pronto.

---

### Polling in Arduino

Il **polling** in Arduino consiste nel controllare periodicamente lo stato di un dispositivo o di un evento. È meno efficiente degli interrupt, ma può essere utile quando gli eventi non sono critici o quando non si ha bisogno di una risposta immediata.

**Esempio:**
Supponiamo di voler controllare periodicamente se un sensore di movimento rileva qualcosa e accendere un LED in caso affermativo.

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Configura il pin del LED
  pinMode(MOTION_SENSOR_PIN, INPUT); // Configura il pin del sensore di movimento
}

void loop() {
  if (digitalRead(MOTION_SENSOR_PIN) == HIGH) { // Controlla lo stato del sensore
    digitalWrite(LED_BUILTIN, HIGH); // Accendi il LED
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Spegni il LED
  }
  delay(100); // Attendi 100 ms prima di controllare di nuovo
}
```

In questo caso, il programma controlla manualmente lo stato del sensore di movimento ad intervalli regolari (ogni 100 ms). Questo è simile al cuoco che controlla periodicamente il pollo alla griglia per assicurarsi che non si bruci.

---

### Confronto tra Interrupt e Polling in Arduino

| **Caratteristica**       | **Interrupt**                                | **Polling**                                  |
|--------------------------|---------------------------------------------|---------------------------------------------|
| **Quando si attiva?**    | Solo quando si verifica un evento specifico. | Ad intervalli regolari, indipendentemente dall'evento. |
| **Efficienza**           | Più efficiente perché agisce solo quando necessario. | Meno efficiente perché richiede controlli continui anche quando non ci sono cambiamenti. |
| **Esempio con Arduino**  | Un pulsante che attiva un LED tramite interrupt. | Un sensore di movimento controllato periodicamente nel loop(). |

---

### Conclusioni

- Il **multi tasking** in Arduino permette di simulare la gestione di più compiti contemporaneamente, dividendosi il tempo tra le varie attività.
- Gli **interrupt** consentono di reagire immediatamente a eventi esterni senza dover controllare periodicamente lo stato del sistema.
- Il **polling** è un metodo semplice ma meno efficiente per verificare lo stato di dispositivi o eventi ad intervalli regolari.

Tornando all'analogia del cuoco:
- Il cuoco che usa un timer è simile a un programma che utilizza interrupt.
- Il cuoco che controlla manualmente il cibo è simile a un programma che utilizza polling.
- Il cuoco che gestisce più piatti contemporaneamente è simile a un programma che implementa il multi tasking.

Scegliere l'approccio giusto dipende dalle esigenze specifiche del progetto, così come il cuoco sceglie il metodo migliore in base ai piatti che sta preparando.

Quando si utilizza il **polling** in Arduino, il programma controlla periodicamente lo stato di un dispositivo o di un evento per decidere se eseguire una determinata azione. Aggiungere nuovi task al software significa semplicemente inserire ulteriori controlli nel ciclo principale (`loop()`), garantendo che vengano eseguiti a intervalli regolari.

Di seguito, vediamo come espandere un programma Arduino basato su polling aggiungendo nuovi task. Partiamo dallo stesso esempio precedente e aggiungiamo altri compiti da gestire.

---

### Esempio Base con Polling

Supponiamo di avere già un programma che:
1. Controlla uno stato di un pulsante.
2. Legge un sensore di temperatura ogni 5 secondi.

```cpp
const int BUTTON_PIN = 2; // Pin del pulsante
const int TEMPERATURE_SENSOR_PIN = A0; // Pin del sensore di temperatura
unsigned long previousMillis = 0; // Variabile per memorizzare il tempo passato
const long TEMPERATURE_INTERVAL = 5000; // Intervallo di lettura del sensore (in millisecondi)

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura il pin del pulsante con resistenza pull-up
}

void loop() {
  unsigned long currentMillis = millis();

  // Task 1: Controlla lo stato del pulsante
  if (digitalRead(BUTTON_PIN) == LOW) { // Se il pulsante è premuto
    Serial.println("Pulsante premuto!");
  }

  // Task 2: Legge il sensore di temperatura ogni 5 secondi
  if (currentMillis - previousMillis >= TEMPERATURE_INTERVAL) {
    previousMillis = currentMillis;
    int sensorValue = analogRead(TEMPERATURE_SENSOR_PIN); // Legge il valore analogico
    float temperature = mapTemperature(sensorValue); // Mappa il valore in gradi Celsius
    Serial.print("Temperatura: ");
    Serial.println(temperature);
  }
}

// Funzione per mappare il valore analogico in temperatura
float mapTemperature(int value) {
  return (value * 0.48828125); // Conversione semplice per LM35
}
```

---

### Aggiunta di Nuovi Task con Polling

#### Task 3: Controlla un Sensore di Luce
Aggiungiamo un sensore di luce (ad esempio, un fototransistore) collegato al pin `A1`. Vogliamo leggere il valore del sensore ogni 2 secondi e stamparlo.

```cpp
const int LIGHT_SENSOR_PIN = A1; // Pin del sensore di luce
unsigned long lightPreviousMillis = 0; // Variabile per il sensore di luce
const long LIGHT_INTERVAL = 2000; // Intervallo di lettura del sensore di luce (in millisecondi)

// Task 3: Legge il sensore di luce ogni 2 secondi
if (currentMillis - lightPreviousMillis >= LIGHT_INTERVAL) {
  lightPreviousMillis = currentMillis;
  int lightValue = analogRead(LIGHT_SENSOR_PIN); // Legge il valore analogico
  Serial.print("Luminosità: ");
  Serial.println(lightValue);
}
```

---

#### Task 4: Gestione di un Timer Software
Aggiungiamo un timer software che stampa un messaggio ogni 10 secondi.

```cpp
unsigned long timerPreviousMillis = 0; // Variabile per il timer
const long TIMER_INTERVAL = 10000; // Intervallo del timer (in millisecondi)

// Task 4: Stampa un messaggio ogni 10 secondi
if (currentMillis - timerPreviousMillis >= TIMER_INTERVAL) {
  timerPreviousMillis = currentMillis;
  Serial.println("Timer scattato!");
}
```

---

### Codice Completato con Tutti i Task

Ecco il codice completo con tutti i task gestiti tramite polling:

```cpp
const int BUTTON_PIN = 2; // Pin del pulsante
const int TEMPERATURE_SENSOR_PIN = A0; // Pin del sensore di temperatura
const int LIGHT_SENSOR_PIN = A1; // Pin del sensore di luce

unsigned long previousMillis = 0; // Variabile per il sensore di temperatura
unsigned long lightPreviousMillis = 0; // Variabile per il sensore di luce
unsigned long timerPreviousMillis = 0; // Variabile per il timer

const long TEMPERATURE_INTERVAL = 5000; // Intervallo di lettura del sensore di temperatura
const long LIGHT_INTERVAL = 2000; // Intervallo di lettura del sensore di luce
const long TIMER_INTERVAL = 10000; // Intervallo del timer

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura il pin del pulsante con resistenza pull-up
}

void loop() {
  unsigned long currentMillis = millis();

  // Task 1: Controlla lo stato del pulsante
  if (digitalRead(BUTTON_PIN) == LOW) { // Se il pulsante è premuto
    Serial.println("Pulsante premuto!");
  }

  // Task 2: Legge il sensore di temperatura ogni 5 secondi
  if (currentMillis - previousMillis >= TEMPERATURE_INTERVAL) {
    previousMillis = currentMillis;
    int sensorValue = analogRead(TEMPERATURE_SENSOR_PIN); // Legge il valore analogico
    float temperature = mapTemperature(sensorValue); // Mappa il valore in gradi Celsius
    Serial.print("Temperatura: ");
    Serial.println(temperature);
  }

  // Task 3: Legge il sensore di luce ogni 2 secondi
  if (currentMillis - lightPreviousMillis >= LIGHT_INTERVAL) {
    lightPreviousMillis = currentMillis;
    int lightValue = analogRead(LIGHT_SENSOR_PIN); // Legge il valore analogico
    Serial.print("Luminosità: ");
    Serial.println(lightValue);
  }

  // Task 4: Stampa un messaggio ogni 10 secondi
  if (currentMillis - timerPreviousMillis >= TIMER_INTERVAL) {
    timerPreviousMillis = currentMillis;
    Serial.println("Timer scattato!");
  }
}

// Funzione per mappare il valore analogico in temperatura
float mapTemperature(int value) {
  return (value * 0.48828125); // Conversione semplice per LM35
}
```

---

### Spiegazione dei Task

1. **Task 1: Pulsante**
   - Controlla continuamente lo stato del pulsante. Quando viene premuto, stampa un messaggio.

2. **Task 2: Sensore di Temperatura**
   - Legge il valore del sensore di temperatura ogni 5 secondi e lo stampa.

3. **Task 3: Sensore di Luce**
   - Legge il valore del sensore di luce ogni 2 secondi e lo stampa.

4. **Task 4: Timer Software**
   - Stampa un messaggio ogni 10 secondi, indipendentemente dagli altri task.

---

### Considerazioni Finali

- Ogni task è gestito separatamente all'interno del ciclo `loop()`, senza bloccare l'esecuzione degli altri.
- L'utilizzo di `millis()` permette di implementare un sistema non bloccante, dove ogni task viene eseguito a intervalli specifici.
- Il polling richiede attenzione alla gestione dei tempi per evitare che il ciclo principale diventi troppo pesante, rallentando la risposta del sistema.

In questo modo, possiamo aggiungere quanti task necessari, sempre mantenendo il controllo sulle operazioni grazie al polling.