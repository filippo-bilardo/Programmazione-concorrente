/**
 * Esempio 3: Lampeggio 3 LED - INTERRUPT HARDWARE (TIMER)
 * 
 * Questo esempio usa Timer Interrupt per temporizzare in modo PRECISO.
 * Il Timer Hardware genera interrupt a intervalli regolari.
 * La CPU può andare in SLEEP tra gli interrupt (risparmio energia).
 * 
 * Richiede libreria: TimerOne
 * (Installare da Library Manager: Sketch -> Include Library -> Manage Libraries -> cerca "TimerOne")
 * 
 * Hardware:
 * - LED1 connesso al pin 8 (lampeggia ogni 1000ms)
 * - LED2 connesso al pin 9 (lampeggia ogni 500ms)
 * - LED3 connesso al pin 10 (lampeggia ogni 200ms)
 * - Resistenze da 220Ω in serie ad ogni LED
 * 
 * NOTA: Questo esempio è specifico per Arduino Uno/Mega (ATmega328P/2560)
 * Per altri board, adattare il timer usato.
 */

#include <TimerOne.h>  // Timer1 (16-bit)

// Pin dei LED
const int LED1_PIN = 8;
const int LED2_PIN = 9;
const int LED3_PIN = 10;

// Intervalli di lampeggio (millisecondi)
const unsigned long LED1_INTERVAL = 1000;
const unsigned long LED2_INTERVAL = 500;
const unsigned long LED3_INTERVAL = 200;

// Contatori per divisione frequenza
volatile unsigned long led1_counter = 0;
volatile unsigned long led2_counter = 0;
volatile unsigned long led3_counter = 0;

// Stati LED (volatile perché modificati in ISR)
volatile int led1_state = LOW;
volatile int led2_state = LOW;
volatile int led3_state = LOW;

// Periodo base dell'interrupt (microscondi)
// Usiamo 1ms (1000us) come base comune
const unsigned long ISR_PERIOD_US = 1000;  // 1ms = 1000us

// Calcola quanti tick servono per ogni LED
const unsigned long LED1_TICKS = LED1_INTERVAL;      // 1000ms / 1ms = 1000 tick
const unsigned long LED2_TICKS = LED2_INTERVAL;      // 500ms / 1ms = 500 tick
const unsigned long LED3_TICKS = LED3_INTERVAL;      // 200ms / 1ms = 200 tick

// Statistiche (volatile per accesso da ISR)
volatile unsigned long led1_toggles = 0;
volatile unsigned long led2_toggles = 0;
volatile unsigned long led3_toggles = 0;
volatile unsigned long isr_count = 0;

void setup() {
  // Inizializza i pin come output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
  // Inizializza la comunicazione seriale
  Serial.begin(9600);
  Serial.println("=== Esempio 3: LED con Timer Interrupt ===");
  Serial.println("Timer Hardware controlla i LED!");
  Serial.println("MASSIMA PRECISIONE - CPU può dormire.\n");
  
  // Mostra configurazione
  Serial.print("ISR period: ");
  Serial.print(ISR_PERIOD_US);
  Serial.println(" us (1ms)");
  Serial.print("LED1 toggle ogni ");
  Serial.print(LED1_TICKS);
  Serial.println(" tick");
  Serial.print("LED2 toggle ogni ");
  Serial.print(LED2_TICKS);
  Serial.println(" tick");
  Serial.print("LED3 toggle ogni ");
  Serial.print(LED3_TICKS);
  Serial.println(" tick\n");
  
  // Inizializza Timer1
  Timer1.initialize(ISR_PERIOD_US);  // 1ms
  Timer1.attachInterrupt(timerISR);  // Associa ISR
  
  Serial.println("Timer interrupt attivo!\n");
}

/**
 * ISR (Interrupt Service Routine)
 * 
 * Chiamata automaticamente ogni 1ms dal Timer Hardware.
 * ATTENZIONE: Deve essere VELOCE! No delay(), no Serial.print() (lento).
 * Usare solo operazioni atomiche e veloci.
 */
void timerISR() {
  isr_count++;
  
  // ========== GESTIONE LED1 ==========
  led1_counter++;
  if (led1_counter >= LED1_TICKS) {
    led1_counter = 0;
    led1_state = !led1_state;
    digitalWrite(LED1_PIN, led1_state);
    led1_toggles++;
  }
  
  // ========== GESTIONE LED2 ==========
  led2_counter++;
  if (led2_counter >= LED2_TICKS) {
    led2_counter = 0;
    led2_state = !led2_state;
    digitalWrite(LED2_PIN, led2_state);
    led2_toggles++;
  }
  
  // ========== GESTIONE LED3 ==========
  led3_counter++;
  if (led3_counter >= LED3_TICKS) {
    led3_counter = 0;
    led3_state = !led3_state;
    digitalWrite(LED3_PIN, led3_state);
    led3_toggles++;
  }
}

void loop() {
  // Il loop() è LIBERO di fare altro!
  // I LED sono gestiti completamente dall'interrupt hardware.
  
  // Mostra statistiche ogni 10 secondi
  static unsigned long lastStatsTime = 0;
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastStatsTime >= 10000) {
    lastStatsTime = currentMillis;
    
    // Disabilita interrupt durante lettura (atomic read)
    noInterrupts();
    unsigned long isr_copy = isr_count;
    unsigned long led1_copy = led1_toggles;
    unsigned long led2_copy = led2_toggles;
    unsigned long led3_copy = led3_toggles;
    interrupts();
    
    Serial.println("\n=== STATISTICHE (10s) ===");
    Serial.print("ISR chiamate: ");
    Serial.println(isr_copy);
    Serial.print("LED1 toggle: ");
    Serial.println(led1_copy);
    Serial.print("LED2 toggle: ");
    Serial.println(led2_copy);
    Serial.print("LED3 toggle: ");
    Serial.println(led3_copy);
    Serial.println("Loop: CPU LIBERA per altri task!\n");
  }
  
  // Simula altro lavoro (o vai in sleep per risparmiare energia)
  simulaAltroLavoro();
  
  // OPZIONALE: Abilita sleep mode per risparmio energetico
  // #include <avr/sleep.h>
  // set_sleep_mode(SLEEP_MODE_IDLE);
  // sleep_mode();  // CPU dorme fino al prossimo interrupt
}

void simulaAltroLavoro() {
  // La CPU può fare lavoro complesso qui.
  // I LED continuano a lampeggiare PERFETTAMENTE grazie all'interrupt!
  
  static unsigned long counter = 0;
  counter++;
  
  // Ogni 500.000 iterazioni, simula operazione lenta
  if (counter % 500000 == 0) {
    Serial.println("[INFO] Loop sta facendo lavoro pesante...");
    
    // Simula operazione che richiede tempo
    // (I LED continuano a lampeggiare SENZA PROBLEMI!)
    unsigned long sum = 0;
    for (int i = 0; i < 10000; i++) {
      sum += i;
    }
    
    Serial.print("[INFO] Calcolo completato: ");
    Serial.println(sum);
  }
  
  // Piccolo delay per non saturare seriale
  delay(10);  // I LED NON sono influenzati da questo delay!
}

/**
 * VANTAGGI INTERRUPT:
 * 
 * 1. PRECISIONE MASSIMA: Timer hardware è preciso al microsecondo
 * 2. NON BLOCCANTE: ISR esegue indipendentemente dal loop()
 * 3. RISPARMIO ENERGIA: CPU può andare in sleep tra interrupt
 * 4. DETERMINISMO: Timing garantito anche con loop() lento
 * 5. PRIORITÀ: Interrupt ha priorità su codice normale
 * 
 * SVANTAGGI:
 * 
 * 1. COMPLESSITÀ: Richiede comprensione interrupt e ISR
 * 2. DEBUGGING: Difficile debug codice ISR
 * 3. RISORSE: Usa timer hardware (risorsa limitata)
 * 4. VINCOLI ISR: ISR deve essere veloce, no blocking calls
 * 5. RACE CONDITION: Serve sincronizzazione variabili condivise
 * 
 * QUANDO USARLO:
 * - Quando serve timing PRECISO
 * - Per applicazioni real-time hard
 * - Quando serve risparmio energetico (sleep mode)
 * - Per sistemi embedded professionali
 * - Quando il polling non basta
 * 
 * BEST PRACTICES ISR:
 * 
 * 1. ISR deve essere VELOCE (< 50us idealmente)
 * 2. NO delay() in ISR
 * 3. NO Serial.print() in ISR (troppo lento)
 * 4. Variabili condivise DEVONO essere volatile
 * 5. Usa noInterrupts()/interrupts() per accesso atomico
 * 6. Minimizza operazioni in ISR (solo essenziale)
 * 
 * TIMER DISPONIBILI (Arduino Uno):
 * - Timer0: 8-bit, usato da millis()/delay() - NON TOCCARE!
 * - Timer1: 16-bit, libero (usato qui con TimerOne library)
 * - Timer2: 8-bit, libero
 * 
 * ALTERNATIVE:
 * - MsTimer2 library (usa Timer2)
 * - FlexiTimer2 library
 * - Direct register manipulation (avanzato)
 * 
 * NOTA ARCHITETTURALE:
 * Questo esempio dimostra programmazione INTERRUPT-DRIVEN,
 * paradigma fondamentale nei sistemi embedded real-time.
 * La CPU risponde a EVENTI (interrupt) invece di fare polling continuo.
 */

/**
 * CONFRONTO TRE APPROCCI:
 * 
 * | Aspetto          | Sequenziale | Polling    | Interrupt    |
 * |------------------|-------------|------------|--------------|
 * | Precisione       | Bassa       | Media      | Alta         |
 * | CPU Libera       | NO          | Parziale   | SI           |
 * | Complessità      | Bassa       | Media      | Alta         |
 * | Risparmio Energia| NO          | NO         | SI (sleep)   |
 * | Scalabilità      | Pessima     | Buona      | Ottima       |
 * | Debug            | Facile      | Medio      | Difficile    |
 * | Uso Risorse      | Minimo      | Minimo     | Timer HW     |
 * 
 * SCELTA APPROCCIO:
 * - Sequenziale: Solo per sketch semplicissimi
 * - Polling: Per la maggior parte delle applicazioni hobbistiche
 * - Interrupt: Per applicazioni professionali, real-time, low-power
 */
