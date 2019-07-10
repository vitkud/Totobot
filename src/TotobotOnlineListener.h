#ifndef TotobotOnlineListener_h
#define TotobotOnlineListener_h

#include <Arduino.h>

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

#define TOTO_MOTOR 90
#define TOTO_MOTOR_RANGE 91
#define TOTO_JOYSTICK 92
#define TOTO_EYE_EFFECT 93

class TotobotOnlineListener {
public:
	static void init();
	static void loop();

private:
	static void parseData();
	static void runModule(byte device);
	static void readSensor(byte device);

	static byte readBuffer(int index);
	static float readFloat(int index);
	static short readShort(int index);
	static long readLong(int index);
	static char* readString(int index, int len);
	static byte* readBytes(int index, int len);

	static int readSerial();
	static void writeSerial(byte c);
	static void writeHead();
	static void writeEnd();
	static void sendByte(byte c);
	static void sendFloat(float value);
	static void sendShort(short value);
	static void sendString(String s);
};

#endif
