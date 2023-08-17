#include "Adxl345.h"
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
