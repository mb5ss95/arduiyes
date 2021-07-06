void setup() 
{   
  Serial.begin(9600);
  pinMode(8,OUTPUT);    // 10번 핀을 출력 핀으로 설정
}

void loop() 
{   
  if(analogRead(A5)<70)
  {
  digitalWrite( 8,1);   
  Serial.println("on");
  delay(5000); 
  } 
  else
  {
  digitalWrite( 8,0);  
  Serial.println("off"); 
  }
   Serial.println(analogRead(A5));
}
