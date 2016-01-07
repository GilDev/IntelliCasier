#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"

static TimerId updateBallTimer;

static struct {
	byte x:3;
	byte y:3;
	signed char xDir:2;
	signed char yDir:2;
} ball;

static void updateBall(byte data)
{
	matrix.setLed(0, ball.y, ball.x, false);

	ball.x += ball.xDir;
	ball.y += ball.yDir;

	if (ball.x <= 1 || ball.x >= 6)
		ball.xDir *= -1;
	if (ball.y <= 0 || ball.y >= 7)
		ball.yDir *= -1;

	matrix.setLed(0, ball.y, ball.x, true);
}

static void printScore(void)
{

}

static byte player1Y, player2Y;

static void up1(byte data)
{
	if (player1Y > 0) {
		matrix.setLed(0, player1Y + 1, 0, false);
		matrix.setLed(0, --player1Y, 0, true);
	}
}

static void down1(byte data)
{
	if (player1Y < 6) {
		matrix.setLed(0, player1Y, 0, false);
		matrix.setLed(0, ++player1Y + 1, 0, true);
	}
}

static void up2(byte data)
{
	if (player2Y > 0) {
		matrix.setLed(0, player2Y + 1, 7, false);
		matrix.setLed(0, --player2Y, 7, true);
	}
}

static void down2(byte data)
{
	if (player2Y < 6) {
		matrix.setLed(0, player2Y, 7, false);
		matrix.setLed(0, ++player2Y + 1, 7, true);
	}
}

static void menu(byte data)
{
	cancelTimerEvent(updateBallTimer);
	showMenu();
}

static void initGame(void)
{
	ball.x = 2;
	ball.y = random(1, 7);
	ball.xDir = ball.yDir = 1;
	player1Y = player2Y = 3;

	countdown();

	clearDisplays();

	updateBallTimer = registerTimerEvent(500, updateBall, 0);
}

void showPong(void)
{
	setRepeatClickHandler(PLAYER1_LEFT, DELAY_BETWEEN_PADDLE_MOVES, up1, 0);
	setRepeatClickHandler(PLAYER1_RIGHT, DELAY_BETWEEN_PADDLE_MOVES, down1, 0);
	setRepeatClickHandler(PLAYER2_LEFT, DELAY_BETWEEN_PADDLE_MOVES, up2, 0);
	setRepeatClickHandler(PLAYER2_RIGHT, DELAY_BETWEEN_PADDLE_MOVES, down2, 0);
	setSingleClickHandler(MENU, menu, 0);

	initGame();
}
