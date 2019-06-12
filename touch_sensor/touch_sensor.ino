/*
 * refer the ESP32-pinout-map in this repo
 * touch wires attached to pins T0 to T9 as per the pinout map.
 */

void setup()
{
Serial.begin(115200);
Serial.println("Touch sensor tutorial ESP32");
}

void loop()
{
Serial.print("Touch0 value is = ");
Serial.println( touchRead(T0));
Serial.println(T0);//4
Serial.println();

Serial.print("Touch2 value is = ");
Serial.println( touchRead(T2));
Serial.println(T2);//2
Serial.println();

Serial.print("Touch3 value is = ");
Serial.println( touchRead(T3));
Serial.println(T3);//15
Serial.println();

Serial.print("Touch4 value is = ");
Serial.println( touchRead(T4));
Serial.println(T4);//13
Serial.println();

Serial.print("Touch5 value is = ");
Serial.println( touchRead(T5));
Serial.println(T5);//12
Serial.println();

Serial.print("Touch6 value is = ");
Serial.println( touchRead(T6));
Serial.println(T6);//14
Serial.println();

Serial.print("Touch7 value is = ");
Serial.println( touchRead(T7));
Serial.println(T7);//27
Serial.println();

Serial.print("Touch8 value is = ");
Serial.println( touchRead(T8));
Serial.println(T8);//33
Serial.println();

Serial.print("Touch9 value is = ");
Serial.println( touchRead(T9));
Serial.println(T9);//32
Serial.println();

delay(1000);
}
