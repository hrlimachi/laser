#include "SD_lib.h"

void SD_lib::writeFile(fs::FS &fs, const char * path, const char * message) {

  File file = fs.open(path, FILE_WRITE);
  if (!file.print(message)) {
    //Serial.println("Write failed");
  }
  file.close();
}

void SD_lib::appendFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_APPEND);
  if (!file.print(message)) {
    // Serial.println("Append failed");
  }
  file.close();
}
