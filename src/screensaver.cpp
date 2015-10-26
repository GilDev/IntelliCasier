#include <Arduino.h>
#include "config.h"
#include "displays.h"
#include "events.h"
#include "screensaver.h"
#include "apps/menu.h"

bool displayingScreensaver = false;
unsigned short screensaverDelay;

static TimerId displayUpdateTimer;
static TimerId lcdUpdateTimer;
static byte y, x;

static void displayUpdate(void)
{
	matrix.setLed(0, y, x, false);
	if (++y == 8) {
		y = 1;
		x = random(8);
	}
	matrix.setLed(0, y, x, true);
}

static void lcdUpdate(void)
{
	static bool displaying = false;

	if (displaying) {
		displaying = false;
		lcd.noDisplay();
	} else {
		displaying = true;
		lcd.display();
	}
}

void exitScreensaver(void)
{
	cancelTimerEvent(displayUpdateTimer);
	cancelTimerEvent(lcdUpdateTimer);
	matrix.setIntensity(0, DEFAULT_MATRIX_INTENSITY);
	lcd.display();
	displayingScreensaver = false;
	showMenu();
}

void showScreensaver(void)
{
	#ifdef DEBUG
		Serial.print("Displaying screensaver\n");
	#endif

	displayingScreensaver = true;

	setSingleClickHandler(PLAYER1_LEFT, exitScreensaver);
	setSingleClickHandler(PLAYER1_RIGHT, exitScreensaver);
	setSingleClickHandler(PLAYER2_LEFT, exitScreensaver);
	setSingleClickHandler(PLAYER2_RIGHT, exitScreensaver);
	setSingleClickHandler(MENU, exitScreensaver);

	clearDisplays();
	cancelAllTimerEvents();

	matrix.setIntensity(0, SCREENSAVER_MATRIX_INTENSITY);
	matrix.setRow(0, 0, 255);
	y = 7;
	x = random(8);

	printLcd(5, 0, "ARCADE");

	displayUpdateTimer = registerTimerEvent(50, displayUpdate);
	lcdUpdateTimer = registerTimerEvent(500, lcdUpdate);
}
