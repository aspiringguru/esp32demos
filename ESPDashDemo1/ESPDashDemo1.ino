#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

AsyncWebServer server(80);

//const char* ssid = "ESP32Asynch"; // Your WiFi SSID
//const char* password = "123456"; // Your WiFi Password
const char* ssid     = "SleepyGuest24";
const char* password = "sleepyHollow";

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
    
    ESPDash.init(server);   // Initiate ESPDash and attach your Async webserver instance
    // Attach Slider Change Events
    ESPDash.attachSliderChanged(sliderChanged);

    ESPDash.addSliderCard("slider1", "Gauge Slider", 3);
    ESPDash.addGaugeChart("gauge1", "Gauge 1");
    ESPDash.addGaugeChart("gauge2", "Gauge 2");
    ESPDash.updateGaugeChart("gauge2", 10);
    ESPDash.addNumberCard("number1", "Number One", 50);
    ESPDash.addTemperatureCard("temp1", "Temp One", 0, 5);
    ESPDash.addHumidityCard("hudmidity1", "Humidity One", 3);
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
  delay(1000);
}
