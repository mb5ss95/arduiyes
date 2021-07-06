#include <Wire.h> // i2C 통신을 위한 라이브러리 포함
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C 용 라이브러리 포함
#include <IRremote.h>
LiquidCrystal_I2C lcd(0x27,16,2); // 접근주소: 0x3F or 0x27

int RECV_PIN = 11; // IR 수신기 D11에 연결
int k=0;
IRrecv irrecv(RECV_PIN); // IR리모콘 사용을 위한 클래스 생성
decode_results decodedSignal; // IR리모콘으로부터 받은 데이터
void setup() {
Serial.begin(9600); // 시리얼 통신 시작
irrecv.enableIRIn(); // 리모콘 수신 시작
lcd.init();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Press the botton");
}


void loop() 
{
if (irrecv.decode(&decodedSignal) == true){ // IR 수신값이 있는지 판단.
    Serial.println(decodedSignal.value);
    lcd.cursor(); 
    lcd.blink();       
    lcd.setCursor(k,1);
    switch (decodedSignal.value) {
 
//CH-
case 0xFFA25D: lcd.print(decodedSignal.value); break;
//CH
case 0xFF629D: lcd.print(decodedSignal.value); break;
//CH+
case 0xFFE21D: lcd.print(decodedSignal.value); break;
//PREV
case 0xFF22DD: lcd.print(decodedSignal.value); break;
//NEXT
case 0xFF02FD: lcd.print(decodedSignal.value); break;
//PLAY
case 0xFFC23D: lcd.print(decodedSignal.value); break;
//VOL-
case 0xFFE01F: k--; break;
//VOL+
case 0xFFA857: k++; break;
//EQ
case 0xFF906F: lcd.clear(); lcd.print("Press the botton"); k=0; lcd.setCursor(k,1); lcd.blink(); break;
//0
case 0xFF6897: lcd.print(0); k++;  break;
//100+
case 0xFF9867: lcd.print(decodedSignal.value); k++; break;
//200+
case 0xFFB04F: lcd.print(decodedSignal.value); k++; break;
//1
case 0xFF30CF: lcd.print(1); k++; break;
//2     
case 0xFF18E7: lcd.print(2); k++; break;
//3     
case 0xFF7A85: lcd.print(3); k++; break;
//4     
case 0xFF10EF: lcd.print(4); k++; break;
//5     
case 0xFF38C7: lcd.print(5); k++; break;
//6     
case 0xFF5AA5: lcd.print(6); k++; break;
//7
case 0xFF42BD: lcd.print(7); k++; break;
//8
case 0xFF4AB5: lcd.print(8); k++; break;
//9
case 0xFF52AD: lcd.print(9); k++; break;
     
   
    }
    irrecv.resume();  // IR 다음 데이터 수신
}
}
