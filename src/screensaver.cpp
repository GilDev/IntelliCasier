#include <Arduino.h>
#include "config.h"
#include "displays.h"
#include "events.h"
#include "localization.h"
#include "screensaver.h"
#include "apps/menu.h"
#include "apps/options.h"

bool displayingScreensaver = false;
unsigned short screensaverDelay;

static TimerId displayUpdateTimer;
static TimerId lcdUpdateTimer;
static byte y, x;

static void displayUpdate(byte data)
{
	matrix.setLed(0, y, x, false);
	if (++y == 8) {
		y = 1;
		x = random(8);
	}
	matrix.setLed(0, y, x, true);

	displayUpdateTimer = registerTimerEvent(50, displayUpdate, 0);
}

static void lcdUpdate(byte data)
{
	static bool displaying = false;

	if (displaying) {
		displaying = false;
		clearLcdLine(0);
	} else {
		displaying = true;
		printLcd(8 - stringsSizes[SMART_LOCKER] / 2, 0, buffer2);
	}

	lcdUpdateTimer = registerTimerEvent(500, lcdUpdate, 0);
}

void exitScreensaver(byte data)
{
	cancelTimerEvent(&displayUpdateTimer);
	cancelTimerEvent(&lcdUpdateTimer);
	matrix.setIntensity(0, options[MATRIX_BRIGHTNESS_O]);
	stopLcdScroll(1);
	displayingScreensaver = false;
	showMenu();
}

void showScreensaver(void)
{
	#ifdef DEBUG
		Serial.print(F("Displaying screensaver\n"));
	#endif

	displayingScreensaver = true;

	setSingleClickHandler(PLAYER1_LEFT, exitScreensaver, 0);
	setSingleClickHandler(PLAYER1_RIGHT, exitScreensaver, 0);
	setSingleClickHandler(PLAYER2_LEFT, exitScreensaver, 0);
	setSingleClickHandler(PLAYER2_RIGHT, exitScreensaver, 0);
	setSingleClickHandler(MENU, exitScreensaver, 0);

	clearDisplays();
	cancelAllTimerEvents();

	matrix.setIntensity(0, options[MATRIX_SCREENSAVER_BRIGHTNESS_O]);
	matrix.setRow(0, 0, 255);
	y = 7;
	x = random(8);

	strcpy_P(buffer, (char *) pgm_read_word(&(strings[PRESS_BUTTON])));
	strcpy_P(buffer2, (char *) pgm_read_word(&(strings[SMART_LOCKER])));

	newLcdScroll(buffer, 1, 200);

	displayUpdateTimer = registerTimerEvent(50, displayUpdate, 0);
	lcdUpdateTimer = registerTimerEvent(500, lcdUpdate, 0);
}
