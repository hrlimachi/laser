#ifndef ButtonH_h
#define ButtonH_h
#ifndef Arduino_h
#include <Arduino.h>
#endif
#include "BlinkLed.h"
#include "MillisTime.h"

class ButtonH{
  public:
    ButtonH(int pin);
    void initButton();
    bool buttonGet();
  private:
    int cont;
    bool statePrevBut;
    unsigned long millisTime;
    int delayMillis;
    bool flag;    //
    int pinButton;
};
#endif
