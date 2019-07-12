#include <SoftwareSerial.h>

SoftwareSerial ss(7, 8); // rx, tx

void speak(char* msg) {
  ss.write(0xFD);
  ss.write((byte)0x0);
  ss.write(2 + strlen(msg));
  ss.write(0x01);
  ss.write((byte)0x0);
  ss.write(msg);
}

void waitForSpeech(unsigned long timeout = 60000) {
  unsigned long start = millis();
  bool done = false;
  while ( ! done && (millis() - start) < timeout ) {
    while ( ss.available() ) {
      if ( ss.read() == 0x4F ) {
        done = true;
        break;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
}

void loop() {

  char buf[64];

  speak("[x0][t6][v5][s6][m51][g2][h2][n1]Hello! I force 2d is the greatest.");
  waitForSpeech();

  for (int i = 101; i < 126; i++) { // message tones
    sprintf(buf, "[x1]sound%d", i);
    speak(buf);
    waitForSpeech();
  }
  for (int i = 201; i < 226; i++) { // ring tones
    sprintf(buf, "[x1]sound%d", i);
    speak(buf);
    waitForSpeech();
  }
  for (int i = 301; i < 331; i++) { // alarm tones
    sprintf(buf, "[x1]sound%d", i);
    speak(buf);
    waitForSpeech();
  }

  int count = 1;
  while ( true ) {
    sprintf( buf, "[m55][t5][n2][s6]Check %d. Battery is %d percent charged.", count, random(1, 100));
    speak(buf);
    waitForSpeech();
    if ( count++ % 10 == 0 ) {
      speak("[s0]I'm feeling sleepy");
      waitForSpeech();
    }
    delay(1000);
  }
}
