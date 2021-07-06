void setup()
{
  pinMode(2, INPUT);
  pinMode(10, OUTPUT);
  }

  void loop()
  {
   int sw_in=digitalRead(2);
 if(sw_in==1)
 {
  digitalWrite(10, 1);
 }
 else digitalWrite(10, 0);
 }
