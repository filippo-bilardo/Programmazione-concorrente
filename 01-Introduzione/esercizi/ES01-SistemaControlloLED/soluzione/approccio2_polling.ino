/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * SOLUZIONE - Approccio 2: Polling (millis)
 * ============================================================================
 * 
 * Implementazione completa con millis() - codice non-bloccante
 */

const int BTN1_PIN = 2;
const int BTN2_PIN = 3;
const int LED1_PIN = 8;
const int LED2_PIN = 9;

const unsigned long BLINK_INTERVAL = 100;  // 100ms
const unsigned long DEBOUNCE_DELAY = 50;   // 50ms

// Variabili stato LED
unsigned long led1_previousMillis = 0;
bool led1_state = LOW;
bool led1_blinking = false;

unsigned long led2_previousMillis = 0;
bool led2_state = LOW;
bool led2_blinking = false;

// Variabili debouncing BTN1
int btn1_lastState = LOW;
int btn1_currentState;
unsigned long btn1_lastDebounceTime = 0;

// Variabili debouncing BTN2
int btn2_lastState = LOW;
int btn2_currentState;
unsigned long btn2_lastDebounceTime = 0;

// Statistiche
unsigned long btn1_pressCount = 0;
unsigned long btn2_pressCount = 0;
unsigned long lastStatsTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=== ES01 - Approccio 2: Polling ===\n");
  
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  Serial.println("Sistema pronto!");
  Serial.println("Codice non-bloccante - gestione simultanea!\n");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // === DEBOUNCING PULSANTE 1 ===
  int reading1 = digitalRead(BTN1_PIN);
  
  if (reading1 != btn1_lastState) {
    btn1_lastDebounceTime = currentMillis;
  }
  
  if ((currentMillis - btn1_lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading1 != btn1_currentState) {
      btn1_currentState = reading1;
      
      if (btn1_currentState == HIGH) {
        // Premuto
        led1_blinking = true;
        btn1_pressCount++;
        Serial.println("[BTN1] Premuto");
      } else {
        // Rilasciato
        led1_blinking = false;
        digitalWrite(LED1_PIN, LOW);
        led1_state = LOW;
        Serial.println("[BTN1] Rilasciato");
      }
    }
  }
  
  btn1_lastState = reading1;
  
  // === DEBOUNCING PULSANTE 2 ===
  int reading2 = digitalRead(BTN2_PIN);
  
  if (reading2 != btn2_lastState) {
    btn2_lastDebounceTime = currentMillis;
  }
  
  if ((currentMillis - btn2_lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading2 != btn2_currentState) {
      btn2_currentState = reading2;
      
      if (btn2_currentState == HIGH) {
        // Premuto
        led2_blinking = true;
        btn2_pressCount++;
        Serial.println("[BTN2] Premuto");
      } else {
        // Rilasciato
        led2_blinking = false;
        digitalWrite(LED2_PIN, LOW);
        led2_state = LOW;
        Serial.println("[BTN2] Rilasciato");
      }
    }
  }
  
  btn2_lastState = reading2;
  
  // === GESTIONE LED1 ===
  if (led1_blinking) {
    if (currentMillis - led1_previousMillis >= BLINK_INTERVAL) {
      led1_previousMillis = currentMillis;
      led1_state = !led1_state;
      digitalWrite(LED1_PIN, led1_state);
    }
  }
  
  // === GESTIONE LED2 ===
  if (led2_blinking) {
    if (currentMillis - led2_previousMillis >= BLINK_INTERVAL) {
      led2_previousMillis = currentMillis;
      led2_state = !led2_state;
      digitalWrite(LED2_PIN, led2_state);
    }
  }
  
  // === STATISTICHE (ogni 10 secondi) ===
  if (currentMillis - lastStatsTime >= 10000) {
    lastStatsTime = currentMillis;
    
    Serial.println("\n--- Statistiche ---");
    Serial.print("BTN1 premuto: ");
    Serial.print(btn1_pressCount);
    Serial.println(" volte");
    Serial.print("BTN2 premuto: ");
    Serial.print(btn2_pressCount);
    Serial.println(" volte");
    Serial.print("Uptime: ");
    Serial.print(currentMillis / 1000);
    Serial.println(" secondi\n");
  }
}

/**
 * RISPOSTE DOMANDE VERIFICA:
 * 
 * D1: Differenza tra lastState e currentState?
 * R1: lastState è la lettura RAW (con bounce).
 *     currentState è lo stato STABILE dopo debouncing.
 *     Cambia solo se reading è stabile per DEBOUNCE_DELAY ms.
 * 
 * D2: Cosa fa (currentMillis - previousMillis >= INTERVAL)?
 * R2: Controlla se è passato tempo sufficiente dall'ultimo aggiornamento.
 *     È modo non-bloccante di misurare tempo (vs delay() bloccante).
 * 
 * D3: Overflow di millis()?
 * R3: millis() va in overflow dopo 49.7 giorni (2^32 ms).
 *     La sottrazione (currentMillis - previousMillis) funziona SEMPRE
 *     grazie all'aritmetica modulo 2^32 di unsigned long.
 *     Esempio: current=100, previous=4294967290 (prima overflow)
 *     → current - previous = 100 - 4294967290 = 110 (corretto!)
 * 
 * D4: Perché debouncing PRIMA di gestione LED?
 * R4: Dobbiamo prima stabilizzare input (pulsanti), poi agire su output (LED).
 *     Se aggiornassimo LED prima, potrebbero lampeggiare con bounce.
 */
