#include "MillisTime.h"

bool MillisTime::millisDelay(int timeMil) {
  bool flag = false;
  if ((millis() - lastTime) > timeMil) {
    lastTime = millis();
    flag = true;
  } else
    flag = false;
  return flag;
}
