int dataFromUser = 0
void setup(){
    //this code will run once
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void loop(){
    if (Serial.available()>0)
    {
        dataFromUser=Serial.read();
    }

    if (dataFromUser == '1')
    {
        digitalWrite(LED_BUILTIN, HIGH)
    }
    else if (dataFromUser == '1')
    {
        digitalWrite(LED_BUILTIN, LOW)
    }
}
