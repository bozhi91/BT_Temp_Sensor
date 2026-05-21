#include "HardwareSerial.h"

#include <Arduino.h>
#include "adc.h"
#include "DHT11.h"
#include "display.h"

DHT11 dht(DHT_11);

  static float temperature = 0;
  static int humidity      = 0;
  static float NTC         = 0;

float getNTCVal(void){
  return NTC;
}

float getTemp(void){
  return temperature;
}

int getHumid(void){
  return humidity;
}

void displayTemp(void){

  //Read data from the NTC
  float NTC = getTemperatureC();
  char data[20] = {0};
  int temp;

  sprintf(data, "NTC:%.1fC", NTC);
  display_printRow(data, 3);

  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht.readTemperatureHumidity(temp, humidity);
  
  if(result == 0){
    temperature = temp/10.0;

    sprintf(data, "T%.1fC|H%d%%", temperature, humidity);
    display_printRow(data, 2);
    Serial.printf("Temp DHT11: %.1fºC | Humid: %d %% \n", temperature, humidity);
  } 
  else{
   // Serial.printf("DHT11 read error: %d \n", result);
  }
}

// --- Promedio de ADC ---
float readADC_avg(int samples, int pin) {

    float sum = 0;
    for (int i = 0; i < samples; i++) {
      sum += analogRead(pin);
      delay(5);
    }
    return sum / samples;
}

// Read temperature in celsius
float getTemperatureC(void) {

    // 1. Read raw ADC value
    //float adc = readADC_avg(50, NTC_PIN);
    float adc = movingAverage(analogRead(NTC_PIN));

    // 2. Convert it to voltage
    float Vout = adc * (VCC / ADC_MAX);

    // 3. Get the NTC value
    float Rntc = R_FIXED * (Vout / (VCC - Vout));
    
    // 4. Validate the readings
    if (adc <= 1 || adc >= 4094) return NAN;  // Invalid reading. REturn NaN
    if (Rntc <= 200) return 150;              // muy caliente (límite superior)
    if (Rntc >= 50000) return -40;            // muy frío (límite inferior)

    // 5. Convert the ADC data to TempC
    float tempK = 1.0 / ( (1.0 / T0) + (1.0 / BETA) * log(Rntc / R0) );
    float tempC = tempK - 273.15;

    return tempC;
}

//////////////////////////////////////// FILTERS ////////////////////////////////////////
float buffer[N];
int idx = 0;
float movingAverage(float newVal) {
    
  buffer[idx] = newVal;
  idx = (idx + 1) % N;

  float sum = 0;
  for (int i = 0; i < N; i++) sum += buffer[i];
  return sum / N;
}

float alpha    = 0.1; // menor = más estable
float filtered = 0;
float ema(float newVal) {
    filtered = alpha * newVal + (1 - alpha) * filtered;
    return filtered;
}
//////////////////////////////////////// FILTERS ////////////////////////////////////////

//Read battery voltage in mV
int readVoltage(void){
  return  analogReadMilliVolts(VCC_PIN)*2;
}

/**
  Verify the current battery voltage before startup.
  If the battery level reaches the MIN_VOLTAGE which is usually 3000-3200mV the program won't bootup.
  In that case, the program will run an endless loop by blinking a LED that will indicate a system malfunction,
  which in that case is an insuficiend power supply levels.

  If the battery drops below 3000mV, this could damage it. So try not to fully discharge it.
*/
int voltageCheck(void){

  int result = 0;

  if (Serial) {
    Serial.print("USB CONNECTED ");
    result = 1;
  }
  else if(readVoltage() <= MIN_VOLTAGE){ //USB not connected and battery power is too low.
    result = 2;

    Serial.print(" BATTERY DEAD!!");
    pinMode(LED_BUILTIN, OUTPUT);
  
    while(1){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }

  return result;
}

int battLevel(int v) {

  int v_min = 3000;
  int v_max = 4200;

  if (v >= v_max) return 100;
  if (v <= v_min) return 0;

  return (v - v_min) * 100 / (v_max - v_min);
}

