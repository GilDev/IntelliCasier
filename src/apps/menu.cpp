#include <Arduino.h>
#include "about.h"
#include "flappybird.h"
#include "pong.h"
#include "snake.h"
#include "snake2.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../screensaver.h"
#include "../localization.h"

static void exitMenu(void);

static byte returnIcon[8] = {
	B00010000,
	B00110000,
	B01111100,
	B00110010,
	B00010010,
	B01000010,
	B00111100,
	B00000000
};

struct selection {
	byte text;
	byte icon[8];
	byte menuToOpen; // -1 = call callback() instead
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
};

static struct selection menu1PlayerGames[] = {
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
		.callback = showSnake
	},
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

	//drawImage(menu[currentMenu][currentSelection].icon);
	newMatrixScroll(strings[menu[currentMenu][currentSelection].text]);

	printLcd(8 - stringsSizes[menu[currentMenu][currentSelection].text] / 2,
		0,
		strings[menu[currentMenu][currentSelection].text]);
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

	printLcd(7 - stringsSizes[CONFIRM], 1, "Valider");
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
