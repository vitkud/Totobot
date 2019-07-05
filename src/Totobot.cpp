/*
	Totobot.cpp - library for Totobot - implementation
	Copyright (c) 2019 Vitaliy.  All right reserved.
	Used effects from https://github.com/AlexGyver/GyverMatrixBT/
*/

#include "Totobot.h"
#include "TotobotFace.h"
#include "TotobotOnlineListener.h"

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

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
	delay(300);
	digitalWrite(LED_PIN, LOW);

	TotobotOnlineListener::init();

	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, HIGH);
	delay(100);
	digitalWrite(BUZZER_PIN, LOW);
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

void Totobot::setEyeEffect(byte eye, int effect) {
	TotobotFace::setEyeEffect(eye, effect);
}
