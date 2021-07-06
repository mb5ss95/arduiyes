#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <stdlib.h>
#include <string.h>

#define PIN       14
#define NUMPIXELS 10

const char* id = "Moon";
const char* PASS = "moonmoon";
const char* SERVER = "192.168.137.234";
const int PORT = 1883;
uint8_t red = 0;
uint8_t gre = 150;
uint8_t blu = 0;
uint16_t delayVal = 500;
uint16_t i = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void callback(const char* topic, byte* payload, unsigned int length)
{
  char str[128];
  unsigned int de;
  unsigned char r, g, b;
  memset(str, 0x00, sizeof(str));

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str[i] = (char)payload[i];
  }
  Serial.println("");
  //strcmp(str1, str2)
  //str1>str2 +val, str1<str2 -val, str1=str2 0
  if (strcmp("secret/RGB/SetVal", topic) == 0)
  {
    sscanf(str, "rgb(%d,%d,%d)", &r, &g, &b);
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    red = r;
    gre = g;
    blu = b;
    i = 10;
  }
  else if (strcmp("secret/RGB/Delay", topic) == 0)
  {
    sscanf(str, "%d", &de);
    Serial.println(delayVal);
    delayVal = de;
    i = 10;
  }
}


void setup()
{
  Serial.begin(115200);
  WiFi.begin(id, PASS);
  pixels.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to AP...");
  }

  Serial.println("Connected to AP!!");

  client.setServer(SERVER, PORT);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client"))
    {
      Serial.println("Connected!!");
    }
    else
    {
      Serial.print("Failure on connect (");
      Serial.print(client.state());
      Serial.println(")");
      delay(2000);
    }
  }

  client.subscribe("secret/RGB/SetVal");
  client.subscribe("secret/RGB/Delay");
}

void loop()
{
  delay(500);
  pixels.clear();
  i = 0;
  while (i < 10)
  {
    delay(delayVal);
    pixels.setPixelColor(i, pixels.Color(red, gre, blu));
    pixels.show();
    i++;
  }
  client.loop();
}
