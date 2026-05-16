
#include <Arduino.h>
#include "tempSensor.h"
#include "DHT11.h"
#include "display.h"

DHT11 dht(DHT_11);

void displayTemp(void){

  float temp = getTemperatureC();
  char data[20] = {0};

  sprintf(data, "NTC:%.1fC", temp);
  display_printAt(data, 0, 20);

  int temperature = 0;
  int humidity    = 0;

  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht.readTemperatureHumidity(temperature, humidity);
  
  if(result == 0){
    sprintf(data, "T%d.%dC|H%d", temperature/10, temperature%10, humidity);
    display_print(data);

    Serial.printf("Temp DHT11: %d.%dºC | Humid: %d %% \n", temperature/10, temperature%10, humidity);
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

float alpha = 0.1; // menor = más estable
float filtered = 0;
float ema(float newVal) {
    filtered = alpha * newVal + (1 - alpha) * filtered;
    return filtered;
}
//////////////////////////////////////// FILTERS ////////////////////////////////////////
