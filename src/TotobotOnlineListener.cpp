#include "TotobotOnlineListener.h"
#include "Totobot.h"

byte buffer[52];
byte index = 0;
byte dataLen;

void TotobotOnlineListener::init() {
	Serial.begin(115200);

	Serial.print("Version: ");
	Serial.println(version);
}

void TotobotOnlineListener::loop() {
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

/*
ff 55 len idx action device port  slot  data a
0  1  2   3   4      5      6     7     8
*/
void TotobotOnlineListener::parseData() {
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
		totobot.reset();
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

void TotobotOnlineListener::runModule(byte device) {
	byte pin = readBuffer(6);
	switch (device) {
	case MOTOR: {
		byte port = readBuffer(6);
		short speed = readShort(7);
		if (port == 9) // M1 (mBot)
			speed = -speed;
		totobot.runMotor(port, speed);
	} break;
	case JOYSTICK: {
		short leftSpeed = readShort(6);
		short rightSpeed = readShort(8);
		totobot.runMotor(LEFT_MOTOR, -leftSpeed);
		totobot.runMotor(RIGHT_MOTOR, rightSpeed);
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
	case LEDMATRIX: {
		// ignore pin/port
		byte action = readBuffer(7);
		if (action == 1) {
			// int px = buffer[8];
			// int py = buffer[9];
			// int len = readBuffer(10);
			// char *s = readString(11, len);
			// ledMx.drawStr(px, py, s);
		} else if (action == 2) {
			int px = readBuffer(8);
			int py = readBuffer(9);
			byte *bytes = readBytes(10, 16);
			totobot.ledShowImage(bytes, 16, px, py);
		} else if (action == 3) {
			// int point = readBuffer(8);
			// int hours = readBuffer(9);
			// int minutes = readBuffer(10);
			// ledMx.showClock(hours, minutes, point);
		} else if (action == 4) {
			// ledMx.showNum(readFloat(8), 3);
		}
	} break;

	case TOTO_MOTOR: {
		byte number = readBuffer(6);
		short speed = readShort(7);
		totobot.runMotor(number, speed);
	} break;
	case TOTO_MOTOR_RANGE: {
		byte number = readBuffer(6);
		byte min = readBuffer(7);
		byte max = readBuffer(8);
		totobot.setMotorRange(number, min, max);
	} break;
	case TOTO_JOYSTICK: {
		int leftSpeed = readShort(6);
		int rightSpeed = readShort(8);
		totobot.runMotor(LEFT_MOTOR, leftSpeed);
		totobot.runMotor(RIGHT_MOTOR, rightSpeed);
	} break;
	case TOTO_EYE_EFFECT: {
		byte eye = readBuffer(6);
		short effect = readShort(7);
		totobot.setEyeEffect(eye, effect);
	} break;
	}
}

void TotobotOnlineListener::readSensor(byte device) {
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

byte TotobotOnlineListener::readBuffer(int index) {
	return buffer[index];
}

float TotobotOnlineListener::readFloat(int index) {
	Union4 union4;
	union4.byteVal[0] = readBuffer(index);
	union4.byteVal[1] = readBuffer(index + 1);
	union4.byteVal[2] = readBuffer(index + 2);
	union4.byteVal[3] = readBuffer(index + 3);
	return union4.floatVal;
}

short TotobotOnlineListener::readShort(int index) {
	Union2 union2;
	union2.byteVal[0] = readBuffer(index);
	union2.byteVal[1] = readBuffer(index + 1);
	return union2.shortVal;
}

long TotobotOnlineListener::readLong(int index) {
	Union4 union4;
	union4.byteVal[0] = readBuffer(index);
	union4.byteVal[1] = readBuffer(index + 1);
	union4.byteVal[2] = readBuffer(index + 2);
	union4.byteVal[3] = readBuffer(index + 3);
	return union4.longVal;
}

char _receiveStr[20] = {};
uint8_t _receiveBytes[16] = {};

char* TotobotOnlineListener::readString(int index, int len) {
	if (len >= sizeof _receiveStr / sizeof *_receiveStr)
		len = sizeof _receiveStr / sizeof *_receiveStr - 1;
	for (int i = 0; i < len; i++) {
		_receiveStr[i] = readBuffer(index + i);
	}
	_receiveStr[len] = '\0';
	return _receiveStr;
}

byte* TotobotOnlineListener::readBytes(int index, int len) {
	if (len > sizeof _receiveBytes / sizeof *_receiveBytes)
		len = sizeof _receiveBytes / sizeof *_receiveBytes;
	for (int i = 0; i < len; i++) {
		_receiveBytes[i] = readBuffer(index + i);
	}
	return _receiveBytes;
}

int TotobotOnlineListener::readSerial() {
	if (Serial.available() > 0)
		return Serial.read();
	else
		return -1;
}

void TotobotOnlineListener::writeHead() {
	writeSerial(0xff);
	writeSerial(0x55);
}

void TotobotOnlineListener::writeEnd() {
	Serial.println();
}

void TotobotOnlineListener::writeSerial(byte c) {
	Serial.write(c);
}

void TotobotOnlineListener::sendByte(byte c) {
	writeSerial(1);
	writeSerial(c);
}
void TotobotOnlineListener::sendFloat(float value) {
     writeSerial(2);
	 Union4 union4;
     union4.floatVal = value;
     writeSerial(union4.byteVal[0]);
     writeSerial(union4.byteVal[1]);
     writeSerial(union4.byteVal[2]);
     writeSerial(union4.byteVal[3]);
}

void TotobotOnlineListener::sendShort(short value) {
	writeSerial(3);
	Union2 union2;
	union2.shortVal = value;
	writeSerial(union2.byteVal[0]);
	writeSerial(union2.byteVal[1]);
}

void TotobotOnlineListener::sendString(String s) {
	int l = s.length();
	writeSerial(4);
	writeSerial(l);
	for (int i = 0; i < l; i++) {
		writeSerial(s.charAt(i));
	}
}
