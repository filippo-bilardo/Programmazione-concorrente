# ES01 - Sistema Controllo LED con Pulsanti

## 📋 Obiettivi dell'Esercitazione

Sviluppare un sistema di controllo LED utilizzando **4 approcci diversi** per comprendere l'evoluzione dai sistemi sequenziali ai sistemi real-time.

**Requisiti funzionali:**
- Pulsante 1 (BTN1) controlla LED1 (rosso)
- Pulsante 2 (BTN2) controlla LED2 (verde)
- Quando un pulsante è premuto, il LED corrispondente lampeggia (100ms ON, 100ms OFF)
- Quando un pulsante è rilasciato, il LED rimane spento
- Il sistema deve rispondere a entrambi i pulsanti contemporaneamente

---

## 🎯 Competenze Sviluppate

- Programmazione sequenziale vs concorrente
- Gestione input/output digitali
- Debouncing software pulsanti
- Multi-tasking cooperativo (polling)
- Interrupt hardware (pin change)
- RTOS e task scheduling
- Sincronizzazione e priorità

---

## 🔧 Hardware Necessario

### Componenti
- 1x Arduino Uno/Nano/Mega (o ESP32 per approccio 4)
- 2x LED (1 rosso, 1 verde)
- 2x Resistenze 220Ω (per LED)
- 2x Pulsanti (normalmente aperti)
- 2x Resistenze 10kΩ (pull-down per pulsanti)
- 1x Breadboard
- Cavi jumper

### Schema Collegamenti

```
Arduino          Componenti
───────────────────────────────────────────────
Pin 8    ───[220Ω]───►|─── GND  (LED1 Rosso)
Pin 9    ───[220Ω]───►|─── GND  (LED2 Verde)

Pin 2    ───┬─── BTN1 ─── +5V   (Pulsante 1)
            │
         [10kΩ]
            │
           GND

Pin 3    ───┬─── BTN2 ─── +5V   (Pulsante 2)
            │
         [10kΩ]
            │
           GND
```

**Nota:** 
- Resistenze 10kΩ = pull-down (pulsante premuto = HIGH)
- Pin 2 e 3 supportano interrupt hardware (Arduino Uno)

---

## 📚 Struttura Esercitazione

L'esercitazione è divisa in **4 approcci progressivi**, ciascuno in una cartella separata:

```
ES01-SistemaControlloLED/
├── README.md                    (questo file)
├── WOKWI.md                     (guida simulazione)
├── soluzione/
│   ├── approccio1_sequenziale.ino
│   ├── approccio2_polling.ino
│   ├── approccio3_interrupt.ino
│   └── approccio4_rtos.ino
├── template/
│   ├── template_approccio1.ino
│   ├── template_approccio2.ino
│   ├── template_approccio3.ino
│   └── template_approccio4.ino
└── test/
    ├── test_hardware.ino         (verifica collegamenti)
    └── diagram.json              (Wokwi)
```

---

## 🚀 Approccio 1: Codice Sequenziale (BLOCCANTE)

### Obiettivo
Implementare il controllo LED usando **delay()** e lettura sequenziale dei pulsanti.

### Concetti Chiave
- Polling pulsanti in sequenza
- Delay bloccante
- Debouncing semplice con delay

### Specifiche Tecniche
```cpp
void loop() {
  1. Leggi stato BTN1
  2. Se premuto: LED1 ON, delay 100ms, LED1 OFF, delay 100ms
  3. Leggi stato BTN2  
  4. Se premuto: LED2 ON, delay 100ms, LED2 OFF, delay 100ms
}
```

### Limitazioni Attese
- ❌ Se BTN1 premuto, BTN2 non risponde (CPU bloccata)
- ❌ LED non lampeggiano simultaneamente
- ❌ Risposta lenta e poco reattiva

### Tempo Stimato
**30 minuti**

### Output Atteso
```
[0ms] BTN1 premuto -> LED1 lampeggia
[200ms] BTN2 premuto -> nessuna risposta (CPU bloccata!)
[400ms] BTN2 finalmente risponde
```

---

## 🚀 Approccio 2: Polling Non Bloccante (MILLIS)

### Obiettivo
Usare **millis()** per gestire timing senza bloccare, permettendo lettura continua di entrambi i pulsanti.

### Concetti Chiave
- Polling continuo pulsanti
- Timer software con millis()
- State machine per gestione LED
- Debouncing accurato

### Specifiche Tecniche
```cpp
// Variabili di stato
unsigned long led1_previousMillis = 0;
unsigned long led2_previousMillis = 0;
bool btn1_pressed = false;
bool btn2_pressed = false;
int led1_state = LOW;
int led2_state = LOW;

void loop() {
  unsigned long currentMillis = millis();
  
  // Leggi pulsanti (sempre!)
  btn1_pressed = digitalRead(BTN1_PIN) == HIGH;
  btn2_pressed = digitalRead(BTN2_PIN) == HIGH;
  
  // Gestione LED1 (se BTN1 premuto)
  if (btn1_pressed) {
    if (currentMillis - led1_previousMillis >= 100) {
      led1_state = !led1_state;
      digitalWrite(LED1_PIN, led1_state);
      led1_previousMillis = currentMillis;
    }
  } else {
    digitalWrite(LED1_PIN, LOW);
  }
  
  // Gestione LED2 (simile)
}
```

### Vantaggi Attesi
- ✅ Entrambi pulsanti sempre monitorati
- ✅ LED lampeggiano contemporaneamente
- ✅ Reattività eccellente

### Tempo Stimato
**45 minuti**

### Esercizio Bonus
Implementare debouncing software (50ms) per evitare falsi trigger.

---

## 🚀 Approccio 3: Interrupt Hardware

### Obiettivo
Usare **interrupt su pin 2 e 3** per rilevare pressione pulsanti, con timer per lampeggio LED.

### Concetti Chiave
- Interrupt External (attachInterrupt)
- Timer interrupt (TimerOne library)
- ISR (Interrupt Service Routine)
- Variabili volatile

### Specifiche Tecniche
```cpp
volatile bool btn1_active = false;
volatile bool btn2_active = false;
volatile int led1_state = LOW;
volatile int led2_state = LOW;

// ISR per BTN1
void btn1ISR() {
  btn1_active = digitalRead(BTN1_PIN);
}

// ISR per BTN2
void btn2ISR() {
  btn2_active = digitalRead(BTN2_PIN);
}

// ISR Timer (chiamata ogni 100ms)
void timerISR() {
  if (btn1_active) {
    led1_state = !led1_state;
    digitalWrite(LED1_PIN, led1_state);
  } else {
    digitalWrite(LED1_PIN, LOW);
  }
  
  if (btn2_active) {
    led2_state = !led2_state;
    digitalWrite(LED2_PIN, led2_state);
  } else {
    digitalWrite(LED2_PIN, LOW);
  }
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), btn1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), btn2ISR, CHANGE);
  
  Timer1.initialize(100000);  // 100ms
  Timer1.attachInterrupt(timerISR);
}
```

### Vantaggi Attesi
- ✅ Risposta immediata ai pulsanti (interrupt)
- ✅ Timing preciso lampeggio (timer hardware)
- ✅ CPU può fare altro (o dormire)

### Tempo Stimato
**60 minuti**

### Sfida
Implementare debouncing hardware con condensatore (0.1µF).

---

## 🚀 Approccio 4: FreeRTOS

### Obiettivo
Creare **3 task separati**: TaskButton1, TaskButton2, TaskStats.

### Concetti Chiave
- Task creation e scheduler
- Priorità task
- Sincronizzazione (semafori opzionale)
- vTaskDelay non bloccante

### Specifiche Tecniche
```cpp
// Task per gestione LED1
void TaskButton1(void *pvParameters) {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(BTN1_PIN, INPUT);
  
  bool led_state = LOW;
  
  for(;;) {
    if (digitalRead(BTN1_PIN) == HIGH) {
      // Pulsante premuto: lampeggia
      led_state = !led_state;
      digitalWrite(LED1_PIN, led_state);
      vTaskDelay(pdMS_TO_TICKS(100));
    } else {
      // Pulsante rilasciato: spento
      digitalWrite(LED1_PIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(50)); // Polling più veloce
    }
  }
}

// Task per gestione LED2 (simile)
void TaskButton2(void *pvParameters) {
  // ... analogo a TaskButton1
}

// Task per statistiche
void TaskStats(void *pvParameters) {
  for(;;) {
    Serial.print("Free heap: ");
    Serial.println(xPortGetFreeHeapSize());
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  xTaskCreate(TaskButton1, "BTN1", 128, NULL, 2, NULL);
  xTaskCreate(TaskButton2, "BTN2", 128, NULL, 2, NULL);
  xTaskCreate(TaskStats, "Stats", 256, NULL, 1, NULL);
}
```

### Vantaggi Attesi
- ✅ Massima modularità (task indipendenti)
- ✅ Facile aggiungere funzionalità
- ✅ Priorità configurabili
- ✅ Codice scalabile

### Tempo Stimato
**75 minuti**

### Sfida Avanzata
Usare Binary Semaphore per sincronizzare LED (quando BTN1+BTN2 premuti insieme, lampeggio sincronizzato).

---

## 📊 Tabella Comparativa Approcci

| Aspetto           | Sequenziale | Polling   | Interrupt | RTOS      |
|-------------------|-------------|-----------|-----------|-----------|
| Reattività BTN    | ❌ Pessima  | ✅ Ottima | ⭐ Istantanea | ✅ Ottima |
| Lampeggio Simult. | ❌ NO       | ✅ SI     | ✅ SI     | ✅ SI     |
| Complessità       | ★☆☆         | ★★☆       | ★★★       | ★★★★      |
| Linee Codice      | ~40         | ~80       | ~100      | ~120      |
| RAM Usage         | Minima      | Minima    | Minima    | Alta      |
| Debouncing        | Difficile   | Facile    | Medio     | Facile    |
| Scalabilità       | Pessima     | Buona     | Media     | Ottima    |

---

## ✅ Criteri di Valutazione

### Funzionalità (40%)
- [ ] BTN1 controlla LED1 correttamente
- [ ] BTN2 controlla LED2 correttamente
- [ ] Lampeggio a 100ms (±10ms tolleranza)
- [ ] Risposta simultanea a entrambi pulsanti
- [ ] LED spento quando pulsante rilasciato

### Codice (30%)
- [ ] Codice commentato adeguatamente
- [ ] Nomi variabili significativi
- [ ] Struttura logica chiara
- [ ] Costanti definite (no numeri magici)
- [ ] Gestione errori (dove applicabile)

### Comprensione (20%)
- [ ] Risposta corretta a domande teoriche
- [ ] Spiegazione differenze tra approcci
- [ ] Identificazione pro/contro ciascun approccio

### Testing (10%)
- [ ] Test con Wokwi/hardware
- [ ] Verifica timing con Serial.print
- [ ] Test casi edge (pressione simultanea)

---

## 🎓 Domande di Verifica

### Approccio 1 - Sequenziale
1. Cosa succede se premi BTN1 e poi BTN2 rapidamente?
2. Perché LED2 non risponde mentre LED1 lampeggia?
3. Come migliorare la reattività senza cambiare approccio?

### Approccio 2 - Polling
4. Perché millis() è meglio di delay() in questo caso?
5. Cosa succede a millis() dopo 49 giorni? (overflow)
6. Come implementeresti debouncing a 50ms?

### Approccio 3 - Interrupt
7. Perché serve la keyword `volatile` per variabili condivise?
8. Cosa fa `attachInterrupt(digitalPinToInterrupt(2), ...)`?
9. Quali operazioni NON si possono fare in ISR?

### Approccio 4 - RTOS
10. Cosa fa lo scheduler FreeRTOS?
11. Differenza tra vTaskDelay() e delay()?
12. Cosa succede se uno stack task è troppo piccolo?

---

## 🔬 Testing e Debug

### Test Hardware

Usare `test/test_hardware.ino` per verificare:
```cpp
void loop() {
  // Test LED
  digitalWrite(LED1_PIN, HIGH);
  delay(500);
  digitalWrite(LED1_PIN, LOW);
  delay(500);
  
  // Test Pulsanti
  Serial.print("BTN1: ");
  Serial.print(digitalRead(BTN1_PIN));
  Serial.print(" | BTN2: ");
  Serial.println(digitalRead(BTN2_PIN));
}
```

### Test su Wokwi

1. Vai su https://wokwi.com
2. Nuovo progetto Arduino Uno (approcci 1-3) o ESP32 (approccio 4)
3. Carica `diagram.json` da cartella `test/`
4. Copia codice e simula
5. Premi pulsanti virtuali e osserva LED

### Debug Serial Monitor

Aggiungi output debug:
```cpp
Serial.print("[");
Serial.print(millis());
Serial.print("ms] BTN1=");
Serial.print(btn1_pressed);
Serial.print(" LED1=");
Serial.println(led1_state);
```

---

## 📖 Materiale di Studio

### Documentazione Arduino
- [digitalRead()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/)
- [digitalWrite()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/)
- [millis()](https://www.arduino.cc/reference/en/language/functions/time/millis/)
- [attachInterrupt()](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Librerie Usate
- **TimerOne:** https://github.com/PaulStoffregen/TimerOne
- **FreeRTOS:** https://www.freertos.org/

### Tutorial Consigliati
- Debouncing: https://www.arduino.cc/en/Tutorial/Debounce
- State Machines: https://majenko.co.uk/blog/software/finite-state-machine-arduino
- FreeRTOS Guide: https://www.freertos.org/fr-content-src/uploads/2018/07/161204_Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf

---

## 🚀 Estensioni Proposte

### Livello Intermedio
1. **Velocità Variabile:** Usa potenziometro per variare frequenza lampeggio (50-500ms)
2. **Contatore:** Conta quante volte ogni pulsante è stato premuto
3. **Pattern:** LED lampeggia con pattern diverso (SOS morse)

### Livello Avanzato
4. **LCD Display:** Mostra stato pulsanti su display 16x2
5. **Modalità:** Pulsante 1+2 insieme cambia modalità (sempre ON, sempre OFF, lampeggio)
6. **EEPROM:** Salva stato in EEPROM, ripristina dopo reset

### Livello Expert
7. **Comunicazione Seriale:** Controllo LED via comandi seriali ("LED1_ON", "LED2_BLINK")
8. **Web Server (ESP32):** Controllo LED via WiFi con pagina web
9. **MQTT (ESP32):** Pubblica stato pulsanti su broker MQTT

---

## 📦 Consegna Finale

### File da Consegnare

```
Cognome_Nome_ES01/
├── approccio1.ino
├── approccio2.ino
├── approccio3.ino
├── approccio4.ino
├── RELAZIONE.md         (o .pdf)
└── video_demo.mp4       (opzionale)
```

### Contenuto Relazione (max 3 pagine)

1. **Introduzione:** Descrizione problema e obiettivi
2. **Approcci Implementati:** Breve descrizione ciascuno (1 paragrafo)
3. **Confronto:** Tabella pro/contro, quale useresti in produzione?
4. **Problemi Incontrati:** Difficoltà e soluzioni adottate
5. **Test Effettuati:** Screenshot/foto setup hardware, output seriale
6. **Conclusioni:** Lezioni apprese, competenze acquisite

---

## 🏆 Grading Rubric

| Voto | Requisiti                                                    |
|------|--------------------------------------------------------------|
| 18-20| Approccio 1 funzionante                                      |
| 21-23| Approccio 1 + 2 funzionanti                                  |
| 24-26| Approccio 1 + 2 + 3 funzionanti                              |
| 27-28| Tutti e 4 approcci funzionanti                               |
| 29-30| Tutti approcci + debouncing + estensione                     |
| 30L  | Tutti approcci + estensione avanzata + relazione eccellente  |

---

## 🆘 Supporto

### FAQ

**Q: I pulsanti danno letture instabili (bounce).**  
A: Implementa debouncing software (delay 50ms o state machine) o hardware (condensatore 0.1µF).

**Q: Approccio 4 non compila su Arduino Uno.**  
A: Installa libreria "FreeRTOS" da Library Manager. Su ESP32 è già integrato.

**Q: Timer1 conflict con altre librerie.**  
A: Usa Timer2 (MsTimer2 library) o cambia libreria conflittuale.

**Q: RTOS crash con Arduino Uno.**  
A: Riduci STACK_SIZE o usa ESP32 (più RAM).

### Contatti Docente

- Email: nome.cognome@scuola.it
- Ricevimento: Martedì 14:00-15:00 (Lab Informatica)
- Forum: [Link Moodle]

---

## 📅 Timeline Suggerita

| Settimana | Attività                                    |
|-----------|---------------------------------------------|
| 1         | Setup hardware + Approccio 1 + Test         |
| 2         | Approccio 2 + Debouncing                    |
| 3         | Approccio 3 + Studio interrupt              |
| 4         | Approccio 4 (RTOS) + Confronto finale       |
| 5         | Relazione + Consegna                        |

---

**Buon lavoro! 🚀**

*Ricorda: L'obiettivo non è solo far funzionare il codice, ma COMPRENDERE le differenze tra approcci e quando usare ciascuno.*
