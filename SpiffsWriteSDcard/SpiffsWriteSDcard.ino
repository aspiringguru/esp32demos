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
uint64_t uS_TO_S_FACTOR = 1000000;
uint64_t time2sleep = 5;//change to longer intervals later, ie 600 seconds

// Save reading number on RTC memory. this is stored during deep sleep
//https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/deep-sleep-stub.html
RTC_DATA_ATTR int readingID = 0;

String dataMessage;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


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
  Serial.print("ssid:");
  Serial.println(ssid);
  Serial.print("password:");
  Serial.println(password);
  Serial.print("uS_TO_S_FACTOR:");
  //Serial.println(uS_TO_S_FACTOR);
  print_uint64_t(uS_TO_S_FACTOR);
  Serial.print("time2sleep:");
  //Serial.println(time2sleep);
  print_uint64_t(time2sleep);
  
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

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  Serial.println("Now create file if it doesn't exist.");
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Reading ID, Date, Hour \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

  Serial.print("go to sleep for ");
  //Serial.print(time2sleep);
  print_uint64_t(time2sleep);
  Serial.println(" seconds");
  // Enable Timer wake_up
  esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * time2sleep);

  //do whatever readings and writing to file here before sleeping
  getTimeStamp();
  logSDCard();

  // Increment readingID on every new reading
  readingID++;
  // Start deep sleep
  Serial.println("DONE! Going to sleep now.");
  esp_deep_sleep_start(); 

}

void loop() {
  //due to deep sleep in setup() this will never run
  Serial.println("loop() started");
  delay(1000);
}


// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();

}


// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

// Write the sensor readings on the SD card
void logSDCard() {
  dataMessage = String(readingID) + "," + String(dayStamp) + "," + String(timeStamp) + "," + "\r\n";
  Serial.print("Saving data to file : ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
}




// Function to get date and time from NTPClient
void getTimeStamp() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.println(timeStamp);
}


void print_uint64_t(uint64_t num) {

  char rev[128]; 
  char *p = rev+1;

  while (num > 0) {
    *p++ = '0' + ( num % 10);
    num/= 10;
  }
  p--;
  /*Print the number which is now in reverse*/
  while (p > rev) {
    Serial.print(*p--);
  }
}
