#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"


#define DHTPIN 0
#define DHTTYPE DHT11

char auth[] = "Au6rJx7ULw0OAA4k_JP8DNfbRlgRBpLV";
char ssid[] = "Moon";
char pass[] = "moonmoon";
int p1 = 33;
int p2 = 32;
int Ana_Val;
int curval, preval;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Debug console
  dht.begin();
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  delay(5000);
  Blynk.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}
