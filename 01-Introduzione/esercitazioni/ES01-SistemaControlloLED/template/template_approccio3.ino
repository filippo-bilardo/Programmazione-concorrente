/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * Approccio 3: INTERRUPT HARDWARE
 * ============================================================================
 * 
 * OBIETTIVO:
 * Usare interrupt hardware per rilevare pressione pulsanti.
 * Usare Timer interrupt per gestire lampeggio LED.
 * 
 * SPECIFICHE:
 * - External interrupt su pin 2 e 3 (BTN1, BTN2)
 * - Timer interrupt ogni 100ms per lampeggio
 * - Variabili condivise dichiarate volatile
 * - ISR veloci e non bloccanti
 * 
 * LIBRERIA RICHIESTA:
 * - TimerOne (installare da Library Manager)
 * 
 * VANTAGGI ATTESI:
 * - Risposta immediata ai pulsanti (interrupt)
 * - Timing preciso lampeggio (hardware timer)
 * - CPU può dormire o fare altro lavoro
 * 
 * TEMPO STIMATO: 60 minuti
 * ============================================================================
 */

#include <TimerOne.h>  // Timer1 library

// ========== DEFINIZIONE PIN ==========
const int BTN1_PIN = 2;   // Pin con interrupt (INT0)
const int BTN2_PIN = 3;   // Pin con interrupt (INT1)
const int LED1_PIN = 8;
const int LED2_PIN = 9;

// ========== COSTANTI ==========
const unsigned long TIMER_PERIOD_US = 100000;  // 100ms in microsecondi

// ========== VARIABILI INTERRUPT (VOLATILE!) ==========
// TODO 1: Dichiara variabili condivise tra ISR e main loop
// IMPORTANTE: DEVONO essere volatile!
// SUGGERIMENTO:
// - volatile bool btn1_active
// - volatile bool btn2_active
// - volatile int led1_state
// - volatile int led2_state

/* SCRIVI IL TUO CODICE QUI */






// ========== STATISTICHE ==========
volatile unsigned long led1_toggles = 0;
volatile unsigned long led2_toggles = 0;
volatile unsigned long timer_isr_count = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=== ES01 - Approccio 3: Interrupt Hardware ===");
  Serial.println("Timer interrupt + External interrupt");
  Serial.println();
  
  // Configura pin
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  
  // ========== CONFIGURA EXTERNAL INTERRUPT ==========
  // TODO 2: Associa ISR ai pulsanti
  // SUGGERIMENTO:
  // attachInterrupt(digitalPinToInterrupt(BTN1_PIN), btn1ISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(BTN2_PIN), btn2ISR, CHANGE);
  //
  // Modalità CHANGE: interrupt su RISING e FALLING edge
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  // ========== CONFIGURA TIMER INTERRUPT ==========
  // TODO 3: Inizializza Timer1
  // SUGGERIMENTO:
  // Timer1.initialize(TIMER_PERIOD_US);
  // Timer1.attachInterrupt(timerISR);
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  Serial.println("Interrupt configurati!");
  Serial.println("Sistema pronto.\n");
}

// ========== ISR PULSANTE 1 ==========
// TODO 4: Implementa ISR per BTN1
// REGOLE ISR:
// - Deve essere VELOCE (<50us idealmente)
// - NO delay()
// - NO Serial.print() (troppo lento)
// - Solo operazioni essenziali
//
// COSA FARE:
// - Leggi stato BTN1
// - Aggiorna btn1_active

/* SCRIVI IL TUO CODICE QUI */
void btn1ISR() {
  
  
  
}

// ========== ISR PULSANTE 2 ==========
// TODO 5: Implementa ISR per BTN2 (analogo a BTN1)

/* SCRIVI IL TUO CODICE QUI */
void btn2ISR() {
  
  
  
}

// ========== ISR TIMER ==========
// TODO 6: Implementa ISR Timer (chiamata ogni 100ms)
// COSA FARE:
// - Se btn1_active: toggle LED1, incrementa contatori
// - Altrimenti: spegni LED1
// - Stesso per btn2/LED2
// - Incrementa timer_isr_count

/* SCRIVI IL TUO CODICE QUI */
void timerISR() {
  timer_isr_count++;
  
  // Gestione LED1
  
  
  
  
  
  
  
  // Gestione LED2
  
  
  
  
  
  
  
}

void loop() {
  // Il loop() è LIBERO di fare altro!
  // I LED sono gestiti completamente dagli interrupt.
  
  // ========== STATISTICHE OGNI 10 SECONDI ==========
  // TODO 7: Stampa statistiche ogni 10 secondi
  // ATTENZIONE: Accesso a variabili volatile richiede protezione!
  //
  // SUGGERIMENTO:
  // static unsigned long lastStatsTime = 0;
  // unsigned long currentMillis = millis();
  //
  // if (currentMillis - lastStatsTime >= 10000) {
  //   lastStatsTime = currentMillis;
  //
  //   // Copia atomica variabili volatile
  //   noInterrupts();  // Disabilita interrupt
  //   unsigned long isr_copy = timer_isr_count;
  //   unsigned long led1_copy = led1_toggles;
  //   unsigned long led2_copy = led2_toggles;
  //   interrupts();    // Riabilita interrupt
  //
  //   // Stampa
  //   Serial.println("\n=== STATISTICHE (10s) ===");
  //   Serial.print("Timer ISR: ");
  //   Serial.println(isr_copy);
  //   // ...
  // }
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  // ========== ALTRE OPERAZIONI ==========
  // TODO 8 (BONUS): Simula altro lavoro pesante
  // Anche con delay() qui, i LED continuano lampeggiare!
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
}

/**
 * ============================================================================
 * DOMANDE DI VERIFICA
 * ============================================================================
 * 
 * D1: Perché serve la keyword 'volatile' per variabili condivise?
 * R1: ___________________________________________________________________
 * 
 * D2: Cosa fa attachInterrupt(digitalPinToInterrupt(2), btn1ISR, CHANGE)?
 * R2: ___________________________________________________________________
 * 
 * D3: Quali operazioni NON si possono fare in ISR?
 * R3: ___________________________________________________________________
 * 
 * D4: Perché usiamo noInterrupts()/interrupts() per leggere statistiche?
 * R4: ___________________________________________________________________
 * 
 * D5: Che differenza c'è tra CHANGE, RISING, FALLING in attachInterrupt?
 * R5: ___________________________________________________________________
 * 
 * ============================================================================
 * TEST
 * ============================================================================
 * 
 * Test 1: Premi BTN1, osserva LED1
 * Risultato atteso: LED1 lampeggia con timing preciso
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 2: Nel loop(), aggiungi delay(1000)
 * Risultato atteso: LED continuano lampeggiare normalmente (interrupt!)
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 3: Misura precisione timing
 * Procedura: Usa oscilloscopio o logic analyzer su LED1
 * Frequenza teorica: 5Hz (periodo 200ms: 100ms ON + 100ms OFF)
 * Frequenza misurata: ____________ Hz
 * Errore: ____________ %
 * 
 * ============================================================================
 * ESERCIZIO BONUS
 * ============================================================================
 * 
 * BONUS 1: Debouncing Hardware
 * Aggiungi condensatore 0.1µF tra pin pulsante e GND.
 * Questo filtra rumore elettrico riducendo bounce.
 * 
 * BONUS 2: Sleep Mode
 * Metti CPU in sleep tra interrupt per risparmio energetico:
 * 
 * #include <avr/sleep.h>
 * 
 * void loop() {
 *   // Statistiche...
 *   
 *   // Sleep mode
 *   set_sleep_mode(SLEEP_MODE_IDLE);
 *   sleep_enable();
 *   sleep_mode();  // CPU dorme, interrupt la svegliano
 *   sleep_disable();
 * }
 * 
 * BONUS 3: Misura ISR Execution Time
 * Aggiungi pin debug che vai HIGH all'inizio ISR, LOW alla fine.
 * Misura con oscilloscopio:
 * 
 * void timerISR() {
 *   digitalWrite(DEBUG_PIN, HIGH);  // Start
 *   
 *   // ... codice ISR ...
 *   
 *   digitalWrite(DEBUG_PIN, LOW);   // End
 * }
 * 
 * Tempo esecuzione ISR tipico: _______ us
 * 
 * ============================================================================
 * NOTE IMPLEMENTATIVE
 * ============================================================================
 * 
 * VOLATILE KEYWORD:
 * 
 * Variabili modificate in ISR DEVONO essere 'volatile'.
 * Questo dice al compilatore: "Non ottimizzare accesso a questa variabile,
 * può cambiare in qualsiasi momento (da interrupt)".
 * 
 * Senza volatile, il compilatore potrebbe:
 * - Cachare valore in registro
 * - Riordinare istruzioni
 * - Ottimizzare via accessi ripetuti
 * 
 * Risultato: main loop legge valore STALE (non aggiornato)!
 * 
 * ATOMIC ACCESS:
 * 
 * Lettura/scrittura variabili multi-byte da ISR e main loop può causare
 * RACE CONDITION se interrotta a metà.
 * 
 * Esempio problema:
 * Main loop sta leggendo led1_toggles (32-bit):
 *   1. Legge byte 0
 *   2. [INTERRUPT! ISR modifica led1_toggles]
 *   3. Legge byte 1,2,3
 * Risultato: valore corrotto!
 * 
 * Soluzione: noInterrupts()/interrupts()
 * 
 * noInterrupts();
 * unsigned long copy = led1_toggles;  // Copia atomica
 * interrupts();
 * 
 * TIMER DISPONIBILI (Arduino Uno):
 * 
 * - Timer0: 8-bit, usato da millis()/delay() → NON TOCCARE!
 * - Timer1: 16-bit, libero → usato qui
 * - Timer2: 8-bit, libero
 * 
 * INTERRUPT PRIORITIES (ATmega328P):
 * 
 * Pin 2 (INT0): Priorità 1 (massima)
 * Pin 3 (INT1): Priorità 2
 * Timer1:       Priorità più bassa
 * 
 * Se due interrupt simultanei, INT0 esegue per primo.
 * 
 * ============================================================================
 */
