#include <WiFi.h>
#include <PubSubClient.h>
#include <PMsensor.h>

PMsensor PM;

#define SensorLED 23
#define SensorRead 34
#define PORT_LED 2

// Wifi AP 에 연결하기 위한 정보 (본인의 환경에 맞게 수정 필요)
const char* SSID = "Moon";
const char* PASS = "moonmoon";
const char* SERVER = "192.168.137.51";
const int PORT = 1883;  // MQTT 통신에서 기본으로 사용하는 포트번호


// Wifi 및 MQTT 연결을 위한 객체생성
WiFiClient espClient;
PubSubClient client(espClient);


// 구독한 MQTT 토픽 메시지를 받으면 호출되는 콜백함수 (다른 클라이언트에 발행하는 로직이 있어야 동작)
void callback(char* topic, byte* payload, unsigned int length) {
  char str[32];

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
  // TTGO 가 동작중인지 표시하기 위해 온보드 LED초기화
  pinMode(PORT_LED, OUTPUT);
  pinMode(SensorRead, INPUT);
  Serial.begin(115200);

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
}

void loop() {
  char buffer[16];

  float filter_Data = PM.read(0.1);

  // 읽은 값이 정상적인 실수숫자인지 검사.
  // 비정상값이면 에러출력 후, 리턴됨
  if (isnan(filter_Data))
  {
    Serial.print("fail to read Dust Sensor.");
    delay(1000);
    return;
  }
  Serial.print("Filter : ");
  Serial.println(filter_Data);

  // 버퍼를 초기화 하고, 센서로 부터 읽은 값으로 문자열 생성
  memset(buffer, 0x00, sizeof(buffer));
  sprintf(buffer, "미세먼지 : %2.1f", filter_Data);

  // MQTT 브로커로 문자열 발행
  client.publish("IHome/Dust", buffer);

  //blink
  digitalWrite(PORT_LED, HIGH);
  delay(1000);
  digitalWrite(PORT_LED, LOW);
  delay(1000);

  client.loop();
}
