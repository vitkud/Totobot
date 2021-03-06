/*
	Totobot.cpp - library for Totobot - implementation
	Copyright (c) 2019 Vitaliy.  All right reserved.
	Used effects from https://github.com/AlexGyver/GyverMatrixBT/
*/

#include "Totobot.h"
#include "TotobotFace.h"
#include "TotobotOnlineListener.h"

#include <avr/eeprom.h>
#include "AFMotor.h"
#include "TimerOne.h"

Totobot totobot;

AF_DCMotor motors[] = {1, 2, 3, 4};
byte motorsMin[] = {0, 0, 0, 0};
byte motorsMax[] = {255, 255, 255, 255};

void Totobot::init() {
	reset();

	TotobotFace::init();

	Timer1.initialize(100000);
	Timer1.attachInterrupt(timer);

	TotobotOnlineListener::init();

	// read/init EEPROM
	if (eeprom_read_byte((byte *)EEPROM_BEGIN) == EEPROM_MAGIC_NUMBER &&
	    eeprom_read_byte((byte *)EEPROM_BEGIN + 1) == EEPROM_DATA_SIZE) {
		for (int i = 0; i < (sizeof motors / sizeof motors[0]); ++i) {
			motorsMin[i] = eeprom_read_byte((byte *)EEPROM_MOTORS_BEGIN + i * 2);
			motorsMax[i] = eeprom_read_byte((byte *)EEPROM_MOTORS_BEGIN + i * 2 + 1);
		}
	} else {
		eeprom_update_byte((byte *)EEPROM_BEGIN, EEPROM_MAGIC_NUMBER);
		eeprom_update_byte((byte *)EEPROM_BEGIN + 1, EEPROM_DATA_SIZE);
		for (int i = 0; i < (sizeof motors / sizeof motors[0]); ++i) {
			eeprom_update_byte((byte *)EEPROM_MOTORS_BEGIN + i * 2, motorsMin[i]);
			eeprom_update_byte((byte *)EEPROM_MOTORS_BEGIN + i * 2 + 1, motorsMax[i]);
		}
	}

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
	delay(300);
	digitalWrite(LED_PIN, LOW);
}

void Totobot::loop() {
	TotobotOnlineListener::loop();
}

void Totobot::timer() {
	TotobotFace::loop();
}

void Totobot::reset() {
	for (int i = 0; i < (sizeof motors / sizeof motors[0]); ++i)
		motors[i].run(RELEASE);
}

void Totobot::setMotorRange(byte number, byte min, byte max) {
	int index = (number - 1) % (sizeof motors / sizeof motors[0]);
	motorsMin[index] = min;
	motorsMax[index] = max;
	eeprom_update_byte((byte *)EEPROM_MOTORS_BEGIN + index * 2, min);
	eeprom_update_byte((byte *)EEPROM_MOTORS_BEGIN + index * 2 + 1, max);
}

void Totobot::runMotor(byte number, short speed) {
	int index = (number - 1) % (sizeof motors / sizeof motors[0]);
	if (index == LEFT_MOTOR - 1)
		speed = -speed; // turn of the left motor by analogy with mBot for M1
	byte absSpeed = speed >= 0 ? speed : -speed;
	absSpeed = map(absSpeed, 0, 255, motorsMin[index], motorsMax[index]);
	motors[index].setSpeed(absSpeed);
	motors[index].run(speed > 0 ? FORWARD : speed < 0 ? BACKWARD : RELEASE);
}

void Totobot::move(byte direction, byte speed) {
	switch (direction) {
	case MOVE_FORWARD: {
		runMotor(LEFT_MOTOR, speed);
		runMotor(RIGHT_MOTOR, speed);
	} break;
	case MOVE_BACKWARD: {
		runMotor(LEFT_MOTOR, -speed);
		runMotor(RIGHT_MOTOR, -speed);
	} break;
	case TURN_LEFT: {
		runMotor(LEFT_MOTOR, -speed);
		runMotor(RIGHT_MOTOR, speed);
	} break;
	case TURN_RIGHT: {
		runMotor(LEFT_MOTOR, speed);
		runMotor(RIGHT_MOTOR, -speed);
	} break;
	default: {
		runMotor(LEFT_MOTOR, 0);
		runMotor(RIGHT_MOTOR, 0);
	}
	}
}
void Totobot::ledShowImage(byte *bytes, byte size, byte x, byte y) {
	TotobotFace::showImage(bytes, size, x, y);
}

void Totobot::ledShowImage(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7,
		byte b8, byte b9, byte bA, byte bB, byte bC, byte bD, byte bE, byte bF, byte x, byte y) {
	byte bytes[16] = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, bA, bB, bC, bD, bE, bF};
	TotobotFace::showImage(bytes, sizeof bytes / sizeof *bytes, x, y);
}

void Totobot::showString(const char *str, int8_t x, int8_t y) {
	TotobotFace::showString(str, x, y);
}

void Totobot::setEyeEffect(byte eye, int effect) {
	TotobotFace::setEyeEffect(eye, effect);
}
