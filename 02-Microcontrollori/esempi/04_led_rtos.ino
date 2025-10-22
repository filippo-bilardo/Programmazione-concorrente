/**
 * Esempio 4: Lampeggio 3 LED - RTOS (Real-Time Operating System)
 * 
 * Questo esempio usa FreeRTOS per creare 3 TASK indipendenti.
 * Ogni LED è gestito da un task separato con priorità configurabile.
 * Lo scheduler RTOS gestisce automaticamente il context switching.
 * 
 * Richiede libreria: FreeRTOS (solo per Arduino AVR)
 * Per ESP32/ESP8266: FreeRTOS è già integrato!
 * Per Arduino Uno/Mega: Installare "FreeRTOS" by Richard Barry
 * 
 * NOTA: FreeRTOS su Arduino Uno ha limitazioni (RAM ridotta).
 * Per progetti seri, preferire ESP32 che ha FreeRTOS nativo.
 * 
 * Hardware:
 * - LED1 connesso al pin 8 (lampeggia ogni 1000ms) - Priorità 1
 * - LED2 connesso al pin 9 (lampeggia ogni 500ms)  - Priorità 2
 * - LED3 connesso al pin 10 (lampeggia ogni 200ms) - Priorità 3
 * - Resistenze da 220Ω in serie ad ogni LED
 */

// ========== CONFIGURAZIONE PLATFORM ==========
// Decommentare la piattaforma corretta:

// #define PLATFORM_AVR     // Arduino Uno/Mega/Nano (ATmega328P/2560)
#define PLATFORM_ESP32   // ESP32 (FreeRTOS nativo)
// #define PLATFORM_ESP8266 // ESP8266 (FreeRTOS nativo)

// ========== INCLUDE RTOS ==========
#if defined(PLATFORM_AVR)
  #include <Arduino_FreeRTOS.h>  // Libreria per AVR
#elif defined(PLATFORM_ESP32) || defined(PLATFORM_ESP8266)
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>
  // ESP32/ESP8266 hanno FreeRTOS integrato
#else
  #error "Platform non supportata! Definire PLATFORM_AVR, PLATFORM_ESP32 o PLATFORM_ESP8266"
#endif

// ========== CONFIGURAZIONE PIN ==========
const int LED1_PIN = 8;   // LED Rosso
const int LED2_PIN = 9;   // LED Verde
const int LED3_PIN = 10;  // LED Blu

// ========== CONFIGURAZIONE TASK ==========
// Intervalli di lampeggio (millisecondi)
const TickType_t LED1_INTERVAL = pdMS_TO_TICKS(1000);  // 1000ms
const TickType_t LED2_INTERVAL = pdMS_TO_TICKS(500);   // 500ms
const TickType_t LED3_INTERVAL = pdMS_TO_TICKS(200);   // 200ms

// Priorità task (1 = bassa, 3 = alta)
// NOTA: In FreeRTOS, priorità più alta = numero maggiore
const UBaseType_t LED1_PRIORITY = 1;
const UBaseType_t LED2_PRIORITY = 2;
const UBaseType_t LED3_PRIORITY = 3;

// Stack size per ogni task (parole, non byte!)
// ESP32: 2048-4096 parole (8-16KB)
// AVR: 128-256 parole (256-512 byte) - RAM limitata!
#if defined(PLATFORM_ESP32)
  #define STACK_SIZE 2048
#elif defined(PLATFORM_AVR)
  #define STACK_SIZE 128
#else
  #define STACK_SIZE 1024
#endif

// ========== STATISTICHE ==========
// Contatori protetti (accesso atomico non necessario su AVR 8-bit)
volatile unsigned long led1_toggles = 0;
volatile unsigned long led2_toggles = 0;
volatile unsigned long led3_toggles = 0;

// ========== HANDLE TASK ==========
TaskHandle_t taskLed1Handle = NULL;
TaskHandle_t taskLed2Handle = NULL;
TaskHandle_t taskLed3Handle = NULL;
TaskHandle_t taskStatsHandle = NULL;

// ========== SETUP ==========
void setup() {
  // Inizializza i pin come output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
  // Inizializza comunicazione seriale
  Serial.begin(115200);
  while (!Serial) { delay(10); }  // Attendi apertura seriale (ESP32)
  
  Serial.println("\n\n=== Esempio 4: LED con FreeRTOS ===");
  Serial.println("Ogni LED è gestito da un TASK indipendente!");
  Serial.println("Lo scheduler RTOS fa context switching automatico.\n");
  
  #if defined(PLATFORM_AVR)
    Serial.println("Platform: Arduino AVR (limited RAM)");
  #elif defined(PLATFORM_ESP32)
    Serial.println("Platform: ESP32 (native FreeRTOS)");
  #elif defined(PLATFORM_ESP8266)
    Serial.println("Platform: ESP8266 (native FreeRTOS)");
  #endif
  
  Serial.print("FreeRTOS Tick Rate: ");
  Serial.print(configTICK_RATE_HZ);
  Serial.println(" Hz\n");
  
  // ========== CREAZIONE TASK ==========
  BaseType_t result;
  
  // Task LED1 (priorità bassa)
  result = xTaskCreate(
    taskLed1,           // Funzione task
    "LED1",             // Nome task (debug)
    STACK_SIZE,         // Stack size
    NULL,               // Parametri (nessuno)
    LED1_PRIORITY,      // Priorità
    &taskLed1Handle     // Handle (per controllo task)
  );
  if (result != pdPASS) {
    Serial.println("ERRORE: Impossibile creare taskLed1!");
    while(1);  // Blocca esecuzione
  }
  Serial.println("[OK] Task LED1 creato (Priorità 1)");
  
  // Task LED2 (priorità media)
  result = xTaskCreate(
    taskLed2,
    "LED2",
    STACK_SIZE,
    NULL,
    LED2_PRIORITY,
    &taskLed2Handle
  );
  if (result != pdPASS) {
    Serial.println("ERRORE: Impossibile creare taskLed2!");
    while(1);
  }
  Serial.println("[OK] Task LED2 creato (Priorità 2)");
  
  // Task LED3 (priorità alta)
  result = xTaskCreate(
    taskLed3,
    "LED3",
    STACK_SIZE,
    NULL,
    LED3_PRIORITY,
    &taskLed3Handle
  );
  if (result != pdPASS) {
    Serial.println("ERRORE: Impossibile creare taskLed3!");
    while(1);
  }
  Serial.println("[OK] Task LED3 creato (Priorità 3)");
  
  // Task Statistiche (priorità minima)
  result = xTaskCreate(
    taskStats,
    "Stats",
    STACK_SIZE * 2,  // Stack maggiore per Serial.print
    NULL,
    tskIDLE_PRIORITY + 1,  // Appena sopra IDLE
    &taskStatsHandle
  );
  if (result != pdPASS) {
    Serial.println("ERRORE: Impossibile creare taskStats!");
    while(1);
  }
  Serial.println("[OK] Task Stats creato (Priorità IDLE+1)\n");
  
  Serial.println("Avvio scheduler FreeRTOS...\n");
  
  // ========== AVVIO SCHEDULER ==========
  // NOTA: Su AVR, vTaskStartScheduler() non ritorna mai!
  // Su ESP32, lo scheduler è già avviato, setup() ritorna normalmente
  #if defined(PLATFORM_AVR)
    vTaskStartScheduler();  // Non ritorna mai!
    
    // Se arriviamo qui, errore critico (RAM insufficiente)
    Serial.println("ERRORE FATALE: Scheduler non avviato!");
    Serial.println("RAM insufficiente per FreeRTOS!");
    while(1);
  #else
    Serial.println("Scheduler già attivo (ESP32/ESP8266)");
  #endif
}

// ========== LOOP ==========
// Su AVR: loop() non viene MAI chiamato (scheduler prende controllo)
// Su ESP32: loop() gira come task separato (priorità 1)
void loop() {
  #if defined(PLATFORM_ESP32) || defined(PLATFORM_ESP8266)
    // Su ESP32, loop() è un task normale
    vTaskDelay(pdMS_TO_TICKS(1000));  // Evita busy-wait
  #else
    // Su AVR, non dovremmo mai arrivare qui
    Serial.println("ERRORE: loop() chiamato su AVR!");
    while(1);
  #endif
}

// ========== TASK LED1 ==========
void taskLed1(void *pvParameters) {
  (void) pvParameters;  // Parametro non usato
  
  Serial.println("[LED1] Task avviato");
  
  // Stato LED
  bool ledState = false;
  
  // Loop infinito del task
  for(;;) {
    // Toggle LED
    ledState = !ledState;
    digitalWrite(LED1_PIN, ledState);
    led1_toggles++;
    
    // Debug (commentare per performance)
    // Serial.print("[LED1] ");
    // Serial.println(ledState ? "ON" : "OFF");
    
    // SLEEP TASK per l'intervallo specificato
    // vTaskDelay() mette il task in stato BLOCKED
    // Lo scheduler passa a task con priorità minore
    vTaskDelay(LED1_INTERVAL);
  }
  
  // NOTA: I task NON devono MAI uscire dal loop!
  // Se serve terminare, usare vTaskDelete(NULL)
}

// ========== TASK LED2 ==========
void taskLed2(void *pvParameters) {
  (void) pvParameters;
  
  Serial.println("[LED2] Task avviato");
  
  bool ledState = false;
  
  for(;;) {
    ledState = !ledState;
    digitalWrite(LED2_PIN, ledState);
    led2_toggles++;
    
    vTaskDelay(LED2_INTERVAL);
  }
}

// ========== TASK LED3 ==========
void taskLed3(void *pvParameters) {
  (void) pvParameters;
  
  Serial.println("[LED3] Task avviato");
  
  bool ledState = false;
  
  for(;;) {
    ledState = !ledState;
    digitalWrite(LED3_PIN, ledState);
    led3_toggles++;
    
    vTaskDelay(LED3_INTERVAL);
  }
}

// ========== TASK STATISTICHE ==========
void taskStats(void *pvParameters) {
  (void) pvParameters;
  
  Serial.println("[STATS] Task avviato\n");
  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t statsInterval = pdMS_TO_TICKS(10000);  // 10 secondi
  
  for(;;) {
    // Attendi 10 secondi (delay assoluto, non relativo)
    vTaskDelayUntil(&lastWakeTime, statsInterval);
    
    // Stampa statistiche
    Serial.println("\n========== STATISTICHE (10s) ==========");
    
    // Contatori LED
    Serial.print("LED1 toggle: ");
    Serial.println(led1_toggles);
    Serial.print("LED2 toggle: ");
    Serial.println(led2_toggles);
    Serial.print("LED3 toggle: ");
    Serial.println(led3_toggles);
    
    // Informazioni Runtime (se disponibile)
    #if (configUSE_TRACE_FACILITY == 1) && (configUSE_STATS_FORMATTING_FUNCTIONS == 1)
      char statsBuffer[512];
      vTaskGetRunTimeStats(statsBuffer);
      Serial.println("\n--- Runtime Stats ---");
      Serial.println(statsBuffer);
    #endif
    
    // Memoria libera heap
    #if defined(PLATFORM_ESP32)
      Serial.print("Free Heap: ");
      Serial.print(esp_get_free_heap_size());
      Serial.println(" bytes");
      Serial.print("Min Free Heap: ");
      Serial.print(esp_get_minimum_free_heap_size());
      Serial.println(" bytes");
    #elif defined(PLATFORM_AVR)
      Serial.print("Free RAM: ");
      Serial.print(freeMemory());
      Serial.println(" bytes (approssimato)");
    #endif
    
    // Stack usage per ogni task (se disponibile)
    #if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
      Serial.println("\n--- Stack High Water Mark (parole libere) ---");
      Serial.print("LED1: ");
      Serial.println(uxTaskGetStackHighWaterMark(taskLed1Handle));
      Serial.print("LED2: ");
      Serial.println(uxTaskGetStackHighWaterMark(taskLed2Handle));
      Serial.print("LED3: ");
      Serial.println(uxTaskGetStackHighWaterMark(taskLed3Handle));
      Serial.print("Stats: ");
      Serial.println(uxTaskGetStackHighWaterMark(taskStatsHandle));
    #endif
    
    // Numero di task attivi
    Serial.print("\nTask attivi: ");
    Serial.println(uxTaskGetNumberOfTasks());
    
    Serial.println("=======================================\n");
  }
}

// ========== UTILITY: FREE MEMORY (AVR) ==========
#if defined(PLATFORM_AVR)
int freeMemory() {
  // Calcola RAM libera approssimativa (AVR)
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

/**
 * ========================================================================
 * APPROFONDIMENTO: FreeRTOS
 * ========================================================================
 * 
 * FreeRTOS è un RTOS (Real-Time Operating System) open-source per
 * microcontrollori. Fornisce:
 * 
 * 1. SCHEDULER: Gestisce esecuzione task con priorità
 * 2. TASK: Thread leggeri con stack privato
 * 3. SINCRONIZZAZIONE: Semafori, mutex, code
 * 4. TIMER SOFTWARE: Callback periodici
 * 5. MEMORY MANAGEMENT: Heap dinamico
 * 
 * ========================================================================
 * CONCETTI FONDAMENTALI
 * ========================================================================
 * 
 * TASK:
 * - Funzione void che gira in loop infinito
 * - Ha priorità (0 = minima, N = massima)
 * - Ha stack privato (dimensione configurabile)
 * - Stati: RUNNING, READY, BLOCKED, SUSPENDED, DELETED
 * 
 * SCHEDULER:
 * - Preemptive: task alta priorità interrompe task bassa priorità
 * - Round-robin: task stessa priorità condividono tempo CPU
 * - Context switch: salva/ripristina stato task (registri, stack, PC)
 * 
 * TICK:
 * - Timer periodico (default 1000Hz = 1ms)
 * - Base temporale per scheduler
 * - Configurable in FreeRTOSConfig.h (configTICK_RATE_HZ)
 * 
 * ========================================================================
 * STATI TASK
 * ========================================================================
 * 
 *     ┌─────────┐
 *     │ CREATED │ (appena creato)
 *     └────┬────┘
 *          │ xTaskCreate()
 *          ▼
 *     ┌─────────┐◄───────────────┐
 *     │  READY  │                │
 *     └────┬────┘                │
 *          │                     │
 *          │ Scheduler switch    │ Task diventa ready
 *          ▼                     │
 *     ┌─────────┐                │
 *     │ RUNNING │ (esegue)       │
 *     └────┬────┘                │
 *          │                     │
 *          │ vTaskDelay()        │
 *          │ vTaskDelayUntil()   │
 *          │ Attesa risorsa      │
 *          ▼                     │
 *     ┌─────────┐                │
 *     │ BLOCKED │────────────────┘
 *     └─────────┘ (dorme)
 * 
 * ========================================================================
 * SCHEDULING PREEMPTIVE
 * ========================================================================
 * 
 * Esempio timeline con 3 task (priorità LED3 > LED2 > LED1):
 * 
 * Time:    0ms    200ms   400ms   500ms   600ms   800ms   1000ms
 *          │       │       │       │       │       │       │
 * LED3: ───██──────██──────██──────────────██──────██──────██───
 * LED2: ───────────────────────────████────────────────────────
 * LED1: ────────────────────────────────────────────────────████
 * IDLE: ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
 * 
 * ██ = Task RUNNING
 * ▒▒ = IDLE task (CPU libera)
 * 
 * LED3 (priorità 3) esegue per primo ogni 200ms
 * LED2 (priorità 2) esegue quando LED3 dorme
 * LED1 (priorità 1) esegue solo quando LED2 e LED3 dormono
 * IDLE esegue quando tutti dormono (può entrare in sleep mode)
 * 
 * ========================================================================
 * VANTAGGI RTOS
 * ========================================================================
 * 
 * 1. ASTRAZIONE: Codice come su sistema operativo completo
 * 2. MODULARITÀ: Ogni funzionalità è un task separato
 * 3. PRIORITÀ: Task critici hanno garanzie timing
 * 4. SINCRONIZZAZIONE: Primitive robuste (semafori, mutex, code)
 * 5. DEBUGGING: Tool per analisi runtime, stack usage, deadlock
 * 6. PORTABILITÀ: Codice trasferibile tra piattaforme
 * 7. SCALABILITÀ: Facile aggiungere task senza riscrivere tutto
 * 
 * ========================================================================
 * SVANTAGGI RTOS
 * ========================================================================
 * 
 * 1. OVERHEAD: Context switch richiede tempo/memoria
 * 2. RAM: Ogni task ha stack privato (256-4096 byte)
 * 3. COMPLESSITÀ: Curva apprendimento ripida
 * 4. DEBUGGING: Race condition, deadlock difficili da trovare
 * 5. OVERKILL: Per progetti semplici è eccessivo
 * 6. DETERMINISMO: Timing meno preciso di interrupt dedicato
 * 
 * ========================================================================
 * QUANDO USARE RTOS
 * ========================================================================
 * 
 * USA RTOS quando:
 * ✅ Molti task concorrenti (>5)
 * ✅ Task con priorità diverse
 * ✅ Comunicazione complessa tra task
 * ✅ Codice deve essere modulare/manutenibile
 * ✅ Piattaforma ha RAM sufficiente (>8KB)
 * ✅ Portabilità è importante
 * 
 * NON usare RTOS quando:
 * ❌ Progetto semplicissimo (<3 task)
 * ❌ RAM limitata (<4KB)
 * ❌ Timing critico (< 1ms precision)
 * ❌ Basso consumo è prioritario
 * ❌ Overhead non è accettabile
 * 
 * ========================================================================
 * CONFRONTO APPROCCI
 * ========================================================================
 * 
 * | Aspetto       | Polling | Interrupt | RTOS        |
 * |---------------|---------|-----------|-------------|
 * | Astrazione    | Bassa   | Media     | Alta        |
 * | Modularità    | Media   | Bassa     | Ottima      |
 * | Overhead      | Basso   | Basso     | Medio-Alto  |
 * | RAM Usage     | Basso   | Basso     | Alto        |
 * | Precisione    | Media   | Alta      | Media       |
 * | Scalabilità   | Media   | Bassa     | Ottima      |
 * | Learning      | Facile  | Medio     | Difficile   |
 * | Portabilità   | Alta    | Bassa     | Ottima      |
 * 
 * ========================================================================
 * CONFIGURAZIONE FreeRTOS (FreeRTOSConfig.h)
 * ========================================================================
 * 
 * Parametri importanti:
 * 
 * - configTICK_RATE_HZ: Frequenza tick (default 1000 = 1ms)
 * - configMAX_PRIORITIES: Livelli priorità (default 5)
 * - configMINIMAL_STACK_SIZE: Stack minimo task
 * - configTOTAL_HEAP_SIZE: RAM per heap FreeRTOS
 * - configUSE_PREEMPTION: Abilita preemption (1 = SI)
 * - configUSE_TIME_SLICING: Round-robin stessa priorità
 * - configUSE_IDLE_HOOK: Callback in IDLE task
 * - configUSE_TICK_HOOK: Callback ogni tick
 * 
 * ========================================================================
 * API FREERTOS ESSENZIALI
 * ========================================================================
 * 
 * GESTIONE TASK:
 * - xTaskCreate(): Crea task
 * - vTaskDelete(): Elimina task
 * - vTaskSuspend(): Sospende task
 * - vTaskResume(): Riprende task
 * - vTaskPrioritySet(): Cambia priorità
 * 
 * DELAY:
 * - vTaskDelay(): Delay relativo (da adesso)
 * - vTaskDelayUntil(): Delay assoluto (timing preciso)
 * 
 * SINCRONIZZAZIONE:
 * - xSemaphoreCreateBinary(): Semaforo binario
 * - xSemaphoreCreateCounting(): Semaforo contatore
 * - xSemaphoreCreateMutex(): Mutex
 * - xQueueCreate(): Coda FIFO
 * 
 * INFO:
 * - xTaskGetTickCount(): Tick corrente
 * - uxTaskGetStackHighWaterMark(): Stack libero
 * - vTaskGetRunTimeStats(): Statistiche CPU
 * 
 * ========================================================================
 * BEST PRACTICES
 * ========================================================================
 * 
 * 1. STACK SIZE: Calcolare stack necessario con uxTaskGetStackHighWaterMark()
 * 2. PRIORITÀ: Non abusare priorità alte (starving task bassi)
 * 3. ISR: Da ISR usare *FromISR() API (xSemaphoreGiveFromISR, etc.)
 * 4. CRITICAL SECTION: Minimizzare taskENTER_CRITICAL() / taskEXIT_CRITICAL()
 * 5. DELAY: Preferire vTaskDelayUntil() per timing periodico
 * 6. IDLE HOOK: Usare per sleep mode (risparmio energia)
 * 7. WATCHDOG: Implementare watchdog per task bloccati
 * 8. HEAP: Monitorare heap libero per evitare frammentazione
 * 
 * ========================================================================
 * DEBUGGING
 * ========================================================================
 * 
 * Tool consigliati:
 * - FreeRTOS+Trace: Visualizzazione timeline task
 * - Stack overflow hook: Rileva overflow stack
 * - Heap usage: Monitora allocazioni dinamiche
 * - Task statistics: CPU usage per task
 * 
 * ========================================================================
 * RISORSE
 * ========================================================================
 * 
 * - FreeRTOS.org: Documentazione ufficiale
 * - "Mastering the FreeRTOS Real Time Kernel" (libro gratuito)
 * - ESP-IDF Programming Guide (per ESP32)
 * - Arduino_FreeRTOS GitHub (per AVR)
 * 
 * ========================================================================
 */
