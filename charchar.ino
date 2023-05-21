#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "MQTT_SERVER_IP";
const char* mqtt_topic = "YOUR_MQTT_TOPIC";

const int sensorPin = A0;
const int ledPin = D1;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  if (strcmp(topic, mqtt_topic) == 0) {
    // Convert payload to integer value
    int value = 0;
    for (unsigned int i = 0; i < length; i++) {
      value = value * 10 + (payload[i] - '0');
    }

    if (value > 0) {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED turned on");
    } else {
      digitalWrite(ledPin, LOW);
      Serial.println("LED turned off");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read analog input from ACS712 sensor
  int sensorValue = analogRead(sensorPin);

  // Adjust the threshold value as per your requirement
  if (sensorValue > 512) {
    // Publish the sensor value to MQTT topic
    char msg[16];
    snprintf(msg, sizeof(msg), "%d", sensorValue);
    client.publish(mqtt_topic, msg);
  }

  delay(1000);
}
