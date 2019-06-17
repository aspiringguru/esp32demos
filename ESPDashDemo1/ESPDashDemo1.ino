#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>


AsyncWebServer server(80);

//const char* ssid = "ESP32Asynch"; // Your WiFi SSID
//const char* password = "123456"; // Your WiFi Password
const char* ssid     = "SleepyGuest24";
const char* password = "sleepyHollow";

const char* button1Name = "btn1";
const char* button2Name = "btn2";

float temperature;
float humidity;

#define LED 2
#define DHTPIN 15
DHT dht;



// Line Chart Data
int x_axis_size = 10;
String x_axis[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; 
int y_axis_size = 10;
int y_axis[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int x2_axis_size = 10;
String x2_axis[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; 
int y2_axis_size = 10;
int y2_axis[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void btnCallback(const char* id){
  Serial.println("Button pressed on Dash, ID - '"+String(id)+"'");
  if(strcmp(button1Name,id)==0) {
    Serial.println("Button One pressed");
    digitalWrite(LED, HIGH); // Turn LED ON
  }
  if(strcmp(button2Name,id)==0) {
    Serial.println("Button Two pressed");
    digitalWrite(LED, LOW); // Turn LED OFF
  }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    dht.setup(DHTPIN);
    Serial.print("dht.getModel():");
    Serial.println(dht.getModel());
    Serial.print("dht.getMinimumSamplingPeriod():");
    Serial.println(dht.getMinimumSamplingPeriod());
    

    ESPDash.init(server);   // Initiate ESPDash and attach your Async webserver instance

    //ESPDash.addGaugeChart("temp", "Temperature");
    //ESPDash.updateGaugeChart("temp", 0);
    //ESPDash.addGaugeChart("humidity", "Humidity");
    //ESPDash.updateGaugeChart("humidity", 0);
    ESPDash.addTemperatureCard("temperature", "Temperature(C)", 0, 0);
    ESPDash.addHumidityCard("humidity", "Humidity", 0);
    ESPDash.addLineChart("temperature", "Temperature(C)", x_axis, x_axis_size, "degrees celcius", y_axis, y_axis_size);
    ESPDash.addLineChart("humidity", "Humidity(%)", x2_axis, x2_axis_size, "percentage", y2_axis, y2_axis_size);
    ESPDash.attachButtonClick(btnCallback);
    ESPDash.addButtonCard(button1Name, "ON");
    ESPDash.addButtonCard(button2Name, "OFF");
    
    server.begin();
}

void loop() {

  delay(dht.getMinimumSamplingPeriod());
  temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  Serial.print("Temperature(C): ");
  Serial.print(temperature);
  Serial.print("  Humidity: ");
  Serial.print(humidity);
  //Serial.print("Temperature(F): ");
  //Serial.print(temp_f);
  Serial.println();
  //ESPDash.updateGaugeChart("temp", temperature);
  //ESPDash.updateGaugeChart("humidity", humidity);
  ESPDash.updateTemperatureCard("temperature", temperature);
  ESPDash.updateHumidityCard("humidity", humidity);

  //Serial.print("y_axis Temperature:");
  for (int i = 1; i < y_axis_size; i++)
   {
     y_axis[i-1] = y_axis[i];
     //Serial.print(y_axis[i]);
     //Serial.print(", ");
   }
  y_axis[y_axis_size-1] = temperature;
  //Serial.println(y_axis[y_axis_size-1]);

  //Serial.print("y_axis2 Humidity:");
  for (int i = 1; i < y2_axis_size; i++)
   {
     y2_axis[i-1] = y2_axis[i];
     //Serial.print(y2_axis[i]);
     //Serial.print(", ");
   }
  y2_axis[y2_axis_size-1] = humidity;

  
  ESPDash.updateLineChart("temperature", x_axis, x_axis_size, y_axis, y_axis_size);  
  ESPDash.updateLineChart("humidity", x2_axis, x2_axis_size, y2_axis, y2_axis_size);  
  
    
  delay(1000);
}
