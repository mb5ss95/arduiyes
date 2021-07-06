#include <Servo.h>
#include <IRremote.h>

int recvPin = 11;
int pos=0;

IRrecv irrecv(recvPin); // IR리모콘 사용을 위한 클래스 생성
decode_results decodedSignal;  // IR리모콘으로부터 받은 데이터
Servo myservo;

void setup() 
{
  Serial.begin(9600); 
  irrecv.enableIRIn(); 
  myservo.attach(2);
}

void loop() 
{
  myservo.write(pos);
if (irrecv.decode(&decodedSignal) == true)
{
    Serial.println(decodedSignal.value);
    switch (decodedSignal.value) {
 

      case 0xFFE01F:
      pos-=45;
      break; //VOL-
      
      case 0xFFA857:  
      pos+=45;
      break; //VOL+
      
      case 0xFF906F: 
      pos=0;
      break; //EQ

    }
    irrecv.resume();  // IR 다음 데이터 수신
}
}
