/*
	Totobot.h - Library for Totobot - description
	Copyright (c) 2019 Vitaliy.  All right reserved.
*/

#ifndef Totobot_h
#define Totobot_h

#include <Arduino.h>

const char version[] = "0.1";
const int analogs[] = {A0, A1, A2, A3, A4, A5, A6, A7};

// Pins
#define LED_PIN A2 //LED_BUILTIN
#define BUZZER_PIN A1

// Constants
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

class Totobot {
public:
	static void init();
	static void loop();

	static void reset();
	static void setCorrection(short value);
	static void moveForward(int duration, byte speed);
	static void moveBackward(int duration, byte speed);
	static void turnLeft(int duration, byte speed);
	static void turnRight(int duration, byte speed);
	static void runMotor(byte number, short speed);

	static void setEyeEffect(byte eye, int effect);

private:
	static void timer();
};

extern Totobot totobot;

#endif
