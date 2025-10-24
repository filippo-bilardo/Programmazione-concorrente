/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * SOLUZIONE - Approccio 3: Interrupt
 * ============================================================================
 * 
 * Implementazione completa con interrupt esterni + TimerOne
 */

#include <TimerOne.h>

const int BTN1_PIN = 2;  // INT0 su Arduino Uno
const int BTN2_PIN = 3;  // INT1 su Arduino Uno
const int LED1_PIN = 8;
const int LED2_PIN = 9;

const unsigned long BLINK_INTERVAL = 100000;  // 100ms in microsecondi

// Variabili condivise (volatile!)
volatile bool led1_blinking = false;
volatile bool led1_state = LOW;

volatile bool led2_blinking = false;
volatile bool led2_state = LOW;

// Statistiche (accesso atomico!)
volatile unsigned long btn1_pressCount = 0;
volatile unsigned long btn2_pressCount = 0;
volatile unsigned long timer_ticks = 0;

// Debouncing interrupt
volatile unsigned long btn1_lastInterruptTime = 0;
volatile unsigned long btn2_lastInterruptTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;  // ms

void setup() {
  Serial.begin(9600);
  Serial.println("=== ES01 - Approccio 3: Interrupt ===\n");
  
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  // Configura interrupt esterni (RISING = pressione)
  attachInterrupt(digitalPinToInterrupt(BTN1_PIN), btn1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN2_PIN), btn2_ISR, CHANGE);
  
  // Configura timer interrupt
  Timer1.initialize(BLINK_INTERVAL);
  Timer1.attachInterrupt(timerISR);
  
  Serial.println("Interrupt configurati!");
  Serial.println("- INT0 (pin 2) → BTN1");
  Serial.println("- INT1 (pin 3) → BTN2");
  Serial.println("- Timer1 @ 100ms → LED blink\n");
}

// ISR Pulsante 1
void btn1_ISR() {
  unsigned long currentTime = millis();
  
  // Debouncing software
  if (currentTime - btn1_lastInterruptTime > DEBOUNCE_DELAY) {
    bool btn_state = digitalRead(BTN1_PIN);
    
    if (btn_state == HIGH) {
      // Premuto
      led1_blinking = true;
      btn1_pressCount++;
    } else {
      // Rilasciato
      led1_blinking = false;
      digitalWrite(LED1_PIN, LOW);
      led1_state = LOW;
    }
    
    btn1_lastInterruptTime = currentTime;
  }
}

// ISR Pulsante 2
void btn2_ISR() {
  unsigned long currentTime = millis();
  
  // Debouncing software
  if (currentTime - btn2_lastInterruptTime > DEBOUNCE_DELAY) {
    bool btn_state = digitalRead(BTN2_PIN);
    
    if (btn_state == HIGH) {
      // Premuto
      led2_blinking = true;
      btn2_pressCount++;
    } else {
      // Rilasciato
      led2_blinking = false;
      digitalWrite(LED2_PIN, LOW);
      led2_state = LOW;
    }
    
    btn2_lastInterruptTime = currentTime;
  }
}

// ISR Timer
void timerISR() {
  timer_ticks++;
  
  // Toggle LED1 se blinking attivo
  if (led1_blinking) {
    led1_state = !led1_state;
    digitalWrite(LED1_PIN, led1_state);
  }
  
  // Toggle LED2 se blinking attivo
  if (led2_blinking) {
    led2_state = !led2_state;
    digitalWrite(LED2_PIN, led2_state);
  }
}

void loop() {
  // CPU libera! Può fare altro...
  
  static unsigned long lastStatsTime = 0;
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastStatsTime >= 10000) {
    lastStatsTime = currentMillis;
    
    // Accesso atomico a variabili volatile
    noInterrupts();
    unsigned long count1 = btn1_pressCount;
    unsigned long count2 = btn2_pressCount;
    unsigned long ticks = timer_ticks;
    interrupts();
    
    Serial.println("\n--- Statistiche ---");
    Serial.print("BTN1 premuto: ");
    Serial.print(count1);
    Serial.println(" volte");
    Serial.print("BTN2 premuto: ");
    Serial.print(count2);
    Serial.println(" volte");
    Serial.print("Timer ticks: ");
    Serial.println(ticks);
    Serial.print("Uptime: ");
    Serial.print(currentMillis / 1000);
    Serial.println(" secondi\n");
  }
  
  // Simula task pesante (CPU libera comunque!)
  // delay(100);  // LED continuano lampeggiare!
}

/**
 * RISPOSTE DOMANDE VERIFICA:
 * 
 * D1: Perché variabili volatile?
 * R1: volatile indica al compilatore che variabile può cambiare
 *     "fuori dal flusso normale" (tramite ISR). Senza volatile,
 *     compilatore potrebbe ottimizzare (cache in registro) e
 *     perdere aggiornamenti fatti da ISR.
 * 
 * D2: Cosa fa attachInterrupt()?
 * R2: Registra funzione ISR da chiamare quando interrupt si attiva.
 *     CHANGE = chiama ISR sia su RISING che su FALLING (pressione+rilascio).
 *     Alternativa: RISING (solo pressione), FALLING (solo rilascio).
 * 
 * D3: Differenza tra interrupt esterni e timer?
 * R3: - Interrupt esterni: generati da evento esterno (pulsante)
 *     - Timer interrupt: generati da hardware timer a intervalli precisi
 *     Entrambi interrompono loop() e chiamano ISR.
 * 
 * D4: Perché noInterrupts() / interrupts()?
 * R4: Crea "sezione critica" dove interrupt sono disabilitati.
 *     Serve per accesso ATOMICO a variabili multi-byte (unsigned long = 4 byte).
 *     Se ISR interrompe durante lettura, potremmo leggere valore parziale!
 *     Esempio: count=0x12345678, leggiamo 0x12340000 → errore!
 * 
 * D5: Debouncing in ISR?
 * R5: Controllo tempo dall'ultimo interrupt. Se < DEBOUNCE_DELAY, ignoriamo.
 *     ISR deve essere VELOCE, quindi debouncing software minimale.
 *     Per debouncing robusto → condensatore hardware (0.1µF).
 */
