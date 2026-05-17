#include "adc.h"
#include "display.h"
#include <Wire.h>

//The setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);

  analogReadResolution(12); // Set 12bits ADC: 0–4095
  analogSetPinAttenuation(VCC_PIN, ADC_0db);
  analogSetPinAttenuation(NTC_PIN, ADC_11db);

  voltageCheck();

  //Start I2C
  Wire.begin();
  initOLED();
}

void loop() {

  static long time = millis();

  //Update sensors every second
  if(millis() - time > 1000){
    time = millis();
    displayVoltage();
    displayTemp();
  }
}

