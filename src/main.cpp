#include <BLEDevice.h>
#include <BLEServer.h> 
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <Wire.h>
#include <BMI160.hpp>
#include <BNO055.hpp>
#include <TCA9548A.hpp>
#include <stdio.h>
#include <ArduinoJson.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "sdkconfig.h"


// /*BLE*/
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
bool IndexInterruptTriggerd = false;
bool MiddleInterruptTriggerd = false;
// uint8_t txValue = 5;
// const int ledPin = 33;
#ifndef LED_BUILTIN
#define LED_BUILTIN 18
#endif

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID			"6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX	"6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX	"6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define SDA						21 
#define SCL						22 
#define FINGER_TAP_INDEX		27
#define FINGER_TAP_MIDDLE		0
#define _MAIN_					0
#define _PINKY_	 				1
#define _RING_	 				2
#define _MIDDLE_	 			3
#define _INDEX_	 				4	
#define _THUMB_					5
#define _FUEL_GAUGE_			6
#define TCA9548A_I2C_ADDR		UINT8_C(0x72)

void index_interrupt_triggerd();
void middle_interrupt_triggered();


TCA9548A I2C_MUX;
BNO055 MAIN(0x28);
BMI160 INDEX(0x68);
BMI160 MIDDLE(0x68);
//BMI160 finger_end(0x69);

void index_interrupt_triggered() {
	IndexInterruptTriggerd = true;
	return;
}

void middle_interrupt_triggered() {
	MiddleInterruptTriggerd = true;
	return;
}

class MyServerCallbacks : public BLEServerCallbacks
{
	void onConnect(BLEServer *pServer)
{
 	deviceConnected = true;
};

void onDisconnect(BLEServer *pServer)
{
	deviceConnected = false;
}
};

class MyCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {
		std::string rxValue = pCharacteristic->getValue();
		if (rxValue.length() > 0) {
			Serial.println("*********");
			Serial.print("Received Value: ");
			for (int i = 0; i < rxValue.length(); i++)
				Serial.print(rxValue[i]);

			Serial.println();
			Serial.println("*********");
		}
	}
};


void setup() {
	delay(3000);
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	Wire.begin(SDA, SCL, 400000);
	I2C_MUX.set_i2c_addr(TCA9548A_I2C_ADDR);
	//MAIN.initialize_I2C(OPR_MODE_IMU);
	MAIN.initialize_I2C(OPR_MODE_AMG);
	INDEX.initialize_interrupt_engines();
	//INDEX.latch_int_reg();
	INDEX.initialize_I2C();
	attachInterrupt(FINGER_TAP_INDEX, index_interrupt_triggered, RISING);
	attachInterrupt(FINGER_TAP_MIDDLE, middle_interrupt_triggered, RISING);
	// Create the BLE Deqvice
	//BLEDevice::init("ESP32"); //REENABLE
	
	/*
	// Create the BLE Server
	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	// Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID);

	// Create a BLE Characteristic
	pTxCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID_TX,
		BLECharacteristic::PROPERTY_NOTIFY);

	pTxCharacteristic->addDescriptor(new BLE2902());

	BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID_RX,
		BLECharacteristic::PROPERTY_WRITE);

	pRxCharacteristic->setCallbacks(new MyCallbacks());

	// Start the service
	pService->start();

	// Start advertising
	pServer->getAdvertising()->addServiceUUID(pService->getUUID());
	pServer->getAdvertising()->start();
	Serial.println("Waiting a client connection to notify...");*/
}

void loop() {
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	I2C_MUX.select_bus(_INDEX_);
	INDEX.get_sensor_data();
	delay(25);
	//delay(0.1);
	if (IndexInterruptTriggerd == true) {
		INDEX.interrupt_detection_index();
		IndexInterruptTriggerd = false;
		//INDEX.unlatch_int_reg();
		//INDEX.latch_int_reg();
	}
	//I2C_MUX.select_bus(_INDEX_);
	//HB_UNIT.get_sensor_data(OPR_MODE_AMG, NONE);
	//HB_UNIT.get_sensor_data(OPR_MODE_IMU, EULE);

	/*if (deviceConnected) {	
		pTxCharacteristic->setValue(&txValue, 1);
		pTxCharacteristic->notify();
		//txValue++;
		delay(10); // bluetooth stack will go into congestion, if too many packets are sent
		//Serial.println(txValue);
	}

	// disconnecting
	if (!deviceConnected && oldDeviceConnected) {
		delay(500);					 // give the bluetooth stack the chance to get things ready
		pServer->startAdvertising(); // restart advertising
		Serial.println("start advertising");
		oldDeviceConnected = deviceConnected;
	}
	// connecting
	if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
		oldDeviceConnected = deviceConnected;
	}*/
}