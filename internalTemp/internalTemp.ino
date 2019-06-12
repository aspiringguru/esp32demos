/*
 * https://circuits4you.com/2019/01/01/esp32-internal-temperature-sensor-example/
 * monitors internal temp, not suitable for external temp sensor.
 */

#ifdef __cplusplus
  extern "C" {
 #endif

  uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();
//====================================================
//         Setup
//====================================================
void setup() {
  Serial.begin(115200);
}

//====================================================
//         Loop
//====================================================
void loop() {
  Serial.print("Temperature: ");

  // Convert raw temperature in F to Celsius degrees
  Serial.print((temprature_sens_read() - 32) / 1.8);
  Serial.println(" C");
  delay(1000);
}
