/*
 * https://circuits4you.com/2019/01/25/esp32-dht11-22-humidity-temperature-sensor-interfacing-example/
 * 
 */

#include "DHTesp.h"
 
#define DHTpin 15    //D15 of ESP32 DevKit
 
DHTesp dht;
 
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  
  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);
 
  // use this instead: 
  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
  //dht.setup(DHTpin, DHTesp::DHT22); //for DHT22 Connect DHT sensor to GPIO 17
  Serial.print("dht.getMinimumSamplingPeriod()");
  Serial.println(dht.getMinimumSamplingPeriod());
}
 
void loop()
{
  delay(dht.getMinimumSamplingPeriod()*5);
 
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
 
  Serial.print(dht.getStatusString());
  Serial.print("\t humidity:");
  Serial.print(humidity, 1);
  Serial.print("\t\t temperature(C):");
  Serial.print(temperature, 1);
  Serial.print("\t\ttemp(F):");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t HeatIndex(C):");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t HeatIndex(C):");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
}
