/**
 * Esempio 4: Lampeggio 3 LED - RTOS (Real-Time Operating System)
 * https://wokwi.com/projects/445620115220373505
 * 
 * Questo esempio usa FreeRTOS per creare 3 TASK indipendenti.
 * Ogni LED è gestito da un task separato con priorità configurabile.
 * Lo scheduler RTOS gestisce automaticamente il context switching.
 *  
 * Hardware:
 * - LED1 connesso al pin 8 (lampeggia ogni 1000ms) - Priorità 1
 * - LED2 connesso al pin 9 (lampeggia ogni 500ms)  - Priorità 1
 * - LED3 connesso al pin 10 (lampeggia ogni 200ms) - Priorità 2
 * - Resistenze da 220Ω in serie ad ogni LED
 */
 #include <Arduino_FreeRTOS.h>

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  Serial.begin(9600);
  xTaskCreate(TaskBlink1, "Led1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "Led2 ", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink3, "Led3 ", 128, NULL, 2, NULL);
  xTaskCreate(Taskprint, "TaskPrint", 128, NULL, 1, NULL);
  vTaskStartScheduler();
}

void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while (1) {
    digitalWrite(8, HIGH);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
    digitalWrite(8, LOW);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(9, OUTPUT);
  while (1) {
    digitalWrite(9, HIGH);
    vTaskDelay( 500 / portTICK_PERIOD_MS );
    digitalWrite(9, LOW);
    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}

void TaskBlink3(void *pvParameters) {
  pinMode(10, OUTPUT);
  while (1) {
    digitalWrite(10, HIGH);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    digitalWrite(10, LOW);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while (1) {
    counter++;
    Serial.println(counter);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
