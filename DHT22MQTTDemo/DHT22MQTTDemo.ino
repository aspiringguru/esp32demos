/*
 * edit mosquitto.conf to allow local connections & nominate port
 * ie: listener 1883 0.0.0.0
 * open mosquitto, mosquitto _sub & mosquitto _pub in three different cmd shells to see interaction
 * mosquitto -v -c mosquitto.conf
 * ipconfig to check ip number of mostquitto server
 * mosquitto_sub -p 1883 -d -t testTopic
 * mosquitto_pub -p 1883 -d -t testTopic -m "Hello world!"
 * mosquitto_sub -p 1883 -d -t topic_humid1
 * mosquitto_sub -p 1883 -d -t topic_temp1
 * 
 * https://xylem.aegean.gr/~modestos/mo.blog/esp32-send-dht-to-mqtt-and-deepsleep/
 * https://github.com/markruys/arduino-DHT
 * https://github.com/adafruit/Adafruit_Sensor
 * wire the dht22 sensor with data cable to pin 15, ground and 3V supply
 * NOTE: update to more reliable and more fully featured DHT22 driver. do not use the Adafruit DHT.h driver!!!
 * 
 * current configuration: moved entire code base from setup()to loop().runs ok a couple of times then fails. 
 * probably rejected by MQTT server due to not shutting down properly?
 * added code to shutdown the MQTT and WIFI connection. ran a bit longer but not reliable.
 * added delay(60000) within loop(). seems reliable.
 * 
 * todo
 * add ntp clock sync on boot & obtain time function : https://github.com/espressif/arduino-esp32/issues/1265
 * implement the sleep functions in the xylem code.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 15 
#define DHTTYPE DHT22 
//DHT dht(DHTPIN, DHTTYPE);
DHT dht;

//const char* ssid = "SleepyGuest24";
//const char* password =  "sleepyHollow";
const char *ssid     =  "slqwireless";
const char *password =  "";

const char* mqttServer = "192.168.56.163";  //SLQ
//const char* mqttServer = "192.168.1.118"; //home

const int mqttPort = 1883;
const char *wifi_device_name =  "ESP32Client";

//dont need to use username and password
//const char* mqttUser = "yourMQTTuser";
//const char* mqttPassword = "yourMQTTpassword";

#define temperature_topic "topic_temp1"       //Topic temperature
#define humidity_topic "topic_humid1"         //Topic humidity
#define debug_topic "debug"                   //Topic for debugging
 
/* likely adjust these during dev/testing */
/* definitions for deepsleep */
#define uS_TO_S_FACTOR 1000000        /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 900              /* Time ESP32 will go to sleep for 15 minutes (in seconds) */
#define TIME_TO_SLEEP_ERROR 3600       /* Time to sleep in case of error (1 hour) */
bool debug = true;             //Display log message if True
float t;
float h;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
}

void getReadings(){
  //initialise the DHT22 sensor. NB new driver detects type and only needs pin#
  dht.setup(DHTPIN);
  delay(dht.getMinimumSamplingPeriod());
  // Read temperature in Celcius
  t = dht.getTemperature();
  while ( isnan(t) ){
    Serial.println("[ERROR] Please check the DHT sensor !");
    delay(dht.getMinimumSamplingPeriod());
    t = dht.getTemperature();
  }
  // Read humidity
  h = dht.getHumidity();
  while ( isnan(h) ){
    Serial.println("[ERROR] Please check the DHT sensor !");
    delay(dht.getMinimumSamplingPeriod());
    h = dht.getHumidity();
  }
}

void doWork() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial port started");
  Serial.println("Connecting to wifi");
  setup_wifi(); 
  Serial.println("Connected to the WiFi network"); 
  client.setServer(mqttServer, mqttPort); 
  if (!client.connected()) {
    Serial.println("mqtt connection attempt failed, retrying."); 
    connectMQTT();
  }
  Serial.println("Connected to MQTT broker. sleeping.....");
  Serial.println("end sleep. publishing to MQTT");
  client.publish("testTopic", "Hello from ESP32");
  getReadings();
  
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print(" | Humidity : ");
  Serial.println(h);
  // Publish values to MQTT topics
  client.publish(temperature_topic, String(t).c_str(), true);
  delay(100); //some delay is needed for the mqtt server to accept the message
  client.publish(humidity_topic, String(h).c_str(), true);
  Serial.println("published to mqtt");
  //close MQTT
  client.disconnect();
  //close terminal
  //close WiFi
  WiFi.disconnect();
}

//Setup connection to wifi
void setup_wifi() {
  delay(20);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
 Serial.println("");
 Serial.println("WiFi is OK ");
 Serial.print("=> ESP32 new IP address is: ");
 Serial.print(WiFi.localIP());
 Serial.println("");
}
 
//Reconnect to MQTT if connection is lost
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    //if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT broker connection OK");
    } else {
      Serial.print("[Error] Not connected to MQTT broker client.state() = : ");
      Serial.print(client.state());
      Serial.println("Wait 5 seconds before retry.");
      delay(5000);
    }
  }
}//end connectMQTT


void loop() {
  // put your main code here, to run repeatedly:
  doWork();
  delay(60000);
}
