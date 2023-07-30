#include <Arduino.h>
#include <TM.1637h>

// Module connection pins (Digital Pins)
#define CLK 16
#define DIO 17

MyTM1637 display(CLK, DIO);

void setup()
{
     Serial.begin(9600);
}

void loop()
{  //点亮全部
  display.allDisplay();
  delay(1000);
  display.showNumberDec(1234);

}