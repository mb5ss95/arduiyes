#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char auth[] = "Au6rJx7ULw0OAA4k_JP8DNfbRlgRBpLV";
char ssid[] = "Moon";
char pass[] = "moonmoon";
int p1 = 33;
int p2 = 27;
int Ana_Val;
int curval, preval;

void setup()
{
  // Debug console
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
  
  Blynk.run();
}
