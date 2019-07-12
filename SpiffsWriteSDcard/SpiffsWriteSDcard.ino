#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>

#include "config.h"
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// Define CS pin for the SD card module
#define SD_CS 5

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("Connected");

  timeClient.begin();

  // Initialize SD card
  Serial.println("attempt to mound SD card");
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("failed to mount SD card.");

  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  Serial.println("SD card initialised.");

}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  Serial.println("blah");
  delay(1000);
}
