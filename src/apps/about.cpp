#include <Arduino.h>
#include "menu.h"
#include "options.h"
#include "schedule.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

static byte optionsCode[] = {0, 3, 1, 2};
static byte optionsPos;
static byte scheduleCode[] = {0, 1, 2, 3};
static byte schedulePos;

static void exitAbout(void);

static void advanceCode(byte button)
{
	if (optionsCode[optionsPos] == button) {
		optionsPos++;
		if (optionsPos == sizeof optionsCode) {
			exitAbout();
			showOptions();
		}
	} else {
		optionsPos = 0;
	}

	if (scheduleCode[schedulePos] == button) {
		schedulePos++;
		if (schedulePos == sizeof scheduleCode) {
			exitAbout();
			showSchedule();
		}
	} else {
		schedulePos = 0;
	}
}

static void menu(byte data)
{
	exitAbout();
	showMenu();
}

static void exitAbout(void)
{
	stopMatrixScroll();
	stopLcdScroll(1);
}

void showAbout(void)
{
	setSingleClickHandler(PLAYER1_LEFT, advanceCode, 0);
	setSingleClickHandler(PLAYER1_RIGHT, advanceCode, 1);
	setSingleClickHandler(PLAYER2_LEFT, advanceCode, 2);
	setSingleClickHandler(PLAYER2_RIGHT, advanceCode, 3);
	setSingleClickHandler(MENU, menu, 0);

	optionsPos = schedulePos = 0;

	strcpy_P(buffer, (char *) pgm_read_word(&(strings[SMART_LOCKER])));
	strcpy_P(buffer2, (char *) pgm_read_word(&(strings[BY_GILDEV])));

	newMatrixScroll(buffer);
	printLcd(7 - stringsSizes[BY_GILDEV] / 2, 0, buffer2); // Not centered in English
	newLcdScroll("https://github.com/GilDev/IntelliCasier", 1, 200);
}
