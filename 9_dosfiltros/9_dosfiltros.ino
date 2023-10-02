//final hasta guardar datos
#include <Wire.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define ADXL345_ADDRESS 0x53
#define led 2
//millis adxl
//-----------
unsigned long adxlmillis = 0;
int adxlDelay = 50;
//---------
//boton
//---------------------------------------------------------------------------------------
int cont = 0;
RTC_DATA_ATTR int bootNum = 0;
char pru[13] = "";
int pinButton = 35;

bool statPrevBut = LOW;
unsigned long millisTime = 0;
//----------------------------------
//  VARIABLES PARA LOS FILTROS
//---------------------------------
float yout=0;
float yant=25;
float xin=0;
float xant=25;
float youtF=0;
float xint2=0;
float xN1=0;
float xN2=0;
float xN3=0;
//------------------------------------
void writeFile(fs::FS &fs, const char * path, const char * message) {

  File file = fs.open(path, FILE_WRITE);
  if (!file.print(message)) {
    //Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_APPEND);
  if (!file.print(message)) {
    // Serial.println("Append failed");
  }
  file.close();
}

bool botonFun(int pin, bool* statePreviousButton, unsigned long* lastTime ) {
  bool stateCurrentButton = digitalRead(pin);
  const int delayMillis = 200;
  bool flag = false;
  if (*statePreviousButton != stateCurrentButton) {
    if (HIGH == stateCurrentButton) {
      if ((millis() - *lastTime) > delayMillis) {
        //inicio
        flag = true;
        //final
        *statePreviousButton = stateCurrentButton;
        *lastTime = millis();
      }
    }
    else {
      *statePreviousButton = LOW;
    }
  }
  return flag;
}
//-----------------------------------------------------------------------------------------
class Adxl345 {
  public:
    Adxl345() {
    }
    void adxlBegin();
    void askAcel();
    float getX();
    float getY();
    float getZ();
    float aTotal();
  private:
    int16_t x;
    int16_t y;
    int16_t z;
};

void Adxl345::adxlBegin() {
  Wire.begin();
  delay(100);
  // Activar el ADXL345
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission();

  // Configurar el ADXL345
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x31);
  Wire.write(0x0B);
  Wire.endTransmission();

  // Establecer la tasa de muestreo
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2C);
  Wire.write(0x08);
  Wire.endTransmission();
}
void Adxl345::askAcel() {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.requestFrom(ADXL345_ADDRESS, 6);
  x = (Wire.read() | (Wire.read() << 8));
  y = (Wire.read() | (Wire.read() << 8));
  z = (Wire.read() | (Wire.read() << 8));
}
float Adxl345::getX() {
  return ((float)x)/10;
}
float Adxl345::getY() {
  return ((float)y)/10;
}
float Adxl345::getZ() {
  return ((float)z)/10;
}
float Adxl345::aTotal() {
  return sqrt(pow((((float)x)/10),2)+pow((((float)y)/10),2)+pow((((float)z)/10),2));
}

Adxl345 acel;

void setup() {
  //Serial.begin(115200);
  delay(10);
  acel.adxlBegin();

  pinMode(pinButton , INPUT);
  pinMode(led, OUTPUT);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 1); //1 = High, 0 = Low

  if (!SD.begin())
  {
    for (int i = 0; i <= 3; i++) {
      digitalWrite(led, HIGH);
      delay(250);
      digitalWrite(led, LOW);
      delay(250);
    }
    esp_deep_sleep_start();
  }

  bootNum++;
  snprintf(pru, sizeof(pru), "/data%d.txt", bootNum);

  writeFile(SD, pru, "x y z total\n");
  digitalWrite(led, HIGH);
}

void loop() {
  if ((millis() - adxlmillis) > adxlDelay) {
    adxlmillis = millis();
    acel.askAcel();
  //  float pastX = acel.getX();
  //  float pastY = acel.getY();
  //  float pastZ = acel.getZ();
    xin = acel.aTotal();
    yout = 0.2283*yant + 38.5870*xin - 38.587*xant;
    youtF = (yout + xN1 + xN2 + xN3)/4;
    yant = yout;
    xant = xin;

    xN3 = xN2;
    xN2 = xN1;
    xN1 = yout;
    
    //Serial.println(String(pastX) + " " + String(pastY) + " " + String(pastZ));
    char str[11] = "";
    snprintf(str, sizeof(str), "%0.2f\n", youtF);
    
    appendFile(SD, pru, str);

  }
  if (botonFun(pinButton , &statPrevBut, &millisTime)) {
    for (int j = 1; j <= bootNum; j++) {
      digitalWrite(led, LOW);
      delay(200);
      digitalWrite(led, HIGH);
      delay(200);
    }
    esp_deep_sleep_start();
  }
}
