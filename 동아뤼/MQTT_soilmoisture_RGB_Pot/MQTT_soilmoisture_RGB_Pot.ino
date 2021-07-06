#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>


#define NUMPIXELS 10
#define RGBPIN    14
#define sensorPin 32



uint16_t soilmoisture[] = {0, 0, 0};
uint16_t sum = 0;
uint16_t ave = 0;
uint16_t cnt = 0;

const char* SSID = "Moon";
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
Adafruit_NeoPixel pixels(NUMPIXELS, RGBPIN, NEO_GRB + NEO_KHZ800);

void callback(char* topic, byte* payload, unsigned int length)
{
  char str[64];
  unsigned char r, g, b;
  unsigned int de;
  memset(str, 0x00, sizeof(str));

  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
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
    i = 6;
  }
  else if (strcmp("secret/RGB/Delay", topic) == 0)
  {
    sscanf(str, "%d", &de);
    Serial.println(delayVal);
    delayVal = de;
    i = 6;
  }
  pixels.clear();
}

void setup()
{
  Serial.begin(115200);
  pinMode(32, INPUT);
  pinMode(2, OUTPUT);
  randomSeed(analogRead(32));

  WiFi.begin(SSID, PASS);
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
    } else
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
  char Buffer[32];
  String std;

  delay(1);
  cnt++;

  if (cnt == 1000)
  {
    soilmoisture[0] = soilmoisture[1];
    soilmoisture[1] = soilmoisture[2];
    soilmoisture[2] = analogRead(sensorPin);
    sum = soilmoisture[0] + soilmoisture[1] + soilmoisture[2];
    ave = sum / 3;
    Serial.println(ave);

    memset(Buffer, 0, sizeof(Buffer));
    sprintf(Buffer, "%d", ave);

    client.publish("soil/Val", Buffer);

    cnt = 0;
  }
  for (int t = 0; t < 6; t++)
  {
    pixels.setPixelColor(random(10), pixels.Color(red, gre, blu));
  }
  pixels.show();
  delay(delayVal);
  pixels.clear();*/
  client.loop();
}
