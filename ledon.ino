const int ACS712_PIN = A0;   // ACS712 analog input pin
const int LED_PIN = 2;       // LED output pin

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  Serial.begin(9600);        // Initialize serial communication
}

void loop() {
  int sensorValue = analogRead(ACS712_PIN);  // Read analog value from ACS712
  Serial.println(sensorValue);               // Print sensor value (for debugging)

  if (sensorValue > 512) {
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED if the sensor value is above 512
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off the LED otherwise
  }

  delay(100);  // Delay for stability (adjust as needed)
}
