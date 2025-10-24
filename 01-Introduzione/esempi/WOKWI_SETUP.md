# Guida Simulazione Wokwi - Esempi LED

Questa guida spiega come simulare gli esempi Arduino su **Wokwi**, il simulatore online gratuito.

## 🌐 Cos'è Wokwi?

[Wokwi](https://wokwi.com) è un simulatore di circuiti Arduino/ESP32 che funziona nel browser. Permette di:
- ✅ Testare codice senza hardware fisico
- ✅ Vedere LED lampeggiare in tempo reale
- ✅ Leggere output seriale
- ✅ Condividere progetti con URL
- ✅ Supporta ESP32 (ideale per FreeRTOS!)

## 🚀 Quick Start

### Metodo 1: Usa File Forniti (CONSIGLIATO)

1. **Vai su Wokwi:**
   - Apri [https://wokwi.com](https://wokwi.com)
   - Click su **"New Project"**
   - Seleziona **"ESP32"**

2. **Carica il codice:**
   - Copia il contenuto di uno degli esempi (es. `04_led_rtos.ino`)
   - Incolla nel tab "sketch.ino" su Wokwi
   - **IMPORTANTE per Esempio 4:** Assicurati che nel codice sia decommentato `#define PLATFORM_ESP32`

3. **Configura circuito:**
   - Click su **"diagram.json"** tab
   - Cancella contenuto esistente
   - Copia e incolla il contenuto di `wokwi-diagram.json` fornito
   - Salva

4. **Simula:**
   - Click sul pulsante **verde "Start Simulation"**
   - Osserva i LED lampeggiare!
   - Click su **"Serial Monitor"** per vedere output

### Metodo 2: Costruisci Manualmente

Se preferisci costruire il circuito da zero:

1. **Crea nuovo progetto ESP32** su Wokwi
2. **Aggiungi componenti:**
   - 3x LED (rosso, verde, blu)
   - 3x Resistenze 220Ω
3. **Collega:**
   - ESP32 D8 → R1 → LED1 Anodo (rosso)
   - ESP32 D9 → R2 → LED2 Anodo (verde)
   - ESP32 D10 → R3 → LED3 Anodo (blu)
   - LED1/2/3 Catodo → GND
4. **Carica sketch** e avvia simulazione

## 📁 File Forniti

### `wokwi-diagram.json`
Schema circuito per ESP32 con 3 LED e resistenze.

**Componenti:**
- ESP32 DevKit v1
- 3 LED (rosso, verde, blu)
- 3 Resistenze 220Ω

**Collegamenti:**
```
ESP32 D8  ──[220Ω]──►|── GND  (LED Rosso)
ESP32 D9  ──[220Ω]──►|── GND  (LED Verde)
ESP32 D10 ──[220Ω]──►|── GND  (LED Blu)
```

### `wokwi.toml` (opzionale)
File di configurazione per Wokwi CLI (per uso avanzato).

## 🔧 Configurazione Esempi per Wokwi

### Esempio 1: `01_led_sequenziale.ino`
✅ **Funziona direttamente** su Wokwi (Arduino Uno o ESP32)

**Note:**
- Nessuna modifica necessaria
- Puoi usare Arduino Uno su Wokwi

### Esempio 2: `02_led_polling.ino`
✅ **Funziona direttamente** su Wokwi (Arduino Uno o ESP32)

**Note:**
- Nessuna modifica necessaria
- Puoi usare Arduino Uno su Wokwi

### Esempio 3: `03_led_interrupt.ino`
⚠️ **Richiede libreria TimerOne**

**Setup su Wokwi:**
1. Usa **Arduino Uno** come board
2. La libreria TimerOne è supportata automaticamente
3. Funziona senza modifiche!

### Esempio 4: `04_led_rtos.ino`
✅ **CONSIGLIATO usare ESP32** su Wokwi

**Setup su Wokwi:**
1. **IMPORTANTE:** Decommentare questa riga nel codice:
   ```cpp
   #define PLATFORM_ESP32   // <-- Deve essere decommentata!
   ```
   
2. Commentare le altre piattaforme:
   ```cpp
   // #define PLATFORM_AVR     // Commentata
   #define PLATFORM_ESP32      // Decommentata
   // #define PLATFORM_ESP8266 // Commentata
   ```

3. ESP32 ha FreeRTOS nativo, funziona perfettamente!

**Perché ESP32?**
- FreeRTOS è già integrato
- 520KB RAM (Arduino Uno ha solo 2KB!)
- Performance eccellenti
- Wokwi simula ESP32 perfettamente

## 🎮 Come Usare la Simulazione

### Avviare Simulazione

1. Click sul pulsante **verde "▶ Start Simulation"**
2. I LED inizieranno a lampeggiare!
3. Osserva i pattern di lampeggio

### Serial Monitor

1. Click su **"Serial Monitor"** in basso
2. Vedrai output debug:
   ```
   === Esempio 4: LED con FreeRTOS ===
   [OK] Task LED1 creato (Priorità 1)
   [OK] Task LED2 creato (Priorità 2)
   [OK] Task LED3 creato (Priorità 3)
   ...
   ```

### Modificare Codice Durante Simulazione

1. **Stop** simulazione (pulsante rosso)
2. Modifica codice
3. **Restart** simulazione
4. Le modifiche sono applicate!

### Condividere Progetto

1. Click su **"Share"** in alto a destra
2. Copia URL generato
3. Chiunque può aprire e simulare il tuo progetto!

## 📊 Cosa Osservare

### Esempio 1: Sequenziale
- LED lampeggiano UNO ALLA VOLTA
- Sequenza rigida: LED1 → LED2 → LED3 → [ripete]
- CPU bloccata durante delay

### Esempio 2: Polling
- LED lampeggiano CONTEMPORANEAMENTE!
- Intervalli indipendenti (1000ms, 500ms, 200ms)
- CPU può fare altro (vedi contatore loop)

### Esempio 3: Interrupt
- LED lampeggiano con PRECISIONE MASSIMA
- Timer hardware controlla timing
- CPU libera tra interrupt

### Esempio 4: RTOS
- LED lampeggiano CONTEMPORANEAMENTE con PRIORITÀ
- Task alta priorità (LED3) esegue per primo
- Statistiche mostrano scheduler in azione
- Ogni 10s vedrai:
  ```
  ========== STATISTICHE (10s) ==========
  LED1 toggle: 20
  LED2 toggle: 40
  LED3 toggle: 100
  Task attivi: 4
  Free Heap: 234560 bytes
  ```

## 🐛 Troubleshooting Wokwi

### LED Non Lampeggiano

**Problema:** Simulazione avviata ma LED spenti

**Soluzioni:**
1. Verifica collegamenti in `diagram.json`
2. Controlla pin numbers nel codice (D8, D9, D10)
3. Prova **"Reset Simulation"**

### Errore Compilazione

```
Error: 'FreeRTOS.h' file not found
```

**Soluzione:** 
- Su ESP32, NON serve `Arduino_FreeRTOS.h`
- Verifica che `#define PLATFORM_ESP32` sia decommentato
- ESP32 usa `<freertos/FreeRTOS.h>` (già incluso)

### Serial Monitor Vuoto

**Problema:** Serial Monitor non mostra niente

**Soluzioni:**
1. Click su **"Serial Monitor"** tab in basso
2. Verifica baud rate: dovrebbe essere automatico
3. Restart simulation
4. Controlla che codice abbia `Serial.begin(115200)`

### Simulazione Lenta/Crash

**Problema:** Wokwi si blocca o va lento

**Soluzioni:**
1. Riduci `Serial.print()` (rallenta simulazione)
2. Chiudi altre tab browser
3. Reload pagina (F5)
4. Per Esempio 4: Riduci frequenza statistiche (da 10s a 30s)

### ESP32 vs Arduino Uno

**Quando usare quale board?**

| Esempio | Board Consigliata | Motivo |
|---------|-------------------|--------|
| 1, 2    | Arduino Uno o ESP32 | Funzionano su entrambe |
| 3       | Arduino Uno | TimerOne è per AVR |
| 4       | ⭐ **ESP32** | RTOS nativo, molta RAM |

## 🎓 Esercizi su Wokwi

### Livello Base

1. **Modifica Intervalli:**
   - Cambia intervalli lampeggio (es. LED1: 2000ms)
   - Osserva differenza nella simulazione

2. **Cambia Colori LED:**
   - In `diagram.json`, cambia `"color": "red"` in `"yellow"` o `"white"`

3. **Aggiungi 4° LED:**
   - Aggiungi LED e resistenza in diagram.json
   - Modifica codice per gestirlo

### Livello Intermedio

4. **Pattern Knight Rider:**
   - Modifica Esempio 2 per creare effetto "vai e vieni"

5. **Frequenza Variabile:**
   - Aggiungi potenziometro virtuale su Wokwi
   - Usa `analogRead()` per variare velocità

6. **Stop/Start con Pulsante:**
   - Aggiungi button virtuale
   - Ferma/avvia lampeggio

### Livello Avanzato

7. **RTOS: Task Sospensione:**
   - Aggiungi pulsante che sospende LED2 (vTaskSuspend)
   - Test dinamico task control

8. **Benchmark Performance:**
   - Misura `micros()` in loop
   - Confronta overhead Polling vs RTOS

9. **Comunicazione Task:**
   - Usa Queue FreeRTOS per far comunicare task LED

## 🔗 Link Utili

- **Wokwi Homepage:** https://wokwi.com
- **Wokwi Docs:** https://docs.wokwi.com
- **Wokwi ESP32 Reference:** https://docs.wokwi.com/parts/wokwi-esp32-devkit-v1
- **Progetti Esempio:** https://wokwi.com/projects (cerca "FreeRTOS")

## 💡 Tips & Tricks

### Velocizzare Simulazione

Wokwi può sembrare lento per delay lunghi. **Soluzione:**
```cpp
// Invece di:
vTaskDelay(pdMS_TO_TICKS(1000));  // 1 secondo reale

// Per debug veloce:
vTaskDelay(pdMS_TO_TICKS(100));   // 100ms reale
```

### Debugging Visivo

Aggiungi `Serial.print()` per vedere cosa succede:
```cpp
void taskLed1(void *pvParameters) {
  for(;;) {
    Serial.println("[LED1] Toggle!");
    digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
    vTaskDelay(LED1_INTERVAL);
  }
}
```

### Salva Progetto

Wokwi salva automaticamente, ma per sicurezza:
1. **Sign In** con Google/GitHub
2. **Save Project** (nome progetto)
3. I tuoi progetti: https://wokwi.com/dashboard

## 📱 Wokwi Mobile

Wokwi funziona anche su **tablet/smartphone**!
- Usa browser mobile
- Funzionalità complete
- Ottimo per dimostrazioni in classe

## 🎬 Demo Video

Per vedere Wokwi in azione:
1. Vai su https://wokwi.com
2. Click su "Examples"
3. Cerca "ESP32 FreeRTOS"
4. Play simulation!

---

**Buona simulazione!** 🚀

Se hai problemi, verifica:
1. ✅ Browser moderno (Chrome/Firefox/Edge)
2. ✅ JavaScript abilitato
3. ✅ Connessione internet stabile
4. ✅ `#define PLATFORM_ESP32` decommentato per Esempio 4

**Domande?** Consulta [Wokwi Docs](https://docs.wokwi.com) o chiedi aiuto!
