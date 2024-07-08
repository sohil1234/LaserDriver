//INA169.cpp

#include "Arduino.h"
#include "INA169.h"

// Create external object
INA169 ina;

/* Defined Constants */
#define ITOT_MON A17

bool INAprint  = 0; 
bool INAreport = 0; 

/* Constructor */
INA169::INA169(){}

void INA169::initINA(){
  pinMode(ITOT_MON, INPUT);
}


float INA169::readINA(bool _read){
    return ITOT = ((analogRead(ITOT_MON)/1023)*3.3); 
}


void INA169::printINA(bool _printINA, bool _reportINA){
  INAprint  = _printINA;
  INAreport = _reportINA; 
  INATimer = 0; 
}

void INA169::reportINA(){
  if( (INAprint || INAreport) & INATimer >= 200){
    iTOT = readINA(1); 
    Serial.println("Total Current: "+String(iTOT,4)+" A"); 
  }
}


float INA169::sendToWebServer(bool toSend){
  return readINA(toSend); 
}
