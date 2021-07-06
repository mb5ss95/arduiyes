#include <SoftwareSerial.h>

SoftwareSerial blue_tooth(2, 3); //Rx, Tx 받기, 보내기

void setup()
{
  Serial.begin(9600);
  blue_tooth.begin(9600);
}


void loop()
{
  byte d[3];
  int a = analogRead(A5);

  d[0] = 'a';
  d[1] = a / 256;
  d[2] = a % 256;
  if (blue_tooth.available())
  {
    byte b = blue_tooth.read();
    if (b == 49)
    {
      blue_tooth.write(d[0]);
      blue_tooth.write(d[1]);
      blue_tooth.write(d[2]);
    }
  }
}

/*void loop()
{
  if(blue_tooth.available())
  {
    Serial.write(blue_tooth.read());
  }
    if(Serial.available())
  {
    blue_tooth.write(Serial.read());
  }
}*/
//AT - OK
//AT+NAMEdevice - oksetname
