#include <Arduino.h>
#include "menu.h"
#include "pong.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"

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
				static char joueur1[] = "1 joueur";
				newScroll(joueur1);
				printLcd(8 - (sizeof joueur1) / 2, 0, joueur1);
				printLcd(0, 1, "Valider ");
				lcd.write(0);
				break;
			case 1:
				static char joueurs2[] = "2 joueurs";
				newScroll(joueurs2);
				printLcd(8 - (sizeof joueurs2) / 2, 0, joueurs2);
				printLcd(0, 1, "Valider ");
				lcd.write(0);
				break;
			default:
				static char aPropos[] = "A propos";
				newScroll(aPropos);
				printLcd(8 - (sizeof aPropos) / 2, 0, aPropos);
				printLcd(0, 1, "Valider ");
				lcd.write(0);
		}
	} else {
		switch (menuSelection) {
			case 0:
				switch (submenuSelection) {
					case 1:
						static char course[] = "Course";
						newScroll(course);
						printLcd(8 - (sizeof course) / 2, 0, course);
						printLcd(2, 1, "Jouer ");
						lcd.write(0);
						break;
					case 2:
						static char snake[] = "Snake";
						newScroll(snake);
						printLcd(8 - (sizeof snake) / 2, 0, snake);
						printLcd(2, 1, "Jouer ");
						lcd.write(0);
						break;
					case 3:
						clearDisplays();
						drawImage(returnIcon);
						printLcd(4, 0, "Retour");
						lcd.setCursor(8, 1);
						lcd.write(0);
				}
				break;
			case 1:
				switch(submenuSelection) {
					case 1:
						static char pong[] = "Pong";
						newScroll(pong);
						printLcd(8 - (sizeof pong) / 2, 0, pong);
						printLcd(2, 1, "Jouer ");
						lcd.write(0);
						break;
					case 2:
						static char tron[] = "Tron";
						newScroll(tron);
						printLcd(8 - (sizeof tron) / 2, 0, tron);
						printLcd(2, 1, "Jouer ");
						lcd.write(0);
						break;
					case 3:
						clearDisplays();
						drawImage(returnIcon);
						printLcd(4, 0, "Retour");
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
	if (submenuSelection == 0) {
		submenuSelection = 1;
		clearLcdLine(1);
	} else {
		switch (menuSelection) {
			case 0:
				switch (submenuSelection) {
					case 3:
						submenuSelection = 0;
				}
				break;
			case 1:
				switch (submenuSelection) {
					case 1:
						clearDisplays();
						showPong();
						return;
						break;
					case 3:
						submenuSelection = 0;
				}
		}
	}

	displayMenu();
}

void showMenu(void)
{
	clearDisplays();

	setSingleClickHandler(PLAYER1_LEFT, left);
	setSingleClickHandler(PLAYER2_LEFT, left);
	setSingleClickHandler(PLAYER1_RIGHT, right);
	setSingleClickHandler(PLAYER2_RIGHT, right);
	setSingleClickHandler(MENU, menu);

	menuSelection = 0;
	submenuSelection = 0;

	displayMenu();
}
