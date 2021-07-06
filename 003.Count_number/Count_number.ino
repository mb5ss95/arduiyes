#include <Wire.h> // i2C 통신을 위한 라이브러리 포함
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C 용 라이브러리 포함
LiquidCrystal_I2C lcd(0x27,16,2); // 접근주소: 0x3F or 0x27
int i=0;
void setup() 
{
   lcd.init(); // LCD 초기화
  lcd.backlight(); // LCD 백라이트 켜기
  delay(1000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("count"); lcd.print(" start ");
delay(1000);
}
void loop()
{
  lcd.clear();
  lcd.print("counting");
  lcd.setCursor(0, 1);
  lcd.print(i);
delay(1000);
  lcd.clear();
  i+=1;
  }
 
