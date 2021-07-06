int p1_Analog = 33;
int Ana_Val;
int curval, preval;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
 curval = analogRead(p1_Analog) * -1 +4095;
 delay(1000);
 if(curval != preval)
 {
  Serial.println("Analog = ");
  Serial.println(curval);
 }
 preval = curval;
}
