#include "SDlib.h"

bool SDLib::writeFile(fs::FS &fs, const char * path, const char * message) {
  flag = true;
  File file = fs.open(path, FILE_WRITE);
  if (!file.print(message)) {
    flag = false;
  }
  file.close();
  return flag;
}

bool SDLib::appendFile(fs::FS &fs, const char * path, const char * message) {
  flag = true;
  File file = fs.open(path, FILE_APPEND);
  if (!file.print(message)) {
    flag = false;
  }
  file.close();
  return flag;
}
bool SDLib::initSD(){
  return SD.begin();
}
bool SDLib::getStat(){
  return flag;
}
