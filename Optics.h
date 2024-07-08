/*                     
  Optical Interface: 
    Includes both the TEC controller and the Laser Diode Controller
*/

#include "Arduino.h"
#include <elapsedMillis.h>

class Optics{
  public:
    Optics();                         // Constructor  
    void initOptics(void);            // initialize the optics interface. Called in setup. Must be done first.
    void updateOptics(void);          // in the main loop. This will update all parameters.
    void EnableVs(bool);              // enable/disable the VS power supply
    //-----------------------------------------TEC Controller
    void EnableTEC(bool);             // enables or disables the TEC Controller
    void setTEMP(int);                // Sets the LD TEC temperature;
      double VAP;                     // Holds the voltage from the Input Temperature using Polynomial
      double Vdiv;                    // converts the VAP value to small voltage
      int Vin;                        // converts to 8-bit voltage to set the pin. 
    float readTecTemperature(bool);   // read the temperature of the TEC controller
      double TEMP_10bit;              // 10bit read by the OBJ TEMP PIN
      double TEMP_Voltage;            // convert to 3.3 voltage Range
      double TEMP;                    // Output Temperature using fitted polynomial
    float readTecCurrent(bool);       // Reads the LD TEC current
      float ITEC_10bit;               // 10bit read by the ITEC MON PIN
      float ITEC_Voltage;             // Linear Conversion to 3.3 voltage range
      double ITEC;                    // Conversion to current using Fitted Polynomial
    //-----------------------------------------LD Controller
    void EnableLD(bool);              // Enables the Laser diode
    void setCurrent(int);             // sets the current at which the current runs.
    float readPD(bool);               // reads the pd Current
      float rPD = 0.4987;             // rPD current sense resistsor (in K_OHM)
      float iPD_10bit;                // 10bit read by the PMON pin
      float iPD_volt;                 // changed to 3.3 voltage range
      float iPD;                      // true current, multiply by 0.125
    float readLD(bool);               // read the LD current
      float Gain = 4.06;              // Amplifer gain = 1 + R2/R1
      float iLD_10bit;                // 10bit read by the ILAS pin
      float iLD_volt;                 // changed to 3.3 voltage range
      float iLD_current;              // changed to 0.8 voltage range. Gain: [4.06]
      float iLD;                      // true current, multiply by 0.125
  private:
    elapsedMillis OpticsTimer; 
    double TecI; 
    double TecT;
    double LiLAS;
    double LiPD;
};
extern Optics optic;
