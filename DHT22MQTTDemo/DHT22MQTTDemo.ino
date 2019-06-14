/*
 * edit mosquitto.conf to allow local connections & nominate port
 * ie: listener 1883 0.0.0.0
 * open mosquitto, mosquitto _sub & mosquitto _pub in three different cmd shells to see interaction
 * mosquitto -v -c mosquitto.conf
 * ipconfig to check ip number of mostquitto server
 * mosquitto_sub -p 1883 -d -t testTopic
 * mosquitto_pub -p 1883 -d -t testTopic -m "Hello world!"
 * 
 * https://xylem.aegean.gr/~modestos/mo.blog/esp32-send-dht-to-mqtt-and-deepsleep/
 * 
 */

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

#define temperature_topic "topic/temp1"       //Topic temperature
#define humidity_topic "topic/humid1"         //Topic humidity
#define debug_topic "debug"                   //Topic for debugging
 
/* likely adjust these during dev/testing */
/* definitions for deepsleep */
#define uS_TO_S_FACTOR 1000000        /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 900              /* Time ESP32 will go to sleep for 15 minutes (in seconds) */
#define TIME_TO_SLEEP_ERROR 3600       /* Time to sleep in case of error (1 hour) */
bool debug = true;             //Display log message if True

 
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
