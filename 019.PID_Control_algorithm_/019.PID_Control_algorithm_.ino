#include <Servo.h>

Servo myservo;
int servopin = 9;
int echopin = 6;
int trigpin = 7;
int input, output = 0;              //입력, 출력
int setpoint, distance;             //목표, 거리
int servooutput, prev_control = 0;  //서보 제어값, 이전 제어값
int err, prev_err = 0;              //오차, 이전 오차
double i_err, d_err;                //오차적분, 오차미분
double p, i, d = 0.04; //p, i ,d항 값
double dt = 0.04;

void setup() 
{
Serial.begin(9600); //시리얼 통신 초기화
myservo.attach(servopin); //서보모터 핀번호 설정

input = readposition(); //막대 위의 공의 위치를 측정값 함수 호출

//초음파 센서 설정
pinMode(trigpin, OUTPUT);
pinMode(echopin, INPUT);
}

void loop() 
{
  //막대 중앙 위치(set point를 150mm로 설정
  setpoint = 17;

  //공의 위치 측정
  input = readposition();

  //제어값 산출
  output=mypid(input, output, setpoint);

  //서보 모터의 각도 설정(100도는 서보모터가 수평을 이루었을때 각도)
  servooutput=100+output;
  delay(50);
  myservo.write(servooutput);
}

int mypid(int intput, int output, int setpoint)
{
  float kp = 5; //offset 튜닝
  float ki = 3;
  float kd = 3;
  int control;

  err = setpoint - input; //오차 = 목표치 - 현재값
  p=kp*err;               //p항 = kp * 오차

  i_err += err * dt; //오차 적분 += 오차 * dt
  i =ki * i_err;    //i항 = ki * 오차 적분

  d_err = (err - prev_err) / dt; //오차 미분 = (오차 - 이전 오차) / dt
  d = kd * d_err;                //d항 = kd * 오차미분  

  prev_err = err; //현재 오차를 이전 오차로
  control = p + i + d;
  int temp= control;
  Serial.println(temp);
  control = map(temp, -600, 600, -70, 70); //제어값 산출
  
  

  //예외 처리
  if(control < -70)
  {
    control = -80;
  }
  else if(control > 70)
  {
    control = 80;
  }
  else if(control < 10 && control > -10)
  {
    control = prev_control;
  }
  prev_control = control;
  Serial.print("제어량 = ");
  Serial.println(control);
  return control;
}

int readposition()
{
  delay(40); //딜레이 설정

  //초음파 센서 거리 측정 부분
  digitalWrite(trigpin, 1);
  delayMicroseconds(10);
  digitalWrite(trigpin, 0);

  //mm단위로 계산 펄스 에코에서 읽어들이는 때까지 걸리는 시간
  distance = pulseIn(echopin, 1)*17/1000;

  //공의 측정 거리가 350mm 이상일 경우
  if(distance > 35)
  {
    distance = 35;
  }

  Serial.print("거리 = ");
  Serial.println(distance);

  return distance;
}
