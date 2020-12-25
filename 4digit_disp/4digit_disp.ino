
#include <TM1637.h>

int CLK = 12;
int DIO = 13;

TM1637 tm(CLK,DIO);

void setup() {

  tm.init();
  tm.set(2);
  Serial.begin(9600);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  tm.display(0,1);
  tm.display(1,4);
  tm.point(0);
  tm.display(2,8);
  tm.display(3,8);
}
