#include "buttonH.h"
#include "Arduino.h"

buttonH::buttonH(int pin)
{
  cont = 0;
  statePrevBut = LOW;
  millisTime = 0;  //last time
  delayMillis = 200;
  flag = false;
  pinButton= pin;
}
void buttonH::initButton(){
  pinMode(pinButton,INPUT);
}
bool buttonH::buttonGet(){
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
