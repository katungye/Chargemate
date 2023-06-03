#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Chargemate";
const char* password = "chargemate123";

// MQTT broker details
const char* mqttServer = "91.121.93.94";
const int mqttPort = 1883;
const char* mqttTopic = "voltage";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int ACS712_PIN = A0;   // ACS712 analog input pin
const int LED_PIN = LED_BUILTIN;

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set built-in LED as output
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up MQTT client
  mqttClient.setServer(mqttServer, mqttPort);

  // Connect to MQTT broker
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT connection failed, error code: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

void loop() {
  // Read analog input from ACS712
  int sensorValue = analogRead(ACS712_PIN);  // Read analog value from ACS712
  float voltage = (sensorValue / 1023.0) * 5.0; // Convert sensor value to voltage (assuming 5.0V reference)

  Serial.print("Voltage: ");
  Serial.println(voltage);

  // Adjust the threshold value as per your requirements
  float threshold = 3.0; // Example threshold value

  if (voltage > threshold) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);   // Turn on the LED if the sensor value is above threshold
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off the LED otherwise
  }

  // Publish voltage to MQTT broker
  if (mqttClient.connected()) {
    String voltageString = String(voltage, 2);  // Convert voltage to string with 2 decimal places
    mqttClient.publish(mqttTopic, voltageString.c_str());
    Serial.print("Published to MQTT topic: ");
    Serial.print(mqttTopic);
    Serial.print(", Value: ");
    Serial.println(voltageString);
  }

  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  mqttClient.loop();
  delay(5000);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT broker...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT connection failed, error code: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}
