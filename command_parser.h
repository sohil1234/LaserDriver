/*
   This file contains the headers for serial parsing functions.
   The functions included are:
      checkSerial()       -should be called as often as possible to take in new serial bytes
      parseCommand()      -parses incoming serial strings
      setupSerialParser() -should be called at startup, initializes serial comms.

*/

class Commandparser {
  public:
    Commandparser();                // Constructor
    void setupSerialParser(void);
    void checkSerial(void);
    static void parseCommand(void);
    static void TECCommand(void);
    static void unrecognized(const char*);
};
extern Commandparser COMparser; 
