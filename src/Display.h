/*
 * 	Display.h
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: Class providing all the display outputs.
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

//for a connecting display via I2C
#include <Wire.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

//navigation arrows
#include "symbols.h"


class Display
{
public:
	//Constructor:
	Display(SSD1306Wire* oled_);

	//Destructor:
	virtual ~Display();

	int counter;

	void init();
	void drawWelcomeScreen();
	void drawConnectionScreen();
	void drawNavigationScreen(uint8_t* direction, uint32_t* distance, const char* street);
	void displayOn();
	void displayOff();

private:
	SSD1306Wire* oled;

};

#endif /* SRC_DISPLAY_H_ */
