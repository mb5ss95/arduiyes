#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>


#define MotorPin 19
#define NUMPIXELS 10
#define RGBPIN    14
#define sensorPin 32



uint16_t soilmoisture[] = {0, 0, 0};
uint16_t ave = 0;
char cnt = 10;

const char* SSID = "Moon";
const char* PASS = "moonmoon";
const char* SERVER = "192.168.137.234";
const int PORT = 1883;

boolean check = false;


WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel pixels(NUMPIXELS, RGBPIN, NEO_GRB + NEO_KHZ800);

void callback(char* topic, byte* payload, unsigned int length)
{
  char str[64];
  unsigned int num;

  memset(str, 0x00, sizeof(str));

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str[i] = (char)payload[i];
  }
  Serial.println("");

  if (strcmp("secret/RGB/Motor", topic) == 0)
  {
    int WaterTime;
    sscanf(str, "%d", &WaterTime);
    digitalWrite(MotorPin, 1);
    delay(WaterTime);
    digitalWrite(MotorPin, 0);
  }
  else if (strcmp("secret/RGB/SetNum", topic) == 0)
  {
    sscanf(str, "%d", &num);
    ave = num;
    pixels.clear();
  }
  else
  {
    digitalWrite(MotorPin, 1);
    delay(2500);
    digitalWrite(MotorPin, 0);
  }
}

void tinkle(char red2, char gre2, char blu2)
{
  for (char t = 0; t < 3; t++)
  {
    char i = 0;
    while (i < 10)
    {
      pixels.setPixelColor(i, pixels.Color(red2, gre2, blu2));
      i++;
    }
    pixels.show();
    delay(300);
    pixels.clear();
    delay(300);
  }
}

void setup()
{

  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(MotorPin, OUTPUT);
  pinMode(RGBPIN, OUTPUT);
  randomSeed(analogRead(sensorPin));

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
  Serial.println("------Color Test------");
  tinkle(255, 0, 0);
  tinkle(0, 255, 0);
  tinkle(0, 0, 255);
  Serial.println("!!!!Complete!!!!");
  client.subscribe("secret/RGB/SetNum");
  client.subscribe("secret/RGB/Motor");
  client.subscribe("secret / RGB / SetWater");
}

void readPub(char red, char gre, char blu, uint16_t delayVal)
{
  uint16_t sum = 0;
  uint16_t timeSec = 5000; // change num if you want
  uint16_t offset = 0; // change num if you want
  uint16_t set_timeSec = timeSec / delayVal;
  char Buffer[32];

  cnt++;
  if (cnt > set_timeSec) // 500*10 = 5000 m/s
  {
    soilmoisture[0] = soilmoisture[1];
    soilmoisture[1] = soilmoisture[2];
    soilmoisture[2] = analogRead(sensorPin) - offset;
    sum = soilmoisture[0] + soilmoisture[1] + soilmoisture[2];
    ave = sum / 3;
    Serial.println(ave);
    memset(Buffer, 0, sizeof(Buffer));
    sprintf(Buffer, " % d", ave);
    client.publish("soil / Val", Buffer);
    cnt = 0;
  }
  for (int t = 0; t < 6; t++)
  {
    pixels.setPixelColor(random(10), pixels.Color(red, gre, blu));
  }
  pixels.show();
  delay(delayVal);
  pixels.clear();
}

void loop()
{
  while (ave < 2100)  // Start color Blue
  {
    readPub(0, 0, 255, 600);
    client.loop();
  }
  while (ave >= 2100 && ave < 2150) // ERROR color White
  {
    if (check == true)
    {
      digitalWrite(MotorPin, 1);
      delay(1500);
      digitalWrite(MotorPin, 0);
      check = false;
    }
    readPub(255, 255, 255, 200);
    client.loop();
  }
  while (ave >= 2200 && ave < 2250)  // No Water color Red
  {
    if (check == false)
    {
      digitalWrite(MotorPin, 1);
      delay(1500);
      digitalWrite(MotorPin, 0);
      check = true;
    }
    readPub(255, 0, 0, 300);
    client.loop();
  }
  while (ave >= 2250 && ave < 2350) // Normal Water color Orange
  {
    readPub(255, 187, 0,  400); //char red, char gre, char blu, uint16_t delayVal
    client.loop();
  }
  while (ave >= 2350)  // Enough Water color Green
  {
    readPub(0, 255, 0, 500);
    if (check == true)
    {
      check = false;
    }
    client.loop();
  }
}
