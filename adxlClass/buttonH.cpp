#include "ButtonH.h"
//#include "Arduino.h"

ButtonH::ButtonH(int pin)
{
  cont = 0;
  statePrevBut = LOW;
  millisTime = 0;  //last time
  delayMillis = 200;
  flag = false;
  pinButton= pin;
}
void ButtonH::initButton(){
  pinMode(pinButton,INPUT);
}
bool ButtonH::buttonGet(){
  bool currentStateButton = digitalRead(pinButton);
  flag = false;
  if(statePrevBut != currentStateButton){
    if(currentStateButton == HIGH){
      if((millis()- millisTime)>delayMillis){
        flag = true;
        statePrevBut = currentStateButton;
        millisTime = millis();
      }
    }
    else{
      statePrevBut = LOW;
    }
  }
  return flag;
}
