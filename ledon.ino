const int ACS712_PIN = A0;   // ACS712 analog input pin


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Set LED pin as output
  Serial.begin(9600);        // Initialize serial communication
}

void loop() {
  int sensorValue = analogRead(ACS712_PIN);  // Read analog value from ACS712
  Serial.println(sensorValue);               // Print sensor value (for debugging)

  if (sensorValue > 512) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);   // Turn on the LED if the sensor value is above 512
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // Turn off the LED otherwise
  }

  delay(2000);  // Delay for stability (adjust as needed)
}
