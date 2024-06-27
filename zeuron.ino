#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* mqttServer = "broker.mqtt-dashboard.com";
const int mqttPort = 1883;
const char* mqttUser = ""; // MQTT username, if any
const char* mqttPassword = ""; // MQTT password, if any
const int DHTPin = 4; // Digital pin connected to the DHT sensor
const int LightPin = 34; // Analog pin connected to the light sensor
DHT dht(DHTPin, DHT11);
WiFiClient wifiClient;
PubSubClient client(wifiClient);
void setup() {
 Serial.begin(115200);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(1000);
 Serial.println("Connecting to WiFi...");
 }
 Serial.println("Connected to WiFi");
 client.setServer(mqttServer, mqttPort);
 // Set MQTT callback functions if needed
 // client.setCallback(callback);
 dht.begin();
}
void loop() {
 float temperature = dht.readTemperature();
 float humidity = dht.readHumidity();
 int lightLevel = analogRead(LightPin);
 if (!client.connected()) {
 reconnect();
 }

 StaticJsonDocument<200> doc;
 doc["temperature"] = temperature;
 doc["humidity"] = humidity;
 doc["light"] = lightLevel;
 char jsonBuffer[256];
 serializeJson(doc, jsonBuffer);
 client.publish("home/environment", jsonBuffer);
 delay(5000); // Adjust delay as needed
}
void reconnect() {
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
 Serial.println("connected");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" try again in 5 seconds");
 delay(5000);

}

}
}
