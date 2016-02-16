#include <Arduino.h>
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

static TimerId countdownTimerEvent = -1;
static void (*endcallback)();
static void (*exitcallback)(byte data);

static byte numbers[][8] = {
	{
	B00111100,
	B01000010,
	B00000010,
	B00011100,
	B00000010,
	B00000010,
	B01000010,
	B00111100
	},
	{
	B00111100,
	B01000010,
	B00000010,
	B00000100,
	B00001000,
	B00010000,
	B00100000,
	B01111110
	},
	{
	B00001000,
	B00011000,
	B00001000,
	B00001000,
	B00001000,
	B00001000,
	B00001000,
	B00011100
	}
};

static void null(byte data) {}

static void count(byte number)
{
	switch (number) {
		case 3:
			drawImage(numbers[0]);
			printLcd(8 - stringsSizes[READY] / 2, 0, strings[READY]);
			break;

		case 2:
			drawImage(numbers[1]);
			clearLcdLine(0);
			printLcd(8 - stringsSizes[SET] / 2, 0, strings[SET]);
			break;

		case 1:
			drawImage(numbers[2]);
			clearLcdLine(0);
			printLcd(8 - stringsSizes[GO] / 2, 0, strings[GO]);
			break;

		case 0:
			cancelTimerEvent(&countdownTimerEvent);
			clearDisplays();
			(*endcallback)();
	}

	if (number > 0)
		countdownTimerEvent = registerTimerEvent(1000, count, number - 1);
}

static void menu(byte data)
{
	cancelTimerEvent(&countdownTimerEvent);
	(*exitcallback)(0);
}

void countdown(void (*endCallback)(void), void (*exitCallback)(byte data))
{
	clearDisplays();

	setSingleClickHandler(PLAYER1_LEFT, null, 0);
	setSingleClickHandler(PLAYER1_RIGHT, null, 0);
	setSingleClickHandler(PLAYER2_LEFT, null, 0);
	setSingleClickHandler(PLAYER2_RIGHT, null, 0);
	setSingleClickHandler(MENU, menu, 0);

	endcallback = endCallback;
	exitcallback = exitCallback;

	count(3);
}

void wipeMatrix(void)
{
	matrix.setRow(0, 0, 0xFF);
	delay(200);
	byte i;
	for (i = 0; i < 7; i++) {
		matrix.setRow(0, i, 0x00);
		matrix.setRow(0, i + 1, 0xFF);
		delay(200);
	}
	matrix.setRow(0, 7, 0x00);
}
