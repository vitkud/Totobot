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
#define LED_PIN 2 //LED_BUILTIN

// Constants
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

enum {MOVE_FORWARD = 1, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT};

class Totobot {
public:
	static void init();
	static void loop();

	static void reset();
	static void setMotorRange(byte number, byte min, byte max);
	static void runMotor(byte number, short speed);
	static void move(byte direction, byte speed);

	static void setEyeEffect(byte eye, int effect);

private:
	static void timer();
};

extern Totobot totobot;

#endif
