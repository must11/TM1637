
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}
#include <TM1637.h>
#include <Arduino.h>
static uint8_t encode(int);
const uint8_t shows[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001,     // F
  };
  static const uint8_t negative_dash = 0b01000000;
MyTM1637::MyTM1637(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay)
{
	// Copy the pin numbers
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;
	m_bitDelay = bitDelay;

  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO,OUTPUT);
	digitalWrite(m_pinClk, LOW);
	digitalWrite(m_pinDIO, LOW);
}


void MyTM1637::writeByte(uint8_t b)
{
    uint8_t data = b;
  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // Serial.println(data);
    digitalWrite(m_pinClk, LOW);
	// Set data bit
    if (data & 0x01)
    {
      digitalWrite(m_pinDIO, HIGH);
    }
    else {
        digitalWrite(m_pinDIO, LOW);
    }

    bitDelay();
    digitalWrite(m_pinClk, HIGH);
    data = data >> 1;
	// CLK high
    bitDelay();
  }

  digitalWrite(m_pinClk, LOW);
  bitDelay();
  digitalWrite(m_pinClk, HIGH);
  bitDelay();
  digitalWrite(m_pinClk, LOW);
  bitDelay();
}

void MyTM1637::start()
{
  digitalWrite(m_pinDIO, HIGH);
  digitalWrite(m_pinClk,HIGH);
  bitDelay();
  digitalWrite(m_pinDIO, LOW);
  //dio电平下降后必须持续一个时钟，才能拉低clk
  bitDelay();
}
void MyTM1637::stop()
{  

  digitalWrite(m_pinDIO, LOW);
  digitalWrite(m_pinClk,HIGH);

  bitDelay();
  digitalWrite(m_pinDIO, HIGH);
}
void MyTM1637::bitDelay()
{
    delayMicroseconds(m_bitDelay);
}

void MyTM1637::command1() {
    start();
    writeByte(TM1637_I2C_COMM1);
    stop();
}

void MyTM1637::allDisplay(void) //写显示寄存器 
{ 
    unsigned char i; 
    command1();
    start(); 
    writeByte(TM1637_I2C_COMM2); //设置首地址， 
    
    for(i=0;i<LED_LENGHT;i++) //地址自加，不必每次都写地址 
    { 
      writeByte(0xff); //送数据  
    } 
    stop(); 
    start(); 
    writeByte(TM1637_I2C_COMM3); //开显示 ，最大亮度 
    stop(); 
}


void MyTM1637::showNumberDec(int num, bool leading_zero)
{ 
  // if (num>9999 ||num<-999) {
  //  allDisplay();
   
  // }else {
    showNumberDecBase(num>0?num:-num,leading_zero,num>0?false:true);
  // } 
}
void MyTM1637::showNumberDecBase(int num, bool leading_zero,bool negative)
{
   int temp =num;
   uint8_t digits[LED_LENGHT];

   unsigned char i =LED_LENGHT -1;
   while (temp>0 &&i ==0) {
      digits[i] =encode(temp%10);
      Serial.println(i);
      Serial.print(digits[i]);
      temp = temp/10;
      i--;
   }
   if (negative) {
      digits[i]=negative_dash;
      i--;
   }else if (leading_zero && i>=0) {
        while (i>=0)
        {
          digits[i]=encode(0);
          i--;
        }       
  }
  if (i>=0) {
     while (i>=0)
        {
          digits[i]=0x00;
          i--;
        }
  }
  showDigit(digits);
}

uint8_t encode(int digit) {
  return shows[digit & 0x0f];
}

void MyTM1637::showDigit(const uint8_t digits[]) //写显示寄存器 
{ 
    unsigned char i; 
    command1();
    start(); 
    writeByte(TM1637_I2C_COMM2); //设置首地址， 
    
    for(i=0;i<LED_LENGHT;i++) //地址自加，不必每次都写地址 
    { 
      writeByte(digits[i]); //送数据  
    } 
    stop(); 
    start(); 
    writeByte(TM1637_I2C_COMM3); //开显示 ，最大亮度 
    stop(); 
}