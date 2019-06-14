#include <WiFi.h>
//#include <WiFi101.h> // Replace this if a different connectivity library is required for your board
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char* ssid = "SleepyGuest24";
const char* password =  "sleepyHollow";
const String deviceId = "dev_42bmEPYH";
const char* deviceSecretKey = "d_sk_U6zqt63XsdsRC5rJhpuZvcL5";

const char* mqttServer = "api.wia.io";
const int mqttPort = 1883;
String topic = "devices/" + deviceId + "/events/";

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<200> jsonBuffer;
JsonObject root = jsonBuffer.to<JsonObject>();

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(500);
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", deviceSecretKey, " ")) {
      Serial.println("Connected to MQTT");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  char topicChar[256];
  topic.toCharArray(topicChar, topic.length());
  
  root["name"] = "temperature";
  root["data"] = 21.5;
  String dataStr = "";
  serializeJson(root, dataStr);
  Serial.println(dataStr);
  size_t len = dataStr.length();
  size_t size = len+1;
  
  byte message[size];
  dataStr.getBytes(message, size);
  client.publish(topicChar, (char*) message);
  delay(5000);
}
