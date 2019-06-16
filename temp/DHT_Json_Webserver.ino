// Now using ESP8266.....
// Sample Arduino Json Web Server
// Created by Benoit Blanchon.
// Heavily inspired by "Web Server" from David A. Mellis and Tom Igoe
//https://www.esp8266.com/viewtopic.php?t=7158#


#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
//check using the up to date driver which autoidentifies the DHT type.
//////////////////////////////
// DHT21 / AMS2301 is at GPIO2
//////////////////////////////
#define DHTPIN 2

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// init DHT; 3rd parameter = 16 works for ESP8266@80MHz
DHT dht(DHTPIN, DHTTYPE,12);

// needed to avoid link error on ram check
extern "C"
{
#include "user_interface.h"
}
ADC_MODE(ADC_VCC);

WiFiServer server(80);
WiFiClient client;
const char* ssid = "Your-SSID";
const char* password = "Your-PASSWORD";
float pfDew,pfHum,pfTemp,pfVcc;
bool readRequest(WiFiClient& client) {
  bool currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n' && currentLineIsBlank) {
        return true;
      } else if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }
  return false;
}

JsonObject& prepareResponse(JsonBuffer& jsonBuffer) {
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& tempValues = root.createNestedArray("temperature");
    tempValues.add(pfTemp);
  JsonArray& humiValues = root.createNestedArray("humidity");
    humiValues.add(pfHum);
  JsonArray& dewpValues = root.createNestedArray("dewpoint");
    dewpValues.add(pfDew);
  JsonArray& EsPvValues = root.createNestedArray("Systemv");
    EsPvValues.add(pfVcc/1000, 3);
  return root;
}

void writeResponse(WiFiClient& client, JsonObject& json) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  //next line CORS
  client.println("Access-Control-Allow-Origin: *");
  //refer https://enable-cors.org/server_apache.html
  client.println("Connection: close");
  client.println();

  json.prettyPrintTo(client);
}

void setup() {
delay(2000);
  dht.begin();
  // inital connect
  WiFi.mode(WIFI_STA);
  delay(1000);
    // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    bool success = readRequest(client);
    if (success) {
         delay(1000);
   pfTemp = dht.readTemperature();
   pfHum = dht.readHumidity();
   float a = 17.67;
   float b = 243.5;
   float alpha = (a * pfTemp)/(b + pfTemp) + log(pfHum/100);
   pfDew = (b * alpha)/(a - alpha);
   delay(500);
   pfVcc = ESP.getVcc();
      StaticJsonBuffer<500> jsonBuffer;
      JsonObject& json = prepareResponse(jsonBuffer);
      writeResponse(client, json);
    }
    delay(1);
    client.stop();
  }
}
