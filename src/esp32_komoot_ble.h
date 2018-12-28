/*
 * 	esp32_komoot_ble.h
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: Main file of esp32_komoot_ble_dev Arduino ESP32 software.
 *  Project created in Eclipse Sloeber.
 */

#ifndef _esp32_komoot_ble_dev2_H_
#define _esp32_komoot_ble_dev2_H_
#include "Arduino.h"

#include "Connection.h"
#include "Display.h"

//We are already including them in Display class
//#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
//#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"

void IRAM_ATTR isr();

#endif /* _esp32_komoot_ble_dev2_H_ */
