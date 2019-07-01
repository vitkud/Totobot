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

short corr = 0;
AF_DCMotor motors[] = {1, 2, 3, 4};

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

void Totobot::setCorrection(short value) {
	corr = value;
}
void Totobot::moveForward(int duration, byte speed) {
	runMotor(LEFT_MOTOR, speed);
	runMotor(RIGHT_MOTOR, speed);
 	delay(duration * 1000);
	runMotor(LEFT_MOTOR, 0);
	runMotor(RIGHT_MOTOR, 0);
}
void Totobot::moveBackward(int duration, byte speed) {
	runMotor(LEFT_MOTOR, -speed);
	runMotor(RIGHT_MOTOR, -speed);
	delay(duration * 1000);
	runMotor(LEFT_MOTOR, 0);
	runMotor(RIGHT_MOTOR, 0);
}
void Totobot::turnLeft(int duration, byte speed) {
	runMotor(LEFT_MOTOR, -speed);
	runMotor(RIGHT_MOTOR, speed);
	delay(duration * 1000);
	runMotor(LEFT_MOTOR, 0);
	runMotor(RIGHT_MOTOR, 0);
}
void Totobot::turnRight(int duration, byte speed) {
	runMotor(LEFT_MOTOR, speed);
	runMotor(RIGHT_MOTOR, -speed);
	delay(duration * 1000);
	runMotor(LEFT_MOTOR, 0);
	runMotor(RIGHT_MOTOR, 0);
}
void Totobot::runMotor(byte number, short speed) {
	int index = (number - 1) % (sizeof motors / sizeof motors[0]);
	if (index == LEFT_MOTOR - 1)
		speed = -speed; // turn of the left motor by analogy with mBot for M1
	int absSpeed = speed >= 0 ? speed : -speed;
	if (index == LEFT_MOTOR - 1 && corr > 0 && absSpeed)
		absSpeed -= corr;
	if (index == RIGHT_MOTOR - 1 && corr < 0)
		absSpeed += corr;
	if (absSpeed < 0)
		absSpeed = 0;
	motors[index].setSpeed(absSpeed);
	motors[index].run(speed > 0 ? FORWARD : speed < 0 ? BACKWARD : RELEASE);
}

void Totobot::setEyeEffect(byte eye, int effect) {
	TotobotFace::setEyeEffect(eye, effect);
}
