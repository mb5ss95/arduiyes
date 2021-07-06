int k;
void setup() {
for(k=2; k<7; k++)
{
  pinMode (k, 1);
}
}

void loop() 
{
for(k=2; k<7; k++)
{
  digitalWrite(k, 1);
  delay(100);
  digitalWrite(k, 0);
  delay(100);
}
}
