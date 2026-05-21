 
#ifndef __TEMP_SENSOR_H__
#define __TEMP_SENSOR_H__

  #define NTC_PIN  A0  
  #define VCC_PIN  A1
  #define DHT_11   A2

  #define VCC         3.3    //Reference voltage
  #define ADC_MAX     4095.0 //Max ADC value(12 bits)
  #define MIN_VOLTAGE 3200   //Minimal Battery voltage 

  // --- NTC PARAMS ---
  #define R0 10800.0    
  #define T0 298.15    
  #define BETA 3950.0
  #define N 30
  #define R_FIXED  1000000.0  //1Mohm resistor for the NTC voltage divider

  //ADC filters
  float movingAverage(float newVal);
  float readADC_avg(int samples, int pin);
  float ema(float newVal);

  void displayTemp(void);
  float getTemperatureC(void);
  
  int readVoltage(void);
  void displayVoltage(void);
  int battLevel(int v);
  int voltageCheck(void);

  float getNTCVal(void);
  float getTemp(void);
  int getHumid(void);

#endif