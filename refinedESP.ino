#include <ESP8266WiFi.h>

const char* ssid = "Chargemate";
const char* password = "chargemate123";

WiFiServer server(8080);
const int ACS712_PIN = A0;   // ACS712 analog input pin


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Set built in LED as output
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
    WiFiClient client = server.available();
  
    // Read analog input from ACS712
    int sensorValue = analogRead(ACS712_PIN);  // Read analog value from ACS712
    float voltage = (sensorValue / 1023.0) * 5.0; // Convert sensor value to voltage (assuming 5.0V reference)

    Serial.print("voltage : ");
    Serial.println(voltage);
    delay(5000);
   
  // Adjust the threshold value as per your requirements
  float threshold = 3.0; // Example threshold value

   if (voltage > threshold) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);   // Turn on the LED if the sensor value is above threshold
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // Turn off the LED otherwise
  }

 // Display current value on the web server
  if (client) {
    Serial.println("New client connected");
    String currentString = String(voltage, 2);  // Convert current to string with 2 decimal places
    String webPage = "Voltage: " + currentString + " A";
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<html><body>");
    client.println("<h1>" + webPage + "</h1>");
    client.println("</body></html>");
    client.println();
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}
