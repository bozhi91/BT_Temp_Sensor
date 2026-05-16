#include "tempSensor.h"
#include "display.h"
#include <Wire.h>

int readVoltage(void);
void displayVoltage(void);
int battLevel(int v);

int readVoltage(void){
  int vcc = analogReadMilliVolts(VCC_PIN);
  return vcc*2;
}

//The setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);

  analogReadResolution(12); // Set 12bits ADC: 0–4095
  analogSetPinAttenuation(VCC_PIN, ADC_0db);
  analogSetPinAttenuation(NTC_PIN, ADC_11db);

  //Start I2C
  Wire.begin();         
  initOLED();
}

void loop() {

  static long time = millis();

  if(millis() - time > 1000){
    time = millis();
    displayVoltage();
    displayTemp();
  }
}

void displayVoltage(void){

  char data[10];
  int vcc = readVoltage();

  sprintf(data, "V:%d%%", battLevel(vcc));
  display_printAt(data, 0,40);
}

int battLevel(int v) {

  int v_min = 3000;
  int v_max = 4200;

  if (v >= v_max) return 100;
  if (v <= v_min) return 0;

  return (v - v_min) * 100 / (v_max - v_min);
}
