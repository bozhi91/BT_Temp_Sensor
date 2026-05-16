#include "display.h"
#include <stdio.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>


//Define lopcal functions
static void displaySplash(void);
static void i2cscan(void);

//Init the display SSD1306
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1);

static void i2cscan(void){

 	unsigned char error, address;
  int devices = 0;

  Serial.println("Escaneando...");

  for (address = 1; address < 127; address++) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      	Serial.print("Dispositivo I2C encontrado en: ");
      	Serial.printf("0x%X, ",address<<1);
        devices++;
    }
    else if (error == 4) {
      Serial.print("Error desconocido en 0x");

      if(address < 16){
  		  Serial.printf("0x%X, ",address);
			}
    }
  }

  if (devices == 0)
    Serial.println("No se encontraron dispositivos I2C\n");
  else
    Serial.println("Escaneo terminado\n");

}

extern unsigned char logo[];
void initOLED(void){

	if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

	display.setRotation(2); 

  // Title screen
  display.clearDisplay();             
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0,0);
  display.setTextSize(2);                // Medium
	display.print("Starting..");
	display.display();

	//i2cscan();
}

void display_print(const char* str){

	display.setCursor(0,0);
	display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
  display.print(str);
	display.display();
}

void display_printAt(const char* str, int x, int y){
	
  display.setCursor(x, y);
	display.fillRect(0, y, 128, 16, SSD1306_BLACK);
  display.print(str);
	display.display();
}

