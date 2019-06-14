#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 15 
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

//const char* ssid = "SleepyGuest24";
//const char* password =  "sleepyHollow";
const char *ssid     =  "slqwireless";
const char *password =  "";

const char* mqttServer = "192.168.56.135";  //SLQ
//const char* mqttServer = "192.168.1.118"; //home

const int mqttPort = 1883;
//dont need to use username and password
//const char* mqttUser = "yourMQTTuser";
//const char* mqttPassword = "yourMQTTpassword";
 
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial port started");
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
  // put your main code here, to run repeatedly:

}
