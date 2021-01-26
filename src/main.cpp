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

bool IndexInterruptTriggerd = false;
bool MiddleInterruptTriggerd = false;
unsigned long start, end, elapsed;

#ifndef LED_BUILTIN
#define LED_BUILTIN 18
#endif

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

void setup() {
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	Wire.begin(SDA, SCL, 400000);
	INDEX.initialize_I2C();
	I2C_MUX.set_i2c_addr(TCA9548A_I2C_ADDR);
	// MAIN.initialize_I2C(OPR_MODE_IMU);
	MAIN.initialize_I2C(OPR_MODE_AMG);
	INDEX.initialize_interrupt_engines();
	attachInterrupt(FINGER_TAP_INDEX, index_interrupt_triggered, CHANGE);
	// attachInterrupt(FINGER_TAP_MIDDLE, middle_interrupt_triggered, RISING);
	
}

void loop() {
	start = micros();
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	delay(1);
	I2C_MUX.select_bus(_INDEX_);
	// INDEX.get_sensor_data();
	if (IndexInterruptTriggerd == true) {
		INDEX.interrupt_detection_index();
		IndexInterruptTriggerd = false;
	}
	I2C_MUX.select_bus(_MAIN_);
	MAIN.get_sensor_data(OPR_MODE_AMG, NONE);
	// MAIN.get_sensor_data(OPR_MODE_IMU, EULE);
	// MAIN.get_sensor_data(OPR_MODE_LIN_ACC, NONE);
	delay(30);
	end = micros();
	elapsed = end - start;
	
}