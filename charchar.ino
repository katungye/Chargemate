#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your MQTT broker IP address
const char* mqtt_server = "your_MQTT_broker_IP";

WiFiClient espClient;
PubSubClient client(espClient);

const int acs712Pin = A0; // ACS712 connected to A0 pin
const int ledPin = D1; // LED connected to D1 pin

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  int sensorValue = analogRead(acs712Pin);
  float voltage = (sensorValue / 1023.0) * 3.3; // Convert sensor value to voltage (assuming 3.3V reference)

  // Adjust the threshold value as per your requirements
  float threshold = 2.0; // Example threshold value

  if (voltage > threshold) {
    digitalWrite(ledPin, HIGH); // Turn on LED
    sendNotification("ACS712 Alert: Current threshold exceeded!");
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
  }

  delay(1000); // Adjust delay as per your requirements
}

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMQTT() {
  client.setServer(mqtt_server, 1883);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT Broker...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void sendNotification(const char* message) {
  if (client.connected()) {
    client.publish("homeassistant/notification", message);
  }
}
