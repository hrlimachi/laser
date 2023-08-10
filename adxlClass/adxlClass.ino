#include <Wire.h>
#define ADXL345_ADDRESS 0x53
#define led 2
//millis adxl
//-----------
unsigned long adxlmillis = 0;
int adxlDelay = 500;
//---------
//boton
//---------------------------------------------------------------------------------------
RTC_DATA_ATTR int bootNum = 0;
int pin=15; //15

class Adxl345 {
  public:
    Adxl345() {
    }
    void adxlBegin();
    void askAcel();
    int16_t getX();
    int16_t getY();
    int16_t getZ();
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
  Wire.write(0x0A);
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
int16_t Adxl345::getX() {
  return x;
}
int16_t Adxl345::getY() {
  return y;
}
int16_t Adxl345::getZ() {
  return z;
}
//-----------------------------------------------------------------------------------------
class button{
  public:
    button(int pin);
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

button::button(int pin)
{
  cont = 0;
  statePrevBut = LOW;
  millisTime = 0;  //last time
  delayMillis = 200;
  flag = false;
  pinButton= pin;
}
void button::initButton(){
  pinMode(pinButton,INPUT);
}
bool button::buttonGet(){
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
//-----------------------------------------------------------------------------------------

Adxl345 acel;
button onButton(pin);

void setup() {
  Serial.begin(115200);
  delay(10);
  acel.adxlBegin();
  onButton.initButton();
  bootNum++;
  Serial.println("numero de boot: " + String(bootNum));

  pinMode(led, OUTPUT);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1); //1 = High, 0 = Low

  Serial.println("deep sleep");
  Serial.flush();
  digitalWrite(led, HIGH);
}

void loop() {
  if ((millis() - adxlmillis) > adxlDelay) {
    acel.askAcel();
    int pastX = (int)acel.getX();
    int pastY = (int)acel.getY();
    int pastZ = (int)acel.getZ();
    Serial.println(String(pastX) + " " + String(pastY) + " " + String(pastZ));
    adxlmillis = millis();

  }
  if (onButton.buttonGet()) {
    //Serial.println(cont);
    //cont++;
    Serial.println("A dormir.............");
    delay(200);
    esp_deep_sleep_start();


  }
}
