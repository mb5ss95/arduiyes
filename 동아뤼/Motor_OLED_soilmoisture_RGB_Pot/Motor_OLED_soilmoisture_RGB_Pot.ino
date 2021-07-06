#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include <Wire.h>

#define MotorPin 19
#define NUMPIXELS 10
#define RGBPIN    14
#define sensorPin 32
#define SCL 22
#define SDA 21



uint16_t soilmoisture[] = {0, 0, 0};
uint16_t ave = 0;
uint16_t count = 0;
char cnt = 10;

/*const char* SSID = "Moon";
  const char* PASS = "moonmoon";
  const char* SERVER = "192.168.137.234";
  const int PORT = 1883;*/

boolean check = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//WiFiClient espClient;
//PubSubClient client(espClient);
Adafruit_NeoPixel pixels(NUMPIXELS, RGBPIN, NEO_GRB + NEO_KHZ800);

/*void callback(char* topic, byte* payload, unsigned int length)
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
    count++;
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
    count++;
  }
  }*/

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
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setFontDirection(0);

  pinMode(sensorPin, INPUT);
  pinMode(MotorPin, OUTPUT);
  pinMode(RGBPIN, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);


  randomSeed(analogRead(sensorPin));

  //WiFi.begin(SSID, PASS);
  pixels.begin();


  /*while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to AP...");
    }*/

  Serial.println("Connected to AP!!");

  /*client.setServer(SERVER, PORT);
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
    }*/
  Serial.println("------Color Test------");
  tinkle(255, 0, 0);
  tinkle(0, 255, 0);
  tinkle(0, 0, 255);
  Serial.println("!!!!Complete!!!!");
  /*client.subscribe("secret/RGB/SetNum");
    client.subscribe("secret/RGB/Motor");
    client.subscribe("secret / RGB / SetWater");*/
}

void readPub(char red, char gre, char blu, uint16_t delayVal)
{
  uint16_t sum = 0;
  uint16_t timeSec = 5000; // change num if you want
  uint16_t offset = 450; // change num if you want
  uint16_t set_timeSec = timeSec / delayVal;
  char Buffer[32];
  char Buffer2[32];
  cnt++;
  if (cnt > set_timeSec) // 500*10 = 5000 m/s
  {
    soilmoisture[0] = soilmoisture[1];
    soilmoisture[1] = soilmoisture[2];
    soilmoisture[2] = analogRead(sensorPin) + offset;
    sum = soilmoisture[0] + soilmoisture[1] + soilmoisture[2];
    ave = sum / 3;
    Serial.println(ave);
    memset(Buffer, 0, sizeof(Buffer));
    memset(Buffer2, 0, sizeof(Buffer2));
    sprintf(Buffer, "VALUE = %d", ave);
    sprintf(Buffer2, "COUNT = %d", count);
    //client.publish("soil / Val", Buffer);
    u8g2.clearBuffer();
    u8g2.drawStr(0, 20, Buffer);
    if (check == true)
    {
      u8g2.drawStr(0, 60, "STATE = TRUE");
    }
    else
    {
      u8g2.drawStr(0, 60, " STATE = FALSE");
    }
    u8g2.drawStr(0, 40, Buffer2);
    u8g2.sendBuffer();
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

void loop() //300 up
{
  while (ave < 2400)  // Start color Blue
  {
    readPub(0, 0, 255, 600);
    //client.loop();
  }
  while (ave >= 2400 && ave < 2450) // ERROR color White
  {
    if (check == true)
    {
      digitalWrite(MotorPin, 1);
      delay(1500);
      digitalWrite(MotorPin, 0);
      check = false;
      count++;
    }
    readPub(255, 255, 255, 200);
    //client.loop();
  }
  while (ave >= 2500 && ave < 2550)  // No Water color Red
  {
    if (check == false)
    {
      digitalWrite(MotorPin, 1);
      delay(1500);
      digitalWrite(MotorPin, 0);
      check = true;
      count++;
    }
    readPub(255, 0, 0, 300);
    //client.loop();
  }
  while (ave >= 2550 && ave < 2650) // Normal Water color Orange
  {
    readPub(255, 187, 0,  400); //char red, char gre, char blu, uint16_t delayVal
    //client.loop();
  }
  while (ave >= 2650)  // Enough Water color Green
  {
    readPub(0, 255, 0, 500);
    if (check == true)
    {
      check = false;
    }
    //client.loop();
  }
}
