#include <Wire.h> // i2C 통신을 위한 라이브러리 포함
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C 용 라이브러리 포함
#include <IRremote.h>
LiquidCrystal_I2C lcd(0x27,16,2); // 접근주소: 0x3F or 0x27

#define RECV_PIN 11 // IR 수신기 D11에 연결
#define r 2
#define g 5
#define b 4

int nv[]={0, 0, 0};//r, g, b
int i=0;
const char *gr[]={"HELLO THE WORLD", "Mode - Red", "Mode - Green", "Mode - Blue"};

IRrecv irrecv(RECV_PIN); // IR리모콘 사용을 위한 클래스 생성
decode_results decodedSignal; // IR리모콘으로부터 받은 데이터

void setup() {
Serial.begin(9600); // 시리얼 통신 시작
irrecv.enableIRIn(); // 리모콘 수신 시작
lcd.init();
lcd.backlight();     
lcd.setCursor(0,0);
lcd.print(gr[0]);
}

    void mode()
    {
      if (i==0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(gr[1]); 
      }
      else if(i==1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(gr[2]);
        }
        else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(gr[3]);
        }
        lcd.setCursor(2,1);
      }

      void clear()
      {
        lcd.clear();
        mode();
      }


void goswitch(){    
  switch (decodedSignal.value) 
  {
    if(decodedSignal.value>0)
    {

      case 0xFFA25D: i=0; mode(); Serial.println(nv[i]); lcd.print(nv[i]); break; //CH-
      case 0xFF629D: i=1; mode(); Serial.println(nv[i]); lcd.print(nv[i]); break;//CH
      case 0xFFE21D: i=2; mode(); Serial.println(nv[i]); lcd.print(nv[i]); break;//CH+
      
      case 0xFF22DD:  Serial.println(nv[i]); lcd.print(nv[i]); break; //PREV
      case 0xFF02FD:  Serial.println(nv[i]); lcd.print(nv[i]);  break; //NEXT
      case 0xFFC23D:  Serial.println(nv[i]); lcd.print(nv[i]);  break; //PLAY
      
      case 0xFFE01F: nv[i]-=10; Serial.println(nv[i]); lcd.print(nv[i]);  break; //VOL-
      case 0xFFA857: nv[i]+=10; Serial.println(nv[i]); lcd.print(nv[i]);  break; //VOL+
      case 0xFF906F: clear(); nv[i]=0; Serial.println(nv[i]); lcd.print(nv[i]);    break; //EQ
      
      case 0xFF6897: nv[i]-=100; Serial.println(nv[i]); lcd.print(nv[i]);   break; //0
      case 0xFF9867: nv[i]+=100; Serial.println(nv[i]); lcd.print(nv[i]);   break; //100+
      case 0xFFB04F: nv[i]+=200; Serial.println(nv[i]); lcd.print(nv[i]);   break; //200+
      
      case 0xFF30CF: lcd.clear(); nv[0]=0; nv[1]=0; nv[2]=0; lcd.setCursor(0,0); lcd.print("MOON BYUNG SOO"); lcd.setCursor(0,1); lcd.print("!!Thank You!!"); break; //1
      case 0xFF18E7: Serial.println(nv[i]); lcd.print(nv[i]);  //2
      case 0xFF7A85: Serial.println(nv[i]); lcd.print(nv[i]);  //3

    }
    }}

void loop() 
{
    analogWrite(r, nv[0]);
    analogWrite(g, nv[1]);
    analogWrite(b, nv[2]);
if (irrecv.decode(&decodedSignal) == true) // IR 수신값이 있는지 판단.
{ 
    lcd.setCursor(2,1);
    goswitch();
    irrecv.resume(); // IR 다음 데이터 수신
    }
}
