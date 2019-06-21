/*
 * 
 * uses examples from https://www.instructables.com/id/Write-Data-to-Beginning-of-File-With-Arduino-or-ES/
 * record data to spiffs
 * when wifi connected transfer data to server via ???
 * 
 */

// Import required libraries
//#include "WiFi.h"
#include "SPIFFS.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted.");

  File f = SPIFFS.open("/data.txt", FILE_WRITE);
  Serial.println("file opened.");
  for (int i = 0; i < 100; i++) {
    f.println(i);
  }  
  Serial.println("data written to spiffs.");
  f.close();
  Serial.println("spiffs file closed.");

  File f2 = SPIFFS.open("/data.txt");
  Serial.println("file opened.");

  int count = 0;  
  while(f2.available()) {
    //Lets read line by line from the file
    String line = f2.readStringUntil('\n');
    Serial.print(count);
    Serial.print("  ");
    Serial.println(line);
    count ++;
  }
  f2.close(); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
