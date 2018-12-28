/*
 * 	Connection.cpp
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: Functions for Bluetooth BLE communication.
 *
 *  Use ESP32 1.0.1-rc1 development release (choose in Library Manager)
 *  Replace BLE source code in /opt/eclipse/arduinoPlugin/packages/esp32/hardware/esp32/1.0.1-rc1/libraries/BLE/src
 *  by commit 11a0aa6 of ESP32_BLE_Arduino with git clone/ git checkout commithash
 */

#include "Connection.h"

BLEAddress *pServerAddress;
BLEAdvertisedDevice* myDevice;
BLERemoteCharacteristic* pRemoteCharacteristic;
BLEClient* pClient;
uint8_t notificationOff[] = {0x0, 0x0}; //actually not used
uint8_t notificationOn[] = {0x1, 0x0};

void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {

  notifyTimeout=millis();
  deviceTimeout=millis();
  //set update flag whenever there is new data available or more than 4s elapsed since last read instruction because komoot will disconnect
  //if no read after 5s -> if this is really the case, we should set to 3s, otherwise we might miss 5s!!
  if(pData[0]!=pData_old[0] || pData[1]!=pData_old[1] || pData[2]!=pData_old[2] || pData[3]!=pData_old[3] || (millis()-startTime) >=3000){
  _update=true;
  Serial.println("new data");
  memcpy(pData_old, pData, 4);
  startTime=millis();
  //Serial.println("Data changed, Timer reseted!");
  }
}

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    // Connect to the remote BLE Server.
    if(!(pClient->connect(myDevice))){ //if you pass BLEAdvertisedDevice instead of address, it will recognize type of peer device address (public or private)
      Serial.print("Failed to connect to server");
      return false;
    }
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      delay(50);
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      delay(50);
      return false;
    }
    Serial.println(" - Found our characteristic");

    startTime = millis();
    notifyTimeout = millis();

    //descriptor obviously needs to be written first. Should be changed in future versions to happen
    //in registerForNotify function!
    pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
    pRemoteCharacteristic->registerForNotify(notifyCallback);
    return true;
}

void scan(){

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  connect();
}

bool reconnect(){
    if(_initialConnect==true){
      //same as connectToServer, but with different print outputs
        Serial.print("Reconnecting to ");
        Serial.println(myDevice->getAddress().toString().c_str());

        // Connect to the remote BLE Server.
        if(!(pClient->connect(myDevice))){ //if you pass BLEAdvertisedDevice instead of address, it will recognize type of peer device address (public or private)
          Serial.print("Failed to connect to server again");
          return false;
        }
        Serial.println(" - Connected to server again");

        // Obtain a reference to the service we are after in the remote BLE server.
        BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
        if (pRemoteService == nullptr) {
          Serial.print("Failed to find our service UUID again: ");
          Serial.println(serviceUUID.toString().c_str());
          pClient->disconnect();
          delay(50);
          return false;
        }
        Serial.println(" - Found our service again");


        // Obtain a reference to the characteristic in the service of the remote BLE server.
        pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
        if (pRemoteCharacteristic == nullptr) {
          Serial.print("Failed to find our characteristic UUID again: ");
          Serial.println(charUUID.toString().c_str());
          pClient->disconnect();
          delay(50);
          return false;
        }
        Serial.println(" - Found our characteristic again");

        startTime = millis();
        notifyTimeout = millis();

        //descriptor obviously needs to be written first. Should be changed in future versions to happen
        //in registerForNotify function!
        pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
        pRemoteCharacteristic->registerForNotify(notifyCallback);
    }
    return true;
}

void connect(){
  if (doConnect == true){
    //this function can also be called independent of scan (save energy), but a previous scan is necessary
    //to have myDevice. If we want to connect to the same server device again and we assume that
    //nothing has changed, we can directly call connectToServer().
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      _initialConnect=true;
    } else {
      Serial.println("We have failed to connect to the server.");
    }
    doConnect = false;
  }
}

// before setup(), to get disconnect reason outputs
void my_gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t* param) {
  ESP_LOGW(LOG_TAG, "custom gattc event handler, event: %d", (uint8_t)event);
        if(event == ESP_GATTC_DISCONNECT_EVT) {
                Serial.print("Disconnect reason: ");
                Serial.println((int)param->disconnect.reason);
        }
}

//class member functions of MyClientCallback:
void MyClientCallback::onConnect(BLEClient* pClient){
  Serial.println("onConnect callback: Connected");
}

void MyClientCallback::onDisconnect(BLEClient* pClient){
  Serial.println("onDisconnect callback: Disconnected");
}

//class member functions of MyAdvertisedDeviceCallbacks:
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 * Called for each advertising BLE server.
 */
void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
  Serial.print("BLE Advertised Device found: ");
  Serial.println(advertisedDevice.toString().c_str());

  // We have found a device, let us now see if it contains the service we are looking for.
  if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

    //
    Serial.println("Found our device!");
    BLEDevice::getScan()->stop();
    myDevice = new BLEAdvertisedDevice(advertisedDevice);
    //pServerAddress = new BLEAddress(advertisedDevice.getAddress());
    doConnect = true; //again directly calling function connect() here in callback is somehow not possible

  } // Found our server
} // onResult
