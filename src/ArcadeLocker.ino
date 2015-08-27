#include "global.h"
#include "config.h"
#include "events.h"
#include "menu.h"

LedControl disp = LedControl(2, 4, 3, 1);

void setup() {
	#ifdef DEBUG
		Serial.begin(9600);
		Serial.print("Ready");
	#endif

	byte i;

	// Inputs
	for (i = 0; i < NUMBER_OF_BUTTONS; i++)
		pinMode(buttonsPins[i], INPUT_PULLUP);

	// Display
	disp.shutdown(0, false);
	disp.setIntensity(0, DEFAULT_DISPLAY_INTENSITY);

	// App start
	eventsInit();
	menuInit();
}

void loop() {
	eventsUpdateLoop();
}
