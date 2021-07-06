#include <WiFi.h>
#include <PubSubClient.h>
#include <PMsensor.h>
#include "DHT.h"
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


#define DHTPIN 4
#define DHTTYPE DHT22
#define SensorLED 5
#define SensorRead 32
//#define PORT_LED 2
#define SCL 22
#define SDA 21


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
PMsensor PM;
DHT dht(DHTPIN, DHTTYPE);

// Wifi AP 에 연결하기 위한 정보 (본인의 환경에 맞게 수정 필요)
const char* SSID = "Moon";
const char* PASS = "moonmoon";
const char* SERVER = "192.168.137.234";
const int PORT = 1883;  // MQTT 통신에서 기본으로 사용하는 포트번호
unsigned int i = 9998;
unsigned int t = 64;


// Wifi 및 MQTT 연결을 위한 객체생성
WiFiClient espClient;
PubSubClient client(espClient);



void callback(char* topic, byte* payload, unsigned int length)
{
  char str[64];
  memset(str, 0x00, sizeof(str));

  Serial.println(sizeof(str));
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
    str[i] = (char)payload[i];
  }
  if (strcmp("secret/hole", topic) == 0)
  {
    Serial.println(" ---Reset Clear---");
    i = 9998;
  }
}

void setup()
{
  // TTGO 가 동작중인지 표시하기 위해 온보드 LED초기화
  //pinMode(PORT_LED, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(SensorRead, INPUT);
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);

  dht.begin();
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setFontDirection(0);


  /////(infrared LED pin, sensor pin)  /////
  PM.init(SensorLED, SensorRead);

  // Wifi AP 연결 시도
  WiFi.begin(SSID, PASS);

  // 연결될 때까지 재시도
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to AP...");
  }

  Serial.println("Connected to AP!!");

  // MQTT 연결을 초기화 하고, 콜백함수를 연결
  client.setServer(SERVER, PORT);
  client.setCallback(callback);

  // MQTT 브로커 서버가 연결 될때까지 대기.
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Connected!!");
    } else {
      Serial.print("Failure on connect (");
      Serial.print(client.state());
      Serial.println(")");
      delay(2000);
    }
  }
  client.subscribe("secret/hole");
}


void loop() {
  char DustBuffer[32];
  char HumiBuffer[32];
  char TempBuffer[32];
  String std;

  i++;

  if (i == 10000)
  {
    float filter_Data = PM.read();
    float humid = dht.readHumidity();
    float tempC = dht.readTemperature();


    if (isnan(filter_Data))
    {
      Serial.print("fail to read Dust Sensor.");
      delay(1000);
      return;
    }
    if (isnan(humid) || isnan(tempC))
    {
      Serial.print("fail to read DHT.");
      delay(1000);
      return;
    }//ug/m^3

    Serial.print("Filter Data : ");
    Serial.println(filter_Data);
    Serial.print("Temperature : ");
    Serial.println(tempC);
    Serial.print(" Humidity   : ");
    Serial.println(humid);
    Serial.println();


    // 버퍼를 초기화 하고, 센서로 부터 읽은 값으로 문자열 생성
    memset(DustBuffer, 0, sizeof(DustBuffer));
    sprintf(DustBuffer, "%2.1f           : Fine dust", filter_Data);
    memset(TempBuffer, 0, sizeof(TempBuffer));
    sprintf(TempBuffer, "%2.1f   [C ]   : Temperature", tempC);
    memset(HumiBuffer, 0, sizeof(HumiBuffer));
    sprintf(HumiBuffer, "%2.1f   [%%]   : Humidity", humid);

    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, TempBuffer);
    u8g2.drawStr(0, 30, HumiBuffer);
    u8g2.drawStr(0, 50, DustBuffer);
    u8g2.sendBuffer();


    // MQTT 브로커로 문자열 발행
    client.publish("IHome/Temp", TempBuffer);
    client.publish("IHome/Humi", HumiBuffer);
    client.publish("IHome/Dust", DustBuffer);

    //blink
    //digitalWrite(PORT_LED, HIGH);
    //delay(1000);
    //digitalWrite(PORT_LED, LOW);
    i = 0;
  }

  client.loop();
  delay(1);
}
