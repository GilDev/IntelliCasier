#include <Arduino.h>
#include <avr/pgmspace.h>
#include "menu.h"
#include "pong.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../screensaver.h"
#include "../localization.h"

static void exitMenu(void);

static byte menuSelection;
static byte submenuSelection; // 0 if not in a submenu

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

static void displayMenu(void)
{
	clearLcdLine(0);

	if (submenuSelection == 0) {
		switch (menuSelection) {
			case 0:
				newScroll(strings[GAMES_1_PLAYER]);
				printLcd(8 - strlen(strings[GAMES_1_PLAYER]) / 2, 0, strings[GAMES_1_PLAYER]);
				printLcd(7 - strlen(strings[CONFIRM]), 1, strings[CONFIRM]);
				// Print arrow
				lcd.setCursor(8, 1);
				lcd.write(0);
				break;
			case 1:
				newScroll(strings[GAMES_2_PLAYERS]);
				printLcd(8 - strlen(strings[GAMES_2_PLAYERS]) / 2, 0, strings[GAMES_2_PLAYERS]);
				printLcd(7 - strlen(strings[CONFIRM]), 1, strings[CONFIRM]);
				// Print arrow
				lcd.setCursor(8, 1);
				lcd.write(0);
				break;
			case 2:
				newScroll(strings[ABOUT]);
				printLcd(8 - strlen(strings[ABOUT]) / 2, 0, strings[ABOUT]);
				printLcd(7 - strlen(strings[CONFIRM]), 1, strings[CONFIRM]);
				// Print arrow
				lcd.setCursor(8, 1);
				lcd.write(0);
		}
	} else {
		switch (menuSelection) {
			case 0:
				switch (submenuSelection) {
					case 1:
						newScroll(strings[SNAKE]);
						printLcd(8 - strlen(strings[SNAKE]) / 2, 0, strings[SNAKE]);
						printLcd(7 - strlen(strings[PLAY]), 1, strings[PLAY]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
						break;
					case 2:
						newScroll(strings[RACE]);
						printLcd(8 - strlen(strings[RACE]) / 2, 0, strings[RACE]);
						printLcd(7 - strlen(strings[PLAY]), 1, strings[PLAY]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
						break;
					case 3:
						drawImage(returnIcon);
						printLcd(8 - strlen(strings[BACK]) / 2, 0, strings[BACK]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
				}
				break;
			case 1:
				switch(submenuSelection) {
					case 1:
						newScroll(strings[PONG]);
						printLcd(8 - strlen(strings[PONG]) / 2, 0, strings[PONG]);
						printLcd(7 - strlen(strings[PLAY]), 1, strings[PLAY]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
						break;
					case 2:
						newScroll(strings[TRON]);
						printLcd(8 - strlen(strings[TRON]) / 2, 0, strings[TRON]);
						printLcd(7 - strlen(strings[PLAY]), 1, strings[PLAY]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
						break;
					case 3:
						drawImage(returnIcon);
						printLcd(8 - strlen(strings[BACK]) / 2, 0, strings[BACK]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
				}
		}
	}
}

static void left(void)
{
	if (submenuSelection == 0) {
		if (menuSelection == 0)
			menuSelection = 2;
		else
			menuSelection--;
	} else {
		if (submenuSelection == 1)
			submenuSelection = 3;
		else
			submenuSelection--;
	}

	displayMenu();
}

static void right(void)
{
	if (submenuSelection == 0) {
		if (menuSelection == 2)
			menuSelection = 0;
		else
			menuSelection++;
	} else {
		if (submenuSelection == 3)
			submenuSelection = 1;
		else
			submenuSelection++;
	}

	displayMenu();
}

static void menu(void)
{
	if (submenuSelection == 0) { // Entering submenu
		submenuSelection = 1;
		clearLcdLine(1);
		displayMenu();
	} else {
		// "Return" menu options
		if (menuSelection == 0 && submenuSelection == 3 ||
			menuSelection == 1 && submenuSelection == 3) {
			submenuSelection = 0;
			displayMenu();
		} else { // App launch
			exitMenu();
			switch (menuSelection) {
				case 0: // 1 player games
					switch (submenuSelection) {
						
					}
					break;
				case 1: // 2 players games
					switch (submenuSelection) {
						case 1:
							showPong();
							break;
					}
			}
		}
	}
}

void showMenu(void)
{
	clearDisplays();

	setSingleClickHandler(PLAYER1_LEFT, left);
	setSingleClickHandler(PLAYER2_LEFT, left);
	setSingleClickHandler(PLAYER1_RIGHT, right);
	setSingleClickHandler(PLAYER2_RIGHT, right);
	setSingleClickHandler(MENU, menu);

	screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_MENU * 1000;
	menuSelection = 0;
	submenuSelection = 0;

	displayMenu();
}

static void exitMenu(void)
{
	clearDisplays();
	screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_APP * 1000;
}
