#include <Arduino.h>
#include "menu.h"
#include "../global.h"
#include "../config.h"
#include "../events.h"
#include "../scrolling.h"

static byte menuSelection    = 0;
static byte submenuSelection = 0; // 0 if not in a submenu

static void displayMenu(void)
{
	if (submenuSelection == 0) {
		switch (menuSelection) {
			case 0:
				newScroll("1 joueur");
				break;
			case 1:
				newScroll("2 joueurs");
				break;
			default:
				//newScroll("A propos");
			stopScrolling();
			for (byte i = 0; i < 8; i++)
				matrix.setRow(0, i, 255);
		}
	} else {
		switch (menuSelection) {
			case 0:
				switch (submenuSelection) {
					case 1:
						newScroll("Course");
						break;
					case 2:
						newScroll("Snake");
						break;
				}
				break;
			case 1:
				switch(submenuSelection) {
					case 1:
						newScroll("Pong");
						break;
					case 2:
						newScroll("Tron");
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
	if (menuSelection == 2) {
		static byte foo = 0;
		if (++foo == 16)
			foo = 0;
		matrix.setIntensity(0, foo);
	} else
	if (submenuSelection == 0)
		submenuSelection = 1;
	else
		submenuSelection = 0;

	displayMenu();
}

void menuInit()
{
	setSingleClickHandler(PLAYER1_LEFT, left);
	setSingleClickHandler(PLAYER2_LEFT, left);
	setSingleClickHandler(PLAYER1_RIGHT, right);
	setSingleClickHandler(PLAYER2_RIGHT, right);
	setSingleClickHandler(MENU, menu);
	displayMenu();
}
