#include <Arduino.h>
#include "common.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

void countdown()
{
	byte one[8] = {
		B00001000,
		B00011000,
		B00001000,
		B00001000,
		B00001000,
		B00001000,
		B00001000,
		B00011100
	};

	byte two[8] = {
		B00111100,
		B01000010,
		B00000010,
		B00000100,
		B00001000,
		B00010000,
		B00100000,
		B01111110
	};

	byte three[8] = {
		B00111100,
		B01000010,
		B00000010,
		B00011100,
		B00000010,
		B00000010,
		B01000010,
		B00111100
	};

	clearDisplays();

	drawImage(three);
	printLcd(8 - strlen(strings[READY]) / 2, 0, strings[READY]);
	delay(1000);
	drawImage(two);
	clearLcdLine(0);
	printLcd(8 - strlen(strings[SET]) / 2, 0, strings[SET]);
	delay(1000);
	drawImage(one);
	clearLcdLine(0);
	printLcd(8 - strlen(strings[GO]) / 2, 0, strings[GO]);
	delay(1000);

	clearDisplays();
}
