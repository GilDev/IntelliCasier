#include <Arduino.h>
#include "menu.h"
#include "../global.h"
#include "../config.h"
#include "../events.h"
#include "../scrolling.h"

static byte menuSelection;
static byte submenuSelection; // 0 if not in a submenu

static void displayMenu(void)
{
	clearDisplays();

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
				}
		}
	}
}

void left(void)
{
	if (submenuSelection == 0) {
		if (menuSelection == 0)
			menuSelection = 2;
		else
			menuSelection--;
	} else {
		if (submenuSelection == 1)
			submenuSelection = 2;
		else
			submenuSelection--;
	}

	displayMenu();
}

void right(void)
{
	if (submenuSelection == 0) {
		if (menuSelection == 2)
			menuSelection = 0;
		else
			menuSelection++;
	} else {
		if (submenuSelection == 2)
			submenuSelection = 1;
		else
			submenuSelection++;
	}

	displayMenu();
}

void menu(void)
{
	if (submenuSelection == 0)
		submenuSelection = 1;
	else
		submenuSelection = 0;

	displayMenu();
}

void showMenu(void)
{
	setSingleClickHandler(PLAYER1_LEFT, left);
	setSingleClickHandler(PLAYER2_LEFT, left);
	setSingleClickHandler(PLAYER1_RIGHT, right);
	setSingleClickHandler(PLAYER2_RIGHT, right);
	setSingleClickHandler(MENU, menu);

	menuSelection = 0;
	submenuSelection = 0;

	displayMenu();
}
