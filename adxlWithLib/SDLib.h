#ifndef FS_h
#include "FS.h"
#endif
#ifndef SD_h
#include "SD.h"
#endif
#ifndef SPI_h
#include "SPI.h"
#endif

class SDLib{
  public:
    SDLib(){}
    bool writeFile(fs::FS &fs, const char * path, const char * message);
    bool appendFile(fs::FS &fs, const char * path, const char * message);
    bool initSD();
    bool getStat();
  private:
    bool flag=true;
};
