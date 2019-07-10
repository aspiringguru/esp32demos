#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A1
#define XM A2
#define YM 7
#define XP 6

#define TS_MINX 940
#define TS_MINY 160
#define TS_MAXX 160
#define TS_MAXY 970

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000

int touch = 0;

void setup() {
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9325);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, 120, 120, GREEN);
  tft.fillRect(120, 0, 120, 120, RED);
  tft.setCursor(15, 45);
  tft.println("M1 ON");
  tft.setCursor(128, 45);
  tft.println("M1 OFF");
  tft.setTextSize(2);
  tft.setCursor(8,200);
  tft.println("SensVal1:204");
  tft.setCursor(8,220);
  tft.println("SensVal2:96");
  tft.setCursor(8,240);
  tft.println("SensVal3:4856");
}

void loop() {

  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    if (p.x > 0 && p.x < 120) {
      if (p.y > 0 && p.y < 120) {
        touch = 1;
      }
    }
    if (p.x > 120 && p.x < 240) {
      if (p.y > 0 && p.y < 120) {
        touch = 2;
      }
    }
  }

  if (touch == 1) {
    digitalWrite(A5, HIGH);
    touch = 0;
    delay(200);
  }

  if (touch == 2) {
    digitalWrite(A5, LOW);
    touch = 0;
    delay(200);
  }
}
