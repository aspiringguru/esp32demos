/*
 * download these git repos as zip file to blah/Arduino/libraries/ & unzip
 * https://github.com/adafruit/DHT-sensor-library  not good replace with DHT.h by markruys
 * https://github.com/markruys/arduino-DHT
 * https://github.com/adafruit/Adafruit_Sensor
 * wire the dht22 sensor with data cable to pin 15, ground and 3V supply
 * NOTE: update to more reliable and more fully featured DHT22 driver
 */

#include <DHT.h>
#include <Adafruit_Sensor.h>


#define DHTPIN 15 
#define DHTTYPE DHT22 
//DHT dht(DHTPIN, DHTTYPE);
//DHT dht(DHTPIN);
DHT dht;
 
void setup()
{
  Serial.begin(115200);
  Serial.println("started");
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
