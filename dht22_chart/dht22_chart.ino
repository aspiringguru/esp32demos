/*
 * https://techtutorialsx.com/2018/05/05/esp32-arduino-temperature-humidity-and-co2-concentration-web-server/
 * needs library ESPAsyncWebServer installed
 * refer > https://github.com/sidoh/path_variable_handlers
 * installing via menu > sketch > libraries creates dir PathVariableHandlers under C:\Users\Matthew\Documents\Arduino\libraries\
 * this does not work!!!
 * submitted issue on git repo for ESPAsyncWebServer
 */

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
 
int dhtPin = 15;
 
DHTesp dht;
 
const char* ssid = "SleepyGuest24";
const char* password =  "sleepyHollow";
 
AsyncWebServer server(80);
 
 
void setup() {
  dht.setup(dhtPin);
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.print("connect to server at http://");
  Serial.println(WiFi.localIP());
 

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
 
    float temperature = dht.getTemperature();
 
    request->send(200, "text/plain", String(temperature) + " ºC");
  });
 
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
 
    float humidity = dht.getHumidity();
 
    request->send(200, "text/plain", String(humidity) + " %");
  });
 
  server.begin();
}
 
void loop() {}
