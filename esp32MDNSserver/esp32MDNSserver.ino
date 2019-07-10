/*
 * http://www.iotsharing.com/2017/05/how-to-use-mdns-to-resolve-hostname-esp32-ipaddress.html
 * 
 * 
 */

#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"


//const char* ssid     = "Accelerando Guest";
//const char* password = "chalky-genetics-flipflop";
const char* ssid     = "sleepyVenus";
const char* password = "Qazwsx123";
const int serverPort = 80;
const char* device_name = "tank";


/* create a server and listen on nominatedd port */
WiFiServer server(serverPort);
//AsyncWebServer server(serverPort);

void setup()
{
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.print(", on port:");
    Serial.println(serverPort);
    /* connecting to WiFi */
    WiFi.begin(ssid, password);

    /*wait until ESP32 connect to WiFi*/
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    if (!MDNS.begin(device_name)) {
        Serial.println("Error setting up MDNS responder!");
        while(1){
            delay(1000);
        }
    }
    Serial.print("WiFi.localIP() : ");
    Serial.println(WiFi.localIP());
    /* start Server */
    Serial.println("MDNS responder setup");
    server.begin();
    Serial.println("server started");
}

void loop(){
    /* listen for client */
    WiFiClient client = server.available(); 
    uint8_t data[30]; 
    if (client) {                   
      Serial.println("new client");         
      /* check client is connected */           
      while (client.connected()) {          
          if (client.available()) {
              int len = client.read(data, 30);
              if(len < 30){
                  data[len] = '\0';  
              }else {
                  data[30] = '\0';
              }    
              Serial.print("client sent: ");            
              Serial.println((char *)data); 
          }
      }   
    }
}
