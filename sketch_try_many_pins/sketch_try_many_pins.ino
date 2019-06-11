int ledPin = 21;
//pin 21 on board is pinMode(21, OUTPUT);


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("ledPin:"+ledPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(ledPin, OUTPUT);
  Serial.println("new value of ledPin");
  Serial.println(ledPin);
  Serial.println("setting ledPin to HIGH");
  digitalWrite(ledPin, HIGH);
  delay(1000);
  Serial.println("pause another 1000 milliseconds");
  delay(1000);
  Serial.println("setting ledPin to LOW");
  digitalWrite(ledPin, LOW);
  delay(500);
  Serial.println("setting ledPin to HIGH");
  digitalWrite(ledPin, HIGH);
  delay(500);
  Serial.println("setting ledPin to LOW");
  digitalWrite(ledPin, LOW);
  delay(500);
  //ledPin++;
}
