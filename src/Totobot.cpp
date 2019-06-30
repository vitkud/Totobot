/*
	Totobot.cpp - library for Totobot - implementation
	Copyright (c) 2019 Vitaliy.  All right reserved.
	Used effects from https://github.com/AlexGyver/GyverMatrixBT/
*/

#include "Totobot.h"
#include "TotobotOnlineListener.h"

#include "AFMotor.h"
#include "TimerOne.h"
#include "FastLED.h"

Totobot totobot;

CRGB leds[EYE_TOTAL_LEDS];

int corr = 0;
AF_DCMotor motor[2] = {1, 2};

volatile bool eyeLoadingFlag[2];
volatile int eyeEffects[2];

void Totobot::timer() {
	for (int i = 0; i < 2; ++i) {
		updateEffect(i, eyeEffects[i], eyeLoadingFlag[i]);
		eyeLoadingFlag[i] = false;
	}
	FastLED.show(); 
}

void Totobot::init() {
	motor[0].run(RELEASE);
	motor[1].run(RELEASE);

	FastLED.addLeds<WS2812, EYE_PIN, GRB>(leds, EYE_TOTAL_LEDS);
	FastLED.setBrightness(EYE_BRIGHTNESS);
	if (EYE_CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, EYE_CURRENT_LIMIT);
	FastLED.clear();
	FastLED.show();

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

void Totobot::reset() {
	for (int i = 0; i < (sizeof motor / sizeof motor[0]); ++i)
		motor[i].run(RELEASE);
}

void Totobot::setCorrection(int value) {
	corr = value;
}
void Totobot::moveForward(int duration, byte speed) {
	motor[0].run(FORWARD);
	motor[0].setSpeed(speed-(corr>0?corr:0));
	motor[1].run(FORWARD);
 	motor[1].setSpeed(speed+(corr<0?corr:0));
 	delay(duration*1000);
 	motor[0].run(RELEASE);
 	motor[1].run(RELEASE);
}
void Totobot::moveBackward(int duration, byte speed) {
	motor[0].run(BACKWARD);
	motor[0].setSpeed(speed-(corr>0?corr:0));
	motor[1].run(BACKWARD);
	motor[1].setSpeed(speed+(corr<0?corr:0));
	delay(duration*1000);
	motor[0].run(RELEASE);
	motor[1].run(RELEASE);
}
void Totobot::turnLeft(int duration, byte speed) {
	motor[0].run(BACKWARD);
	motor[0].setSpeed(speed-(corr>0?corr:0));
	motor[1].run(FORWARD);
	motor[1].setSpeed(speed+(corr<0?corr:0));
	delay(duration*1000);
	motor[0].run(RELEASE);
	motor[1].run(RELEASE);
}
void Totobot::turnRight(int duration, byte speed) {
	motor[0].run(FORWARD);
	motor[0].setSpeed(speed-(corr>0?corr:0));
	motor[1].run(BACKWARD);
	motor[1].setSpeed(speed+(corr<0?corr:0));
	delay(duration*1000);
	motor[0].run(RELEASE);
	motor[1].run(RELEASE);
}
void Totobot::runMotor(int port, short speed) {
	port = port % (sizeof motor / sizeof motor[0]);
	motor[port].setSpeed(speed >= 0 ? speed : -speed);
	motor[port].run(speed > 0 ? FORWARD : speed < 0 ? BACKWARD : RELEASE);
}

void Totobot::setEyeEffect(int eye, int effect) {
	if (eye < 0 || eye >= 2) return;
	eyeEffects[eye] = effect;
	eyeLoadingFlag[eye] = true;
}

#define _WIDTH EYE_WIDTH
// #define THIS_X x
// #define THIS_Y (EYE_HEIGHT - y - 1)
#define THIS_X (EYE_WIDTH - x - 1)
#define THIS_Y y

uint16_t getPixelNumber(int eye, int8_t x, int8_t y) {
	if (THIS_Y % 2 == 0) {
		return (THIS_Y * _WIDTH + THIS_X) + EYE_LEDS * eye;
	} else {
		return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1) + EYE_LEDS * eye;
	}
}

void drawPixelXY(int eye, int8_t x, int8_t y, CRGB color) {
	if (x < 0 || x >= EYE_WIDTH || y < 0 || y >= EYE_HEIGHT)
		return;
	leds[getPixelNumber(eye, x, y)] = color;
}

uint32_t getPixColor(int pixelNumber) {
	if (pixelNumber < 0 || pixelNumber >= EYE_TOTAL_LEDS)
		return 0;
	return (((uint32_t)leds[pixelNumber].r << 16) |
	        ((long)leds[pixelNumber].g << 8) |
	        (long)leds[pixelNumber].b); // XXX uint32_t, long, long
}

uint32_t getPixColorXY(int eye, int8_t x, int8_t y) {
	return getPixColor(getPixelNumber(eye, x, y));
}

void noneRoutine(int eye, boolean loadingFlag) {
	if (loadingFlag) {
		for (int i = eye; i < EYE_LEDS; ++i)
			leds[i + EYE_LEDS * eye] = 0;
	}
}

void snowRoutine(int eye, boolean loadingFlag) {
	for (byte x = 0; x < EYE_WIDTH; x++) {
		for (byte y = 0; y < EYE_HEIGHT - 1; y++) {
			drawPixelXY(eye, x, y, getPixColorXY(eye, x, y + 1));
		}
	}

	for (byte x = 0; x < EYE_WIDTH; x++) {
		if (getPixColorXY(eye, x, EYE_HEIGHT - 2) == 0 && (random(0, EYE_SNOW_DENSE) == 0))
			drawPixelXY(eye, x, EYE_HEIGHT - 1, 0xE0FFFF - 0x101010 * random(0, 4));
		else
			drawPixelXY(eye, x, EYE_HEIGHT - 1, 0x000000);
	}
}

void testRoutine(int eye, boolean loadingFlag) {
	static int x = 0;
	static int y = 0;
	drawPixelXY(eye, x, y, 0x000000);
	if (++x == EYE_WIDTH) {
		x = 0;
		if (++y == EYE_HEIGHT)
			y = 0;
	}
	drawPixelXY(eye, x, y, 0x00FF00);
}

void matrixRoutine(int eye, boolean loadingFlag) {
	if (loadingFlag)
		noneRoutine(eye, loadingFlag);

	for (byte x = 0; x < EYE_WIDTH; x++) {
		uint32_t thisColor = getPixColorXY(eye, x, EYE_HEIGHT - 1);
		if (thisColor == 0)
			drawPixelXY(eye, x, EYE_HEIGHT - 1, 0x00FF00 * (random(0, 10) == 0));
		else if (thisColor < 0x002000)
			drawPixelXY(eye, x, EYE_HEIGHT - 1, 0);
		else
			drawPixelXY(eye, x, EYE_HEIGHT - 1, thisColor - 0x002000);
	}

	for (byte x = 0; x < EYE_WIDTH; x++) {
		for (byte y = 0; y < EYE_HEIGHT - 1; y++) {
			drawPixelXY(eye, x, y, getPixColorXY(eye, x, y + 1));
		}
	}
}

void Totobot::updateEffect(int eye, int effect, boolean loadingFlag) {
	switch (effect) {
	case 0: noneRoutine(eye, loadingFlag); break;
	case 2: snowRoutine(eye, loadingFlag); break;
	case 7: matrixRoutine(eye, loadingFlag); break;
	default: testRoutine(eye, loadingFlag); break;
	}
}
