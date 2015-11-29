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
				newMatrixScroll(strings[GAMES_1_PLAYER_MATRIX]);
				printLcd(8 - strlen(strings[GAMES_1_PLAYER_LCD]) / 2, 0, strings[GAMES_1_PLAYER_LCD]);
				printLcd(7 - strlen(strings[CONFIRM]), 1, strings[CONFIRM]);
				// Print arrow
				lcd.setCursor(8, 1);
				lcd.write(0);
				break;
			case 1:
				stopMatrixScroll();
				//newMatrixScroll(strings[GAMES_2_PLAYERS_MATRIX]);
				printLcd(8 - strlen(strings[GAMES_2_PLAYERS_LCD]) / 2, 0, strings[GAMES_2_PLAYERS_LCD]);
				printLcd(7 - strlen(strings[CONFIRM]), 1, strings[CONFIRM]);
				// Print arrow
				lcd.setCursor(8, 1);
				lcd.write(0);
				newLcdScroll("Hey nique ta race", 1, 250);
				newLcdScroll("Hey toi !", 0, 500);
				break;
			case 2:
				newMatrixScroll(strings[ABOUT]);
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
						newMatrixScroll(strings[SNAKE]);
						printLcd(8 - strlen(strings[SNAKE]) / 2, 0, strings[SNAKE]);
						printLcd(7 - strlen(strings[PLAY]), 1, strings[PLAY]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
						break;
					case 2:
						newMatrixScroll(strings[RACE]);
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
						newMatrixScroll(strings[PONG]);
						printLcd(8 - strlen(strings[PONG]) / 2, 0, strings[PONG]);
						printLcd(7 - strlen(strings[PLAY]), 1, strings[PLAY]);
						// Print arrow
						lcd.setCursor(8, 1);
						lcd.write(0);
						break;
					case 2:
						newMatrixScroll(strings[TRON]);
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

static void left(byte data)
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

static void right(byte data)
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

static void menu(byte data)
{
	if (submenuSelection == 0) { // Entering submenu
		submenuSelection = 1;
		printLcd(0, 1, F("        "));
		displayMenu();
	} else {
		// "Return" menu options
		if (menuSelection == 0 && submenuSelection == 3 ||
			menuSelection == 1 && submenuSelection == 3) {
			submenuSelection = 0;
			printLcd(0, 1, F("        "));
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

	setSingleClickHandler(PLAYER1_LEFT, left, 0);
	setSingleClickHandler(PLAYER2_LEFT, left, 0);
	setSingleClickHandler(PLAYER1_RIGHT, right, 0);
	setSingleClickHandler(PLAYER2_RIGHT, right, 0);
	setSingleClickHandler(MENU, menu, 0);

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
