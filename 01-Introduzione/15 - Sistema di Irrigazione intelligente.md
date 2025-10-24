Ecco un esempio pratico in cui è necessario eseguire più task contemporaneamente su Arduino, usando tecniche di **multi-tasking non bloccante** con `millis()`. Questo esempio coinvolge la gestione di una serie di compiti che richiedono tempi diversi e devono essere eseguiti indipendentemente l'uno dall'altro.

### Scenario: Sistema di Irrigazione intelligente

Immagina di voler costruire un sistema di irrigazione intelligente per un giardino. Il sistema deve:
1. Controllare periodicamente il livello di umidità del terreno.
2. Accendere/spegnere un pompa d'acqua se l'umidità è troppo bassa.
3. Visualizzare lo stato attuale (umidità e pompaggio) su uno schermo LCD ogni 5 secondi.
4. Emettere un segnale sonoro (buzzer) quando l'umidità scende sotto una soglia critica.
5. Gestire un pulsante per avviare/fermare manualmente la pompa.

---

### Codice Completo

```cpp
#include <LiquidCrystal.h> // Libreria per lo schermo LCD

// Pin configurazione
const int SOIL_MOISTURE_PIN = A0; // Sensore di umidità del terreno
const int PUMP_PIN = 8;           // Pompa d'acqua
const int BUZZER_PIN = 9;         // Buzzer
const int BUTTON_PIN = 2;         // Pulsante manuale
const int LCD_RS = 12, LCD_EN = 11, LCD_D4 = 5, LCD_D5 = 4, LCD_D6 = 3, LCD_D7 = 2;

// Variabili per millis()
unsigned long soilPreviousMillis = 0; // Per il sensore di umidità
unsigned long lcdPreviousMillis = 0;  // Per lo schermo LCD
unsigned long buzzerPreviousMillis = 0; // Per il buzzer

// Intervalli di tempo (in millisecondi)
const long SOIL_INTERVAL = 1000;   // Controllo umidità ogni 1 secondo
const long LCD_INTERVAL = 5000;    // Aggiornamento LCD ogni 5 secondi
const long BUZZER_INTERVAL = 2000; // Segnale sonoro ogni 2 secondi

// Stato della pompa
bool pumpOn = false;
bool manualMode = false; // Modalità manuale attivata dal pulsante

// Inizializzazione dello schermo LCD
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  Serial.begin(9600);
  
  // Configurazione dei pin
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Configurazione dello schermo LCD
  lcd.begin(16, 2); // Schermo 16x2
  lcd.print("Irrigazione ON");
}

void loop() {
  unsigned long currentMillis = millis();

  // Task 1: Leggi il sensore di umidità ogni 1 secondo
  if (currentMillis - soilPreviousMillis >= SOIL_INTERVAL) {
    soilPreviousMillis = currentMillis;
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN); // Legge il valore analogico
    float moistureLevel = mapMoisture(soilMoisture); // Mappa il valore in percentuale

    // Controlla l'umidità e accendi/spegni la pompa
    if (!manualMode) { // Solo in modalità automatica
      if (moistureLevel < 30) {
        digitalWrite(PUMP_PIN, HIGH); // Accende la pompa
        pumpOn = true;
      } else {
        digitalWrite(PUMP_PIN, LOW); // Spegne la pompa
        pumpOn = false;
      }
    }

    // Debug
    Serial.print("Umidità: ");
    Serial.println(moistureLevel);
  }

  // Task 2: Aggiorna lo schermo LCD ogni 5 secondi
  if (currentMillis - lcdPreviousMillis >= LCD_INTERVAL) {
    lcdPreviousMillis = currentMillis;

    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    float moistureLevel = mapMoisture(soilMoisture);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Umidità: ");
    lcd.print(moistureLevel);
    lcd.setCursor(0, 1);
    lcd.print("Pompa: ");
    lcd.print(pumpOn ? "ON" : "OFF");
  }

  // Task 3: Emetti un segnale sonoro se l'umidità è troppo bassa
  if (currentMillis - buzzerPreviousMillis >= BUZZER_INTERVAL) {
    buzzerPreviousMillis = currentMillis;

    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    float moistureLevel = mapMoisture(soilMoisture);

    if (moistureLevel < 20) { // Soglia critica
      tone(BUZZER_PIN, 1000, 500); // Emette un suono per 500 ms
    } else {
      noTone(BUZZER_PIN); // Disattiva il buzzer
    }
  }

  // Task 4: Gestione del pulsante manuale
  if (digitalRead(BUTTON_PIN) == LOW) { // Se il pulsante è premuto
    manualMode = !manualMode; // Cambia modalità manuale
    if (manualMode) {
      digitalWrite(PUMP_PIN, HIGH); // Accende la pompa manualmente
      pumpOn = true;
      lcd.clear();
      lcd.print("Manuale: ON");
    } else {
      digitalWrite(PUMP_PIN, LOW); // Spegne la pompa manualmente
      pumpOn = false;
      lcd.clear();
      lcd.print("Manuale: OFF");
    }
    delay(200); // Anti-bounce
  }
}

// Funzione per mappare il valore analogico in percentuale di umidità
float mapMoisture(int value) {
  return map(value, 0, 1023, 100, 0); // Mappa il valore inverso (0-1023 -> 100-0)
}
```

---

### Spiegazione dei Task

1. **Task 1: Lettura del sensore di umidità**
   - Legge il valore del sensore di umidità ogni 1 secondo.
   - Controlla se l'umidità è inferiore a una soglia (30%) e accende/spegne la pompa di conseguenza.
   - Usa `millis()` per garantire che il controllo venga eseguito periodicamente senza bloccare il ciclo principale.

2. **Task 2: Aggiornamento dello schermo LCD**
   - Aggiorna lo schermo LCD ogni 5 secondi per mostrare il livello di umidità e lo stato della pompa.
   - Usa `lcd.clear()` per pulire lo schermo prima di stampare nuovi dati.

3. **Task 3: Segnale sonoro con buzzer**
   - Emette un segnale sonoro ogni 2 secondi se l'umidità scende sotto una soglia critica (20%).
   - Usa `tone()` per generare il suono e `noTone()` per disattivarlo.

4. **Task 4: Gestione del pulsante manuale**
   - Consente all'utente di avviare o fermare manualmente la pompa premendo un pulsante.
   - Cambia lo stato della variabile `manualMode` per passare dalla modalità automatica a quella manuale.
   - Aggiorna lo schermo LCD per indicare la modalità corrente.

---

### Funzionamento del Sistema

1. Il sistema controlla continuamente il livello di umidità del terreno e agisce di conseguenza, mantenendo l'irrigazione automatica.
2. Lo schermo LCD fornisce informazioni visive sullo stato del sistema.
3. Il buzzer avverte l'utente quando l'umidità scende sotto una soglia critica.
4. Il pulsante permette all'utente di intervenire manualmente per attivare o disattivare la pompa.

---

### Vantaggi dell'Approccio

- **Non bloccante**: Ogni task viene eseguito indipendentemente dagli altri, grazie all'utilizzo di `millis()`.
- **Efficiente**: Nessun uso di `delay()`, il che consente al microcontrollore di rimanere sempre responsivo.
- **Scalabile**: È facile aggiungere nuovi task senza influenzare quelli già esistenti.

Questo esempio dimostra come un sistema complesso possa essere implementato su Arduino utilizzando il polling e le tecniche di multi-tasking non bloccante.