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
#define LEFT 0
#define RIGHT 1

// Eyes
#define EYE_PIN A0
#define EYE_BRIGHTNESS 3 // (0-255)
#define EYE_CURRENT_LIMIT 600
#define EYE_WIDTH 4
#define EYE_HEIGHT 4
#define EYE_LEDS 16 // EYE_WIDTH * EYE_HEIGHT
#define EYE_COUNT 2
#define EYE_TOTAL_LEDS 32 // EYE_LEDS * EYE_COUNT

#define EYE_SNOW_DENSE 10

class Totobot {
public:
	static void init();
	static void loop();

	static void reset();
	static void setCorrection(int value);
	static void moveForward(int duration, byte speed);
	static void moveBackward(int duration, byte speed);
	static void turnLeft(int duration, byte speed);
	static void turnRight(int duration, byte speed);
	static void runMotor(int port, short speed);

	void setEyeEffect(int eye, int effect);

private:
	static void timer();
	static void updateEffect(int eye, int effect, bool loadingFlag);
};

extern Totobot totobot;

#endif
