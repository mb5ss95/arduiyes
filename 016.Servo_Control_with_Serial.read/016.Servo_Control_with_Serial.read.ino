#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
   myservo.write(0);
}

int readSerial(){
  String str = "";
  char ch;
  char toch[4];
  while(Serial.available() > 0){
    ch = Serial.read(); // Serial.parseInt() --> int로 가져오지만 enter도 가져옴
    str.concat(ch);
    delay(10);
  }
  str.substring(0,str.length()).toCharArray(toch, 4);
  int nr = atoi(toch);
  return nr;
}

void loop() {
  if(Serial.available()) {
    int num = readSerial();
    Serial.println(num);
    myservo.write(num);
  }
}
