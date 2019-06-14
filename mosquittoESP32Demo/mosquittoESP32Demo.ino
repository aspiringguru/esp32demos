/*
 * edit mosquitto.conf to allow local connections & nominate port
 * ie: listener 1883 0.0.0.0
 * open mosquitto, mosquitto _sub & mosquitto _pub in three different cmd shells to see interaction
 * mosquitto -v -c mosquitto.conf
 * ipconfig to check ip number of mostquitto server
 * mosquitto_sub -p 1883 -d -t testTopic
 * mosquitto_pub -p 1883 -d -t testTopic -m "Hello world!"
 * 
 */

#include <WiFi.h>
#include <PubSubClient.h>
 
//const char* ssid = "SleepyGuest24";
//const char* password =  "sleepyHollow";
const char *ssid     =  "slqwireless";
const char *password =  "";

int count = 0;


const char* mqttServer = "192.168.56.135";  //SLQ
//const char* mqttServer = "192.168.1.118"; //home

const int mqttPort = 1883;
//dont need to use username and password
//const char* mqttUser = "yourMQTTuser";
//const char* mqttPassword = "yourMQTTpassword";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  Serial.println("Connecting to wifi");
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...");
  }
  Serial.println("Connected to the WiFi network"); 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    //if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
    if (client.connect("ESP32Client")) {
      Serial.println("connected to MQTT server");
    } else {
      Serial.print("connection to mqtt server failed with state ");
      Serial.println(client.state());
      delay(2000);
 
    }
  }
  Serial.println("Connected to MQTT broker.");
  client.publish("testTopic", "Hello from ESP32");
}
 
void loop() {
  client.loop();
  delay(1000);
  count++;
  char b[2];
  String str;
  str=String(count);
  str.toCharArray(b,2);
  client.publish("testTopic", b);
}
