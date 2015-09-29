#include <Arduino.h>
#include "screensaver.h"
#include "global.h"
#include "config.h"
#include "events.h"
#include "apps/menu.h"

static TimerId id;
static byte y, x;
bool displayingScreensaver = false;
unsigned short screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_MENU * 1000;

static void displayUpdate(void)
{
	matrix.setLed(0, y, x, false);
	if (++y == 8) {
		y = 1;
		x = random(8);
	}
	matrix.setLed(0, y, x, true);
}

void exitScreensaver(void)
{
	cancelTimerEvent(id);
	matrix.setIntensity(0, DEFAULT_MATRIX_INTENSITY);
	displayingScreensaver = false;
	showMenu();
}

void showScreensaver(void)
{
	displayingScreensaver = true;

	setSingleClickHandler(PLAYER1_LEFT, exitScreensaver);
	setSingleClickHandler(PLAYER1_RIGHT, exitScreensaver);
	setSingleClickHandler(PLAYER2_LEFT, exitScreensaver);
	setSingleClickHandler(PLAYER2_RIGHT, exitScreensaver);
	setSingleClickHandler(MENU, exitScreensaver);

	clearDisplays();
	matrix.setIntensity(0, SCREENSAVER_MATRIX_INTENSITY);
	matrix.setRow(0, 0, 255);

	y = 7;
	x = random(8);

	id = registerTimerEvent(50, displayUpdate);
}
