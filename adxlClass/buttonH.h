//#ifndef _buttonH_h_
//#define _buttonH_h_
#include "Arduino.h"

class buttonH{
  public:
    buttonH(int pin);
    void initButton();
    bool buttonGet();
  private:
    int cont;
    bool statePrevBut;
    unsigned long millisTime;
    int delayMillis = 200;
    bool flag = false;    //
    int pinButton;
};
