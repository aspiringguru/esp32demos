#include <NTPClient.h>
#include <WiFi.h>


const char *ssid     = "SleepyGuest24";
const char *password = "sleepyHollow";

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

  //timeClient.begin();
}

void loop() {
  //timeClient.update();

  //Serial.println(timeClient.getFormattedTime());
  Serial.println("blah");
  delay(1000);
}
