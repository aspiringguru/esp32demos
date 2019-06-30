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
#include <WiFiClientSecure.h>
#include "Gsender_32.h"
 
const char *ssid = "FreeWIFI";
const char *password = "password";
char UserSsid[] = "";
char UserPassword[] = "";

AsyncWebServer server(80);
const char HTML[] PROGMEM = "<form action=\".\\myFormSubmit\"><label class=\"label\">Network Name</label><input type=\"text\" name=\"ssid\"/><br/><label>Password</label><input type=\"text\" name=\"pass\"/><br/><input type=\"submit\" value=\"Submit\"></form>";

const char* ssid2 = "sleepyVenus";                           // WIFI network name
const char* password2 = "Qazwsx123";                       // WIFI network password
String address[] = {"sleepyhollowinoz@gmail.com", "latindancevideos@gmail.com"};


//from https://arduino.stackexchange.com/questions/3079/how-to-retrieve-the-data-type-of-a-variable
//my moment of coding genius :)
// Generic catch-all implementation.
template <typename T_ty> struct TypeInfo { static const char * name; };
template <typename T_ty> const char * TypeInfo<T_ty>::name = "unknown";

// Handy macro to make querying stuff easier.
#define TYPE_NAME(var) TypeInfo< typeof(var) >::name
// Handy macro to make defining stuff easier.
#define MAKE_TYPE_INFO(type)  template <> const char * TypeInfo<type>::name = #type;
// Type-specific implementations.
MAKE_TYPE_INFO( int )
MAKE_TYPE_INFO( float )
MAKE_TYPE_INFO( short )


String types(String a){return ("it's a String");}
String types(int a)   {return ("it's an int");}
String types(char* a) {return ("it's a char*");}
String types(float a) {return ("it's a float");} 

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr){
  static uint16_t attempt = 0;
  Serial.print("method WiFiConnect : Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  } else {
    WiFi.begin(ssid, password);
    Serial.println(ssid);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50)
  {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("method WiFiConnect : Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("method WiFiConnect : Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("method WiFiConnect : Connection: ESTABLISHED");
  Serial.print("method WiFiConnect : Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}//end WiFiConnect


 
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
        String temp = p->name();
        temp.toCharArray(UserSsid, 32);
        Serial.print("Param value: ");
        Serial.println(p->value());
        temp = p->value();
        temp.toCharArray(UserPassword, 32);
        Serial.println("------");
    } 
    Serial.println("turning off WifiAccess Point");
    //turn off access point
    WiFi.softAPdisconnect(true);
    Serial.println("connect to wifi using UserSsid:");
    Serial.print(UserSsid);
    Serial.print(",  ");
    Serial.println(types(UserPassword));
    Serial.print("UserPassword:");
    Serial.println(UserPassword);
    Serial.print(",  ");
    Serial.println(types(UserPassword));
    /*
    WiFiConnect(UserSsid, UserPassword); //needs to be char*
    Serial.print("connected to wifi ssid:");
    Serial.println(UserSsid);
    Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "ESP32 Test!";
    if (gsender->Subject(subject)->Send(2, address, "ESP32 Test Email!"))
    {
      Serial.println("Message send.");
    }
    else
    {
      Serial.print("Error sending message: ");
      Serial.println(gsender->getError());
    }
    */
  });

  server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", HTML);
    Serial.println("HTTP_GET for /html "); 
    /*
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
    */
  });
 
  server.begin();
}
 
void loop(){
}
