#ifndef Arduino_h
#include "Arduino.h"
#endif

class BlinkLed{
  public:
    BlinkLed(int pin);
    void initLed();
    bool light(int delayMillis, bool countFlag = false, int times = 1);
    void ledOn();
    void ledOff();
    void delayLight(int delayTime,int times);
  private:
    unsigned long millisTime = millis();
    bool newState = false;    //
    int pinLed;
    int counter;
};
