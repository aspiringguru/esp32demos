void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("LED_BUILTIN :"+LED_BUILTIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("setting LED_BUILTIN to HIGH");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println("pause another 1000 milliseconds");
  delay(1000);
  Serial.println("setting LED_BUILTIN to LOW");
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  Serial.println("setting LED_BUILTIN to HIGH");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  Serial.println("setting LED_BUILTIN to HIGH");
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
