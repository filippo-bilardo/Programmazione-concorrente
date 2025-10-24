/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * Approccio 4: RTOS (FreeRTOS)
 * ============================================================================
 * 
 * OBIETTIVO:
 * Creare 3 task FreeRTOS indipendenti per gestire sistema in modo modulare.
 * 
 * SPECIFICHE:
 * - Task1: Gestione BTN1 e LED1
 * - Task2: Gestione BTN2 e LED2
 * - Task3: Statistiche sistema
 * - Ogni task ha priorità configurabile
 * - Scheduler gestisce context switching automatico
 * 
 * LIBRERIA RICHIESTA:
 * - FreeRTOS (Arduino Uno/Mega)
 * - ESP32: FreeRTOS già integrato!
 * 
 * PIATTAFORMA CONSIGLIATA:
 * - ESP32 (520KB RAM, FreeRTOS nativo)
 * - Arduino Uno ha solo 2KB RAM → limitazioni!
 * 
 * TEMPO STIMATO: 75 minuti
 * ============================================================================
 */

#include <Arduino_FreeRTOS.h>  // Per Arduino Uno/Mega
// Per ESP32, usare: #include <freertos/FreeRTOS.h>

// ========== DEFINIZIONE PIN ==========
const int BTN1_PIN = 2;
const int BTN2_PIN = 3;
const int LED1_PIN = 8;
const int LED2_PIN = 9;

// ========== COSTANTI ==========
const TickType_t BLINK_PERIOD = pdMS_TO_TICKS(100);  // 100ms
const TickType_t POLL_PERIOD = pdMS_TO_TICKS(50);    // 50ms polling

// ========== PRIORITÀ TASK ==========
const UBaseType_t BTN1_PRIORITY = 2;  // Alta priorità
const UBaseType_t BTN2_PRIORITY = 2;  // Alta priorità
const UBaseType_t STATS_PRIORITY = 1; // Bassa priorità

// ========== STACK SIZE ==========
#define STACK_SIZE 128  // Parole (non byte!). Arduino Uno: max 128-256

// ========== HANDLE TASK ==========
// TODO 1: Dichiara handle per i task
// SUGGERIMENTO:
// TaskHandle_t taskBtn1Handle = NULL;
// TaskHandle_t taskBtn2Handle = NULL;
// TaskHandle_t taskStatsHandle = NULL;

/* SCRIVI IL TUO CODICE QUI */




// ========== STATISTICHE CONDIVISE ==========
volatile unsigned long btn1_pressCount = 0;
volatile unsigned long btn2_pressCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }  // Attendi apertura seriale
  
  Serial.println("\n=== ES01 - Approccio 4: FreeRTOS ===");
  Serial.println("Task indipendenti con scheduler preemptive");
  Serial.println();
  
  // ========== CREAZIONE TASK ==========
  
  // TODO 2: Crea task per BTN1/LED1
  // SUGGERIMENTO:
  // BaseType_t result = xTaskCreate(
  //   TaskButton1,        // Funzione task
  //   "BTN1",             // Nome (debug)
  //   STACK_SIZE,         // Stack size
  //   NULL,               // Parametri
  //   BTN1_PRIORITY,      // Priorità
  //   &taskBtn1Handle     // Handle
  // );
  // if (result != pdPASS) {
  //   Serial.println("ERRORE: Task BTN1 non creato!");
  //   while(1);
  // }
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  Serial.println("[OK] Task BTN1 creato");
  
  // TODO 3: Crea task per BTN2/LED2 (analogo)
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  Serial.println("[OK] Task BTN2 creato");
  
  // TODO 4: Crea task statistiche
  
  /* SCRIVI IL TUO CODICE QUI */
  
  
  
  
  
  
  
  
  
  Serial.println("[OK] Task Stats creato");
  
  Serial.println("\nAvvio scheduler FreeRTOS...\n");
  
  // NOTA: Su Arduino Uno, vTaskStartScheduler() NON RITORNA MAI!
  // Su ESP32, scheduler è già attivo
}

// ========== TASK BUTTON 1 ==========
// TODO 5: Implementa task per gestione BTN1 e LED1
//
// STRUTTURA:
// void TaskButton1(void *pvParameters) {
//   (void) pvParameters;  // Non usato
//   
//   // Setup pin (ogni task configura i propri pin)
//   pinMode(BTN1_PIN, INPUT);
//   pinMode(LED1_PIN, OUTPUT);
//   
//   bool led_state = LOW;
//   
//   // Loop infinito task
//   for(;;) {
//     if (digitalRead(BTN1_PIN) == HIGH) {
//       // Pulsante premuto: lampeggia
//       led_state = !led_state;
//       digitalWrite(LED1_PIN, led_state);
//       
//       // Incrementa contatore (accesso atomico!)
//       taskENTER_CRITICAL();
//       btn1_pressCount++;
//       taskEXIT_CRITICAL();
//       
//       vTaskDelay(BLINK_PERIOD);  // Delay NON bloccante!
//     } else {
//       // Pulsante rilasciato: spento
//       digitalWrite(LED1_PIN, LOW);
//       vTaskDelay(POLL_PERIOD);   // Polling più veloce
//     }
//   }
// }

/* SCRIVI IL TUO CODICE QUI */
void TaskButton1(void *pvParameters) {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}

// ========== TASK BUTTON 2 ==========
// TODO 6: Implementa task per BTN2/LED2 (analogo a TaskButton1)

/* SCRIVI IL TUO CODICE QUI */
void TaskButton2(void *pvParameters) {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}

// ========== TASK STATISTICHE ==========
// TODO 7: Implementa task statistiche (stampa ogni 5 secondi)
//
// COSA STAMPARE:
// - Numero task attivi: uxTaskGetNumberOfTasks()
// - Stack libero per ogni task: uxTaskGetStackHighWaterMark(handle)
// - Heap libero (ESP32): xPortGetFreeHeapSize()
// - Contatori premute pulsanti

/* SCRIVI IL TUO CODICE QUI */
void TaskStats(void *pvParameters) {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}

void loop() {
  // Su Arduino Uno: loop() NON viene chiamato (scheduler ha controllo)
  // Su ESP32: loop() è un task normale, esegue in background
  
  vTaskDelay(pdMS_TO_TICKS(1000));  // Evita busy-wait
}

/**
 * ============================================================================
 * DOMANDE DI VERIFICA
 * ============================================================================
 * 
 * D1: Cosa fa lo scheduler FreeRTOS?
 * R1: ___________________________________________________________________
 * 
 * D2: Differenza tra vTaskDelay() e delay()?
 * R2: ___________________________________________________________________
 * 
 * D3: Cosa succede se stack di un task è troppo piccolo?
 * R3: ___________________________________________________________________
 * 
 * D4: Perché serve taskENTER_CRITICAL() / taskEXIT_CRITICAL()?
 * R4: ___________________________________________________________________
 * 
 * D5: Cosa significa "preemptive scheduling"?
 * R5: ___________________________________________________________________
 * 
 * ============================================================================
 * TEST
 * ============================================================================
 * 
 * Test 1: Verifica task indipendenti
 * Procedura: Premi BTN1 e BTN2 contemporaneamente
 * Risultato atteso: Entrambi LED lampeggiano indipendentemente
 * Risultato ottenuto: ___________________________________________________
 * 
 * Test 2: Monitor stack usage
 * Procedura: Leggi uxTaskGetStackHighWaterMark() dalle statistiche
 * Stack BTN1: _______ parole libere (minimo durante test)
 * Stack BTN2: _______ parole libere
 * Stack Stats: _______ parole libere
 * Stack OK se > 10 parole libere
 * 
 * Test 3: Heap memory (ESP32)
 * Free Heap iniziale: _______ bytes
 * Free Heap dopo 10 min: _______ bytes
 * Differenza: _______ bytes (dovrebbe essere ~0, no memory leak!)
 * 
 * ============================================================================
 * ESERCIZIO BONUS
 * ============================================================================
 * 
 * BONUS 1: Semaforo Binario
 * Quando BTN1 e BTN2 premuti CONTEMPORANEAMENTE, lampeggio sincronizzato:
 * 
 * SemaphoreHandle_t xSemaphore = NULL;
 * 
 * void setup() {
 *   xSemaphore = xSemaphoreCreateBinary();
 *   // ...
 * }
 * 
 * void TaskButton1(...) {
 *   if (btn1_pressed && btn2_pressed) {
 *     xSemaphoreGive(xSemaphore);  // Rilascia semaforo
 *   }
 * }
 * 
 * void TaskButton2(...) {
 *   if (xSemaphoreTake(xSemaphore, 0) == pdTRUE) {
 *     // Sincronizzato con TaskButton1!
 *   }
 * }
 * 
 * BONUS 2: Queue FIFO
 * TaskButton1 e TaskButton2 inviano eventi a TaskStats via queue:
 * 
 * QueueHandle_t eventQueue;
 * 
 * struct Event {
 *   int button;
 *   unsigned long timestamp;
 * };
 * 
 * void TaskButton1(...) {
 *   Event e = {1, millis()};
 *   xQueueSend(eventQueue, &e, 0);
 * }
 * 
 * void TaskStats(...) {
 *   Event e;
 *   if (xQueueReceive(eventQueue, &e, portMAX_DELAY) == pdTRUE) {
 *     Serial.print("BTN");
 *     Serial.print(e.button);
 *     Serial.print(" @ ");
 *     Serial.println(e.timestamp);
 *   }
 * }
 * 
 * BONUS 3: Priorità Dinamica
 * Cambia priorità task BTN1 quando premuto:
 * 
 * if (digitalRead(BTN1_PIN) == HIGH) {
 *   vTaskPrioritySet(NULL, 3);  // Aumenta priorità
 * } else {
 *   vTaskPrioritySet(NULL, 2);  // Ripristina
 * }
 * 
 * ============================================================================
 * NOTE IMPLEMENTATIVE
 * ============================================================================
 * 
 * TASK STATES (Stati Task):
 * 
 * RUNNING:    Task in esecuzione su CPU
 * READY:      Task pronto, in attesa di CPU
 * BLOCKED:    Task in attesa (vTaskDelay, semaforo, queue)
 * SUSPENDED:  Task sospeso manualmente
 * DELETED:    Task cancellato
 * 
 * Transizioni:
 * READY → RUNNING:   Scheduler assegna CPU
 * RUNNING → READY:   Task con priorità maggiore diventa ready
 * RUNNING → BLOCKED: vTaskDelay() o attesa risorsa
 * BLOCKED → READY:   Timeout o risorsa disponibile
 * 
 * SCHEDULER PREEMPTIVE:
 * 
 * Se task alta priorità diventa READY mentre task bassa priorità è RUNNING,
 * lo scheduler INTERROMPE (preempt) task basso e passa a task alto.
 * 
 * Esempio:
 * - TaskButton1 (priorità 2) è RUNNING
 * - TaskButton2 (priorità 2) è BLOCKED (in vTaskDelay)
 * - Timer scade, TaskButton2 diventa READY
 * - Scheduler fa context switch: TaskButton1→READY, TaskButton2→RUNNING
 * 
 * Se priorità uguali: Round-robin (time slicing)
 * 
 * STACK PER TASK:
 * 
 * Ogni task ha stack PRIVATO allocato alla creazione.
 * Stack contiene:
 * - Variabili locali
 * - Parametri funzione
 * - Indirizzi ritorno
 * - Registri CPU salvati (context switch)
 * 
 * STACK_SIZE in PAROLE (non byte!):
 * - Arduino Uno (AVR): 1 word = 2 bytes → 128 words = 256 bytes
 * - ESP32: 1 word = 4 bytes → 2048 words = 8192 bytes
 * 
 * Stack overflow → CRASH!
 * Usa uxTaskGetStackHighWaterMark() per monitorare.
 * 
 * CRITICAL SECTION:
 * 
 * taskENTER_CRITICAL() disabilita interrupt e scheduler.
 * Usare per proteggere accesso a variabili condivise.
 * 
 * ATTENZIONE: Mantieni critical section BREVE (<10us)!
 * Disabilitare scheduler troppo a lungo riduce reattività sistema.
 * 
 * Alternativa: Mutex (migliore per operazioni lunghe)
 * 
 * VTASKDELAY() vs DELAY():
 * 
 * delay(100):
 * - Busy-wait, CPU occupata
 * - Blocca task corrente
 * - Altri task NON possono eseguire
 * 
 * vTaskDelay(pdMS_TO_TICKS(100)):
 * - Task entra in stato BLOCKED
 * - Scheduler passa ad altro task
 * - CPU può essere usata da altri task o andare in IDLE
 * 
 * IDLE TASK:
 * 
 * FreeRTOS crea automaticamente task IDLE (priorità 0, minima).
 * Esegue quando nessun altro task è READY.
 * 
 * Idle task può:
 * - Mettere CPU in sleep mode (risparmio energia)
 * - Garbage collection (cancellare task terminati)
 * - Hook per codice utente (configUSE_IDLE_HOOK)
 * 
 * RAM USAGE (Arduino Uno):
 * 
 * FreeRTOS su Arduino Uno usa circa:
 * - Kernel: ~500 bytes
 * - Ogni task: 128-256 bytes (stack)
 * - Code/Semafori: ~20-50 bytes ciascuno
 * 
 * Totale: ~1.5-2KB → Poco spazio rimasto su Uno (2KB totali)!
 * 
 * Per Arduino Uno, limitare a 2-3 task piccoli.
 * Per progetti seri → ESP32 (520KB RAM!)
 * 
 * ============================================================================
 */
