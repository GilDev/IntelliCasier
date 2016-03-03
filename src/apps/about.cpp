#include <Arduino.h>
#include "menu.h"
#include "options.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

static byte code[] = {0, 3, 1, 2};
static byte pos;

static void exitAbout(void);

static void advanceCode(byte button)
{
	if (code[pos] == button) {
		pos++;
		if (pos == sizeof code) {
			exitAbout();
			showOptions();
		}
	} else {
		pos = 0;
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

	pos = 0;

	newMatrixScroll(strings[SMART_LOCKER]);
	printLcd(7 - stringsSizes[BY_GILDEV] / 2, 0, strings[BY_GILDEV]); // Not centered in English
	newLcdScroll("https://github.com/GilDev/IntelliCasier", 1, 200);
}
