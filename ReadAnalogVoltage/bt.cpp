

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "bt.h"
#include "GasSensor.h"
#include "display.h"

  static int bt_status = 0;

  int bt_init(void){

    Serial.print("Initializing Bluetooth....");
    return bt_status;
  }
  
  int getBtStatus(void){
    bt_status=!bt_status;
    return bt_status;
  }

  void btDriver(void){
    
    unsigned short aq = getAqData();
    float ntc  = getNTCVal();
    float temp = getTemp();
    int humid  = getHumid();
	  
    int bat_level = battLevel(readVoltage());
    int voltage   = readVoltage();

    
  }

