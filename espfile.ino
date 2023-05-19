#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* mqtt_server = "your_mqtt_server_address";
const char* mqtt_topic = "your_mqtt_topic";

const int sensorPin = A0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(sensorPin, INPUT);
  
  Serial.begin(115200);
  delay(10);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, 1883);
  
  while (!client.connected()) {
    Serial.print("Connecting to MQTT server...");
    
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

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  // Adjust the following line based on the calibration of your ACS712 sensor
  float current = (sensorValue - 512) / 102.3;
  
  char msg[50];
  sprintf(msg, "%.2f", current);
  
  client.publish(mqtt_topic, msg);
  
  delay(1000);
}
