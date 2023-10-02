#ifndef FS_h
  #include "FS.h"
#endif
#ifndef SD_h
  #include "SD.h"
#endif
#ifndef SPI_h
  #include "SPI.h"
#endif

class SD_lib{
  public:
  SD_lib(){}
  void writeFile(fs::FS &fs, const char * path, const char * message);
  void appendFile(fs::FS &fs, const char * path, const char * message);
  private:
  bool flag=false;
};
