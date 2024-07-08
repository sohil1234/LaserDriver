#include "Arduino.h"
#include <elapsedMillis.h>

class INA169
{
  public:
  //constructor
  INA169();
  
  void initINA(void);
  float readINA(bool); 
  void printINA(bool,bool); 
  void reportINA(void); 
  float sendToWebServer(bool);
  float iTOT;
  
  private:
    elapsedMillis INATimer;
    float ITOT;
};
extern INA169 ina;
