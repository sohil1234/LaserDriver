#include "ADS8689.h"
#include <SPI.h>

// Create external object
ADS8689 adc; 

/* Defined Constants */
#define CS0 43              // chip Select
#define RVS 14              // status chip: not necessary if we need to save pins
#define RST 0               // reset Pin of the module - also not necessary

bool ADCprint = 0; 
bool ADCprintCont = 0; 

SPISettings ADC_SPI(SPI_CLOCK, MSBFIRST, SPI_MODE0);

/* Constructor */
ADS8689::ADS8689(){}

void ADS8689::initADC(){
  pinMode(CS0, OUTPUT);
  pinMode(RVS, INPUT); 
  pinMode(RST, OUTPUT); 
  // reset the ADC
  digitalWrite(RST,LOW); 
  delay(100); 
  digitalWrite(RST,HIGH); 
  SPI2.begin(); 
  ADCregisterSettings(READ_HWORD,RANGE_SEL_REG,0x00,0x00);    // read current register
  delay(100); 
  ADCregisterSettings(WRITE_LS_BYTE,RANGE_SEL_REG,0x00,0x0B); // write the correct internal reference voltage
  delay(100);
  initConstants(); 
  Serial.println("ADC initialized"); 
}


void ADS8689::printADC(bool _ADCprint, bool _ADCprintCont){
  ADCprint = _ADCprint; 
  ADCprintCont = _ADCprintCont;
}

double ADS8689::readADC(){
  digitalWrite(CS0,LOW);                  // begin by pulling the CS low;
  SPI2.beginTransaction(ADC_SPI);    
  msb = SPI2.transfer(0x00);              // not sending, just taking a reading
  lsb = SPI2.transfer(0x00); 
  SPI2.endTransaction();                  // end the transaction  
  digitalWrite(CS0,HIGH);                 // end the connection by toggling CS HIGH
  return ((msb*256)+lsb); 
}


void ADS8689::updateADC(){
  if(ADCprint || ADCprintCont){
    if(adcTimer <= SamplingTime){
      VAL = readADC();
      SUM += VAL; 
      count++; 
    }
    else{
      Voltage = ((SUM/count)/65535)*vREF; 
      Serial.println("The average voltage: "+String(Voltage,6)); 
      Serial.println("Samples: "+String(count)); 
      initConstants(); 
      ADCprint = 0; 
    }
  }
}


void ADS8689::initConstants(){
  SUM = 0; 
  adcTimer = 0; 
  count = 0; 
  VAL = 0; 
  Voltage = 0; 
}


double ADS8689::ADCregisterSettings(byte _RWcommand, byte _address, byte _Mcommand, byte _Lcommand ){  
  command = _RWcommand;
  registerAddress = _address;
  msb_command = _Mcommand; 
  lsb_command = _Lcommand; 

  switch(command){
    case CLEAR_HWORD:
      break;      
    case READ_HWORD:
      registerReadFlag = true; // raise flag to request a reading
      break; 
    case READ_BYTE:
      registerReadFlag = true; 
      break;
    case WRITE_TWO_BYTES: 
      break; 
    case WRITE_MS_BYTE:
      break;
    case WRITE_LS_BYTE:
      break;
    case SET_HWORD:    
      break; 
    default: 
      break; 
  }

  digitalWrite(CS0,LOW); 
  SPI2.beginTransaction(ADC_SPI); 
  SPI2.transfer(command); 
  SPI2.transfer(registerAddress);
  SPI2.transfer(msb_command);
  SPI2.transfer(lsb_command); 
  SPI2.endTransaction(); 
  digitalWrite(CS0,HIGH); 

  if(!registerReadFlag){
    return -1; 
  }
  else{ // we print here only if we ask for a reading
    delay(100); 
    digitalWrite(CS0,LOW); 
    SPI2.beginTransaction(ADC_SPI); 
    msb = SPI2.transfer(0x00); 
    lsb = SPI2.transfer(0x00); 
    SPI2.endTransaction(); 
    digitalWrite(CS0,HIGH); 
    VAL = msb * 256 + lsb;
    return VAL; 
  }
}
