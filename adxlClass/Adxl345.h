#ifndef Adxl345_h
#define Adxl345_h
#ifndef Wire_h
#include <Wire.h>
#endif

#ifndef Arduino_h
#include <Arduino.h>
#endif
#define ADXL345_ADDRESS 0x53
class Adxl345 {
  public:
    Adxl345(){}
    void adxlBegin();
    void askAcel();
    int getX();
    int getY();
    int getZ();
  private:
    int16_t x;
    int16_t y;
    int16_t z;
};
#endif
