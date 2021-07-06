#include <Wire.h> // i2C 통신을 위한 라이브러리 포함
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C 용 라이브러리 포함
#include <IRremote.h>
LiquidCrystal_I2C lcd(0x27,16,2); // 접근주소: 0x3F or 0x27

#define RECV_PIN 11 // IR 수신기 D11에 연결


IRrecv irrecv(RECV_PIN); // IR리모콘 사용을 위한 클래스 생성
decode_results decodedSignal; // IR리모콘으로부터 받은 데이터

void setup() {
Serial.begin(9600); // 시리얼 통신 시작
irrecv.enableIRIn(); // 리모콘 수신 시작
lcd.init();
lcd.backlight();
}

void goswitch(){    
  switch (decodedSignal.value) 
  {

      case 0xFFA25D:  break; //CH-
      case 0xFF629D:  break; //CH
      case 0xFFE21D:  break; //CH+
      case 0xFF22DD:  break; //PREV
      case 0xFF02FD:  break; //NEXT
      case 0xFFC23D:  break; //PLAY
      case 0xFFE01F:  break; //VOL-
      case 0xFFA857:  break; //VOL+
      case 0xFF906F:  break; //EQ
      case 0xFF6897:  break; //0
      case 0xFF9867:  break; //100+
      case 0xFFB04F:  break; //200+
      case 0xFF30CF:  break; //1
      case 0xFF18E7:  break; //2
      case 0xFF7A85:  break; //3
      case 0xFF10EF:  break; //4
      case 0xFF38C7:  break; //5
      case 0xFF5AA5:  break; //6
      case 0xFF42BD:  break; //7
      case 0xFF4AB5:  break; //8
      case 0xFF52AD:  break; //9
    }
    Serial.println(decodedSignal.value);
    lcd.print(decodedSignal.value);
    }

void loop() 
{

if (irrecv.decode(&decodedSignal) == true){ // IR 수신값이 있는지 판단.
    lcd.init();        
    lcd.setCursor(2,0);
    lcd.print("               ");
    lcd.setCursor(2,1);
    goswitch();
    irrecv.resume();  // IR 다음 데이터 수신

    }}
