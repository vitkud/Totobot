/*
	TotobotCore.h - Core library for Totobot - description
	Copyright (c) 2019 Vitaliy.  All right reserved.
*/

#ifndef TotobotCore_h
#define TotobotCore_h

#include "Arduino.h"

#define BRIGHTNESS 3 // (0-255)
#define CURRENT_LIMIT 600
#define WIDTH 4
#define HEIGHT 4
#define EYE_LEDS 16 // WIDTH * HEIGHT
#define NUM_LEDS 32 // EYE_LEDS * 2
#define LED_PIN A0

#define SNOW_DENSE 10

class Totobot {
public:
	static void init();

	static void setCorrection(int value);
	static void moveForward(int duration, byte speed);
	static void moveBackward(int duration, byte speed);
	static void turnLeft(int duration, byte speed);
	static void turnRight(int duration, byte speed);
	static void runMotor(int port, int speed);

	void setEyeEffect(int eye, int effect);

private:
	static void timer();
	static void updateEffect(int eye, int effect, bool loadingFlag);
};

extern Totobot totobot;

#endif
