#include <WiFi.h>
#include <PubSubClient.h>


// Wifi id, password
const char* SSID = "Moon";
const char* PASS = "moonmoon";

// sever, topic, port
const char* SERVER = "192.168.137.234";
const char* TOPIC = "topic_lolin1";
const int PORT = 1883;


WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length)
{
  char str[128];
  int temp_t1, temp_t2, temp_t3;
  memset(str, 0x00, sizeof(str));

  Serial.print(topic);
  Serial.print(" -> ");

  for (int i = 0; i < length; i++)
  {
    str[i] = (char)payload[i];
    Serial.print(str[i]);
  }
  Serial.println();

  if (!strcmp("all", topic))
  {
    char buffer[5];
    memset(buffer, 0x00, sizeof(buffer));
    client.publish(TOPIC, buffer);
  }
}


void setup()
{
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WIFI...");
  }
  Serial.println("Connected!!!");

  client.setServer(SERVER, PORT);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("Connected!!!");
    }
    else
    {
      Serial.print("Failure on connect (");
      Serial.print(client.state());
      Serial.println(")");
      delay(2000);
    }
  }
  client.subscribe(TOPIC);
}

void loop()
{
  /*



  code start


  */
  client.loop();
}
