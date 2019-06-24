/*
	TotobotCore.h - Core library for Totobot - description
	Copyright (c) 2019 Vitaliy.  All right reserved.
*/

#ifndef TotobotCore_h
#define TotobotCore_h

#include <Arduino.h>

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

// actions
#define GET 1
#define RUN 2
#define RESET 4
#define START 5

// devices
#define VERSION 0
#define JOYSTICK 5
#define MOTOR 10
#define DIGITAL 30
#define ANALOG 31
#define PWM 32
#define ULTRASONIC_ARDUINO 36
#define PULSEIN 37
#define LEDMATRIX 41

#define TO_MOTOR 90

class Totobot {
public:
	static void init();
	static void loop();

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

	static void parseData();
	static void runModule(byte device);
	static void readSensor(byte device);

	static byte readBuffer(int index);
	static float readFloat(int index);
	static short readShort(int index);
	static long readLong(int index);

	static int readSerial();
	static void writeSerial(byte c);
	static void writeHead();
	static void writeEnd();
	static void sendByte(byte c);
	static void sendFloat(float value);
	static void sendShort(short value);
	static void sendString(String s);
};

extern Totobot totobot;

#endif
