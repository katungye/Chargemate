#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Enter your network details
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress broker(192, 168, 0, 10);  // IP address of your MQTT broker
EthernetClient ethClient;
PubSubClient client(ethClient);

const int ACS712_PIN = A0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize Ethernet and MQTT
  Ethernet.begin(mac);
  delay(1500); // Allow time for Ethernet to initialize
  client.setServer(broker, 1883);

  // Connect to the MQTT broker
  connectToMQTTBroker();
}

void loop() {
  // Read the value from ACS712 sensor
  int sensorValue = analogRead(ACS712_PIN);

  // Convert the analog value to current
  float current = map(sensorValue, 0, 1023, -5, 5);

  // Publish the current value to the MQTT topic
  char message[10];
  snprintf(message, sizeof(message), "%.2f", current);
  client.publish("sensor/current", message);

  // Print the current value to the serial monitor
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  // Wait for a few seconds before publishing the next value
  delay(5000);
}

void connectToMQTTBroker() {
  // Loop until connected to the MQTT broker
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    
    // Attempt to connect
    if (client.connect("ArduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      
      // Wait before retrying
      delay(5000);
    }
  }
}
