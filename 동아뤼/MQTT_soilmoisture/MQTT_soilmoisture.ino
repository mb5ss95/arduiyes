#include <WiFi.h>
#include <PubSubClient.h>

char sensorPin = 32;
char ledPin = 2;
uint16_t soilmoisture[] = {0, 0, 0};
uint16_t sum = 0;
uint16_t ave = 0;
uint16_t cnt = 0;

const char* SSID = "Moon";
const char* PASS = "moonmoon";
const char* SERVER = "192.168.137.234";
const int PORT = 1883;
const int timesec = 3000;

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
  if (topic == "secret/soil") //(strcmp("secret/soil", topic) == 0)
  {
    Serial.println(" ---Reset Clear---");
    cnt = timesec-5;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(32, INPUT);
  pinMode(2, OUTPUT);

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to AP...");
  }

  Serial.println("Connected to AP!!");

  client.setServer(SERVER, PORT);
  client.setCallback(callback);

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

    client.subscribe("secret/soil");
}

void loop()
{
  char Buffer[32];
  String std;

  if (cnt == timesec)
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

    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);

    cnt = 0;
  }
  client.loop();
  delay(1);
  cnt++;
}
