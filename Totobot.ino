#include "src/Totobot.h"

void setup() {
	totobot.init();
	totobot.setEyeEffect(0, -1);
}

void loop() {
	totobot.loop();
}
