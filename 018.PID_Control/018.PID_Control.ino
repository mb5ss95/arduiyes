#include <PID_v1.h>
#include <Servo.h>

Servo myServo;
int servoPin = 9;        
int echoPin = 6;                 
int trigPin = 7;

float Kp = 2.5;                //P게인 값
float Ki = 0;                  //I게인 값 
float Kd = 1;                  //D게인 값
double Setpoint, Input, Output, ServoOutput;                                       

float duration, distance;

//PID객체 생성
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);   

void setup() {
  Serial.begin(9600);                  
  myServo.attach(servoPin);            

  //막대 위의 공의 위치를 측정값 함수 호출
  Input = readPosition();             
  
  //초음파 센서 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);                                                                  

  //PID모드를 AUTOMATIC으로 설정
  myPID.SetMode(AUTOMATIC);
  //PID의 값을 최소 -80부터 최대 80까지 설정               
  myPID.SetOutputLimits(-80,80);          
}

void loop() {
  //막대 중앙 위치(Set Point를 15cm로 설정)
  Setpoint = 20;      
  //공의 위치 측정           
  Input = readPosition();                                          

  //PID계산 
  myPID.Compute();                       

  //서보모터의 각도 설정(100도는 서보모터가 수평을 이루었을 때 각도)
  ServoOutput=100+Output;             
  //서보모터에게 값 전달    
  myServo.write(ServoOutput);            
}


float readPosition() {
  delay(40);                             

  //초음파 센서 거리 측정 부분
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); 
  distance = ((float)(340 * duration) / 10000) / 2;  


  if(distance > 30) {                   
    distance=30;
  }

  Serial.println(distance);             

  return distance;                     
}
