void setup() {
pinMode(2, OUTPUT);
Serial.begin(9600);

}

void loop() {
if(Serial.available())
{
  int a;
  a= Serial.parseInt();
  if(a>5)

  {
    digitalWrite(2, 1);
    Serial.println("Led 온");
    Serial.print("Led 온");
  }
  else
  {
    digitalWrite(2, 0);
    Serial.print("Inpunt data : ");
    Serial.println(a);
    Serial.println("Led off");
    }
}
}
