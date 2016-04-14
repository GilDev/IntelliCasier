#include <Arduino.h>
#include "config.h"
#include "displays.h"
#include "events.h"
#include "time.h"
#include "apps/menu.h"

#ifdef RTC
	#include <Wire.h>
	#include "time.h"
#endif


void setup() {
	#ifdef DEBUG
		Serial.begin(SERIAL_BAUD);
		Serial.print(F("Ready"));
	#endif

	#ifdef RTC
		timeInit();
		randomSeed(getTime().getEpoch());
	#else
		randomSeed(analogRead(A7));
	#endif

	// Inputs
	byte i;
	for (i = 0; i < 5; i++)
		pinMode(buttonsPins[i], USE_INTERNAL_PULLUPS ? INPUT_PULLUP : INPUT);

	displaysInit();
	eventsInit();
	showMenu();
}

void loop() {
	eventsUpdateLoop();

	#ifdef DEBUG
		// Internal LED change state every 1000 loop cycle
		static bool ledState = LOW;
		static unsigned short cycleCounter = 0;

		if (++cycleCounter == 1000) {
			cycleCounter = 0;
			if (ledState)
				ledState = LOW;
			else
				ledState = HIGH;
		}

		digitalWrite(13, ledState);
	#endif
}
