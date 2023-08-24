


#include <Wire.h>
#include "buttonH.h"
#include "Adxl345.h"

#define led 2
#define pin 15
//millis adxl
//-----------
//unsigned long adxlmillis = 0;
//int adxlDelay = 500;
//---------
//boton
//---------------------------------------------------------------------------------------
//RTC_DATA_ATTR int bootNum = 0;
int cont = 0;
//
Adxl345 acel;
ButtonH onButton(pin);
BlinkLed tryLed(led);
MillisTime sDelay;

void setup() {
  Serial.begin(115200);
  delay(10);
  acel.adxlBegin();
  onButton.initButton();
  tryLed.initLed();
  //  bootNum++;
  //  Serial.println("numero de boot: " + String(bootNum));

  //  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1); //1 = High, 0 = Low

  //  Serial.println("deep sleep");
  //Serial.flush();
  Serial.println("comenzando");
  int c = 0;
  while (true) {
    if (c == 0) {
      Serial.println(c);
      Serial.println("esperando y brillando");
      c++;
      Serial.println(c);
    }
    if (tryLed.light(1000, true, 10)) {
      Serial.println("termine de brillar");
      break;
    }
  }
  Serial.println("ya salí");
  delay(5000);
}

void loop() {
  //  if (sDelay.millisDelay(500)) {
  //    acel.askAcel();
  //    int pastX = (int)acel.getX();
  //    int pastY = (int)acel.getY();
  //    int pastZ = (int)acel.getZ();
  //    Serial.println(String(pastX) + " " + String(pastY) + " " + String(pastZ));
  //  }
  tryLed.light(1000);
  if (onButton.buttonGet()) {
    Serial.println(cont);
    cont++;
    //    Serial.println("A dormir.............");
    //    delay(200);
    //    esp_deep_sleep_start();


  }
}
