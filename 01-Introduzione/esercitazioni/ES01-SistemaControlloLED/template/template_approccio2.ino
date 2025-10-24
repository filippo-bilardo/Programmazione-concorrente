/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * Approccio 2: POLLING NON BLOCCANTE (MILLIS)
 * ============================================================================
 * 
 * OBIETTIVO:
 * Implementare controllo LED usando millis() per timing non bloccante.
 * Permettere lettura simultanea di entrambi i pulsanti.
 * 
 * SPECIFICHE:
 * - Usa millis() invece di delay()
 * - Ogni LED ha il proprio timer indipendente
 * - Polling continuo di entrambi i pulsanti
 * - Debouncing software implementato
 * 
 * VANTAGGI ATTESI:
 * - CPU sempre disponibile
 * - Risposta simultanea a entrambi pulsanti
 * - LED lampeggiano indipendentemente
 * 
 * TEMPO STIMATO: 45 minuti
 * ============================================================================
 */

// ========== DEFINIZIONE PIN ==========
const int BTN1_PIN = 2;
const int BTN2_PIN = 3;
const int LED1_PIN = 8;
const int LED2_PIN = 9;

// ========== COSTANTI ==========
const unsigned long BLINK_INTERVAL = 100;  // Intervallo lampeggio (ms)
const unsigned long DEBOUNCE_DELAY = 50;   // Debouncing (ms)

// ========== VARIABILI LED1 ==========
unsigned long led1_previousMillis = 0;     // Ultimo cambio stato LED1
int led1_state = LOW;                      // Stato corrente LED1
bool btn1_active = false;                  // BTN1 attivo (dopo debounce)

// ========== VARIABILI LED2 ==========
// TODO 1: Dichiara variabili analoghe per LED2

/* SCRIVI IL TUO CODICE QUI */




// ========== VARIABILI DEBOUNCING ==========
// TODO 2: Dichiara variabili per debouncing BTN1 e BTN2
// SUGGERIMENTO:
// - unsigned long btn1_lastDebounceTime
// - int btn1_lastState
// - int btn1_currentState

/* SCRIVI IL TUO CODICE QUI */






void setup() {
  Serial.begin(9600);
  Serial.println("=== ES01 - Approccio 2: Polling (millis) ===");
  Serial.println("CPU NON BLOCCANTE - Risposta simultanea!");
  Serial.println();
  
  // Configura pin
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  
  Serial.println("Sistema pronto. Premi pulsanti!\n");
}

void loop() {
  // Leggi tempo corrente UNA VOLTA per tutto il loop
  unsigned long currentMillis = millis();
  
  // ========== DEBOUNCING BTN1 ==========
  // TODO 3: Implementa debouncing per BTN1
  // ALGORITMO:
  // 1. Leggi stato corrente pulsante
  // 2. Se diverso da ultimo stato, aggiorna lastDebounceTime
  // 3. Se stato stabile per DEBOUNCE_DELAY ms, aggiorna btn1_active
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  
  // ========== GESTIONE LED1 ==========
  if (btn1_active) {
    // TODO 4: Pulsante premuto -> lampeggia LED1
    // SUGGERIMENTO:
    // if (currentMillis - led1_previousMillis >= BLINK_INTERVAL) {
    //   led1_previousMillis = currentMillis;
    //   led1_state = !led1_state;  // Toggle
    //   digitalWrite(LED1_PIN, led1_state);
    // }
    
    /* SCRIVI IL TUO CODICE QUI */
    
    
    
    
    
    
  } else {
    // TODO 5: Pulsante rilasciato -> spegni LED1
    
    /* SCRIVI IL TUO CODICE QUI */
    
    
  }
  
  // ========== DEBOUNCING BTN2 ==========
  // TODO 6: Implementa debouncing per BTN2 (analogo a BTN1)
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  
  // ========== GESTIONE LED2 ==========
  // TODO 7: Implementa gestione LED2 (analogo a LED1)
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  
  // ========== DEBUG (opzionale) ==========
  // TODO 8: Stampa stato ogni secondo
  // SUGGERIMENTO: usa variabile static lastPrintTime
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
}

/**
 * ============================================================================
 * DOMANDE DI VERIFICA
 * ============================================================================
 * 
 * D1: Perché millis() è meglio di delay() in questo caso?
 * R1: ___________________________________________________________________
 * 
 * D2: Cosa succede a millis() dopo 49 giorni? (overflow)
 * R2: ___________________________________________________________________
 * 
 * D3: Come funziona l'algoritmo di debouncing implementato?
 * R3: ___________________________________________________________________
 * 
 * D4: Cosa significa "unsigned long" e perché è necessario per millis()?
 * R4: ___________________________________________________________________
 * 
 * ============================================================================
 * TEST
 * ============================================================================
 * 
 * Test 1: Premi BTN1 e BTN2 contemporaneamente
 * Risultato atteso: Entrambi LED lampeggiano indipendentemente
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 2: Premi/rilascia BTN1 rapidamente (bounce simulato)
 * Risultato atteso: LED1 risponde in modo stabile (no flickering)
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 3: Misura tempo risposta BTN → LED
 * Procedura: Serial.print(millis()) quando premi e quando LED cambia
 * Risultato (latenza): ____________ ms
 * 
 * ============================================================================
 * ESERCIZIO BONUS
 * ============================================================================
 * 
 * BONUS 1: Contatore premute
 * Aggiungi contatori che contano quante volte ogni pulsante è stato premuto:
 * 
 * unsigned long btn1_pressCount = 0;
 * 
 * // Nel debouncing, quando rilevi transizione LOW→HIGH:
 * if (btn1_currentState == HIGH && btn1_lastState == LOW) {
 *   btn1_pressCount++;
 *   Serial.print("BTN1 premuto: ");
 *   Serial.println(btn1_pressCount);
 * }
 * 
 * BONUS 2: Frequenza variabile
 * Usa potenziometro su A0 per variare BLINK_INTERVAL (50-500ms):
 * 
 * int potValue = analogRead(A0);
 * unsigned long interval = map(potValue, 0, 1023, 50, 500);
 * 
 * ============================================================================
 * NOTE IMPLEMENTATIVE
 * ============================================================================
 * 
 * DEBOUNCING SOFTWARE:
 * 
 * Il problema del "bounce" si verifica quando un pulsante meccanico viene
 * premuto/rilasciato: i contatti metallici "rimbalzano" generando multipli
 * segnali ON/OFF in pochi millisecondi.
 * 
 * Soluzione: Consideriamo lo stato STABILE solo se rimane costante per
 * almeno DEBOUNCE_DELAY (50ms tipicamente).
 * 
 * Algoritmo:
 * 1. Leggi stato pulsante
 * 2. Se diverso da lettura precedente → reset timer
 * 3. Se uguale per > 50ms → stato considerato valido
 * 
 * OVERFLOW MILLIS():
 * 
 * millis() ritorna unsigned long (32-bit) che va in overflow dopo:
 * 2^32 ms = 4.294.967.296 ms ≈ 49.7 giorni
 * 
 * Dopo overflow, millis() riparte da 0.
 * 
 * La sottrazione (currentMillis - previousMillis) funziona CORRETTAMENTE
 * anche con overflow grazie all'aritmetica modulo 2^32!
 * 
 * Esempio overflow:
 * previousMillis = 4294967290  (5ms prima overflow)
 * currentMillis  = 5           (5ms dopo overflow)
 * differenza = 5 - 4294967290 = -4294967285 (negativo?)
 * 
 * MA in unsigned long:
 * -4294967285 mod 2^32 = 11 ✓ (corretto!)
 * 
 * ============================================================================
 */
