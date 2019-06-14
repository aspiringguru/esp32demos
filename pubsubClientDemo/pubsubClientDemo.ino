/*
 * https://diyprojects.io/esp8266-dht22-mqtt-make-iot-include-home-assistant/#.XQCUyVwzZEY
 * netsh wlan show profile name=YOURWIFINAME key=clear
 *
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

char ssid[] = "SleepyGuest24";
char pass[] = "sleepyHollow";
//char ssid[] = "slqwireless";
//char pass[] = "";


#define mqtt_server "ipMOSQUITTO"
#define mqtt_user "guest"
#define mqtt_password "guest"

#define temperature_topic "sensor/temperature"  //Topic temperature
#define humidity_topic "sensor/humidity"        //Topic humidity

//Buffer to decode MQTT messages
char message_buff[100];

long lastMsg = 0;
long lastRecu = 0;
bool debug = false;  //Display log message if True

#define DHTpin 15    //D15 of ESP32 DevKit

// Create abjects
DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial port opened");
    Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("local IP address: ");
  Serial.println(WiFi.localIP());
  dht.setup(DHTpin, DHTesp::DHT11);
  Serial.println("dht.setup completed");
  Serial.println("setting up mqtt_server");
  client.setServer(mqtt_server, 1883);    // Configure MQTT connexion
  Serial.println("mqtt_server setup, set Callback");
  //client.setCallback(callback);           // callback function to execute when a MQTT message
  //Serial.println("callback setup");
}

void loop() {
  // put your main code here, to run repeatedly:

}
