
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "GasSensor.h"
#include "display.h"

#define MIN_REF_VOLTAGE   1000
#define MAX_REF_VOLTAGE   2600

static int map(int x, int in_min, int in_max,int out_min,int out_max);

static int status = 0;
int sensorInit(void){

  //TODO: DO A CALIBRATION HERE...
  Serial.println(" Initializing Co2 sensor...");
  return status;
}

unsigned short getAqData(void){

  unsigned short sensor = analogReadMilliVolts(MQ_PIN);

  if(sensor>4096){
    sensor = MQ_DATA_INVALID;
  }
  else{
    sensor = (100-map(sensor, MIN_REF_VOLTAGE, MAX_REF_VOLTAGE, 0, 100));
  }

  Serial.printf(" Air Quality: %d %% \n", sensor);
  return sensor;
}

/***
  Displays the air quality in %.
  This module is using the MQ135 sensor, which detects a different types of gas such as:
  CO2, Methane, smoke, propane, etc.

  To detect the air quality, read the input voltage at MQ_PIN.
  The voltage range is: 0-2.5v. 
  
  In a clean room, the voltage reading of the MQ_PIN is around 1v-1.2v.
  In a highly contaminated space, the voltage could reach around 2.5v.

  These values are used as a reference and mapped as values between 0-100%
*/
int displayAirQuality(void){

  char data[10];

  sprintf(data, "Air Q.:%d%%", getAqData());
  display_printRow(data, 4);
  
  return status;
}

int map(int x, int in_min, int in_max,int out_min,int out_max){
  return (x - in_min) *(out_max - out_min) / (in_max - in_min) + out_min;
}
