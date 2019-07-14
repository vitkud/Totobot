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

// EEPROM
#define EEPROM_BEGIN 0x0200
#define EEPROM_MAGIC_NUMBER 0x44
#define EEPROM_DATA_SIZE 8
#define EEPROM_MOTORS_BEGIN 0x0202

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

	static void ledShowImage(byte *bytes, byte size, byte x, byte y);
	static void ledShowImage(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7,
		byte b8, byte b9, byte bA, byte bB, byte bC, byte bD, byte bE, byte bF, byte x = 0, byte y = 0);
	static void showString(const char *str, int8_t x, int8_t y);
	static void setEyeEffect(byte eye, int effect);

private:
	static void timer();
};

extern Totobot totobot;

#endif
