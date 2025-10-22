/**
 * Esempio 2: Lampeggio 3 LED - POLLING CON MILLIS()
 * 
 * Questo esempio usa millis() per temporizzare senza bloccare.
 * Tecnica "Blink Without Delay" - ogni LED ha il proprio timer.
 * La CPU è LIBERA di fare altro mentre i LED lampeggiano.
 * 
 * Hardware:
 * - LED1 connesso al pin 8 (lampeggia ogni 1000ms)
 * - LED2 connesso al pin 9 (lampeggia ogni 500ms)
 * - LED3 connesso al pin 10 (lampeggia ogni 200ms)
 * - Resistenze da 220Ω in serie ad ogni LED
 */

// Pin dei LED
const int LED1_PIN = 8;
const int LED2_PIN = 9;
const int LED3_PIN = 10;

// Intervalli di lampeggio (millisecondi)
const unsigned long LED1_INTERVAL = 1000;
const unsigned long LED2_INTERVAL = 500;
const unsigned long LED3_INTERVAL = 200;

// Variabili di stato per ogni LED
unsigned long led1_previousMillis = 0;
unsigned long led2_previousMillis = 0;
unsigned long led3_previousMillis = 0;

int led1_state = LOW;
int led2_state = LOW;
int led3_state = LOW;

// Contatori per statistiche
unsigned long led1_toggles = 0;
unsigned long led2_toggles = 0;
unsigned long led3_toggles = 0;

void setup() {
  // Inizializza i pin come output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
  // Inizializza la comunicazione seriale
  Serial.begin(9600);
  Serial.println("=== Esempio 2: LED con Polling (millis) ===");
  Serial.println("I LED lampeggiano CONTEMPORANEAMENTE!");
  Serial.println("La CPU è LIBERA di fare altro.\n");
  
  // Mostra intervalli
  Serial.print("LED1 (pin 8): ");
  Serial.print(LED1_INTERVAL);
  Serial.println(" ms");
  Serial.print("LED2 (pin 9): ");
  Serial.print(LED2_INTERVAL);
  Serial.println(" ms");
  Serial.print("LED3 (pin 10): ");
  Serial.print(LED3_INTERVAL);
  Serial.println(" ms\n");
}

void loop() {
  // Leggi il tempo corrente (timer di sistema)
  unsigned long currentMillis = millis();
  
  // ========== GESTIONE LED1 ==========
  if (currentMillis - led1_previousMillis >= LED1_INTERVAL) {
    // Salva l'ultimo tempo di cambio
    led1_previousMillis = currentMillis;
    
    // Cambia stato LED1
    led1_state = !led1_state;
    digitalWrite(LED1_PIN, led1_state);
    
    // Debug
    led1_toggles++;
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print(" ms] LED1 ");
    Serial.println(led1_state ? "ON" : "OFF");
  }
  
  // ========== GESTIONE LED2 ==========
  if (currentMillis - led2_previousMillis >= LED2_INTERVAL) {
    led2_previousMillis = currentMillis;
    
    led2_state = !led2_state;
    digitalWrite(LED2_PIN, led2_state);
    
    led2_toggles++;
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print(" ms] LED2 ");
    Serial.println(led2_state ? "ON" : "OFF");
  }
  
  // ========== GESTIONE LED3 ==========
  if (currentMillis - led3_previousMillis >= LED3_INTERVAL) {
    led3_previousMillis = currentMillis;
    
    led3_state = !led3_state;
    digitalWrite(LED3_PIN, led3_state);
    
    led3_toggles++;
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print(" ms] LED3 ");
    Serial.println(led3_state ? "ON" : "OFF");
  }
  
  // ========== ALTRE OPERAZIONI ==========
  // Qui la CPU è LIBERA di fare altro!
  // Esempio: leggi sensori, gestisci comunicazione, calcoli, etc.
  
  // Mostra statistiche ogni 10 secondi
  static unsigned long lastStatsTime = 0;
  if (currentMillis - lastStatsTime >= 10000) {
    lastStatsTime = currentMillis;
    
    Serial.println("\n=== STATISTICHE (10s) ===");
    Serial.print("LED1 toggle: ");
    Serial.println(led1_toggles);
    Serial.print("LED2 toggle: ");
    Serial.println(led2_toggles);
    Serial.print("LED3 toggle: ");
    Serial.println(led3_toggles);
    Serial.print("Loop iterations: molte migliaia!\n\n");
  }
  
  // Simula altro lavoro (polling sensori, comunicazione, etc.)
  // Questa operazione NON blocca il lampeggio dei LED
  simulaAltroLavoro();
}

void simulaAltroLavoro() {
  // Simula lettura sensore o altra operazione veloce
  // La CPU è disponibile per fare questo mentre i LED lampeggiano!
  static unsigned long counter = 0;
  counter++;
  
  // Ogni 100.000 iterazioni stampa un messaggio
  if (counter % 100000 == 0) {
    Serial.println("[INFO] CPU sta lavorando su altri task...");
  }
}

/**
 * VANTAGGI POLLING CON MILLIS():
 * 
 * 1. NON BLOCCANTE: La CPU è sempre disponibile
 * 2. PARALLELISMO: LED lampeggiano veramente in contemporanea
 * 3. TIMING FLESSIBILE: Ogni LED ha il proprio intervallo
 * 4. REATTIVO: Può rispondere immediatamente a eventi
 * 5. SCALABILE: Facile aggiungere più task
 * 
 * SVANTAGGI:
 * 
 * 1. PRECISIONE: Dipende dalla frequenza del loop (polling)
 * 2. CPU USAGE: loop() gira continuamente (spreco energia)
 * 3. COMPLESSITÀ: Più codice rispetto a delay()
 * 4. DERIVA: Piccoli errori di timing si accumulano
 * 
 * QUANDO USARLO:
 * - Quando servono più task concorrenti
 * - Quando serve reattività
 * - Per applicazioni real-time soft
 * - Quando gli interrupt non sono necessari
 * 
 * NOTA IMPORTANTE:
 * millis() overflow dopo ~49 giorni. Per applicazioni long-running,
 * gestire overflow:
 *   if ((unsigned long)(currentMillis - previousMillis) >= interval)
 */
