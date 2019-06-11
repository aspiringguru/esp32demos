#define BLYNK_PRINT Serial

int pin        = 21;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "57b13b2ddc5f4b3296fcfa83affa3615";
char auth[] = "711c64a93cf045e2a2d2ff75549c8418";


// Your WiFi credentials.
// Set password to "" for open networks.
//nb: works for WPA2-PSK [AES], does not work for WPA-PSK [TKIP] + WPA2-PSK [AES]
char ssid[] = "SleepyGuest24";
char pass[] = "sleepyHollow";


void setup() {  
  pinMode(pin, OUTPUT); 
  pinMode(pin, HIGH);
  Serial.begin(115200);

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Serial.println("connecting to blynk with auth:");  
  Serial.println(auth);
  Blynk.begin(auth, ssid, pass);
}

void loop(){
    Blynk.run();
    
}
