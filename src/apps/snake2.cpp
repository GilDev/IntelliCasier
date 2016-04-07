#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "options.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"

#define LEFT_P1  0
#define RIGHT_P1 1
#define LEFT_P2  2
#define RIGHT_P2 3

static TimerId updateSnakesTimer = -1, blinkAppleTimer = -1, blinkSecondSnakeTimer = -1;

static struct {
	byte x:3;
	byte y:3;
	byte on:1; // Used to blink apple
} apple;

static struct {
	byte score;
	byte length:6;
	byte hasTurned:1; // Avoid turning multiple times without moving
	signed char xDir:2;
	signed char yDir:2;
	struct {
		byte x[64];
		byte y[64];
	} body;
} snake[2];

bool secondSnakeOn;

static void generateApple()
{
	bool collision = true;
	while (collision) {
		apple.x = random(8);
		apple.y = random(8);
		collision = false;
		byte s;
		for (s = 0; s < 2 && collision == false; s++) {
			byte i;
			for (i = 0; i < snake[s].length; i++)
				if (apple.x == snake[s].body.x[i] && apple.y == snake[s].body.y[i]) {
					collision = true;
					break;
				}
		}
	}

	apple.on = false;
}

static void gameOver(byte s);

static void updateSnakes(byte data)
{
	byte s;
	for (s = 0; s < 2; s++) {
		matrix.setLed(0, snake[s].body.y[snake[s].length - 1], snake[s].body.x[snake[s].length - 1], false);

		byte i;
		for (i = snake[s].length - 1; i > 0; i--) {
			snake[s].body.x[i] = snake[s].body.x[i - 1];
			snake[s].body.y[i] = snake[s].body.y[i - 1];
		}

		snake[s].body.x[0] += snake[s].xDir;
		snake[s].body.y[0] += snake[s].yDir; 

		if (snake[s].body.x[0] == 8 || snake[s].body.y[0] == 8 || snake[s].body.x[0] == 0xFF || snake[s].body.y[0] == 0xFF) {
			gameOver(s);
			return;
		}

		// Colisions check
		byte s2;
		for (s2 = 0; s2 < 2; s2++) {
			for (i = (s2 == s) ? 4 : 1; i < snake[s2].length; i++) {
				if (snake[s].body.x[0] == snake[s2].body.x[i] && snake[s].body.y[0] == snake[s2].body.y[i]) {
					gameOver(s);
					return;
				}

			}
		}

		matrix.setLed(0, snake[s].body.y[0], snake[s].body.x[0], true);

		if (snake[s].body.x[0] == apple.x && snake[s].body.y[0] == apple.y) {
			printLcd((s == 0) ? 2 : 13, 1, ++snake[s].length - 3);
			generateApple();
		}

		snake[s].hasTurned = false;
	}

	if (snake[0].body.x[0] == snake[1].body.x[0] && snake[0].body.y[0] == snake[1].body.y[0]) // Collision head to head
		gameOver(2);

	updateSnakesTimer = registerTimerEvent(options[SNAKE_MOVE_DELAY_O], updateSnakes, 0);
}

static void blinkApple(byte data)
{
	if (apple.on)
		apple.on = false;
	else
		apple.on = true;

	matrix.setLed(0, apple.y, apple.x, apple.on);

	blinkAppleTimer = registerTimerEvent(50, blinkApple, 0);
}

static void blinkSecondSnake(byte data)
{
	if (secondSnakeOn)
		secondSnakeOn = false;
	else
		secondSnakeOn = true;

	byte i;
	for (i = 0; i < snake[1].length; i++)
		matrix.setLed(0, snake[1].body.y[i], snake[1]. body.x[i], secondSnakeOn);

	blinkSecondSnakeTimer = registerTimerEvent(100, blinkSecondSnake, 0);
}

static void move(byte move)
{
	byte s = (move == LEFT_P2 || move == RIGHT_P2) ? 1 : 0;

	if (!snake[s].hasTurned) {
		byte tmp;
		if (move == RIGHT_P1 || move == RIGHT_P2) {
			tmp = snake[s].xDir;
			snake[s].xDir = snake[s].yDir * -1;
			snake[s].yDir = tmp;
		} else {
			tmp = snake[s].yDir;
			snake[s].yDir = snake[s].xDir * -1;
			snake[s].xDir = tmp;
		}
		snake[s].hasTurned = true;
	}
}

static void endRound(void)
{
	cancelTimerEvent(&updateSnakesTimer);
	cancelTimerEvent(&blinkAppleTimer);
	cancelTimerEvent(&blinkSecondSnakeTimer);
}

static void menu(byte data)
{
	endRound();
	showMenu();
}

static void start(void)
{
	setSingleClickHandler(PLAYER1_LEFT, move, LEFT_P1);
	setSingleClickHandler(PLAYER1_RIGHT, move, RIGHT_P1);
	setSingleClickHandler(PLAYER2_LEFT, move, LEFT_P2);
	setSingleClickHandler(PLAYER2_RIGHT, move, RIGHT_P2);
	setSingleClickHandler(MENU, menu, 0);

	byte s;
	for (s = 0; s < 2; s++) {
		byte i;
		for (i = 0; i < snake[s].length; i++)
			matrix.setLed(0, snake[s].body.y[i], snake[s].body.x[i], true);
	}

	matrix.setLed(0, apple.y, apple.x, apple.on);

	printLcd(3, 0, "Scores :");
	printLcd(2, 1, snake[0].score);
	printLcd(7, 1, "--");
	printLcd(13, 1, snake[1].score);

	delay(500);

	updateSnakesTimer = registerTimerEvent(options[SNAKE_MOVE_DELAY_O], updateSnakes, 0);
	blinkApple(0);
	blinkSecondSnake(0);
}

static void newRound(void)
{
	endRound();

	snake[0].yDir = snake[1].yDir = 0;
	snake[0].length = snake[1].length = 3;
	snake[0].hasTurned = snake[1].hasTurned = false;
	
	snake[0].xDir = 1;
	snake[0].body.x[0] = 2;
	snake[0].body.y[0] = 1;
	snake[0].body.x[1] = 1;
	snake[0].body.y[1] = 1;
	snake[0].body.x[2] = 1;
	snake[0].body.y[2] = 2;

	snake[1].xDir = -1;
	snake[1].body.x[0] = 5;
	snake[1].body.y[0] = 6;
	snake[1].body.x[1] = 6;
	snake[1].body.y[1] = 6;
	snake[1].body.x[2] = 6;
	snake[1].body.y[2] = 5;

	generateApple();
	apple.on = false;
	secondSnakeOn = false;

	countdown(start, menu);
}

static void gameOver(byte s)
{
	endRound();
	matrix.setLed(0, apple.y, apple.x, true); // In case the apple is off at that time
	printLcd(3, 0, strings[GAME_OVER]);
	switch (s) {
		case 0:
			printLcd(0, 1, strings[PLAYER_WIN]);
			printLcd(7, 1, "2");
			snake[1].score++;
			break;

		case 1:
			printLcd(0, 1, strings[PLAYER_WIN]);
			printLcd(7, 1, "1");
			snake[0].score++;
			break;

		case 2:
			printLcd(4, 1, strings[TIE]);
	}
	wipeMatrix();
	delay(500);
	newRound();
}

void showSnake2(void)
{
	updateSnakesTimer = blinkAppleTimer = -1;
	snake[0].score == snake[1].score == 0;

	newRound();
}
