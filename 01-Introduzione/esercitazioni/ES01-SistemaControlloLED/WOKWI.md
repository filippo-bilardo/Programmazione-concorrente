# Guida Simulazione Wokwi - ES01

## 🌐 Testare l'Esercitazione su Wokwi

Wokwi permette di testare tutto il codice **senza hardware fisico**!

### Quick Start

1. **Vai su** https://wokwi.com
2. **Nuovo progetto** → Arduino Uno
3. **Carica diagram.json:**
   - Click su "diagram.json" tab
   - Copia contenuto da `test/diagram.json`
   - Salva

4. **Carica sketch:**
   - Scegli uno dei 4 approcci
   - Copia codice nel tab "sketch.ino"
   - Per Approccio 4: usa ESP32 invece di Uno!

5. **Simula:**
   - Click ▶ **Start Simulation**
   - Premi pulsanti virtuali BTN1 e BTN2
   - Osserva LED lampeggiare!

---

## 📦 Circuito Wokwi

Il file `diagram.json` configura:

```
Arduino Uno
├── Pin 2  → BTN1 (rosso) con pull-down 10kΩ
├── Pin 3  → BTN2 (verde) con pull-down 10kΩ
├── Pin 8  → LED1 (rosso) con R 220Ω
└── Pin 9  → LED2 (verde) con R 220Ω
```

---

## 🧪 Test per Ogni Approccio

### Approccio 1: Sequenziale

**Test:**
1. Premi BTN1 → LED1 lampeggia
2. **MENTRE BTN1 premuto**, premi BTN2
3. **Risultato atteso:** BTN2 NON risponde (CPU bloccata!)

**Osservazione chiave:**
```
[0ms] Premo BTN1 → LED1 lampeggia
[50ms] Premo BTN2 → NESSUNA risposta
[200ms] Rilascio BTN1 → LED2 finalmente risponde
```

---

### Approccio 2: Polling

**Test:**
1. Premi BTN1 e BTN2 contemporaneamente
2. **Risultato atteso:** Entrambi LED lampeggiano INSIEME!

**Serial Monitor output:**
```
[1000ms] LED1 ON
[1000ms] LED2 ON
[1100ms] LED1 OFF
[1100ms] LED2 OFF
```

**Test debouncing:**
- Premi/rilascia pulsante rapidamente
- LED dovrebbe rispondere in modo stabile (no flickering)

---

### Approccio 3: Interrupt

**Test precisione timing:**
1. Premi BTN1
2. Osserva Serial Monitor:
   ```
   [0ms] BTN1 premuto
   [100ms] LED1 toggle
   [200ms] LED1 toggle
   [300ms] LED1 toggle
   ```
3. Intervalli dovrebbero essere ESATTAMENTE 100ms (±1ms)

**Test interrupt:**
- Nel `loop()`, aggiungi `delay(5000);`
- LED continuano lampeggiare normalmente (interrupt!)

---

### Approccio 4: RTOS

**IMPORTANTE:** Usa **ESP32** su Wokwi, non Arduino Uno!

**Setup ESP32:**
1. Nuovo progetto ESP32
2. Modifica pin:
   ```cpp
   const int BTN1_PIN = 15;  // ESP32
   const int BTN2_PIN = 4;
   const int LED1_PIN = 2;
   const int LED2_PIN = 16;
   ```
3. Include corretto:
   ```cpp
   #include <freertos/FreeRTOS.h>
   #include <freertos/task.h>
   ```

**Test task indipendenti:**
- Serial Monitor mostra statistiche ogni 5s
- Verifica stack usage: dovrebbe rimanere costante

---

## 🐛 Troubleshooting Wokwi

### LED non lampeggiano

**Soluzione:**
- Verifica collegamenti in diagram.json
- Reset simulation (Ctrl+R)
- Controlla Serial Monitor per errori

### Approccio 3: TimerOne non funziona

**Problema:** Wokwi non simula TimerOne library

**Soluzione:** Usa approccio alternativo con `millis()`:
```cpp
// Invece di Timer1.initialize()
unsigned long lastToggle = 0;

void loop() {
  if (millis() - lastToggle >= 100) {
    lastToggle = millis();
    timerISR();  // Chiama ISR manualmente
  }
}
```

### Approccio 4: Errore compilazione

**Errore:** `Arduino_FreeRTOS.h not found`

**Soluzione:** 
1. Wokwi Arduino Uno non supporta FreeRTOS nativamente
2. Passa a **ESP32** (FreeRTOS integrato)
3. Oppure usa approccio simulato con `millis()`

---

## 📊 Confronto Risultati

Compila questa tabella testando su Wokwi:

| Approccio   | Risposta BTN (ms) | Precisione (±ms) | CPU Libera | Note |
|-------------|-------------------|------------------|------------|------|
| Sequenziale | _____             | _____            | NO         | ____ |
| Polling     | _____             | _____            | SI         | ____ |
| Interrupt   | _____             | _____            | SI         | ____ |
| RTOS        | _____             | _____            | SI         | ____ |

---

## 🎯 Esercizi Wokwi

### 1. Misura Latenza

Aggiungi timestamp quando premi pulsante e quando LED cambia:

```cpp
if (btn1_pressed) {
  Serial.print("[");
  Serial.print(millis());
  Serial.println("] BTN1 premuto");
}

// Quando LED cambia
Serial.print("[");
Serial.print(millis());
Serial.println("] LED1 toggle");
```

**Calcola:** Latenza = tempo_led - tempo_btn

### 2. Pattern Personalizzati

Modifica per creare pattern SOS morse (· · · − − − · · ·):
- BTN1: SOS continuo
- BTN2: Lampeggio normale

### 3. Sincronizzazione

(Solo Approccio 4 - RTOS)

Quando BTN1 e BTN2 premuti insieme, LED lampeggiano sincronizzati.

---

## 💾 Salva Progetto Wokwi

1. **Sign in** con Google/GitHub
2. **Save** progetto
3. **Share** → Copia URL
4. Incolla URL nella relazione!

**Esempio URL:**
```
https://wokwi.com/projects/12345678901234567890
```

---

## 📱 Wokwi Mobile

Funziona anche su **tablet/smartphone**!
- Ideale per demo in classe
- Touch per premere pulsanti virtuali

---

**Buona simulazione! 🚀**

Se hai problemi, verifica:
- ✅ Browser aggiornato (Chrome/Firefox)
- ✅ JavaScript abilitato
- ✅ Diagram.json caricato correttamente
