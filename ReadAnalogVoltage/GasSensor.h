 
#ifndef __GAS_SENSOR_H__
#define __GAS_SENSOR_H__
  
  #define MQ_PIN  A3

  #define MQ_DATA_INVALID   0xFFFF

  int sensorInit(void);
  int displayAirQuality(void);
  unsigned short getAqData(void);
  
#endif
