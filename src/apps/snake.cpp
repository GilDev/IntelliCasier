#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "options.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"

#define LEFT  0
#define RIGHT 1

static TimerId updateSnakeTimer = -1;
static TimerId blinkAppleTimer  = -1;

static struct {
	byte x:3;
	byte y:3;
	byte on:1; // Used to blink apple
} apple;

static struct {
	byte length:6;
	byte hasTurned:1; // Avoid turning multiple times without moving
	signed char xDir:2;
	signed char yDir:2;
	struct {
		byte x[64];
		byte y[64];
	} body;
} snake;

static void generateApple()
{
	bool collision = true;
	while (collision) {
		apple.x = random(8);
		apple.y = random(8);
		collision = false;
		byte i;
		for (i = 0; i < snake.length; i++)
			if (apple.x == snake.body.x[i] && apple.y == snake.body.y[i]) {
				collision = true;
				break;
			}
	}

	apple.on = false;
}

static void gameOver(void);

static void updateSnake(byte data)
{
	matrix.setLed(0, snake.body.y[snake.length - 1], snake.body.x[snake.length - 1], false);

	byte i;
	for (i = snake.length - 1; i > 0; i--) {
		snake.body.x[i] = snake.body.x[i - 1];
		snake.body.y[i] = snake.body.y[i - 1];
	}

	snake.body.x[0] += snake.xDir;
	snake.body.y[0] += snake.yDir; 

	if (snake.body.x[0] == 8 || snake.body.y[0] == 8 || snake.body.x[0] == 0xFF || snake.body.y[0] == 0xFF) {
		gameOver();
		return;
	}

	for (i = 4; i < snake.length; i++) {
		if (snake.body.x[0] == snake.body.x[i] && snake.body.y[0] == snake.body.y[i]) {
			gameOver();
			return;
		}

	}

	matrix.setLed(0, snake.body.y[0], snake.body.x[0], true);

	if (snake.body.x[0] == apple.x && snake.body.y[0] == apple.y) {
		printLcd(10, 0, ++snake.length - 3);
		generateApple();
	}

	snake.hasTurned = false;

	updateSnakeTimer = registerTimerEvent(500, updateSnake, 0);
}

static void blinkApple(byte data)
{
	if (apple.on)
		apple.on = false;
	else
		apple.on = true;

	matrix.setLed(0, apple.y, apple.x, apple.on);

	blinkAppleTimer = registerTimerEvent(100, blinkApple, 0);
}

static void move(byte turnRight)
{
	if (!snake.hasTurned) {
		byte tmp;
		if (turnRight) {
			tmp = snake.xDir;
			snake.xDir = snake.yDir * -1;
			snake.yDir = tmp;
		} else {
			tmp = snake.yDir;
			snake.yDir = snake.xDir * -1;
			snake.xDir = tmp;
		}
		snake.hasTurned = true;
	}
}

static void endRound(void)
{
	cancelTimerEvent(&updateSnakeTimer);
	cancelTimerEvent(&blinkAppleTimer);
}

static void menu(byte data)
{
	endRound();
	showMenu();
}

static void start(void)
{
	setSingleClickHandler(PLAYER1_LEFT, move, LEFT);
	setSingleClickHandler(PLAYER1_RIGHT, move, RIGHT);
	setSingleClickHandler(PLAYER2_LEFT, move, LEFT);
	setSingleClickHandler(PLAYER2_RIGHT, move, RIGHT);
	setSingleClickHandler(MENU, menu, 0);

	byte i;
	for (i = 0; i < snake.length; i++)
		matrix.setLed(0, snake.body.y[i], snake.body.x[i], true);
	matrix.setLed(0, apple.y, apple.x, apple.on);

	printLcd(2, 0, "Score :");
	printLcd(10, 0, snake.length - 3);

	delay(500);

	updateSnakeTimer = registerTimerEvent(500, updateSnake, 0);
	blinkAppleTimer = registerTimerEvent(100, blinkApple, 0);
}

static void newRound(void)
{
	endRound();

	snake.xDir = 1;
	snake.yDir = 0;
	snake.length = 3;
	snake.hasTurned = false;

	snake.body.x[0] = 2;
	snake.body.y[0] = 3;
	snake.body.x[1] = 1;
	snake.body.y[1] = 3;
	snake.body.x[2] = 1;
	snake.body.y[2] = 4;

	generateApple();
	apple.on = false;

	countdown(start, menu);
}

static void gameOver(void)
{
	endRound();
	matrix.setLed(0, apple.y, apple.x, true); // In case the apple is off at that time
	printLcd(3, 1, "Game Over !");
	wipeMatrix();
	delay(500);
	newRound();
}

void showSnake(void)
{
	updateSnakeTimer = blinkAppleTimer = -1;

	newRound();
}
