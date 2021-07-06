#include <Wire.h> // i2C 통신을 위한 라이브러리 포함
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C 용 라이브러리 포함
LiquidCrystal_I2C lcd(0x27,16,2); // 접근주소: 0x3F or 0x27

// this constant won't change:
const int  Up_buttonPin   = 2;    // the pin that the pushbutton is attached to
const int  Down_buttonPin = 3;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int up_buttonState = 0;         // current state of the up button
int up_lastButtonState = 0;     // previous state of the up button

int down_buttonState = 0;         // current state of the up button
int down_lastButtonState = 0;     // previous state of the up button
bool bPress = false;

void setup()
{
  Serial.begin(9600);
  pinMode( Up_buttonPin , INPUT_PULLUP);
  pinMode( Down_buttonPin , INPUT_PULLUP);
 
  lcd.init();                      // initialize the lcd
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Please Select:");
  lcd.setCursor(2,1);
  lcd.print(buttonPushCounter);
 
}


void loop()
{
   checkUp();
   checkDown();

   if( bPress){
       bPress = false;
      lcd.setCursor(2,1);
      lcd.print("               ");
      lcd.setCursor(2,1);
      lcd.print(buttonPushCounter);
   }
  
}

void checkUp()
{
  up_buttonState = digitalRead(Up_buttonPin);
 
  if (up_buttonState != up_lastButtonState) {
  
    if (up_buttonState == LOW) {
        bPress = true;
    
      buttonPushCounter++;

      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    }
   
    delay(50);
  }

  up_lastButtonState = up_buttonState;
}


void checkDown()
{
  down_buttonState = digitalRead(Down_buttonPin);

  if (down_buttonState != down_lastButtonState) {
    
    if (down_buttonState == LOW) {
        bPress = true;
     
      buttonPushCounter--;
      
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    }
   
    delay(50);
  }
 
  down_lastButtonState = down_buttonState;
}
