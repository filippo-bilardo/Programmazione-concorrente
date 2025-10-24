# Esempi Arduino: Lampeggio LED - Quattro Approcci

Questa cartella contiene quattro esempi progressivi che dimostrano diversi paradigmi di programmazione per il lampeggio simultaneo di 3 LED.

## 📁 Esempi

Quattro esempi progressivi che mostrano l'evoluzione da codice bloccante a sistema operativo real-time:

### 1. `01_led_sequenziale.ino` - Codice Sequenziale (BLOCCANTE)

**Concetto:** Uso di `delay()` per temporizzare.

**Caratteristiche:**
- ❌ CPU bloccata durante i delay
- ❌ Nessun parallelismo reale
- ❌ Non reattivo a eventi esterni
- ✅ Codice semplicissimo
- ✅ Ottimo per learning iniziale

**Quando usarlo:**
- Sketch semplici per apprendimento
- Nessuna necessità di reattività
- Prototipi veloci

**Output:**
```
LED1 ON → delay → LED1 OFF → delay → 
LED2 ON → delay → LED2 OFF → delay → 
LED3 ON → delay → LED3 OFF → delay → [ripete]
```

---

### 2. `02_led_polling.ino` - Polling con `millis()` (NON BLOCCANTE)

**Concetto:** Tecnica "Blink Without Delay" - ogni LED ha il proprio timer.

**Caratteristiche:**
- ✅ CPU sempre disponibile
- ✅ LED lampeggiano veramente in parallelo
- ✅ Timing flessibile per ogni LED
- ✅ Reattivo a eventi
- ✅ Scalabile (facile aggiungere task)
- ❌ Loop gira continuamente (no power saving)
- ❌ Precisione dipende dalla frequenza del loop

**Quando usarlo:**
- Maggior parte applicazioni Arduino
- Quando serve multi-tasking cooperativo
- Applicazioni interattive (sensori, pulsanti)
- Real-time "soft"

**Output:**
```
[1000ms] LED1 ON
[1200ms] LED3 ON
[1400ms] LED3 OFF
[1500ms] LED2 ON
[1600ms] LED3 ON
...
(LED lampeggiano CONTEMPORANEAMENTE)
```

---

### 3. `03_led_interrupt.ino` - Interrupt Hardware (TIMER)

**Concetto:** Timer Hardware genera interrupt a intervalli precisi.

**Caratteristiche:**
- ✅ Precisione massima (μs)
- ✅ CPU può dormire (sleep mode)
- ✅ Determinismo garantito
- ✅ Real-time "hard"
- ✅ Risparmio energetico
- ❌ Complessità maggiore
- ❌ Richiede comprensione interrupt
- ❌ Usa risorse hardware (timer)

**Quando usarlo:**
- Timing critico (preciso al microsecondo)
- Applicazioni embedded professionali
- Sistemi low-power (sleep tra interrupt)
- Quando polling non basta

**Output:**
```
[INFO] ISR chiamate: 10000
LED1 toggle: 20
LED2 toggle: 40
LED3 toggle: 100
Loop: CPU LIBERA per altri task!
```

---

### 4. `04_led_rtos.ino` - RTOS (Real-Time Operating System)

**Concetto:** FreeRTOS gestisce task indipendenti con scheduler preemptive.

**Caratteristiche:**
- ✅ Astrazione massima (come OS completo)
- ✅ Task veramente indipendenti
- ✅ Priorità configurabili
- ✅ Sincronizzazione robusta (semafori, mutex, code)
- ✅ Portabilità eccellente
- ✅ Scalabilità ottima
- ❌ Overhead context switching
- ❌ RAM usage elevato (stack per task)
- ❌ Curva apprendimento ripida

**Quando usarlo:**
- Progetti complessi (>5 task)
- Task con priorità diverse
- Necessità di modularità/manutenibilità
- Piattaforme con RAM adeguata (ESP32/ESP8266)
- Codice portabile tra piattaforme

**Output:**
```
[OK] Task LED1 creato (Priorità 1)
[OK] Task LED2 creato (Priorità 2)
[OK] Task LED3 creato (Priorità 3)
Scheduler avviato!

========== STATISTICHE (10s) ==========
LED1 toggle: 20
LED2 toggle: 40
LED3 toggle: 100
Task attivi: 4
Free Heap: 234560 bytes
```

---

## 🔧 Hardware Setup

### Componenti Necessari

- Arduino Uno/Nano/Mega
- 3x LED (rosso, verde, blu)
- 3x Resistenze 220Ω
- Breadboard
- Cavi jumper

### Schema Collegamenti

```
Arduino          Componente
─────────────────────────────
Pin 8    ───┬──[ R 220Ω ]──── LED1 (Anodo) ──── GND
Pin 9    ───┼──[ R 220Ω ]──── LED2 (Anodo) ──── GND
Pin 10   ───┴──[ R 220Ω ]──── LED3 (Anodo) ──── GND
```

**Nota:** Verifica polarità LED (gamba lunga = Anodo (+), gamba corta = Catodo (-))

### Schema Breadboard

```
     Arduino
    ┌───────┐
    │  D8   │──────[220Ω]────►|──┐
    │  D9   │──────[220Ω]────►|──┤
    │  D10  │──────[220Ω]────►|──┤
    │  GND  │──────────────────┴──┤
    └───────┘                     GND
    
    ►| = LED (anodo a sinistra)
```

---

## 🚀 Come Usare

### Prerequisiti

1. **Arduino IDE** installato
2. Board Arduino connessa via USB
3. **Librerie richieste:**
   
   **Per Esempio 3 (Interrupt):**
   ```
   Sketch → Include Library → Manage Libraries
   Cerca: "TimerOne"
   Installa: TimerOne by Paul Stoffregen
   ```
   
   **Per Esempio 4 (RTOS):**
   
   - **Arduino Uno/Mega (AVR):**
     ```
     Cerca: "FreeRTOS"
     Installa: FreeRTOS by Richard Barry
     ```
     ⚠️ **ATTENZIONE:** Arduino Uno ha RAM limitata (~2KB)!
     FreeRTOS funziona ma con forti limitazioni.
   
   - **ESP32/ESP8266:**
     ```
     FreeRTOS è già integrato nella board!
     Nessuna installazione necessaria.
     ```
     ✅ **CONSIGLIATO:** ESP32 ha 520KB RAM, perfetto per RTOS!

### Caricamento Sketch

1. Apri Arduino IDE
2. Carica uno degli sketch (`.ino`)
3. Seleziona board: `Tools → Board → Arduino Uno`
4. Seleziona porta: `Tools → Port → COM3` (o tua porta)
5. Compila e carica: `Sketch → Upload` (o Ctrl+U)
6. Apri Serial Monitor: `Tools → Serial Monitor` (115200 baud)

### Osservazione Risultati

- **Visivo:** Osserva i LED lampeggiare
- **Seriale:** Leggi output su Serial Monitor
- **Confronto:** Prova tutti e 3 gli esempi per vedere le differenze

---

## 📊 Confronto Approcci

| Aspetto              | Sequenziale | Polling (millis) | Interrupt (Timer) | RTOS (FreeRTOS)    |
|----------------------|-------------|------------------|-------------------|--------------------|
| **Precisione**       | Bassa       | Media (~1ms)     | Alta (~1μs)       | Media (~1ms)       |
| **CPU Libera**       | ❌ NO       | ⚠️ Parziale      | ✅ SI             | ✅ SI              |
| **Parallelismo**     | ❌ Falso    | ✅ Vero          | ✅ Vero           | ✅ Vero            |
| **Astrazione**       | Bassa       | Media            | Media             | ⭐ Altissima       |
| **Modularità**       | ❌ Pessima  | ⚠️ Media         | ⚠️ Media          | ⭐ Ottima          |
| **Complessità**      | ★☆☆         | ★★☆              | ★★★               | ★★★★☆              |
| **RAM Usage**        | Minimo      | Minimo           | Minimo            | ⚠️ Alto (stack/task)|
| **Overhead**         | Nessuno     | Basso            | Basso             | Medio-Alto         |
| **Power Saving**     | ❌          | ❌               | ✅ (con sleep)    | ✅ (IDLE hook)     |
| **Reattività**       | Pessima     | Buona            | Ottima            | Ottima             |
| **Scalabilità**      | Pessima     | Buona            | Media             | ⭐ Ottima          |
| **Priorità Task**    | ❌ NO       | ❌ NO            | ⚠️ Limitata       | ✅ SI (configurable)|
| **Sincronizzazione** | ❌ NO       | Manuale          | Manuale           | ⭐ Semafori/Mutex  |
| **Portabilità**      | Alta        | Alta             | Bassa (HW-dep)    | ⭐ Ottima          |
| **Risorse HW**       | Nessuna     | Nessuna          | Timer HW          | Nessuna            |
| **Debugging**        | Facile      | Medio            | Difficile         | Difficile          |
| **Learning Curve**   | ★☆☆         | ★★☆              | ★★★☆              | ★★★★★              |
| **Uso Real-World**   | Raro        | 80% progetti     | Sistemi critical  | Sistemi enterprise |

---

## 🎓 Concetti Didattici

### Esempio 1: Programmazione Sequenziale

**Paradigma:** Imperativo, esecuzione lineare

**Lezioni:**
- Flusso di controllo sequenziale
- Problema del blocking code
- Limitazioni delay()

**Analogia:** "Fare una cosa alla volta, aspettare che finisca prima di passare alla successiva"

---

### Esempio 2: Multi-tasking Cooperativo

**Paradigma:** Event-driven, stato-driven

**Lezioni:**
- Non-blocking code
- State machines
- Timer software
- Multi-tasking cooperativo (i task cooperano)

**Analogia:** "Cuoco che controlla 3 pentole sul fuoco, passando da una all'altra senza aspettare"

---

### Esempio 3: Interrupt-Driven Programming

**Paradigma:** Event-driven, interrupt-driven

**Lezioni:**
- Interrupt Service Routine (ISR)
- Hardware timer
- Preemptive scheduling (interrupt ha priorità)
- Atomic operations
- Volatile variables
- Race conditions

**Analogia:** "Timer da cucina che suona quando la pasta è pronta, mentre il cuoco fa altro"

---

### Esempio 4: Real-Time Operating System

**Paradigma:** Multi-threaded, preemptive scheduling

**Lezioni:**
- Task (thread leggeri)
- Scheduler preemptive con priorità
- Context switching
- Task states (RUNNING, READY, BLOCKED)
- Sincronizzazione (semafori, mutex, code)
- Stack allocation per task
- RTOS APIs e configurazione

**Analogia:** "Ristorante con chef (scheduler) che assegna cuochi (task) a piatti diversi in base all'urgenza (priorità)"

---

## 💡 Esercizi Proposti

### Livello Base

1. **Modifica Intervalli:** Cambia gli intervalli di lampeggio
2. **Aggiungi LED:** Aggiungi un 4° LED
3. **Pattern:** Crea pattern tipo "Knight Rider"

### Livello Intermedio

4. **Pulsante:** Aggiungi pulsante per start/stop lampeggio (Esempio 2)
5. **Potenziometro:** Controlla velocità LED con potenziometro (Esempio 2)
6. **Fade:** Implementa fade in/out con PWM (Esempio 2)

### Livello Avanzato

7. **Stato Finito:** Implementa FSM (Finite State Machine) per pattern complessi
8. **Sleep Mode:** Aggiungi sleep mode per risparmio energetico (Esempio 3)
9. **Multiple Timer:** Usa Timer1 e Timer2 per LED indipendenti (Esempio 3)
10. **Benchmark:** Misura precision timing e CPU usage dei quattro approcci

### Livello Expert (RTOS)

11. **Semafori:** Aggiungi pulsante che stoppa tutti i LED (binary semaphore)
12. **Mutex:** Proteggi accesso a risorsa condivisa (es. display LCD)
13. **Code FIFO:** Task producer/consumer con queue
14. **Timer Software:** Crea timer software con callback
15. **Priorità Dinamica:** Cambia priorità task a runtime
16. **Stack Analysis:** Monitora stack usage e ottimizza STACK_SIZE
17. **Idle Hook:** Implementa sleep mode in IDLE task per low-power
18. **Watchdog:** Implementa watchdog per rilevare task bloccati

---

## 🐛 Troubleshooting

### LED Non Lampeggiano

- ✅ Verifica collegamenti (pin corretti, polarità LED)
- ✅ Controlla resistenze (220Ω in serie)
- ✅ Test LED con corrente diretta (3.3V → R → LED → GND)

### Esempio 3 Non Compila

```
Error: 'Timer1' was not declared
```
**Soluzione:** Installa libreria TimerOne

```
Tools → Manage Libraries → cerca "TimerOne" → Install
```

### Esempio 4 Non Compila

**Errore su Arduino Uno/Mega:**
```
Error: FreeRTOS.h: No such file
```
**Soluzione:** Installa libreria FreeRTOS
```
Tools → Manage Libraries → cerca "FreeRTOS" → Install "FreeRTOS" by Richard Barry
```

**Errore su ESP32:**
```
Error: Arduino_FreeRTOS.h: No such file
```
**Soluzione:** Su ESP32 devi usare `#include <freertos/FreeRTOS.h>` (nota: path diverso!)  
Modifica nel codice la sezione `PLATFORM_ESP32` o decommentala.

### Esempio 4 Crash/Reset

**Sintomo:** Arduino si resetta continuamente dopo upload

**Cause possibili:**
1. **RAM insufficiente** (Arduino Uno ha solo 2KB!)
   - Riduci STACK_SIZE (es. da 128 a 64)
   - Rimuovi task Stats (usa molta RAM per Serial.print)
   - Usa solo 2 LED invece di 3

2. **Stack overflow**
   - Abilita `configCHECK_FOR_STACK_OVERFLOW` in FreeRTOSConfig.h
   - Aumenta STACK_SIZE per task che crashano

3. **Configurazione errata**
   - Verifica FreeRTOSConfig.h sia corretto per tua board
   - Per AVR: `configTOTAL_HEAP_SIZE` < 1500 bytes

**Soluzione consigliata:** Usa ESP32 invece di Arduino Uno per RTOS!

### Serial Monitor Non Mostra Output

- ✅ Verifica baud rate: **9600** (Esempi 1-2) o **115200** (Esempio 3)
- ✅ Chiudi altri programmi che usano porta seriale
- ✅ Riconnetti USB

### LED Lampeggiano in Modo Strano

**Esempio 1:** Normale, è l'effetto del blocking code  
**Esempio 2:** Verifica overflow millis() (raro, dopo 49 giorni)  
**Esempio 3:** Controlla ISR non sia troppo lenta

---

## 📚 Approfondimenti

### Documentazione Arduino

- [millis() Reference](https://www.arduino.cc/reference/en/language/functions/time/millis/)
- [Blink Without Delay Tutorial](https://www.arduino.cc/en/Tutorial/BuiltInExamples/BlinkWithoutDelay)
- [attachInterrupt() Reference](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Timer Hardware

- [ATmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) (Timer registers)
- [TimerOne Library GitHub](https://github.com/PaulStoffregen/TimerOne)

### Concetti Avanzati

- **FreeRTOS:** [FreeRTOS.org](https://www.freertos.org/) - Documentazione ufficiale
- **FreeRTOS Book:** ["Mastering the FreeRTOS Real Time Kernel"](https://www.freertos.org/Documentation/RTOS_book.html) (gratuito!)
- **ESP-IDF Guide:** [ESP32 FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
- **Protothreads:** Lightweight threading library (alternativa a RTOS)
- **State Machines:** Pattern per gestione stati complessi

---

## 🔗 Collegamenti con il Corso

Questi esempi dimostrano concetti fondamentali del **Corso Programmazione Concorrente**:

- **Modulo 1:** Processi sequenziali vs concorrenti (Esempio 1 vs 2-3-4)
- **Modulo 2:** Multi-processing (Esempio 2 = multi-tasking cooperativo)
- **Modulo 3:** Thread e sincronizzazione (Esempio 4 = RTOS con task/semafori)
- **Interrupt:** Programmazione event-driven (Esempio 3 = ISR)

**Analogie con Sistemi Operativi:**

| Arduino                  | Sistema Operativo           | Modulo Corso |
|--------------------------|-----------------------------|--------------|
| `delay()`                | Processo sequenziale        | Modulo 1     |
| `millis()` polling       | Multi-tasking cooperativo   | Modulo 2     |
| Timer interrupt          | Interrupt handler (IRQ)     | Modulo 3     |
| FreeRTOS task            | Thread (pthread)            | Modulo 3     |
| `vTaskDelay()`           | `sleep()` / `pthread_yield()`| Modulo 3    |
| Scheduler RTOS           | Scheduler Linux/Windows     | Modulo 1     |
| Semafori FreeRTOS        | `sem_wait()` / `sem_post()` | Modulo 3     |
| Mutex FreeRTOS           | `pthread_mutex_lock()`      | Modulo 3     |
| Queue FreeRTOS           | Pipe / Message Queue        | Modulo 2     |

**Progressione Didattica:**

```
Esempio 1 (Sequenziale)
    ↓
Esempio 2 (Polling)        ← 80% progetti hobbistici
    ↓
Esempio 3 (Interrupt)      ← Sistemi real-time hard
    ↓
Esempio 4 (RTOS)           ← Sistemi embedded enterprise
```

---

## ⚡ Performance

### Misurazione Overhead

Aggiungi questo codice nel loop():

```cpp
// Misura tempo di esecuzione loop()
unsigned long loopStart = micros();
// ... codice loop ...
unsigned long loopTime = micros() - loopStart;

if (loopTime > 1000) {  // > 1ms
  Serial.print("Loop lento: ");
  Serial.print(loopTime);
  Serial.println(" us");
}
```

**Risultati Tipici (Arduino Uno 16MHz):**

- Esempio 1: N/A (bloccato in delay)
- Esempio 2: ~50-200 μs per iterazione loop
- Esempio 3: ~10-50 μs per iterazione loop (ISR: ~5-15 μs)
- Esempio 4: ~100-500 μs per context switch (dipende da stack size)

**Risultati Tipici (ESP32 240MHz):**

- Esempio 2: ~5-20 μs per iterazione loop
- Esempio 3: ~3-10 μs per iterazione loop (ISR: ~1-3 μs)
- Esempio 4: ~10-50 μs per context switch (scheduler molto efficiente)

---

## 🎯 Quale Esempio Usare?

### Decision Tree

```
Progetto semplice (<3 task)?
├─ SI → Esempio 1 o 2
│   └─ Serve reattività? → Esempio 2
│       └─ NO → Esempio 1 (ok per demo)
│
└─ NO → Progetto complesso (≥3 task)
    │
    ├─ Timing critico (<1ms)?
    │   └─ SI → Esempio 3 (Interrupt)
    │
    ├─ Molti task (>5)?
    │   └─ SI → Esempio 4 (RTOS)
    │
    ├─ Serve sincronizzazione complessa?
    │   └─ SI → Esempio 4 (RTOS)
    │
    ├─ Codice deve essere modulare/portabile?
    │   └─ SI → Esempio 4 (RTOS)
    │
    ├─ RAM limitata (<4KB)?
    │   └─ SI → Esempio 2 o 3 (NO RTOS!)
    │
    └─ Default → Esempio 2 (polling con millis)
```

### Raccomandazioni per Piattaforma

**Arduino Uno/Nano (ATmega328P - 2KB RAM):**
- ✅ Esempio 1, 2, 3
- ⚠️ Esempio 4: Solo per apprendimento, limitazioni severe

**Arduino Mega (ATmega2560 - 8KB RAM):**
- ✅ Esempio 1, 2, 3
- ⚠️ Esempio 4: Funziona ma con max 3-4 task

**ESP32 (520KB RAM):**
- ✅ Tutti gli esempi
- ⭐ **IDEALE per Esempio 4** (RTOS nativo, performance eccellenti)

**ESP8266 (80KB RAM):**
- ✅ Tutti gli esempi
- ✅ Esempio 4: Buone performance (RTOS nativo)

---

## 📈 Evoluzione Paradigmi

Questi 4 esempi rappresentano l'**evoluzione storica** della programmazione embedded:

1. **Anni '70-'80:** Codice sequenziale con delay (Esempio 1)
2. **Anni '90:** Super-loop con polling (Esempio 2)
3. **Anni 2000:** Interrupt-driven (Esempio 3)
4. **Anni 2010+:** RTOS embedded (Esempio 4)

Oggi, l'**industria embedded** usa prevalentemente:
- **Esempio 2** (polling): Progetti semplici/consumer
- **Esempio 3** (interrupt): Automotive, controllo industriale
- **Esempio 4** (RTOS): IoT, robotica, aerospace, medicale

---

**Autore:** Corso Programmazione Concorrente  
**Licenza:** MIT  
**Versione:** 1.0
