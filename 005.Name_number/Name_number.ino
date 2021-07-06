#include <Wire.h> // i2C 통신을 위한 라이브러리 포함
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C 용 라이브러리 포함
LiquidCrystal_I2C lcd(0x27,16,2); // 접근주소: 0x3F or 0x27

void setup()
{
}

void loop()
{ 
  lcd.init(); // LCD 초기화
lcd.backlight(); // LCD 백라이트 켜기
delay(500);
lcd.setCursor(0,0); // 1번째 칸, 첫째 줄부터 글을 쓰기 시작
lcd.print("Moon Byung Soo");
delay(1000);
lcd.setCursor(0,1); // 1번째 칸, 둘째 줄부터 글을 쓰기 시작
lcd.print("010)8799-8046");
delay(1000);
}
