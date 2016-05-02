#include <Arduino.h>
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../screensaver.h"
#include "../localization.h"
#include <avr/pgmspace.h>

#ifdef APP_ABOUT
#include "about.h"
#endif

#ifdef APP_FLAPPYBIRD
#include "flappybird.h"
#endif

#ifdef APP_PONG
#include "pong.h"
#endif

#ifdef APP_SNAKE
#include "snake.h"
#endif

#ifdef APP_SNAKE2
#include "snake2.h"
#endif

static void exitMenu(void);

struct selection {
	byte text;
	byte icon[8];
	byte menuToOpen; // 0xFF = call callback() instead
	void (*callback)(void);
};

byte currentMenu;
byte currentSelection;

static struct selection menuMain[] = {
	{
		.text = GAMES_1_PLAYER_LCD,
		.icon = {
			B00001000,
			B00011000,
			B00001000,
			B00001000,
			B00001000,
			B00001000,
			B00001000,
			B00011100
		},
		.menuToOpen = 1
	},
	{
		.text = GAMES_2_PLAYERS_LCD,
		.icon = {
			B00111100,
			B01000010,
			B00000010,
			B00000100,
			B00001000,
			B00010000,
			B00100000,
			B01111110
		},
		.menuToOpen = 2
	},
	#ifdef APP_ABOUT
	{
		.text = ABOUT,
		.icon = {
			B00111100,
			B01000010,
			B00000010,
			B00000100,
			B00001000,
			B00001000,
			B00000000,
			B00001000
		},
		.menuToOpen = -1,
		.callback = showAbout
	}
	#endif
};

static struct selection menu1PlayerGames[] = {
	#ifdef APP_SNAKE
	{
		.text = SNAKE,
		.icon = {
			B00111100,
			B01000010,
			B00000010,
			B00000100,
			B00001000,
			B00001000,
			B00000000,
			B00001000
		},
		.menuToOpen = -1,
		.callback = showSnake
	},
	#endif
	#ifdef APP_FLAPPYBIRD
	{
		.text = FLAPPYBIRD,
		.icon = {
			B00111100,
			B01000010,
			B00000010,
			B00000100,
			B00001000,
			B00001000,
			B00000000,
			B00001000
		},
		.menuToOpen = -1,
		.callback = showFlappyBird
	},
	#endif
	{
		.text = BACK,
		.icon = {
			B00010000,
			B00110000,
			B01111100,
			B00110010,
			B00010010,
			B01000010,
			B00111100,
			B00000000
		},
		.menuToOpen = 0,
	}
};
static struct selection menu2PlayerGames[] = {
	#ifdef APP_PONG
	{
		.text = PONG,
		.icon = {
			B00111100,
			B01000010,
			B00000010,
			B00000100,
			B00001000,
			B00001000,
			B00000000,
			B00001000
		},
		.menuToOpen = -1,
		.callback = showPong
	},
	#endif
	#ifdef APP_SNAKE2
	{
		.text = SNAKE,
		.icon = {
			B00111100,
			B01000010,
			B00000010,
			B00000100,
			B00001000,
			B00001000,
			B00000000,
			B00001000
		},
		.menuToOpen = -1,
		.callback = showSnake2
	},
	#endif
	{
		.text = BACK,
		.icon = {
			B00010000,
			B00110000,
			B01111100,
			B00110010,
			B00010010,
			B01000010,
			B00111100,
			B00000000
		},
		.menuToOpen = 0,
	}
};

static byte numberOfMenuItems[] = {
	sizeof menuMain / sizeof *menuMain - 1,
	sizeof menu1PlayerGames / sizeof *menu1PlayerGames - 1,
	sizeof menu2PlayerGames / sizeof *menu2PlayerGames - 1
};

static struct selection *menu[] = {
	menuMain,
	menu1PlayerGames,
	menu2PlayerGames
};

static void displayMenu(void)
{
	clearLcdLine(0);

	static char buffer[40];
	strcpy_P(buffer, (char *) pgm_read_word(&(strings[menu[currentMenu][currentSelection].text])));

	if (menu[currentMenu][currentSelection].menuToOpen == 0)
		drawImage(menu[currentMenu][currentSelection].icon);
	else {
		newMatrixScroll(buffer);
	}

	printLcd(8 - stringsSizes[menu[currentMenu][currentSelection].text] / 2, 0, buffer);
}

static void changeSelection(byte right)
{
	if (right) {
		if (currentSelection++ == numberOfMenuItems[currentMenu])
			currentSelection = 0;
	} else {
		if (currentSelection-- == 0)
			currentSelection = numberOfMenuItems[currentMenu];
	}

	displayMenu();
}

static void enter(byte data)
{
	if (menu[currentMenu][currentSelection].menuToOpen == 0xFF) {
		(*(menu[currentMenu][currentSelection].callback))();
	} else {
		currentMenu = menu[currentMenu][currentSelection].menuToOpen;
		currentSelection = 0;
		displayMenu();
	}
}

void showMenu(void)
{
	clearDisplays();

	strcpy_P(buffer2, (char *) pgm_read_word(&(strings[CONFIRM])));
	printLcd(7 - stringsSizes[CONFIRM], 1, buffer2);
	lcd.setCursor(8, 1);
	lcd.write(0);

	setSingleClickHandler(PLAYER1_LEFT, changeSelection, 0);
	setSingleClickHandler(PLAYER2_LEFT, changeSelection, 0);
	setSingleClickHandler(PLAYER1_RIGHT, changeSelection, 1);
	setSingleClickHandler(PLAYER2_RIGHT, changeSelection, 1);
	setSingleClickHandler(MENU, enter, 0);

	screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_MENU * 1000;

	currentMenu = currentSelection = 0;

	displayMenu();
}

static void exitMenu(void)
{
	clearDisplays();
	screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_APP * 1000;
}
