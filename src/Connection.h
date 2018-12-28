/*
 * 	Connection.h
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: Functions for Bluetooth BLE communication.
 */

#ifndef SRC_CONNECTION_H_
#define SRC_CONNECTION_H_

#include "Arduino.h"
#include "BLEDevice.h"
#include <string.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("503DD605-9BCB-4F6E-B235-270A57483026");

extern BLEAddress *pServerAddress;
extern BLEAdvertisedDevice* myDevice;
extern BLERemoteCharacteristic* pRemoteCharacteristic;
extern BLEClient* pClient;
extern uint8_t notificationOff[]; //actually not used
extern uint8_t notificationOn[];

extern bool doConnect;
extern bool _update;
extern bool _initialConnect;
extern uint8_t pData_old[4];
extern unsigned long startTime; //Timer to read signal every 5s
extern unsigned long notifyTimeout; //Timer to check if no notifications anymore
extern unsigned long deviceTimeout; //Timer to check if we should scan again for other devices

void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
bool connectToServer();
void scan();
bool reconnect();
void connect();
void my_gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t* param);

class MyClientCallback: public BLEClientCallbacks {
  void onConnect(BLEClient* pClient);
  void onDisconnect(BLEClient* pClient);
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice);
};

#endif /* SRC_CONNECTION_H_ */
