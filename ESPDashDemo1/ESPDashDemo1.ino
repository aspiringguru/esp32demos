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

float temperature;
float humidity;

#define DHTPIN 15
DHT dht;


void sliderChanged(const char* id, int value){
   Serial.println("Slider Changed - "+String(id)+" "+String(value));
   ESPDash.updateGaugeChart("gauge1", value);
   ESPDash.updateStatusCard("status1", 5);
}

// Line Chart Data
int x_axis_size = 7;
String x_axis[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 
int y_axis_size = 7;
int y_axis[7] = {2, 5, 10, 12, 18, 8, 5};



void setup() {
    Serial.begin(115200);
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
    // Attach Slider Change Events
    ESPDash.attachSliderChanged(sliderChanged);

    ESPDash.addSliderCard("slider1", "Gauge Slider", 3);
    ESPDash.addGaugeChart("gauge1", "Gauge 1");
    ESPDash.addGaugeChart("temp", "Temperature");
    ESPDash.updateGaugeChart("temp", 0);
    ESPDash.addGaugeChart("humidity", "Humidity");
    ESPDash.updateGaugeChart("humidity", 0);
    ESPDash.addNumberCard("number1", "Number One", 50);
    ESPDash.addTemperatureCard("temperature", "Temperature(C)", 0, 0);
    ESPDash.addHumidityCard("humidity", "Humidity", 0);
    ESPDash.addStatusCard("status1", "Status One", 0);
    ESPDash.addButtonCard("button1", "Button One");

    ESPDash.addLineChart("linechart1", "Line Chart One", x_axis, x_axis_size, "y_axis_name", y_axis, y_axis_size);
    
    server.begin();
}

void loop() {

  // Fill Data with random Values
  for(int i=0; i < 7; i++){
      y_axis[i] = random(2, 20);
  }
  ESPDash.updateLineChart("chart1", x_axis, x_axis_size, y_axis, y_axis_size);

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
  ESPDash.updateGaugeChart("temp", temperature);
  ESPDash.updateGaugeChart("humidity", humidity);
  ESPDash.updateTemperatureCard("temperature", temperature);
  ESPDash.updateHumidityCard("humidity", humidity);
    
  delay(1000);
}
