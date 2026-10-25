/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * Approccio 1: CODICE SEQUENZIALE (BLOCCANTE)
 * ============================================================================
 * 
 * OBIETTIVO:
 * Implementare controllo di 2 LED tramite 2 pulsanti usando delay().
 * 
 * SPECIFICHE:
 * - BTN1 (pin 2) controlla LED1 (pin 8 - rosso)
 * - BTN2 (pin 3) controlla LED2 (pin 9 - verde)
 * - Quando pulsante premuto: LED lampeggia (100ms ON, 100ms OFF)
 * - Quando pulsante rilasciato: LED spento
 * 
 * HARDWARE:
 * - Pin 2: BTN1 (con pull-down 10kΩ)
 * - Pin 3: BTN2 (con pull-down 10kΩ)
 * - Pin 8: LED1 rosso (con resistenza 220Ω)
 * - Pin 9: LED2 verde (con resistenza 220Ω)
 * 
 * LIMITAZIONI ATTESE:
 * - CPU bloccata durante delay
 * - Se BTN1 premuto, BTN2 non risponde
 * - LED non lampeggiano contemporaneamente
 * 
 * TEMPO STIMATO: 30 minuti
 * ============================================================================
 */

// ========== DEFINIZIONE PIN ==========
const int BTN1_PIN = 2;   // Pulsante 1
const int BTN2_PIN = 3;   // Pulsante 2
const int LED1_PIN = 8;   // LED rosso
const int LED2_PIN = 9;   // LED verde

// ========== COSTANTI ==========
const int BLINK_INTERVAL = 100;   // Intervallo lampeggio (ms)
const int DEBOUNCE_DELAY = 50;    // Delay debouncing (ms)

void setup() {
  // Inizializza Serial Monitor
  Serial.begin(9600);
  Serial.println("=== ES01 - Approccio 1: Sequenziale ===");
  Serial.println("ATTENZIONE: Codice BLOCCANTE!");
  Serial.println();
  
  // Configura pin pulsanti come INPUT
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  
  // Configura pin LED come OUTPUT
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  // Spegni LED inizialmente
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  
  Serial.println("Sistema inizializzato.");
  Serial.println("Premi BTN1 o BTN2 per controllare i LED.\n");
}

void loop() {
  // ========== GESTIONE BTN1 e LED1 ==========
  
  // TODO 1: Leggi stato BTN1
  // SUGGERIMENTO: usa digitalRead(BTN1_PIN)
  bool btn1_pressed = /* COMPLETA QUI */;
  
  if (btn1_pressed) {
    Serial.println("[BTN1] Premuto -> LED1 lampeggia");
    
    // TODO 2: Lampeggio LED1
    // - Accendi LED1
    // - Aspetta 100ms
    // - Spegni LED1
    // - Aspetta 100ms
    
    /* SCRIVI IL TUO CODICE QUI */
    
    
    
    
    
  } else {
    // TODO 3: Spegni LED1 quando BTN1 non premuto
    
    /* SCRIVI IL TUO CODICE QUI */
    
  }
  
  // ========== GESTIONE BTN2 e LED2 ==========
  
  // TODO 4: Implementa gestione BTN2 e LED2
  // (analogo a BTN1/LED1)
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  
  
}

/**
 * ============================================================================
 * DOMANDE DI VERIFICA (rispondi nei commenti)
 * ============================================================================
 * 
 * D1: Cosa succede se premi BTN1 e poi BTN2 rapidamente?
 * R1: ___________________________________________________________________
 *     ___________________________________________________________________
 * 
 * D2: Perché LED2 non risponde mentre LED1 lampeggia?
 * R2: ___________________________________________________________________
 *     ___________________________________________________________________
 * 
 * D3: Come migliorare la reattività senza cambiare approccio?
 * R3: ___________________________________________________________________
 *     ___________________________________________________________________
 * 
 * ============================================================================
 * TEST
 * ============================================================================
 * 
 * Test 1: Premi solo BTN1
 * Risultato atteso: LED1 lampeggia, LED2 spento
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 2: Premi solo BTN2
 * Risultato atteso: LED2 lampeggia, LED1 spento
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 3: Premi BTN1, poi BTN2 mentre BTN1 ancora premuto
 * Risultato atteso: LED1 continua lampeggiare, LED2 non risponde
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 4: Rilascia entrambi pulsanti
 * Risultato atteso: Entrambi LED spenti
 * Risultato ottenuto: ___________________________________________________
 * 
 * ============================================================================
 * NOTE IMPLEMENTATIVE
 * ============================================================================
 * 
 * PROBLEMA PRINCIPALE: CPU BLOCCATA
 * 
 * Durante delay(), il microcontrollore NON PUÒ:
 * - Leggere altri pulsanti
 * - Rispondere a eventi
 * - Eseguire altre operazioni
 * 
 * Questo approccio è adatto SOLO per:
 * - Sketch semplicissimi
 * - Prototipi veloci
 * - Dimostrazioni didattiche
 * 
 * Per applicazioni reali → Approccio 2 (polling) o superiore!
 * 
 * ============================================================================
 * ESERCIZIO BONUS
 * ============================================================================
 * 
 * Implementa debouncing semplice:
 * 
 * bool btn1_pressed = digitalRead(BTN1_PIN);
 * if (btn1_pressed) {
 *   delay(50);  // Attendi stabilizzazione
 *   if (digitalRead(BTN1_PIN)) {  // Ricontrolla
 *     // Pulsante veramente premuto
 *   }
 * }
 * 
 * ============================================================================
 */
