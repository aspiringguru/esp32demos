/*
 * download these git repos as zip file to blah/Arduino/libraries/ & unzip
 * https://github.com/adafruit/DHT-sensor-library  not good replace with DHT.h by markruys
 * https://github.com/markruys/arduino-DHT
 * https://github.com/adafruit/Adafruit_Sensor
 * wire the dht22 sensor with data cable to pin 15, ground and 3V supply
 * NOTE: update to more reliable and more fully featured DHT22 driver
 */

#include <WiFi.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

//const char* ssid = "SleepyGuest24";
//const char* password =  "sleepyHollow";
const char *ssid     =  "slqwireless";
const char *password =  "";


#define uS_TO_S_FACTOR 1000000        /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60              /* Time ESP32 will go to sleep for 1 minutes (in seconds) */
#define DHTPIN 15 
#define DHTTYPE DHT22 
//DHT dht(DHTPIN, DHTTYPE);
//DHT dht(DHTPIN);
DHT dht;
 
void setup()
{
  Serial.begin(115200);
  Serial.println("started");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi is OK ");
  obtain_time();

  dht.setup(DHTPIN);
  Serial.print("dht.getMinimumSamplingPeriod():");
  Serial.println(dht.getMinimumSamplingPeriod());
  Serial.print("dht.getModel():");
  Serial.println(dht.getModel());
  Serial.print("dht.getNumberOfDecimalsTemperature():");
  Serial.println(dht.getNumberOfDecimalsTemperature());
  Serial.print("dht.getLowerBoundTemperature():");
  Serial.println(dht.getLowerBoundTemperature());
  Serial.print("dht.getUpperBoundTemperature():");
  Serial.println(dht.getUpperBoundTemperature());
  Serial.print("dht.getLowerBoundHumidity():");
  Serial.println(dht.getLowerBoundHumidity());
  Serial.print("dht.getUpperBoundHumidity():");
  Serial.println(dht.getUpperBoundHumidity());
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
  Serial.println("Going to sleep as normal now.");
  esp_deep_sleep_start();
 }
 
void loop()
{
  delay(dht.getMinimumSamplingPeriod());
  //float temperature = dht.readTemperature();
  float temperature = dht.getTemperature();
  //float h = dht.readHumidity();
  float h = dht.getHumidity();
  //float temp_f = dht.readTemperature(true);
  Serial.print("Temperature(C): ");
  Serial.print(temperature);
  Serial.print("  Humidity: ");
  Serial.print(h);
  //Serial.print("Temperature(F): ");
  //Serial.print(temp_f);
  Serial.println();

  
  delay(5000);
}


static void initialize_sntp(void) {
  Serial.println("Initializing SNTP");
  configTime(0,0,"pool.ntp.org");
}

static void obtain_time(void) {
  initialize_sntp();
  // wait for time to be set
  time_t now = 0;
  struct tm timeinfo = {0};
  int retry = 0;
  const int retry_count = 10;
  Serial.print("Waiting for system time to be set.");
  while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
    Serial.print(".");
    delay(100);
    time(&now);
    localtime_r(&now, &timeinfo);
  }
  Serial.println("ready.");
}
