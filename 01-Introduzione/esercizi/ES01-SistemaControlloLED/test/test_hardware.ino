/**
 * Test Hardware - Verifica Collegamenti
 * 
 * Questo sketch testa LED e pulsanti per verificare collegamenti corretti.
 * Carica questo PRIMA di iniziare l'esercitazione!
 */

const int BTN1_PIN = 2;
const int BTN2_PIN = 3;
const int LED1_PIN = 8;
const int LED2_PIN = 9;

void setup() {
  Serial.begin(9600);
  Serial.println("=== TEST HARDWARE ES01 ===\n");
  
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  Serial.println("Test LED...");
  
  // Test LED1
  digitalWrite(LED1_PIN, HIGH);
  Serial.println("LED1 (rosso) dovrebbe essere ACCESO");
  delay(2000);
  digitalWrite(LED1_PIN, LOW);
  Serial.println("LED1 spento\n");
  delay(500);
  
  // Test LED2
  digitalWrite(LED2_PIN, HIGH);
  Serial.println("LED2 (verde) dovrebbe essere ACCESO");
  delay(2000);
  digitalWrite(LED2_PIN, LOW);
  Serial.println("LED2 spento\n");
  delay(500);
  
  Serial.println("Test LED completato!");
  Serial.println("Se LED non si accendono, verifica:");
  Serial.println("- Polarità LED (gamba lunga = anodo)");
  Serial.println("- Resistenze 220 ohm in serie");
  Serial.println("- Collegamenti pin 8 e 9\n");
  
  Serial.println("Test Pulsanti...");
  Serial.println("Premi BTN1 e BTN2 per testarli");
  Serial.println("(Valori dovrebbero passare da 0 a 1)\n");
}

void loop() {
  int btn1 = digitalRead(BTN1_PIN);
  int btn2 = digitalRead(BTN2_PIN);
  
  Serial.print("BTN1: ");
  Serial.print(btn1);
  Serial.print(" | BTN2: ");
  Serial.println(btn2);
  
  // LED seguono pulsanti (test visivo)
  digitalWrite(LED1_PIN, btn1);
  digitalWrite(LED2_PIN, btn2);
  
  delay(200);
}

/**
 * DIAGNOSTICA:
 * 
 * PROBLEMA: LED non si accendono
 * - Verifica pin corretti (8 e 9)
 * - Controlla polarità LED
 * - Misura tensione LED con multimetro (dovrebbe essere ~2V rosso, ~3V verde)
 * 
 * PROBLEMA: Pulsanti sempre 0 o sempre 1
 * - Verifica resistenze pull-down 10kΩ
 * - Controlla collegamenti pulsanti
 * - Prova invertire logica (usa INPUT_PULLUP e pulsante a GND)
 * 
 * PROBLEMA: Valori pulsanti instabili (0-1-0-1...)
 * - Bounce meccanico pulsante
 * - Aggiungi condensatore 0.1µF tra pin e GND
 */
