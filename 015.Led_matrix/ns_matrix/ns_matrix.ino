#include <NS_Rainbow.h>

#define PIN 9
#define N_CELL 64



// Parameter 1 = number of cells (max: 512)
// Parameter 2 = Arduino pin number (default pin: 9)
//NS_Rainbow ns_matrix = NS_Rainbow(N_CELL);
NS_Rainbow ns_matrix = NS_Rainbow(N_CELL,PIN);

void setup() {
  delay(100);
  ns_matrix.begin();
  ns_matrix.clear();
  ns_matrix.show();
  ns_matrix.setBrightness(100);  // range: 0 ~ 255
  Serial.begin(9600);
}

void loop() {
  byte           cycle = 1; // cycles of all colors on wheel
  unsigned int   t = 1000;   // t: delay time
   
  int r = random(255);
  int g = random(255);
  int b = random(255);  
 // for(byte i=0; i<ns_matrix.numCells(); i++) {

    ns_matrix.setColor(9, r,  g,   b);
    ns_matrix.setColor(10, r,  g,   b);
    ns_matrix.setColor(13, r,  g,   b);
    ns_matrix.setColor(14, r,  g,   b);
    
    ns_matrix.setColor(16, r,  g,   b);
    ns_matrix.setColor(17, r,  g,   b);
    ns_matrix.setColor(18, r,  g,   b);
    ns_matrix.setColor(19, r,  g,   b);
    ns_matrix.setColor(20, r,  g,   b);
    ns_matrix.setColor(21, r,  g,   b);
    ns_matrix.setColor(22, r,  g,   b);
    ns_matrix.setColor(23, r,  g,   b);
    
    ns_matrix.setColor(24, r,  g,   b); 
    ns_matrix.setColor(25, r,  g,   b);
    ns_matrix.setColor(26, r,  g,   b);
    ns_matrix.setColor(27, r,  g,   b);
    ns_matrix.setColor(28, r,  g,   b); 
    ns_matrix.setColor(29, r,  g,   b);
    ns_matrix.setColor(30, r,  g,   b);
    ns_matrix.setColor(31, r,  g,   b);
    
    ns_matrix.setColor(33, r,  g,   b);
    ns_matrix.setColor(34, r,  g,   b);
    ns_matrix.setColor(35, r,  g,   b);
    ns_matrix.setColor(36, r,  g,   b);
    ns_matrix.setColor(37, r,  g,   b);
    ns_matrix.setColor(38, r,  g,   b);

    ns_matrix.setColor(42, r,  g,   b);
    ns_matrix.setColor(43, r,  g,   b);
    ns_matrix.setColor(44, r,  g,   b);
    ns_matrix.setColor(45, r,  g,   b);

    ns_matrix.setColor(51, r,  g,   b);
    ns_matrix.setColor(52, r,  g,   b);
    ns_matrix.show();
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("g = ");
    Serial.println(g);
    Serial.print("b = ");
    Serial.println(b);
    delay(1250);
}
    
    
  //}
 /* 
  for(int i=0; i<2; i++) {
    rainbow(10);      // interval: 10ms
  } 
  
  ns_matrix.clear();
  ns_matrix.show();
  delay(t); 
}

void rainbow(uint16_t interval) {
  uint16_t n = ns_matrix.numCells();

  for(uint16_t j=0; j<255; j++) {  // one cycle 
    for(uint16_t i=0; i<n; i++) {
      byte r_pos = (((i<<8) -  i) / n + j) % 0xFF;
      byte sect = (r_pos / 0x55) % 0x03, pos = (r_pos % 0x55) * 0x03;
  
      switch(sect) {
      case 0: 
        ns_matrix.setColor(i,ns_matrix.RGBtoColor(0xFF - pos, pos, 0x00)); break;
      case 1: 
        ns_matrix.setColor(i,ns_matrix.RGBtoColor(0x00, 0xFF - pos, pos)); break;
      case 2:
        ns_matrix.setColor(i,ns_matrix.RGBtoColor(pos, 0x00, 0xFF - pos)); break;
      }  
    }
    ns_matrix.show();
    delay(interval);
  }
}*/
