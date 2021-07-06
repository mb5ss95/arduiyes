#include "BluetoothSerial.h"
#include <Adafruit_NeoPixel.h>

#define PIN       14
#define NUMPIXELS 64

BluetoothSerial SerialBT;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t i = 0;
uint8_t u = 0;
uint8_t red = 0;
uint8_t gre = 150;
uint8_t blu = 0;
boolean yorn = false;

void setup()
{
  Serial.begin(115200);
  pixels.begin();
  SerialBT.begin("RGB_Control");
  Serial.println("The device started, now you can pair it with bluetooth!");
}


void loop()
{
  char check[64];
  memset(check, 0x00, sizeof(check));
  /*if (Serial.available())
    {
    SerialBT.write(Serial.read());
    }*/
  while (SerialBT.available())
  {
    check[i] = SerialBT.read();
    if (check[i - 1] == ')')
    {
      u = i;
    }
    i++;
  }
  if (!SerialBT.available() && i)
  {
    unsigned char r, g, b;
    char str[16];
    memset(str, 0x00, sizeof(str));
    for (int t = u; t < i - 1; t++)
    {
      str[t - u] = (char)check[t];
      Serial.print(str[t - u]);
    }
    Serial.println("");

    sscanf(str, "%d.%d.%d", &r, &g, &b);

    red = r;
    gre = g;
    blu = b;

    Serial.println(red);
    Serial.println(gre);
    Serial.println(blu);
    pixels.clear();
    for (int b = 0; b < NUMPIXELS; b++)
    {
      pixels.setPixelColor(b, pixels.Color(red, gre, blu));
    }
    pixels.show();
    delay(500);
    
    i = 0;
  }
}
