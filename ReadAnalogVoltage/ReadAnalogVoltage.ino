
#include "GasSensor.h"
#include "adc.h"
#include "bt.h"
#include "GasSensor.h"
#include "display.h"
#include <Wire.h>

//The setup routine runs once when you press reset:
void setup() {

  analogReadResolution(12); // Set 12bits ADC: 0–4095
  analogSetPinAttenuation(VCC_PIN, ADC_0db);
  analogSetPinAttenuation(NTC_PIN, ADC_11db);
  analogSetPinAttenuation(MQ_PIN, ADC_0db);
  
  Serial.begin(9600);

  int result = voltageCheck();

  if(result == 0 || result == 1){   
    //Start I2C
    Wire.begin();
    initOLED();
    bt_init();
    sensorInit();
  }
}

void loop() {

  static long time = millis();

  //Update sensors every second
  if(millis() - time > 1000){
    time = millis();

    upd_statusBar();
    displayTemp();
    displayAirQuality();

    btDriver();
  }
}

