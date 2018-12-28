/*
 * 	Display.cpp
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: Class providing all the display outputs.
 */

#include "Display.h"

Display::Display(SSD1306Wire* oled_)
	: counter(0), oled(oled_) 	//initialization list
{

}

Display::~Display()
{

}

void Display::init()
{
	//initialize OLED display
	oled->init();
	oled->flipScreenVertically();
	oled->setFont(ArialMT_Plain_10);
}

void Display::drawWelcomeScreen()
{
	// clear the display
	oled->clear();
	oled->displayOn();
	oled->setTextAlignment(TEXT_ALIGN_CENTER);
	oled->setFont(ArialMT_Plain_24);
	oled->drawStringMaxWidth(64,10,128, "Welcome");
	oled->setTextAlignment(TEXT_ALIGN_RIGHT);
	oled->setFont(ArialMT_Plain_10);
	oled->drawStringMaxWidth(128,50,128, "jstiefel Komoot BLE v1.0.0");
	oled->display();
}

void Display::drawConnectionScreen()
{
	// clear the display
	oled->clear();
	oled->setTextAlignment(TEXT_ALIGN_CENTER);
	oled->setFont(ArialMT_Plain_10);
	oled->drawStringMaxWidth(64,10,128, "Please start navigation in Komoot app");

	//int progress = (counter / 5) % 100;
	int progress = ((counter % 5)+1) * 20;
	// draw the progress bar
	oled->drawProgressBar(0, 50, 120, 10, progress);

	oled->display();
}

void Display::drawNavigationScreen(uint8_t* direction, uint32_t* distance, const char* street)
{
	oled->clear();
	oled->drawXbm(0, 0, 64, 64, nav_symbols[*direction]);
	//  oled->drawVerticalLine(64, 0, 64);
	//  oled->drawVerticalLine(65, 0, 64);
	oled->setTextAlignment(TEXT_ALIGN_LEFT);
	oled->setFont(ArialMT_Plain_10);
	oled->drawStringMaxWidth(67,41,61, street);
	//implement hyphenation
	oled->setTextAlignment(TEXT_ALIGN_RIGHT);
	oled->setFont(ArialMT_Plain_24);
	oled->drawStringMaxWidth(128,0,61, "In");
	oled->setTextAlignment(TEXT_ALIGN_RIGHT);
	oled->setFont(ArialMT_Plain_24);

	//using correct units:
	if (*distance>9999){
	  oled->drawStringMaxWidth(128,19,80, String("> km"));
	}
	else if (*distance>=1000){
	  oled->drawStringMaxWidth(128,19,80, String(*distance/1000) + String(".") + String(int(round(float(*distance % 1000)/100))) + String("km"));
	}
	else if (*distance<1000){
	oled->drawStringMaxWidth(128,19,61, String(int(round(float(*distance)/10)*10)) + String("m"));
	//display->drawStringMaxWidth(67,39,61, *distance);
	}
	oled->display();
}

void Display::displayOn()
{
	oled->displayOn();
}

void Display::displayOff()
{
	oled->displayOff();
}


