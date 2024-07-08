
/* Local Includes */
#include "command_parser.h"
#include "ADS8689.h"
#include "INA169.h"
#include "Optics.h"
#include "WebServer.h"

#define SERIAL_BAUD     115200  // baud rate of serial comms

void setup() {
   optic.initOptics();                    // initialize the Optics Interface - must be done first.
   Serial.begin(115200);                  // setup Serial Communication
   while(!Serial) {}                      // Wait until Serial COMS are established
   adc.initADC();                         // initialize ADS8689 ADC
   COMparser.setupSerialParser();  // initialize the communication parser
   ina.initINA();               // initialize Current Sensor
   Serial.println(F("Laser Board Firmware V1.0"));     // Give the Green Light
}

void loop() {
  adc.updateADC();                  //update ADS8689 ADC reading
  COMparser.checkSerial();   //handle Command requests
  ina.reportINA();        //handle Current Reporting
  optic.updateOptics();  //handle the Optical Interface 
}
