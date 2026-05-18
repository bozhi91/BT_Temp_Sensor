
#include "display.h"
#include "bt.h"
#include "adc.h"

#include <stdio.h>
#include <stdlib.h>
#include <pgmspace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Fonts/FreeMono9pt7b.h"
#include <Wire.h>
#include <Arduino.h>

static char line_space   = 4;
static char font_height  = 9;
static char current_row  = 0;
static char status_bar_height = 10;

//Define lopcal functions
static void displaySplash(void);
static void i2cscan(void);
static unsigned char getRow(char row);
static void clr_row(char row);

//Init the display SSD1306
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1);

//Initialize and configure the display. This function must be called first
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
	display.drawFastHLine(0, status_bar_height, DISPLAY_WIDTH, 1);
	display.display();
	upd_statusBar();
}

/**
	Display a relevant device status data such as battery level, bluetooth connection status, etc.
*/
void upd_statusBar(void) {
	
	int bat_level = battLevel(readVoltage());
  int voltage   = readVoltage();

	display.setFont();
	display.setCursor(0, 0);
		
	//Clear status bar field	
	for(int i=0; i<status_bar_height; i++){
		display.drawFastHLine(0, i, DISPLAY_WIDTH, 0);
	}

	//Update status bar values
	drawBMP(5, 0, 14, 8, battery_bmp, 1, 0);
	display.setCursor(22, 0);
	display.printf("%d%%| ", bat_level);

	//Display the Bluetooth icon. 
	//Display a normal icon if connected. Display a crossed out icon otherwise.

	drawBMP(50, 0, 9, 9, bt_bmp, 1, 0);

	if(getBtStatus()==0){
		display.drawLine(51, 9, 61, 0, 1);
	}
	
	display.display();

	//Set back the previous font
	display.setFont(&FreeMono9pt7b);
}

/**Delete the row's content by a given row id.
  @Params: 
	- row: The row number. Only values between 1 and 4 allowed
*/
static void clr_row(char row){

	char pos = getRow(row-1)+line_space-1;
	
	for(int i=pos; i<(pos+font_height+line_space); i++){
		display.drawFastHLine(0, i, DISPLAY_WIDTH, 0);
	}
}

//Returns the row 'Y' coordinates by a given row id
static unsigned char getRow(char row){
	return row>1 ? (font_height*row + ((row-1)*line_space)) : (font_height*row);
}

void display_fontSize(int size){
	display.setTextSize(size);
}

/**
	Write a text on a given row. The previous content will be deleted.

	@Params:
	-	str: The new text to be displayed
	- row: the row id(values between 1-4)
*/
void display_printRow(const char* str, int row){
	
	clr_row(row);
	display.setCursor(0, getRow(row));
	display.print(str);
	display.display();
}

void display_printAt(const char* str, int x, int y){
/*
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
	*/
	display.setCursor(x, y);
	display.print(str);
	display.display();
}


//Draw a monochromatic bitmap on the screen using the format RGB-565
void drawBMP(unsigned char startX, unsigned char  startY, unsigned char width, unsigned char  height,
  const unsigned char* bitmap, unsigned char color, unsigned short bg_color) {

  int x = 0, y = 0;

  for(int idx = 0; x < width; idx++){
    for(int i = 0; i<8; i++){ //Split the byte in bits. Draw a pixel for each bit.

      //Draw a single pixel.
      display.drawPixel(x + startX, y + startY, (bitmap[idx] >>i) & 1 ? color : bg_color);
      y++;

      //The height of the image is reached. Move to the next row
      if(y == height){
        if(height%8 == 0){//The end of the current byte is reached. Move to the next one.
          idx++;
        }
        x++;
        y = 0;
        break;
      }
    }
  }
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
