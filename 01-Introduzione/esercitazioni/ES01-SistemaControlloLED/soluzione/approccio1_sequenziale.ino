/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * SOLUZIONE - Approccio 1: Sequenziale (Delay)
 * ============================================================================
 * 
 * Implementazione completa con delay() - codice bloccante
 */

const int BTN1_PIN = 2;
const int BTN2_PIN = 3;
const int LED1_PIN = 8;
const int LED2_PIN = 9;

const unsigned long BLINK_INTERVAL = 100;  // 100ms

void setup() {
  Serial.begin(9600);
  Serial.println("=== ES01 - Approccio 1: Sequenziale ===\n");
  
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  Serial.println("Sistema pronto!");
  Serial.println("NOTA: Codice bloccante - un pulsante alla volta!\n");
}

void loop() {
  // Gestione BTN1
  if (digitalRead(BTN1_PIN) == HIGH) {
    Serial.println("[BTN1] Premuto - LED1 lampeggia");
    
    while (digitalRead(BTN1_PIN) == HIGH) {
      digitalWrite(LED1_PIN, HIGH);
      delay(BLINK_INTERVAL);
      
      digitalWrite(LED1_PIN, LOW);
      delay(BLINK_INTERVAL);
    }
    
    digitalWrite(LED1_PIN, LOW);
    Serial.println("[BTN1] Rilasciato\n");
  }
  
  // Gestione BTN2
  if (digitalRead(BTN2_PIN) == HIGH) {
    Serial.println("[BTN2] Premuto - LED2 lampeggia");
    
    while (digitalRead(BTN2_PIN) == HIGH) {
      digitalWrite(LED2_PIN, HIGH);
      delay(BLINK_INTERVAL);
      
      digitalWrite(LED2_PIN, LOW);
      delay(BLINK_INTERVAL);
    }
    
    digitalWrite(LED2_PIN, LOW);
    Serial.println("[BTN2] Rilasciato\n");
  }
}

/**
 * RISPOSTE DOMANDE VERIFICA:
 * 
 * D1: Cosa succede premendo BTN1 e BTN2 contemporaneamente?
 * R1: Solo UN pulsante viene gestito (quello controllato prima nel loop).
 *     L'altro viene ignorato perché CPU è bloccata nel while().
 * 
 * D2: Perché questo codice è "bloccante"?
 * R2: delay() ferma completamente l'esecuzione. Durante delay(100),
 *     il programma NON può fare altro (leggere pulsanti, calcolare, ecc).
 * 
 * D3: Come migliora debouncing?
 * R3: Il while() aspetta rilascio pulsante prima di continuare.
 *     Questo evita letture multiple, MA non gestisce bounce meccanico
 *     (serve condensatore o controllo temporale).
 */
