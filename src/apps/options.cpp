#include <Arduino.h>
#include "options.h"
#include "menu.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

#define NUMBER_OF_OPTIONS (sizeof options / sizeof *options)

static byte selectedOption;

unsigned short options[] = { // Default values
	DEFAULT_MATRIX_INTENSITY,
	SCREENSAVER_MATRIX_INTENSITY,
	TEXT_SCROLLING_SPEED,
	DELAY_BETWEEN_PADDLE_MOVES,
	STARTING_SPEED,
	MAXIMUM_SPEED,
	DELAY_BETWEEN_SPEED_INCREASE
};

static void updateValue(void)
{
	switch (selectedOption) {
		case BRIGHTNESS:
			matrix.setIntensity(0, options[BRIGHTNESS]);
			break;
	}

	clearLcdLine(1);
	printLcd(7, 1, options[selectedOption]);
}

static void print(void)
{
	stopLcdScroll(0);
	clearLcdLine(0);

	if (stringsSizes[OPTION_1 + selectedOption] > 16)
		newLcdScroll(strings[OPTION_1 + selectedOption], 0, 200);
	else
		printLcd(8 - stringsSizes[OPTION_1 + selectedOption], 0, strings[OPTION_1 + selectedOption]);

	updateValue();
}

static void changeValue(byte plus)
{
	char multiplier = (plus == 0) ? -1 : 1;

	switch (selectedOption) {
		case MATRIX_SCROLLING_SPEED:
		case PONG_PADDLE_SPEED:
		case PONG_START_DELAY:
		case PONG_MIN_DELAY:
			options[selectedOption] += 10 * multiplier;
			break;

		case SPEED_INCREASE_DELAY:
			options[selectedOption] += 100 * multiplier;
			break;

		default:
			options[selectedOption] += 1 * multiplier;
	}
	
	updateValue();
}

static void next(byte data)
{
	if (selectedOption == NUMBER_OF_OPTIONS - 1)
		selectedOption = 0;
	else
		selectedOption++;

	print();
}

static void previous(byte data)
{
	if (selectedOption == 0)
		selectedOption = NUMBER_OF_OPTIONS - 1;
	else
		selectedOption--;

	print();
}

static void menu(byte data)
{
	stopLcdScroll(0);
	stopMatrixScroll();
	showMenu();
}

void showOptions(void)
{
	clearDisplays();

	setSingleClickHandler(PLAYER1_LEFT, previous, 0);
	setSingleClickHandler(PLAYER1_RIGHT, next, 0);
	setRepeatClickHandler(PLAYER2_LEFT, 100, changeValue, 1);
	setRepeatClickHandler(PLAYER2_RIGHT, 100, changeValue, 0);
	setSingleClickHandler(MENU, menu, 0);

	selectedOption = 0;

	newMatrixScroll("SECRET");

	print();
}
