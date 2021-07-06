#include <WiFi.h>
#include <PubSubClient.h>

#define pin_t1 12
#define pin_t2 13
#define pin_t3 14

const char* MyTOPIC = "topic_lolin1";
const char* YourTop = "from__lolin1";
const char* SSID = "U+Net49EC";
const char* PASS = "DD62060560";
const char* SERVER = "192.168.219.102";
const int PORT = 1883;

boolean t1_state, t2_state, t3_state = false;


WiFiClient espClient;
PubSubClient client(espClient);

void WIFI_Reconnect()
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(SSID, PASS);
    delay(100);
  }
   Serial.println("WIFI Reconnected!!");
}

void MQTT_Reconnect() 
{
  while (!client.connected()) 
  {
    if (client.connect("ESP32Client")) 
    {
      Serial.println("MQTT Reconnected!!");
      client.subscribe(MyTOPIC);
    }
    delay(100);
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  char str[64];
  int temp_t1, temp_t2, temp_t3;
  memset(str, 0x00, sizeof(str));

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    str[i] = (char)payload[i];
  }
  Serial.println();

  //strcmp(str1, str2)
  //str1>str2 +val, str1<str2 -val, str1=str2 0
  if (!strcmp("all", str))
  {
    char buffer[5];
    memset(buffer, 0x00, sizeof(buffer));
    sprintf(buffer, "%d%d%d", t1_state, t2_state, t3_state); // 000, 001, 010, 011.....
    Serial.print("test ok!!!");
    Serial.println(buffer);
    client.publish(YourTop, buffer);
  }
  else if (!strcmp("allon", str))
  {
    t1_state = t2_state = t3_state = true;
  }
  else if (!strcmp("alloff", str))
  {
    t1_state = t2_state = t3_state = false;
  }
  else if (!strcmp("t1_1", str))
  {
    t1_state = true;
    // active
  }
  else if (!strcmp("t1_0", str))
  {
    t1_state = false;
    // active
  }
  else if (!strcmp("t2_1", str))
  {
    t2_state = true;
    // active
  }
  else if (!strcmp("t2_0", str))
  {
    t2_state = false;
    // active
  }
  else if (!strcmp("t3_1", str))
  {
    t3_state = true;
    // active
  }
  else if (!strcmp("t3_0", str))
  {
    t3_state = false;
    // active
  }
  Serial.print("Target State = ");
  Serial.print(t1_state);
  Serial.print(t2_state);
  Serial.println(t3_state);
}



void setup()
{
  Serial.begin(115200);
  pinMode(pin_t1, OUTPUT);
  pinMode(pin_t2, OUTPUT);
  pinMode(pin_t3, OUTPUT);

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
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failure on connect (");
      Serial.print(client.state());
      Serial.println(")");
      delay(2000);
    }
  }

  client.subscribe(MyTOPIC);
}

void loop()
{
  digitalWrite(pin_t1, t1_state);
  digitalWrite(pin_t2, t2_state);
  digitalWrite(pin_t3, t3_state);
  if (WiFi.status() != WL_CONNECTED) 
  {
    WIFI_Reconnect();
  }
  if (!client.connected()) 
  {
    MQTT_Reconnect();
  }
  client.loop();
}
