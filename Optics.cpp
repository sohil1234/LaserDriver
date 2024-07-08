#include "Arduino.h"
#include "Optics.h"

// Create external object
Optics optic;

/* Defined Constants */
#define TEMP_ALM_PIN  24
#define TEC_EN_PIN    25
#define LD_EN_PIN     26 
#define ILAS_MON_PIN  A9
#define VS_EN_PIN     22
#define PD_MON_PIN    A7
#define LD_SET_PIN    A22
#define TEMP_SET_PIN  A21
#define ITEC_MON_PIN  A16
#define OBJ_TEMP_PIN  A15

bool report_optics_data;
bool print_optics_data;  

/* Constructor */
Optics::Optics(){}

void Optics::initOptics(){
  pinMode(TEMP_ALM_PIN, INPUT);
  pinMode(TEC_EN_PIN, OUTPUT); 
  pinMode(LD_EN_PIN, OUTPUT); 
  pinMode(ILAS_MON_PIN, INPUT);
  pinMode(VS_EN_PIN, OUTPUT); 
  pinMode(PD_MON_PIN, INPUT); 
  pinMode(LD_SET_PIN, OUTPUT);
  pinMode(TEMP_SET_PIN, OUTPUT);
  pinMode(ITEC_MON_PIN, INPUT);
  pinMode(OBJ_TEMP_PIN, INPUT);
  // setting the constant pins 
  digitalWrite(TEC_EN_PIN, LOW);  // disable the Tec on startup 
  analogWrite(TEMP_SET_PIN, 126); // set to 8-bit 126 = approx 25 C on startup
  digitalWrite(VS_EN_PIN,LOW);    // Low to turn OFF the Vs
  digitalWrite(LD_EN_PIN,HIGH);   // High to turn OFF the LD
  analogWrite(LD_SET_PIN,0);      // Turn off laser. 
}


void Optics::EnableVs(bool _enVs){
  digitalWrite(VS_EN_PIN, (_enVs*255)); 
  if (_enVs){
    Serial.println("VS Enabled");
  }
  else{
    Serial.println("VS Disabled"); 
  }
}


void Optics::updateOptics(){
  if (OpticsTimer >= 1000){
    double TecI = readTecCurrent(1); 
    double TecT = readTecTemperature(1); 
//    double LiLDv = readLD();
//    double LiPDv = readPD(); 
//    Serial.println("Tec Current: "+String(TecI));
//    Serial.println("Tec Temperature: "+String(TecT)); 
//    Serial.println("LD Current: "+String(LDiLAS,4)+" mA");  // print the iLD with 4 decimal places
//    Serial.println("PD Current: "+String(LDiPD, 4)+" mA");  // print the iPD with 4 decimal places

    iPD_10bit  = analogRead(PD_MON_PIN);      // [0-1023]
    iPD_volt = (iPD_10bit/1023)*3.3;          // convert to 3.3 voltage range
    iPD = (iPD_volt/(2*rPD));                 // convert to mA

    iLD_10bit = analogRead(ILAS_MON_PIN);     //[0-1023]
    iLD_volt = (iLD_10bit/1023)*3.3;          //convert to voltage.
    iLD_current = iLD_volt/Gain;              //convert to 0.8 voltage range using Gain. 
    iLD = (iLD_current * 0.125)*1000; // convert to mA
    
    //Serial.println(String(TecI,4)+";"+String(TecT)+";"+String(iLD_volt)+";"+String(iLD,4)+";"+String(iPD_volt)+";"+String(iPD,6));
    OpticsTimer = 0; 
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          TEC CONTROLLER                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void Optics::EnableTEC(bool _enTEC){
  digitalWrite(TEC_EN_PIN, (_enTEC * 255));
}


void Optics::setTEMP(int Tin){
  if( Tin >= 15 && Tin <= 45) {
  VAP = 0.000076373626374*Tin*Tin - 0.017719230769231*Tin + 1.138901098901099; //will give you corresponding voltage from temperature
  Vdiv = VAP/0.4534883721;                                                     //will linearly convert from small voltage to output of microcontroller
  Vin = round(Vdiv*(255)/(3.3)); //convert to 8 bit
  analogWrite(TEMP_SET_PIN, Vin);  
  }
  else {
    Serial.println(F("Error: 15 - 45 [C] req"));
    return;
  }
}


float Optics::readTecTemperature(bool _readTecT){
  if(_readTecT){
    TEMP_10bit = analogRead(OBJ_TEMP_PIN);   // [0-1023]
    TEMP_Voltage = TEMP_10bit*(3.3/1023);    // [0-3.3]V
    TEMP = 2.507833437255981*(TEMP_Voltage)*(TEMP_Voltage) +  10.780033211497653*(TEMP_Voltage) +   2.284436416645446; // Convert to Celsius
    return TEMP;
  }
  else{
    return -1; 
  }
}


float Optics::readTecCurrent(bool _readTecI){
  if(_readTecI){
    ITEC_10bit = analogRead(ITEC_MON_PIN); // [0-1023]
    ITEC_Voltage = ITEC_10bit*(3.3/1023);  // [0-3.3]V
    ITEC = (ITEC_Voltage-1.5)/0.48;        // Convert to A
    return ITEC; 
  }
  else{
    return -1; 
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           LD Controller                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void Optics::EnableLD(bool _enLD){
  digitalWrite(LD_EN_PIN, !(_enLD*255));
  if (!_enLD){
    setCurrent(0);
    Serial.println("LD Disabled"); 
  }
  else {
    Serial.println("LD Enabled");
  }
}


void Optics::setCurrent(int iIN){
    Serial.println("CURRENT SET TO: "+String(iIN));
    analogWrite(LD_SET_PIN, iIN);
}


float Optics::readPD(bool _readPD){
  if(_readPD){
    iPD_10bit  = analogRead(PD_MON_PIN);  // [0-1023]
    iPD_volt = (iPD_10bit/1023)*3.3;      // convert to 3.3 voltage range
    iPD = (iPD_volt/(2*rPD));      // convert to mA
    //Serial.println("iPD: "+String(iPD));
    return iPD*1000; 
  }
  else{
    return -1; 
  }
}


float Optics::readLD(bool _readLD){
  if(_readLD){
    iLD_10bit = analogRead(ILAS_MON_PIN);    //[0-1023]
    iLD_volt = (iLD_10bit/1023)*3.3;         //convert to voltage.
    iLD_current = iLD_volt/Gain;             //convert to 0.8 voltage range using Gain. 
    iLD = (iLD_current * 0.125)*1000; // convert to mA
    return iLD;
  }
  else{
    return -1; 
  }
}
