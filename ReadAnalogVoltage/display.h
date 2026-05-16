/*
 * display.h
 *
 *  Created on: Sep 24, 2025
 *      Author: bozhi
 */

#ifndef INC_PERIPHERALS_DISPLAY_H_
#define INC_PERIPHERALS_DISPLAY_H_

	#define DISPLAY_WIDTH   128
	#define DISPLAY_HEIGHT  64
	#define DISPLAY_ADDR    0x3c //The address is shifted 1 bit to the left

	void display_print(const char* str);
	void display_printAt(const char* str, int x, int y);
	void initOLED(void);

#endif /* INC_PERIPHERALS_DISPLAY_H_ */
