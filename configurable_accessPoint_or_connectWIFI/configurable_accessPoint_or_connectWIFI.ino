/*
 * this will run as own access point or connect to existing wifi
 * example code to create form > https://www.dfrobot.com/blog-827.html
 * text to c++  string converter > https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
 * look at this example https://forum.arduino.cc/index.php?topic=590802.0
 * look at this also > https://techtutorialsx.com/2018/10/12/esp32-http-web-server-handling-body-data/
 * 
 * 
 */

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
 
const char *ssid = "MyESP32AP";
const char *password = "testpassword";
 
AsyncWebServer server(80);
const char HTML[] PROGMEM = "<form onSubmit=\"myFormSubmit\"><label class=\"label\">Network Name</label><input type=\"text\" name=\"ssid\"/><br/><label>Password</label><input type=\"text\" name=\"pass\"/><br/><input type=\"submit\" value=\"Submit\"></form>";
 
void setup(){
  Serial.begin(115200);
 
  WiFi.softAP(ssid, password);
 
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello World");
    Serial.println("HTTP_GET for /hello ");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello /");
    Serial.println("HTTP_GET for / ");
    Serial.print("Received request from client with IP: ");
    Serial.println(request->client()->remoteIP());
  });

  server.on("/myFormSubmit", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello /myFormSubmit");
    Serial.println("HTTP_GET for /myFormSubmit ");
    int paramsNr = request->params();
    Serial.println("# of params in form = paramsNr:");
    Serial.println(paramsNr);
    for(int i=0;i<paramsNr;i++){
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
    } 
    
  });




  server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", HTML);
    Serial.println("HTTP_GET for /html "); 
    int paramsNr = request->params();
    Serial.println("# of params in form = paramsNr:");
    Serial.println(paramsNr);
    for(int i=0;i<paramsNr;i++){
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
    } 
  });
 
  server.begin();
}
 
void loop(){}
