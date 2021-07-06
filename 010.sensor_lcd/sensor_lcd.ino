#include <Servo.h>

int led=13; // 13번에 있는 led를 사용하기 위해 선언
int sensor=7; // 센서의 signal 선을 7번에 연결


void setup()

{
pinMode(13, OUTPUT); // 13번핀 출력으로 설정
pinMode(7, INPUT);   // 7번핀을 입력으로 설정
Serial.begin(9600);  // 통신속도 9600bps로 시리얼 통신 시작
}


void loop()
{
int val= digitalRead(sensor);

if(val==1)

{
digitalWrite(13,1);
Serial.println("감지 O");

delay(1000);
}

else
{
digitalWrite(13,0);
Serial.println("감지 X");
delay(1000);
}

}
