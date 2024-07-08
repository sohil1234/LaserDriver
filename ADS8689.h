#include <elapsedMillis.h>

/* Input Commands */
#define CLEAR_HWORD       0b11000000
#define READ_HWORD        0b11001000
#define READ_BYTE         0b01001000
#define WRITE_TWO_BYTES   0b11010000
#define WRITE_MS_BYTE     0b11010010
#define WRITE_LS_BYTE     0b11010100
#define SET_HWORD         0b11011000

/* REGISTER NAME          Address       Function */
#define DEVICE_ID_REG       0x00     // Device ID register
#define RST_PWRCTL_REG      0x04     // Reset and power control register
#define SDI_CTL_REG SDI     0x08     // data input control register
#define SDO_CTL_REG SDO     0x0C     // data input control register
#define DATAOUT_CTL_REG     0x10     // Output data control register
#define RANGE_SEL_REG       0x14     // Input range selection control register
#define ALARM_REG ALARM     0x20     // output register
#define ALARM_H_TH_REG      0x24     // ALARM high threshold and hysteresis register
#define ALARM_L_TH_REG      0x28     // ALARM low threshold register

/* SPI/Sampling Settings */
#define SPI_CLOCK         4000000    // spi clock frequency
#define SamplingTime      5       // how often the adc is read. This affects the number of averages (initially every 5ms)
#define vREF              5.13       // Internal ADC refference voltage.(V)


class ADS8689{
  public:
    ADS8689(); 

    void initADC(void);                               // initialize the ADC
    void printADC(bool,bool);                         // decide whether we want to print adc or not
    double readADC(void);                             // reads the adc and returns the 16 bit value
    void updateADC(void);                             // adc averaging
    void initConstants(void);                         // initialize the constant values; 
    double ADCregisterSettings(byte,byte,byte,byte);  // R/W operation of various registers.  
    
  private: 
    byte msb, lsb, command, msb_command,lsb_command, registerAddress; 
    bool registerReadFlag = false;   // Flag goes high when reading, LOW when writing
    uint8_t _transmit_bytes[4];
    double VAL, Voltage,SUM,average, RegisterReading; 
    int count; 
    elapsedMillis adcTimer; 
    byte a,b;
};

extern ADS8689 adc; 
