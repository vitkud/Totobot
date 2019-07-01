#ifndef TotobotFace_h
#define TotobotFace_h

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

class TotobotFace {
public:
	static void init();
	static void loop();

	static void setEyeEffect(byte eye, short effect);

private:
	static void updateEffect(byte eye, short effect, bool loadingFlag);
};

#endif
