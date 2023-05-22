#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your MQTT broker IP address
const char* mqtt_server = "your_MQTT_broker_IP";

WiFiClient espClient;
PubSubClient client(espClient);


const int ACS712_PIN = A0;   // ACS712 analog input pin


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Set LED pin as output

  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  int sensorValue = analogRead(ACS712_PIN);  // Read analog value from ACS712
  float voltage = (sensorValue / 1023.0) * 3.3; // Convert sensor value to voltage (assuming 3.3V reference)

  // Adjust the threshold value as per your requirements
  float threshold = 2.0; // Example threshold value

   if (voltage > threshold) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);   // Turn on the LED if the sensor value is above 512
    digitalWrite(LED_BUILTIN, LOW);
    sendNotification("ACS712 Alert: Current threshold exceeded!");
  
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // Turn off the LED otherwise
  }

  delay(2000);  // Delay for stability (adjust as needed)
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
