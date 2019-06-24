/*
	TotobotCore.cpp - Core library for Totobot - implementation
	Copyright (c) 2019 Vitaliy.  All right reserved.
	Used effects from https://github.com/AlexGyver/GyverMatrixBT/
*/

#include "Totobot.h"

#include "AFMotor.h"
#include "TimerOne.h"
#include "FastLED.h"

Totobot totobot;

String version = "0.1";

byte buffer[52];
byte index = 0;
byte dataLen;

CRGB leds[TOTAL_LEDS];

int corr = 0;
AF_DCMotor motor[2] = {1, 2};

volatile bool eyeLoadingFlag[2];
volatile int eyeEffects[2];

int analogs[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

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

	FastLED.addLeds<WS2812, EYE_PIN, GRB>(leds, TOTAL_LEDS);
	FastLED.setBrightness(BRIGHTNESS);
	if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.clear();
	FastLED.show();

	Timer1.initialize(100000);
	Timer1.attachInterrupt(timer);

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
	delay(300);
	digitalWrite(LED_PIN, LOW);
	Serial.begin(115200);
	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, HIGH);
	delay(100);
	digitalWrite(BUZZER_PIN, LOW);
	Serial.print("Version: ");
	Serial.println(version);
}

void Totobot::loop() {
	int c = readSerial();
	if (c >= 0) {
		if (index > 1 || index == 0 && c == 0xff || index == 1 && c == 0x55) {
			if (index == 2) {
				dataLen = c;
			} else if (index > 2) {
				dataLen--;
			}
			buffer[index++] = c;
			if (index > 2 && dataLen == 0) {
				if (index > 4)
					parseData();
				index = 0;
			} else if (index >= (sizeof buffer / sizeof buffer[0])) {
				index = 0;
			}
		}
	}
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


/*
ff 55 len idx action device port  slot  data a
0  1  2   3   4      5      6     7     8
*/
void Totobot::parseData() {
	byte idx = readBuffer(3);
	byte action = readBuffer(4);
	byte device = readBuffer(5);
	switch (action) {
	case GET: {
		writeHead();
		writeSerial(idx);
		readSensor(device);
		writeEnd();
	} break;
	case RUN: {
		runModule(device);
		writeHead();
		writeEnd();
	} break;
	case RESET: {
		for (int i = 0; i < (sizeof motor / sizeof motor[0]); ++i)
			motor[i].run(RELEASE);
		writeHead();
		writeEnd();
	} break;
	case START: {
		// start
		writeHead();
		writeEnd();
	} break;
	}
}

void Totobot::runModule(byte device) {
	byte pin = readBuffer(6);
	switch (device) {
	case MOTOR:	case TO_MOTOR: {
		byte port = readBuffer(6);
		short speed = readShort(7);
		runMotor(port, speed);
	} break;
	case JOYSTICK: {
		int leftSpeed = readShort(6);
		int rightSpeed = readShort(8);
		runMotor(LEFT, leftSpeed);
		runMotor(RIGHT, rightSpeed);
	} break;
	case DIGITAL: {
		pinMode(pin, OUTPUT);
		byte val = readBuffer(7);
		digitalWrite(pin, val);
	} break;
	case PWM: {
		pinMode(pin, OUTPUT);
		byte val = readBuffer(7);
		analogWrite(pin, val);
	} break;
	}
}

void Totobot::readSensor(byte device) {
	byte pin = readBuffer(6);
	switch (device) {
	case VERSION: {
		sendString(version);
	} break;
	case DIGITAL: {
		pinMode(pin, INPUT);
		sendFloat(digitalRead(pin));
	} break;
	case ANALOG: {
		pin = analogs[pin % (sizeof analogs / sizeof analogs[0])];
		pinMode(pin, INPUT);
		sendFloat(analogRead(pin));
	} break;
	case PULSEIN: {
		int pw = readShort(7);
		pinMode(pin, INPUT);
		sendShort(pulseIn(pin, HIGH, pw));
	} break;
	case ULTRASONIC_ARDUINO: {
		int trig = readBuffer(6);
		int echo = readBuffer(7);
		pinMode(trig, OUTPUT);
		digitalWrite(trig, LOW);
		delayMicroseconds(2);
		digitalWrite(trig, HIGH);
		delayMicroseconds(10);
		digitalWrite(trig, LOW);
		pinMode(echo, INPUT);
		sendFloat(pulseIn(echo, HIGH, 30000) / 58.0);
	} break;
	}
}

union Union2 {
	byte byteVal[2];
	short shortVal;
};

union Union4 {
	byte byteVal[4];
	float floatVal;
	long longVal;
};

byte Totobot::readBuffer(int index) {
	return buffer[index];
}

float Totobot::readFloat(int index) {
	Union4 union4;
	union4.byteVal[0] = readBuffer(index);
	union4.byteVal[1] = readBuffer(index + 1);
	union4.byteVal[2] = readBuffer(index + 2);
	union4.byteVal[3] = readBuffer(index + 3);
	return union4.floatVal;
}

short Totobot::readShort(int index) {
	Union2 union2;
	union2.byteVal[0] = readBuffer(index);
	union2.byteVal[1] = readBuffer(index + 1);
	return union2.shortVal;
}

long Totobot::readLong(int index) {
	Union4 union4;
	union4.byteVal[0] = readBuffer(index);
	union4.byteVal[1] = readBuffer(index + 1);
	union4.byteVal[2] = readBuffer(index + 2);
	union4.byteVal[3] = readBuffer(index + 3);
	return union4.longVal;
}

int Totobot::readSerial() {
	if (Serial.available() > 0)
		return Serial.read();
	else
		return -1;
}

void Totobot::writeHead() {
	writeSerial(0xff);
	writeSerial(0x55);
}

void Totobot::writeEnd() {
	Serial.println();
}

void Totobot::writeSerial(byte c) {
	Serial.write(c);
}

void Totobot::sendByte(byte c) {
	writeSerial(1);
	writeSerial(c);
}
void Totobot::sendFloat(float value) {
     writeSerial(2);
	 Union4 union4;
     union4.floatVal = value;
     writeSerial(union4.byteVal[0]);
     writeSerial(union4.byteVal[1]);
     writeSerial(union4.byteVal[2]);
     writeSerial(union4.byteVal[3]);
}

void Totobot::sendShort(short value) {
	writeSerial(3);
	Union2 union2;
	union2.shortVal = value;
	writeSerial(union2.byteVal[0]);
	writeSerial(union2.byteVal[1]);
}

void Totobot::sendString(String s) {
	int l = s.length();
	writeSerial(4);
	writeSerial(l);
	for (int i = 0; i < l; i++) {
		writeSerial(s.charAt(i));
	}
}

#define _WIDTH WIDTH
// #define THIS_X x
// #define THIS_Y (HEIGHT - y - 1)
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

uint16_t getPixelNumber(int eye, int8_t x, int8_t y) {
	if (THIS_Y % 2 == 0) {
		return (THIS_Y * _WIDTH + THIS_X) + EYE_LEDS * eye;
	} else {
		return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1) + EYE_LEDS * eye;
	}
}

void drawPixelXY(int eye, int8_t x, int8_t y, CRGB color) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;
	leds[getPixelNumber(eye, x, y)] = color;
}

uint32_t getPixColor(int pixelNumber) {
	if (pixelNumber < 0 || pixelNumber >= TOTAL_LEDS)
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
	for (byte x = 0; x < WIDTH; x++) {
		for (byte y = 0; y < HEIGHT - 1; y++) {
			drawPixelXY(eye, x, y, getPixColorXY(eye, x, y + 1));
		}
	}

	for (byte x = 0; x < WIDTH; x++) {
		if (getPixColorXY(eye, x, HEIGHT - 2) == 0 && (random(0, SNOW_DENSE) == 0))
			drawPixelXY(eye, x, HEIGHT - 1, 0xE0FFFF - 0x101010 * random(0, 4));
		else
			drawPixelXY(eye, x, HEIGHT - 1, 0x000000);
	}
}

void testRoutine(int eye, boolean loadingFlag) {
	static int x = 0;
	static int y = 0;
	drawPixelXY(eye, x, y, 0x000000);
	if (++x == WIDTH) {
		x = 0;
		if (++y == HEIGHT)
			y = 0;
	}
	drawPixelXY(eye, x, y, 0x00FF00);
}

void matrixRoutine(int eye, boolean loadingFlag) {
	if (loadingFlag)
		noneRoutine(eye, loadingFlag);

	for (byte x = 0; x < WIDTH; x++) {
		uint32_t thisColor = getPixColorXY(eye, x, HEIGHT - 1);
		if (thisColor == 0)
			drawPixelXY(eye, x, HEIGHT - 1, 0x00FF00 * (random(0, 10) == 0));
		else if (thisColor < 0x002000)
			drawPixelXY(eye, x, HEIGHT - 1, 0);
		else
			drawPixelXY(eye, x, HEIGHT - 1, thisColor - 0x002000);
	}

	for (byte x = 0; x < WIDTH; x++) {
		for (byte y = 0; y < HEIGHT - 1; y++) {
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
