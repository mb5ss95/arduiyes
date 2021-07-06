#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <stdio.h>

#define DHTPIN 0
#define DHTTYPE DHT11
#define PORT_LED 2

// Wifi AP 에 연결하기 위한 정보 (본인의 환경에 맞게 수정 필요)
const char* SSID = "Moon";
const char* PASS = "moonmoon";
const char* SERVER = "192.168.137.51";
const int PORT = 1883;  // MQTT 통신에서 기본으로 사용하는 포트번호


DHT dht(DHTPIN, DHTTYPE);
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
    Serial.print((char)payload[i]);
    str[i] = (char)payload[i];
  }
  Serial.println();
}



void setup()
{
  pinMode(PORT_LED, OUTPUT);
  pinMode(DHTPIN, INPUT);
  Serial.begin(115200);
  dht.begin();

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

  client.subscribe("IHome/DHT2");
}

void loop()
{
  static int i = 0;
  char buffer[128];
  i++;

  if (i >= 5000)
  {
    // dht센서로 부터 습도와 온도를 읽어서 로컬변수에 저장
    float humid = dht.readHumidity();
    float tempC = dht.readTemperature();

    // 읽은 값이 정상적인 실수숫자인지 검사.
    // 비정상값이면 에러출력 후, 리턴됨
    if (isnan(humid) || isnan(tempC))
    {
      Serial.print("fail to read DHT.");
      delay(1000);
      return;
    }
    Serial.print("습도 : ");
    Serial.print(humid);
    Serial.print("온도 : ");
    Serial.println(tempC);

    // 버퍼를 초기화 하고, 센서로 부터 읽은 값으로 문자열 생성
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "습도 : %2.1f, 온도 : %2.1f", humid, tempC);

    // MQTT 브로커로 문자열 발행
    client.publish("IHome/DHT", buffer);
    
    digitalWrite(PORT_LED, 1);
    delay(1000);
    digitalWrite(PORT_LED, 0);
    i = 0;
  }
  
  client.loop();
  delay(1);
}
