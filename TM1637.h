/*
 * 我用的数码管左边第一位是0xC0,

*/

#ifndef __TM1637__
#define __TM1637__
#include <inttypes.h>
#include <Arduino.h>
#include <string.h>
#define DEFAULT_BIT_DELAY  100
#define LED_LENGHT  4 
#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80
class TM1637 {
      public:
        TM1637(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY); 
        void writeByte(uint8_t b);
        void start();
        void stop();
        void allDisplay();
          //!
  //! @param num  需要显示的数字，支持正数4位，负数3位
  //! @param leading_zero 正数补零，true 左边空位显示0，false:左边不显示，
      void showNumberDec(int num, bool leading_zero = false);
    private:
        uint8_t m_pinClk;
        uint8_t m_pinDIO;
        unsigned int m_bitDelay;

        void bitDelay();
        void showNumberDecBase(int num, bool leading_zero,bool negative);
        void showDigit(const uint8_t digit[]);
        //设置数据
        void command1();
};

#endif // __TM1637__