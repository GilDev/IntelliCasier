#include <Arduino.h>
#include "global.h"
#include "config.h"
#include "events.h"
#include "time.h"
#include "apps/menu.h"

#ifdef RTC
	#include <Wire.h>
	#include "time.h"
#endif

LedControl matrix = LedControl(MAX_CHIP_DIN_PIN, MAX_CHIP_CLK_PIN, MAX_CHIP_LOAD_PIN, 1);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte downArrow[] = {
B00100,
B00100,
B00100,
B00100,
B00100,
B11111,
B01110,
B00100};


void setup() {
	#ifdef SERIAL_DEBUG
		Serial.begin(9600);
		Serial.print("Ready");
	#endif

	Wire.begin();

	#ifdef RTC
		timeInit();
		randomSeed(getTime().getEpoch());
	#else
		randomSeed(analogRead(A7));
	#endif

	// Inputs
	byte i;
	for (i = 0; i < 5; i++)
		pinMode(buttonsPins[i], INPUT_PULLUP);

	// LED Matrix
	matrix.shutdown(0, false);
	matrix.setIntensity(0, DEFAULT_MATRIX_INTENSITY);

	// LCD
	lcd.createChar(0, downArrow);
	lcd.begin(16, 2);
	clearDisplays();

	// App start
	eventsInit();
	timeInit();
	showMenu();
}

void loop() {
	eventsUpdateLoop();
}
