
#include "display.h"
#include <stdio.h>

#include <pgmspace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Fonts/FreeMono9pt7b.h"
#include <Wire.h>
#include <Arduino.h>

static char line_space   = 4;
static char font_height  = 9;
static char current_row  = 0;

//Define lopcal functions
static void displaySplash(void);
static void i2cscan(void);
static unsigned char getRow(char row);
static void clr_row(char row);

//Init the display SSD1306
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1);

void initOLED(void){

	if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

	//Set the display params
	display.setRotation(2); 
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);  
	display.setFont(&FreeMono9pt7b);

	//Set contrast: 0-255(Recommended: 127)  
	display.ssd1306_command(SSD1306_SETCONTRAST);
	display.ssd1306_command(0); // 0-255

	//Display loading message
	display.setCursor(0, font_height); //row 1
	display.print("Starting...");
	display.display();
	delay(1000);
	
	//Display the status bar
	display.clearDisplay();
	display.drawFastHLine(0, 9, DISPLAY_WIDTH, 1);
	display.display();
	upd_statusBar(0,0);

	//Draw the rest of rows
	/*display.setCursor(0, getRow(2));
	display.print("Hello");
	display.display();
	delay(1000);

	clr_row(2);
	display.setCursor(0, getRow(2));
	display.print("World");
	display.display();
	delay(1000);*/

	//display.clearDisplay();
	//i2cscan();
}

void upd_statusBar(int batt, int voltage) {
	
	display.setFont();
	display.setCursor(0, 0);
		
	//Clear status bar field	
	for(int i=0; i<7; i++){
		display.drawFastHLine(0, i, DISPLAY_WIDTH, 0);
	}

	//Update status bar value
	display.printf("Batt:%d%%|Vcc:%dmV",batt, voltage);
	display.display();

	//Set back the previous font
	display.setFont(&FreeMono9pt7b);
}

static void clr_row(char row){

	char pos = getRow(row-1)+line_space-1;
	
	for(int i=pos; i<(pos+font_height+line_space); i++){
		display.drawFastHLine(0, i, DISPLAY_WIDTH, 0);
	}
}

static unsigned char getRow(char row){
	return row>1 ? (font_height*row + ((row-1)*line_space)) : (font_height*row);
}

void display_fontSize(int size){
	display.setTextSize(size);
}

void display_print(const char* str){

	//Set the cursor at the last known position
	/*display.setCursor(0, row_coords[current_row]);
	display.fillRect(0,  row_coords[current_row], DISPLAY_WIDTH, font_height, SSD1306_BLACK);

	display.setCursor(0, 10);
	display.print(str);
	display.display();*/
}

void display_printRow(const char* str, int row){
	
	clr_row(row);
	display.setCursor(0, getRow(row));
	display.print(str);
	display.display();
}

void display_printAt(const char* str, int x, int y){

	//USe a static 
	static int16_t x1=-1, y1;
	static uint16_t w, h;

	current_row = y;
	y = getRow(y);

	//Function called for the first time
	if(x1 == -1){
		display.getTextBounds(str, x, y, &x1, &y1, &w, &h);
		display.fillRect(x1, y1, w, h, SSD1306_BLACK);
	}
	else{
		display.fillRect(x1, y1, w, h, SSD1306_BLACK);
		display.getTextBounds(str, x, y, &x1, &y1, &w, &h);
	}
	
	display.setCursor(0, y);
	display.print(str);
	display.display();
}


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
