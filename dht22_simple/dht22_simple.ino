/*
 * download these git repos as zip file to blah/Arduino/libraries/ & unzip
 * https://github.com/adafruit/DHT-sensor-library
 * https://github.com/adafruit/Adafruit_Sensor
 * wire the dht22 sensor with data cable to pin 15, ground and 3V supply
 * 
 */

#include <DHT.h>
#include <Adafruit_Sensor.h>


#define DHTPIN 15 
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
 
void setup()
{
  Serial.begin(115200);
  Serial.println("started");
 }
 
void loop()
{
  float temperature = dht.readTemperature();
  float h = dht.readHumidity();
  float temp_f = dht.readTemperature(true);
  Serial.print("Temperature(C): ");
  Serial.print(temperature);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("Temperature(F): ");
  Serial.print(temp_f);
  Serial.println();

  
  delay(5000);
}
