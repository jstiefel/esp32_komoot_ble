/*
 * 	esp32_komoot_ble.cpp
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: Main file of esp32_komoot_ble Arduino ESP32 software.
 *  Project created in Eclipse Sloeber. Add corresponding libraries and ESP32 library.
 */

#include "esp32_komoot_ble.h"
//create SSD1306 object here and pass a pointer to Display class
SSD1306Wire oled(0x3c, 27, 33); //SDA pin 27, SCL pin 33
Display Display(&oled);

//Deep sleep:
bool _sleep=false;
unsigned long timedelay;

//BLE:
bool doConnect = false;
bool _update = false;
bool _initialConnect = false;
uint8_t pData_old[4];
unsigned long startTime; //Timer to read signal every 5s
unsigned long notifyTimeout; //Timer to check if no notifications anymore
unsigned long deviceTimeout; //Timer to check if we should scan again for other devices

void setup()
{
	//Display setup:
	Display.init();
	Display.drawWelcomeScreen();

	Serial.begin(115200);
	delay(1000);

	//Deep sleep setup:
	_sleep = false;
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
	pinMode(32, INPUT_PULLDOWN); //pin 34-39 do not offer internal resistors; This will only work if RTC peripherals
	//are on in sleep, otherwise use 10kOhm external resistor for pull down
	Serial.println("Wake up!");
	attachInterrupt(digitalPinToInterrupt(32), isr, RISING);

	//BLE:
	Serial.println("Starting Arduino BLE Client application...");
	notifyTimeout = millis();
	startTime = millis();
	deviceTimeout = millis();

	BLEDevice::setCustomGattcHandler(my_gattc_event_handler);  // before BLEDevice::init(); to get disconnect reason output

	BLEDevice::init("");

	pClient  = BLEDevice::createClient();
	Serial.println(" - Created client");

	pClient->setClientCallbacks(new MyClientCallback());

	Display.drawConnectionScreen();
	Display.counter=1;
	scan(); //initial scan which has a callback calling connectToServer() if the service is detected
}

void loop()
{
	//Deep sleep:
	if (_sleep && millis()-timedelay>1000){
		Serial.println("Going to sleep now");
		Display.displayOff();
		esp_sleep_enable_ext0_wakeup(GPIO_NUM_32,1); //1 = High, 0 = Low connected to GPIO32
		esp_deep_sleep_start();
	}


	//BLE:

  if(!pClient->isConnected() || (millis()-notifyTimeout) > 5000){
	//if no connection or notifications/navigation stop, we do a proper
	//disconnect and reconnect
	Serial.println("Connection lost or navigation turned off! Reconnecting...");
	pClient->disconnect();
	delay(50);

  //first try to connect to same device (necessary if app in background and we lost connection)
  //if this fails we scan again; it fails. e.g. when there was no initial connection (=no myDevice)
  //After 100s without notifications we scan for new devices even we still have connection

	if ((millis()-deviceTimeout)<100000){
	  //it is checked that we had initial connection and therefore a valid address
	  reconnect(); //uses myDevice from before again, also works in iOS background
	}

	if(!(pClient->isConnected())){
	  Serial.println("Scan again...");
	  //if reconnect was not successful or not executed (>100s), scan again
	  Display.drawConnectionScreen();
	  Display.counter++;
	  scan();
	}

	if ((millis()-deviceTimeout)>200000){
		_sleep=true;
	}
  }

  //to avoid abort we need to check if client is connected before reading or writing characteristics

  //in the worst case we lose connection between checking and reading -> better don't use flag and directly check in if-condition

  //Read characteristic if new data via notified or more than 4s elapsed since last read (to keep connection)
  if(_update && pClient->isConnected()){
	std::string value = pRemoteCharacteristic->readValue(); //this crashes sometimes
	if (value.length()>4){
	  //in case we have update flag but characteristic changed due to navigation stop between
	  std::string street;
	  street = value.substr(9); //this causes abort when there are not at least 9 bytes available
	  Serial.print("Street: ");
	  Serial.println(street.c_str());

	  std::string direction;
	  direction = value.substr(4,4);
	  uint8_t d=direction[0];
	  Serial.print("Direction: ");
	  Serial.println(d);

	  std::string distance;
	  distance = value.substr(5,8);
	  uint32_t dist=distance[0] | distance[1] << 8 | distance[2] << 16 | distance[3] << 24;
	  Serial.print("Distance: ");
	  Serial.println(dist);

	  //Display - show the navigation instruction:
	  Display.drawNavigationScreen(&d, &dist, street.c_str());

	}
	else {
	  Serial.println("Update flag was set but characteristic changed between notify and reading");
	}
	_update=false; //set flag back
  }

  delay(1000); // Delay a second between loops.

}

//Functions down here:

//Deep sleep:
void IRAM_ATTR isr(){
  detachInterrupt(digitalPinToInterrupt(32)); //because later used for wake up
  //setting flag instead to get a little bit of delay
  _sleep=true;
  timedelay=millis();
}


