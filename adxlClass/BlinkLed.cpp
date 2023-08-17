#include "blinkLed.h"

BlinkLed::BlinkLed(int pin)
{
  newState = false;
  millisTime = 0;
  pinLed = pin;
  counter = 0;
}
void BlinkLed::initLed(){
  pinMode(pinLed,OUTPUT);
}
bool BlinkLed::light(int delayMillis, bool countFlag, int times){
  if((millis()- millisTime)>delayMillis/2){
    digitalWrite(pinLed,newState);
    newState = !newState;
    millisTime = millis();
    counter = countFlag ? counter+1 : counter;
    if (countFlag && counter == times){
      counter = 0;
      return true;
    }
  }
  return false;
}


void BlinkLed::ledOn(){
  digitalWrite(pinLed,true);
  newState = true;
}
void BlinkLed::ledOff(){
  digitalWrite(pinLed,false);
  newState = false;
}
bool BlinkLed::countBlink(int delayMillis,int times){
  
}
