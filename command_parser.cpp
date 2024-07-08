/* Local Includes */
#include "SerialCommand.h"
#include "command_parser.h"
#include "ADS8689.h"
#include "INA169.h"
#include "Optics.h"

// Create external object
Commandparser COMparser;

/* Class Initializations */
SerialCommand serialParser;

//serial Parameters
#define BEGIN_CHAR                "{"
#define END_CHAR                  '}'

/* Constructor */
Commandparser::Commandparser() {}

//should be called as often as possible to parse incoming serial
void Commandparser::checkSerial() {
  //reads in new serial chars
  serialParser.readSerial();
  return;
}

//converts a long of the form "10101010" into 0b10101010
byte longToByte(long input) {
  //int to keep track of output
  int out = 0;

  //loop across lowest 8 numbers (8 "bits")
  for (uint8_t i = 0; i < 8; i++) {
    if ((input % 10) == 1) {        //if ones place is 1
      out = (out | (1 << i));       //set output bit at current place
    }
    input = (input / 10);           //shift input by one place right
  }
  return out;
}

//converts a long of the form "10101010" into 0b10101010
byte longToShort(long input) {
  //int to keep track of output
  int out = 0;

  //loop across lowest 4 numbers (4 "bits")
  for (uint8_t i = 0; i < 4; i++) {
    if ((input % 10) == 1) {        //if ones place is 1
      out = (out | (1 << i));       //set output bit at current place
    }
    input = (input / 10);           //shift input by one place right
  }
  return out;
}

//called upon serial receive
void Commandparser::parseCommand() {
  char *arg;
  bool parseDone = 0;
  long arguments[9];
  uint8_t num_arguments = 0;

  //loop over max number of arguments
  for (int i = 0; i < 10; i++) {
    //take in a new argument string
    arg = serialParser.next();

    //if null string, we have no end character
    if (*arg == NULL) { //change to null
      Serial.println(F("Malformed syntax (too long or no end char)"));
      break;
    }
    //if end character, we're done, set parseDone flag
    else if (*arg == END_CHAR) {
      //Serial.println(F("Command ACK"));
      parseDone = 1;
      break;
    }
    //if no end character and we've exhausted argument buffer, command is too long or there is no end terminator
    else if ((i == 9)) {
      Serial.println(F("Command too long!"));
      break;
    }
    //if we're here, we have a new argument to process
    else {
      //process first character as command character
      if ( i == 0) {
        arguments[i] = (char) * arg;
      }
      //process subsequent commands as either strings or longs
      else if (isAlpha((int) *arg)) {
        arguments[i] = (long) * arg;
      } else {
        arguments[i] = atol(arg);
      }
      //increment number of arguments for command length checking later
      num_arguments++;
    }
  }

  //if parseDone is true, we have received a command with beginning and end terminator
  if (parseDone) {

    //the command identifier is the first argument
    char identifier = (char)arguments[0];

//Begin the Commands Here =====================================================

    // handle ADC query (read once and print)
    if(identifier == 'A' && num_arguments == 1){
      adc.printADC(1,0); 
    }
    // handle ADC Read/Report ON/OFF
    else if(identifier == 'A' && num_arguments == 2){
      adc.printADC(0,bool(arguments[1])); 
    }
    // handle Current Monitor Reporting ON/OFF
    else if(identifier == 'i' && num_arguments == 2){
      //OMParser.EnableINA(0,(bool)arguments[1]); 
    }
    // handle Vs Enable ON/OFF
    else if (identifier == 'O' && arguments[1] == 'E' && num_arguments == 3){
      optic.EnableVs((bool)arguments[2]);
    }
    // handle TEC Enable ON/OFF
    else if (identifier == 'O' && arguments[1] == 'T' && arguments[2] == 'E' && num_arguments == 4){
      optic.EnableTEC((bool)arguments[3]);
    }
    // handle TEC Temperature Set [15-45] input
    else if (identifier == 'O' && arguments[1] == 'T' && arguments[2] == 'T' && num_arguments == 4){
      optic.setTEMP((int)arguments[3]);
    }
    // handle Laser Enable ON/OFF
    else if (identifier == 'O' && arguments[1] == 'L' && arguments[2] == 'E' && num_arguments == 4){
      optic.EnableLD((int)arguments[3]);
    }
    // handle Laser Current Set [0-500]
    else if (identifier == 'O' && arguments[1] == 'L' && arguments[2] == 'I' && num_arguments == 4){
      int current = (int)arguments[3];
      if (current >= 0 && current <= 500) { // check for valid range 0-500
        optic.setCurrent(current);
      } else {
        Serial.println(F("Error: 0-500 [mA] required"));
      }
    }

//END the Commands Here =======================================================

    //else, we have a bad command, print command.
    else {
      Serial.print(F("Malformed command: { "));
      //Serial.print((char)identifier);
      Serial.print(F(" "));
      for (int i = 1; i < num_arguments; i++) {
        //print out the arguments
        Serial.print(arguments[i], DEC);
        Serial.print(F(" "));
      }
      Serial.println(F("}"));

    }
    return;
  }
}

//gets called upon malformed character
//i.e. command does not start with '{'
void Commandparser::unrecognized(const char *command) {
  Serial.println(F("Unrecognized command"));
  return;
}


//should be called upon startup
void Commandparser::setupSerialParser() {
  //setup serial preamble
  serialParser.addCommand(BEGIN_CHAR, COMparser.parseCommand);
  serialParser.setDefaultHandler(COMparser.unrecognized);
  //return;
}
