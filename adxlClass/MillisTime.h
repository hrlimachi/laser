#ifndef Arduino_h
#include <Arduino.h>
#endif

class MillisTime{
  public:
    MillisTime(){
      lastTime = millis();
    }
    bool millisDelay(int timeMil=100);
  private:
    unsigned long lastTime;
};
