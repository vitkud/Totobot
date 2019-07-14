/*
	Used effects from https://github.com/AlexGyver/GyverMatrixBT/
*/

#include "TotobotFace.h"
#include "Totobot.h"
#include "TotobotTinyFont.h"

#include "FastLED.h"

CRGB leds[EYE_TOTAL_LEDS];

volatile bool eyeLoadingFlag[2];
volatile short eyeEffects[2];

#define _WIDTH EYE_WIDTH
#define THIS_X x
#define THIS_Y (EYE_HEIGHT - y - 1)
//#define THIS_X (EYE_WIDTH - x - 1)
//#define THIS_Y y

void TotobotFace::init() {
	FastLED.addLeds<WS2812, EYE_PIN, GRB>(leds, EYE_TOTAL_LEDS);
	FastLED.setBrightness(EYE_BRIGHTNESS);
	if (EYE_CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, EYE_CURRENT_LIMIT);
	FastLED.clear();
	FastLED.show();
}

void TotobotFace::loop() {
	bool needRefresh = false;
	for (int i = 0; i < EYE_COUNT; ++i) {
		needRefresh = needRefresh || eyeEffects[i] != 0 || eyeLoadingFlag[i];
		updateEffect(i, eyeEffects[i], eyeLoadingFlag[i]);
		eyeLoadingFlag[i] = false;
	}
	if (needRefresh)
		FastLED.show();
}

void TotobotFace::setEyeEffect(byte eye, short effect) {
	eye = eye % EYE_COUNT;
	eyeEffects[eye] = effect;
	eyeLoadingFlag[eye] = true;
}

uint16_t getPixelNumber(byte eye, int8_t x, int8_t y) {
	if (THIS_Y % 2 == 0) {
		return (THIS_Y * _WIDTH + THIS_X) + EYE_LEDS * eye;
	} else {
		return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1) + EYE_LEDS * eye;
	}
}

void drawPixelXY(byte eye, int8_t x, int8_t y, CRGB color) {
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

uint32_t getPixColorXY(byte eye, int8_t x, int8_t y) {
	return getPixColor(getPixelNumber(eye, x, y));
}

void noneRoutine(byte eye, boolean loadingFlag) {
	if (loadingFlag) {
		for (int i = eye; i < EYE_LEDS; ++i)
			leds[i + EYE_LEDS * eye] = 0;
	}
}

void snowRoutine(byte eye, boolean loadingFlag) {
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

void testRoutine(byte eye, boolean loadingFlag) {
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

void matrixRoutine(byte eye, boolean loadingFlag) {
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

void TotobotFace::updateEffect(byte eye, short effect, boolean loadingFlag) {
	switch (effect) {
	case 0: noneRoutine(eye, loadingFlag); break;
	case 2: snowRoutine(eye, loadingFlag); break;
	case 7: matrixRoutine(eye, loadingFlag); break;
	default: testRoutine(eye, loadingFlag); break;
	}
}

void TotobotFace::showImage(byte *bytes, byte size, byte x, byte y) {
	//drawPixelXY(0, 2, 2, )
	for (int i = 0; i < size / 2; ++i) {
		for (int j = 0; j < 4; ++j) {
			int d = bytes[i * 2] >> j * 2 & 1;
			int r = bytes[i * 2] >> j * 2 + 1 & 1;
			int g = bytes[i * 2 + 1] >> j * 2 + 1 & 1;
			int b = bytes[i * 2 + 1] >> j * 2 & 1;
			drawPixelXY((x + i) / 4, (x + i) % 4, y + j, (d + r + g + b) * 40);
			// drawPixelXY((x + i) / 4, (x + i) % 4, y + j, (r * 0x7f0000 + g * 0x007f00 + b * 0x00007f) * (d + 1));
		}
	}
	FastLED.show();
}

const int separatorWidth = 1;
const int displayWidth = EYE_WIDTH + separatorWidth + EYE_WIDTH;
const int displayHeight = EYE_HEIGHT;
// const int fontWidth = 4;
// const int fontHeight = 4;
const int charWidth = 5;

void setPixel(byte x, byte y, CRGB color) {
	if (x < EYE_WIDTH) {
		drawPixelXY(0, x, 3 - y, color);
	} else if (x >= EYE_WIDTH + separatorWidth) {
		drawPixelXY(1, x - (EYE_WIDTH + separatorWidth), 3 - y, color);
	}
}

bool getBit(byte byteData, byte bitNumber) {
	return (byteData >> bitNumber) & 1;
}

void TotobotFace::showString(const char *str, int8_t x, int8_t y) {
	int strLen = strlen(str);
	int curChar = -1;
	byte charData[4];
	for (int cx = 0; cx < displayWidth; ++cx) {
		if (cx < x || cx >= x + strLen * charWidth) {
			curChar = -1;
		} else {
			byte c = str[(cx - x) / charWidth];
			if (c != curChar) {
				for (int i = 0; i < sizeof charData / sizeof *charData; ++i) {
					charData[i] = pgm_read_byte(&font4x4[(c / 2) * 4 + i]) >> (c % 2 == 0 ? 4 : 0);
				}
				curChar = c;
			}
		}
		for (int cy = 0; cy < displayHeight; ++cy) {
			if (curChar == -1 || cy < y || cy >= y + 4) {
				setPixel(cx, cy, 0x000000);
			} else {
				byte bit = (cx - x) % charWidth;
				if (bit > 3)
					setPixel(cx, cy, 0x000000);
				else
					setPixel(cx, cy, getBit(charData[cy - y], 3 - bit) ? 0x00ff00 : 0x000000);
			}
		}
	}
	FastLED.show();
}
