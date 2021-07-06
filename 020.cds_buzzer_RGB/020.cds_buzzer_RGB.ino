int cds = 8;
int buz = 6;
int red = 13;
int gre = 12;
int curval, preval;

void setup()
{
 Serial.begin(9600);
}

void loop()
{
 curval = digitalRead(cds);
 delay(10);
 if(curval!=0 && preval ==0)
 {
  //tone(buz, 1000);
  noTone(buz);
  Serial.println("어둡다");
  analogWrite(gre, 0);
  analogWrite(red, 255);
 }
 else if(curval==0 && preval !=0)
 {
  //noTone(buz);
  tone(buz, 1000);
  Serial.println("밝다");
  analogWrite(gre, 255);
  analogWrite(red, 0);
 }

 preval = curval;
}
