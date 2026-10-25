# 📁 Struttura Esercitazione ES01

## Panoramica

Esercitazione guidata per apprendere 4 approcci alla programmazione Arduino:
1. **Sequenziale** (delay) - Codice bloccante
2. **Polling** (millis) - Codice non-bloccante
3. **Interrupt** - Gestione asincrona eventi
4. **RTOS** - Real-Time Operating System

---

## 📂 Struttura Directory

```
ES01-SistemaControlloLED/
├── README.md                    # Questo file
├── ESERCITAZIONE.md             # Guida completa studente
├── WOKWI.md                     # Guida simulazione online
│
├── template/                    # File per studenti (con TODO)
│   ├── template_approccio1.ino  # Sequenziale
│   ├── template_approccio2.ino  # Polling
│   ├── template_approccio3.ino  # Interrupt
│   └── template_approccio4.ino  # RTOS
│
├── soluzione/                   # Codice completo (docente)
│   ├── approccio1_sequenziale.ino
│   ├── approccio2_polling.ino
│   ├── approccio3_interrupt.ino
│   └── approccio4_rtos.ino
│
└── test/                        # Verifica hardware
    ├── test_hardware.ino        # Test collegamenti
    └── diagram.json             # Circuito Wokwi
```

---

## 🎯 Per gli Studenti

### Inizio Esercitazione

1. **Leggi** `ESERCITAZIONE.md` - obiettivi e specifiche
2. **Testa hardware** con `test/test_hardware.ino`
3. **Inizia da** `template/template_approccio1.ino`
4. **Completa TODO** seguendo commenti
5. **Verifica** con domande di verifica
6. **Procedi** agli approcci successivi

### Ordine Consigliato

```
Approccio 1 (1-2 ore)
    ↓
Approccio 2 (2-3 ore)
    ↓
Approccio 3 (1.5-2 ore)
    ↓
Approccio 4 (2-3 ore)
```

### Consegna

Crea file ZIP con:
```
CognomeNome_ES01/
├── approccio1.ino
├── approccio2.ino
├── approccio3.ino
├── approccio4.ino
└── relazione.pdf
```

---

## 👨‍🏫 Per i Docenti

### File Template

Ogni template contiene:
- ✅ Setup hardware completo
- ✅ Struttura codice base
- ✅ TODO markers con suggerimenti
- ✅ Domande di verifica
- ✅ Test procedures
- ✅ Esercizi bonus

### Soluzioni

File in `soluzione/` sono implementazioni **complete e commentate**.

Contengono:
- Codice funzionante
- Risposte domande verifica
- Best practices
- Note implementative

### Valutazione

Rubrica in `ESERCITAZIONE.md`:
- Approccio 1: 18-21/30
- Approccio 2: 22-25/30
- Approccio 3: 26-28/30
- Approccio 4: 29-30L/30

---

## 🔧 Hardware Richiesto

### Componenti

| Componente | Quantità | Note |
|------------|----------|------|
| Arduino Uno/Mega | 1 | O compatibile |
| LED Rosso | 1 | 5mm standard |
| LED Verde | 1 | 5mm standard |
| Pulsante | 2 | Push button |
| Resistenza 220Ω | 2 | Per LED |
| Resistenza 10kΩ | 2 | Pull-down |
| Breadboard | 1 | 400 punti |
| Cavi jumper | ~10 | Maschio-maschio |

### Schema Collegamenti

```
Arduino Uno
├── Pin 2  ─→ BTN1 (con pull-down 10kΩ a GND)
├── Pin 3  ─→ BTN2 (con pull-down 10kΩ a GND)
├── Pin 8  ─→ LED1 rosso (+ resistenza 220Ω)
├── Pin 9  ─→ LED2 verde (+ resistenza 220Ω)
├── 5V     ─→ Alimentazione pulsanti
└── GND    ─→ Massa comune
```

---

## 💻 Software Richiesto

### IDE Arduino

- **Download:** https://www.arduino.cc/en/software
- **Versione:** 2.x (consigliata)

### Librerie

**Approccio 3 (Interrupt):**
```
TimerOne by Paul Stoffregen
```

**Approccio 4 (RTOS):**

Arduino Uno/Mega:
```
Arduino_FreeRTOS by Phillip Stevens
```

ESP32:
```
FreeRTOS già integrato!
```

### Installazione Librerie

1. Apri Arduino IDE
2. Menu → **Tools** → **Manage Libraries**
3. Cerca nome libreria
4. Click **Install**

---

## 🌐 Simulazione Online

### Wokwi Simulator

**NO hardware fisico? Usa Wokwi!**

1. Vai su https://wokwi.com
2. Nuovo progetto **Arduino Uno**
3. Carica `test/diagram.json`
4. Copia codice template
5. Simula online!

**Vantaggi:**
- ✅ Gratis
- ✅ No hardware richiesto
- ✅ Debug integrato
- ✅ Funziona su mobile

**Guida completa:** `WOKWI.md`

---

## 📚 Competenze Sviluppate

### Approccio 1: Sequenziale
- Programmazione imperativa
- Gestione I/O digitale
- Problemi codice bloccante

### Approccio 2: Polling
- Macchine a stati
- Timing non-bloccante
- Debouncing software
- Overflow handling

### Approccio 3: Interrupt
- Interrupt esterni
- Timer hardware
- ISR (Interrupt Service Routine)
- Variabili volatile
- Sezioni critiche

### Approccio 4: RTOS
- Task scheduling
- Priorità task
- Context switching
- Risorse condivise
- Sincronizzazione

---

## 🎓 Obiettivi Didattici

### Conoscenze
- Comprendere differenze architetturali tra approcci
- Conoscere concetti RTOS base
- Capire gestione interrupt

### Abilità
- Implementare debouncing
- Usare timer hardware
- Creare task FreeRTOS
- Debug codice concorrente

### Competenze
- Scegliere approccio adeguato al problema
- Progettare sistemi embedded reattivi
- Ottimizzare uso risorse (CPU, RAM)

---

## 🚀 Estensioni Possibili

### Livello Base
- Aggiungere 3° LED e pulsante
- Pattern lampeggio personalizzati
- Contatore premute su display

### Livello Intermedio
- Comunicazione UART tra Arduino
- LCD per visualizzare stato
- Salvataggio contatori in EEPROM

### Livello Avanzato
- Rete di Arduino con protocollo custom
- Interfaccia web (ESP32)
- Machine learning per pattern riconoscimento

---

## 🐛 Troubleshooting

### Problemi Comuni

**LED non lampeggiano**
- Verifica polarità LED (gamba lunga = +)
- Controlla pin corretti (8 e 9)
- Misura tensione con multimetro

**Pulsanti non funzionano**
- Verifica resistenze pull-down
- Controlla collegamenti
- Prova INPUT_PULLUP (pulsante a GND)

**Valori instabili (bounce)**
- Aggiungi condensatore 0.1µF
- Aumenta DEBOUNCE_DELAY
- Usa pulsanti di qualità migliore

**Approccio 3: TimerOne non compila**
- Installa libreria TimerOne
- Su ESP32, usa timer nativo

**Approccio 4: Out of memory**
- Riduci STACK_SIZE
- Limita numero task (Arduino Uno)
- Passa a ESP32 (520KB RAM)

---

## 📖 Riferimenti

### Documentazione Ufficiale
- Arduino Reference: https://www.arduino.cc/reference/en/
- TimerOne Library: https://playground.arduino.cc/Code/Timer1/
- FreeRTOS: https://www.freertos.org/

### Tutorial
- Debouncing: https://www.arduino.cc/en/Tutorial/Debounce
- Interrupt: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
- RTOS: https://www.freertos.org/Documentation/RTOS_book.html

### Video
- Interrupt tutorial: https://www.youtube.com/watch?v=J61_PKyWjxU
- FreeRTOS basics: https://www.youtube.com/watch?v=qsflCf6ahXU

---

## 📝 Licenza

Materiale didattico open source.
Libero utilizzo per scopi educativi.

---

## 📬 Contatti

**Domande?** 
- Apri issue su GitHub
- Email docente
- Forum corso

---

**Buon lavoro! 🚀**

*Ultima revisione: Novembre 2024*
