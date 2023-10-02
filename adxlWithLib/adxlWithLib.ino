#include "ButtonH.h"
#include "Adxl345.h"
#include "SDLib.h"


#define led 2
#define BUTTON_ACT 35

RTC_DATA_ATTR int bootNum = 0;
char pru[13] = "";

SDLib memory;
Adxl345 acelerometer;
BlinkLed ledBlue(led);
ButtonH comandButton(BUTTON_ACT);
MillisTime timer1;


void setup() {
  Serial.begin(115200);
  delay(500);
  acelerometer.adxlBegin();
  ledBlue.initLed();
  comandButton.initButton();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 1);

  if (!SD.begin())
  {
    ledBlue.delayLight(250, 3);
    esp_deep_sleep_start();
  }
  bootNum++;
  snprintf(pru, sizeof(pru), "/data%d.txt", bootNum);
  memory.writeFile(SD, pru, " x y z\n");
  ledBlue.ledOn();
}
void loop() {
  if (timer1.millisDelay(50)) {
    acelerometer.askAcel();
    int pastX = acelerometer.getX();
    int pastY = acelerometer.getY();
    int pastZ = acelerometer.getZ();
    char str[19] = "";
    snprintf(str, sizeof(str), "%d %d %d\n", pastX, pastY, pastZ);
    Serial.print(str);
    memory.appendFile(SD, pru, str);
  }
  if (comandButton.buttonGet()) {
    ledBlue.delayLight(250, bootNum);
    esp_deep_sleep_start();
  }
}
