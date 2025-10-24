/**
 * ============================================================================
 * ES01 - Sistema Controllo LED con Pulsanti
 * SOLUZIONE - Approccio 4: RTOS (FreeRTOS)
 * ============================================================================
 * 
 * Implementazione completa con task FreeRTOS
 * 
 * NOTA: Questo codice è per ESP32 (FreeRTOS nativo).
 * Per Arduino Uno, usare Arduino_FreeRTOS.h
 */

#include <Arduino.h>

// Per ESP32 (FreeRTOS nativo)
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Per Arduino Uno:
// #include <Arduino_FreeRTOS.h>

// ========== PIN (ESP32) ==========
const int BTN1_PIN = 15;
const int BTN2_PIN = 4;
const int LED1_PIN = 2;
const int LED2_PIN = 16;

// Per Arduino Uno:
// const int BTN1_PIN = 2;
// const int BTN2_PIN = 3;
// const int LED1_PIN = 8;
// const int LED2_PIN = 9;

// ========== COSTANTI ==========
const TickType_t BLINK_PERIOD = pdMS_TO_TICKS(100);
const TickType_t POLL_PERIOD = pdMS_TO_TICKS(50);
const TickType_t STATS_PERIOD = pdMS_TO_TICKS(5000);

// ========== PRIORITÀ ==========
const UBaseType_t BTN1_PRIORITY = 2;
const UBaseType_t BTN2_PRIORITY = 2;
const UBaseType_t STATS_PRIORITY = 1;

// ========== STACK SIZE ==========
#define STACK_SIZE 2048  // ESP32: parole da 4 byte = 8KB

// Arduino Uno:
// #define STACK_SIZE 128  // parole da 2 byte = 256 bytes

// ========== HANDLE TASK ==========
TaskHandle_t taskBtn1Handle = NULL;
TaskHandle_t taskBtn2Handle = NULL;
TaskHandle_t taskStatsHandle = NULL;

// ========== STATISTICHE ==========
volatile unsigned long btn1_pressCount = 0;
volatile unsigned long btn2_pressCount = 0;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  
  Serial.println("\n=== ES01 - Approccio 4: FreeRTOS ===");
  Serial.println("Platform: ESP32 (520KB RAM, FreeRTOS nativo)");
  Serial.println("Task indipendenti con scheduler preemptive\n");
  
  // Crea task BTN1
  BaseType_t result1 = xTaskCreate(
    TaskButton1,
    "BTN1",
    STACK_SIZE,
    NULL,
    BTN1_PRIORITY,
    &taskBtn1Handle
  );
  
  if (result1 != pdPASS) {
    Serial.println("[ERRORE] Task BTN1 non creato!");
    while(1);
  }
  Serial.println("[OK] Task BTN1 creato");
  
  // Crea task BTN2
  BaseType_t result2 = xTaskCreate(
    TaskButton2,
    "BTN2",
    STACK_SIZE,
    NULL,
    BTN2_PRIORITY,
    &taskBtn2Handle
  );
  
  if (result2 != pdPASS) {
    Serial.println("[ERRORE] Task BTN2 non creato!");
    while(1);
  }
  Serial.println("[OK] Task BTN2 creato");
  
  // Crea task statistiche
  BaseType_t result3 = xTaskCreate(
    TaskStats,
    "Stats",
    STACK_SIZE,
    NULL,
    STATS_PRIORITY,
    &taskStatsHandle
  );
  
  if (result3 != pdPASS) {
    Serial.println("[ERRORE] Task Stats non creato!");
    while(1);
  }
  Serial.println("[OK] Task Stats creato");
  
  Serial.println("\nScheduler FreeRTOS attivo!\n");
  
  // Su ESP32, scheduler è già attivo
  // Su Arduino Uno, chiamare: vTaskStartScheduler();
}

// ========== TASK BUTTON 1 ==========
void TaskButton1(void *pvParameters) {
  (void) pvParameters;
  
  // Setup pin (ogni task configura i propri)
  pinMode(BTN1_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  
  bool led_state = LOW;
  
  Serial.println("[TaskBtn1] Avviato");
  
  // Loop infinito task
  for(;;) {
    if (digitalRead(BTN1_PIN) == HIGH) {
      // Pulsante premuto: lampeggia
      led_state = !led_state;
      digitalWrite(LED1_PIN, led_state);
      
      // Incrementa contatore (accesso atomico)
      taskENTER_CRITICAL();
      btn1_pressCount++;
      taskEXIT_CRITICAL();
      
      vTaskDelay(BLINK_PERIOD);  // Delay NON bloccante
    } else {
      // Pulsante rilasciato: spento
      digitalWrite(LED1_PIN, LOW);
      led_state = LOW;
      vTaskDelay(POLL_PERIOD);
    }
  }
}

// ========== TASK BUTTON 2 ==========
void TaskButton2(void *pvParameters) {
  (void) pvParameters;
  
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  bool led_state = LOW;
  
  Serial.println("[TaskBtn2] Avviato");
  
  for(;;) {
    if (digitalRead(BTN2_PIN) == HIGH) {
      led_state = !led_state;
      digitalWrite(LED2_PIN, led_state);
      
      taskENTER_CRITICAL();
      btn2_pressCount++;
      taskEXIT_CRITICAL();
      
      vTaskDelay(BLINK_PERIOD);
    } else {
      digitalWrite(LED2_PIN, LOW);
      led_state = LOW;
      vTaskDelay(POLL_PERIOD);
    }
  }
}

// ========== TASK STATISTICHE ==========
void TaskStats(void *pvParameters) {
  (void) pvParameters;
  
  Serial.println("[TaskStats] Avviato");
  
  for(;;) {
    vTaskDelay(STATS_PERIOD);  // Ogni 5 secondi
    
    Serial.println("\n========== STATISTICHE ==========");
    
    // Numero task attivi
    Serial.print("Task attivi: ");
    Serial.println(uxTaskGetNumberOfTasks());
    
    // Stack libero per task
    Serial.print("Stack BTN1 libero: ");
    Serial.print(uxTaskGetStackHighWaterMark(taskBtn1Handle));
    Serial.println(" parole");
    
    Serial.print("Stack BTN2 libero: ");
    Serial.print(uxTaskGetStackHighWaterMark(taskBtn2Handle));
    Serial.println(" parole");
    
    Serial.print("Stack Stats libero: ");
    Serial.print(uxTaskGetStackHighWaterMark(taskStatsHandle));
    Serial.println(" parole");
    
    // Heap libero (ESP32)
    Serial.print("Free Heap: ");
    Serial.print(xPortGetFreeHeapSize());
    Serial.println(" bytes");
    
    // Contatori pulsanti (accesso atomico)
    taskENTER_CRITICAL();
    unsigned long count1 = btn1_pressCount;
    unsigned long count2 = btn2_pressCount;
    taskEXIT_CRITICAL();
    
    Serial.print("BTN1 premuto: ");
    Serial.print(count1);
    Serial.println(" volte");
    Serial.print("BTN2 premuto: ");
    Serial.print(count2);
    Serial.println(" volte");
    
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" secondi");
    Serial.println("=================================\n");
  }
}

// ========== LOOP ==========
void loop() {
  // Su ESP32: loop() è un task normale (priorità 1)
  // Esegue in background
  
  // Su Arduino Uno: loop() NON viene chiamato
  // (scheduler ha controllo totale)
  
  vTaskDelay(pdMS_TO_TICKS(1000));
}

/**
 * RISPOSTE DOMANDE VERIFICA:
 * 
 * D1: Cosa fa lo scheduler FreeRTOS?
 * R1: Gestisce context switching tra task. Decide quale task eseguire
 *     in base a:
 *     - Priorità (task alta priorità eseguito per primo)
 *     - Stato (READY vs BLOCKED)
 *     - Time slicing (se priorità uguali, round-robin)
 * 
 * D2: Differenza tra vTaskDelay() e delay()?
 * R2: delay(100):
 *     - Busy-wait, CPU occupata
 *     - Blocca tutto il programma
 *     - Altri task NON possono eseguire
 *     
 *     vTaskDelay(pdMS_TO_TICKS(100)):
 *     - Task entra in stato BLOCKED
 *     - Scheduler passa ad altro task READY
 *     - CPU libera per altri task o IDLE (sleep)
 * 
 * D3: Cosa succede se stack troppo piccolo?
 * R3: STACK OVERFLOW → crash!
 *     Sintomi:
 *     - Reset improvvisi
 *     - Comportamento imprevedibile
 *     - Corruzione variabili
 *     
 *     Prevenzione:
 *     - Monitora uxTaskGetStackHighWaterMark()
 *     - Margine sicurezza: almeno 20% stack libero
 *     - Evita variabili locali grandi (usa heap)
 * 
 * D4: Perché taskENTER_CRITICAL() / taskEXIT_CRITICAL()?
 * R4: Crea sezione critica dove:
 *     - Scheduler disabilitato (no context switch)
 *     - Interrupt disabilitati (su alcuni port)
 *     
 *     Serve per accesso ATOMICO a variabili condivise.
 *     Esempio: btn1_pressCount++ NON è atomico (3 operazioni assembly):
 *     1. LOAD btn1_pressCount → registro
 *     2. INC registro
 *     3. STORE registro → btn1_pressCount
 *     
 *     Se context switch tra step 1 e 3 → race condition!
 * 
 * D5: Cosa significa "preemptive scheduling"?
 * R5: Scheduler può INTERROMPERE task in esecuzione per passare
 *     a task con priorità maggiore.
 *     
 *     Esempio:
 *     - TaskBtn1 (priorità 2) è RUNNING
 *     - TaskBtn2 (priorità 3) diventa READY
 *     - Scheduler PREEMPT TaskBtn1 (→ READY)
 *     - TaskBtn2 diventa RUNNING
 *     
 *     Alternativa: Cooperative scheduling (task deve cedere volontariamente)
 *     FreeRTOS usa preemptive (migliore reattività).
 */
