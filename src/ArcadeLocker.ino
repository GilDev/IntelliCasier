#include "global.h"
#include "config.h"
#include "events.h"
#include "menu.h"

LedControl matrix = LedControl(MAX_CHIP_DIN_PIN, MAX_CHIP_CLK_PIN, MAX_CHIP_LOAD_PIN, 1);

void setup() {
	#ifdef SERIAL_DEBUG
		Serial.begin(9600);
		Serial.print("Ready");
	#endif

	byte i;

	// Inputs
	for (i = 0; i < 5; i++)
		pinMode(buttonsPins[i], INPUT_PULLUP);

	// LED Matrix
	matrix.shutdown(0, false);
	matrix.setIntensity(0, DEFAULT_MATRIX_INTENSITY);

	// App start
	eventsInit();
	menuInit();
}

void loop() {
	eventsUpdateLoop();
}
