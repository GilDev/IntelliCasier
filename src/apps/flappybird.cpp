#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "options.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

static TimerId updateBirdTimer = -1, updateWallsTimer = -1, updateSpeedTimer = -1;

static struct {
	byte y:3;
	byte wallPassed;
	unsigned short speed;
} bird;

static struct {
	byte x:4;
	byte yHole:3;
} walls[2];

static void gameOver(void);

static void updateSpeed(byte data)
{
	if (bird.speed > options[FLAPPY_BIRD_MIN_DELAY_O]) {
		bird.speed -= 50;
		updateSpeedTimer = registerTimerEvent(options[FLAPPY_BIRD_SPEED_INCREASES_DELAY_O], updateSpeed, 0);
	}
}

static bool collision(void)
{
	byte i;
	for (i = 0; i < sizeof walls; i++) {
		if (walls[i].x == 0) {
			if (bird.y < walls[i].yHole || bird.y > walls[i].yHole + 2) {
				gameOver();
				return true;
			}
		}
	}

	return false;
}

static void generateWall(byte id)
{
	walls[id].x = 8;
	walls[id].yHole = random(0, 6);
}

static void updateWalls(byte data)
{
	byte i;
	for (i = 0; i < sizeof walls; i++) {
		if (walls[i].x == 0) {
			matrix.setColumn(0, 0, 0x00);
			matrix.setLed(0, bird.y, 0, true);
			generateWall(i);
			bird.wallPassed++;
			printLcd(11, 0, bird.wallPassed);
		} else {
			matrix.setColumn(0, walls[i].x, 0x00);
			walls[i].x--;
			byte j;
			for (j = 0; j < 8; j++) { // Draw wall
				if (j >= walls[i].yHole && j <= walls[i].yHole + 2) { // Don't draw holes
					continue;
				}
				matrix.setLed(0, j, walls[i].x, true);
			}
		}
	}

	if (collision())
		return;

	updateWallsTimer = registerTimerEvent(bird.speed, updateWalls, 0);
}

static void updateBird(byte data)
{
	if (bird.y == 7) {
		gameOver();
		return;
	}

	matrix.setLed(0, bird.y, 0, false);
	bird.y++;
	matrix.setLed(0, bird.y, 0, true);

	if (collision())
		return;

	updateBirdTimer = registerTimerEvent(options[FLAPPY_BIRD_FALL_DELAY_O], updateBird, 0);
}

static void fly(byte data)
{
	if (bird.y != 0) {
		matrix.setLed(0, bird.y, 0, false);
		bird.y--;
		matrix.setLed(0, bird.y, 0, true);

		collision();
	}
}

static void endRound(void)
{
	cancelTimerEvent(&updateBirdTimer);
	cancelTimerEvent(&updateWallsTimer);
}

static void menu(byte data)
{
	endRound();
	showMenu();
}

static void start(void)
{
	setSingleClickHandler(PLAYER1_LEFT, fly, 0);
	setSingleClickHandler(PLAYER1_RIGHT, fly, 0);
	setSingleClickHandler(PLAYER2_LEFT, fly, 0);
	setSingleClickHandler(PLAYER2_RIGHT, fly, 0);
	setSingleClickHandler(MENU, menu, 0);

	printLcd(3, 0, "Score :");
	printLcd(11, 0, bird.wallPassed);

	delay(500);

	updateBirdTimer = registerTimerEvent(options[FLAPPY_BIRD_FALL_DELAY_O], updateBird, 0);
	updateWallsTimer = registerTimerEvent(bird.speed, updateWalls, 0);
	updateSpeedTimer = registerTimerEvent(options[FLAPPY_BIRD_SPEED_INCREASES_DELAY_O], updateSpeed, 0);
}

static void newRound(void)
{
	endRound();

	bird.y = 2;
	bird.speed = options[FLAPPY_BIRD_START_DELAY_O];
	bird.wallPassed = 0;

	walls[0].x = 8;
	walls[1].x = 12;
	walls[0].yHole = 2;
	walls[1].yHole = 4;

	countdown(start, menu);
}

static void gameOver(void)
{
	endRound();
	strcpy_P(buffer, (char *) pgm_read_word(&(strings[GAME_OVER])));
	printLcd(3, 1, buffer);
	wipeMatrix();
	delay(500);
	newRound();
}

void showFlappyBird(void)
{
	updateBirdTimer = updateWallsTimer = -1;

	newRound();
}
