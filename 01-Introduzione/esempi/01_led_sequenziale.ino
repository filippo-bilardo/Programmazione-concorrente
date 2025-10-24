/**
 * Esempio 1: Lampeggio 3 LED - CODICE SEQUENZIALE
 * https://wokwi.com/projects/445614989333189633
 * 
 * Questo esempio usa delay() per temporizzare il lampeggio.
 * PROBLEMA: Il microcontrollore è bloccato durante i delay,
 * non può fare altro (polling sensori, rispondere a eventi, etc.)
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
const int LED1_INTERVAL = 1000;
const int LED2_INTERVAL = 500;
const int LED3_INTERVAL = 200;

void setup() {
  // Inizializza i pin come output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
  // Inizializza la comunicazione seriale per debug
  Serial.begin(9600);
  Serial.println("=== Esempio 1: LED Sequenziale ===");
  Serial.println("ATTENZIONE: Il codice è BLOCCANTE durante i delay!");
  Serial.println();
}

void loop() {
  // Lampeggio LED1 (1000ms)
  digitalWrite(LED1_PIN, HIGH);
  Serial.println("LED1 ON");
  delay(LED1_INTERVAL / 2);
  digitalWrite(LED1_PIN, LOW);
  Serial.println("LED1 OFF");
  delay(LED1_INTERVAL / 2);
  
  // Lampeggio LED2 (500ms)
  digitalWrite(LED2_PIN, HIGH);
  Serial.println("LED2 ON");
  delay(LED2_INTERVAL / 2);
  digitalWrite(LED2_PIN, LOW);
  Serial.println("LED2 OFF");
  delay(LED2_INTERVAL / 2);
  
  // Lampeggio LED3 (200ms)
  digitalWrite(LED3_PIN, HIGH);
  Serial.println("LED3 ON");
  delay(LED3_INTERVAL / 2);
  digitalWrite(LED3_PIN, LOW);
  Serial.println("LED3 OFF");
  delay(LED3_INTERVAL / 2);
  
  // PROBLEMA: Durante ogni delay(), il microcontrollore è BLOCCATO
  // Non può eseguire altre operazioni!
  // I LED non lampeggiano veramente in modo indipendente.
}

/**
 * SVANTAGGI APPROCCIO SEQUENZIALE:
 * 
 * 1. CPU BLOCCATA: Durante delay(), il microcontrollore non può fare altro
 * 2. NESSUN PARALLELISMO: I LED non lampeggiano contemporaneamente
 * 3. TIMING RIGIDO: Impossibile avere intervalli diversi simultanei
 * 4. NON REATTIVO: Non può rispondere a eventi esterni (pulsanti, sensori)
 * 5. SCALABILITÀ ZERO: Aggiungere più task blocca ulteriormente il sistema
 * 
 * QUANDO USARLO:
 * - Solo per sketch molto semplici
 * - Quando NON serve reattività
 * - Per prototipi veloci
 * 
 * OUTPUT SERIALE ATTESO:
 * LED1 ON
 * LED1 OFF
 * LED2 ON
 * LED2 OFF
 * LED3 ON
 * LED3 OFF
 * [ciclo ripete]
 */
