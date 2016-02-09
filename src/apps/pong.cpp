#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"

static TimerId updateBallTimer, updateSpeedTimer;

static struct {
	byte x:3;
	byte y:3;
	signed char xDir:2;
	signed char yDir:2;
	byte speed; // In milliseconds / 10
} ball;

static struct {
	byte y:3;
	byte score;
} players[2];

static void menu(byte data);
static void newRound(byte playerStarting);
static void endRound(void);

static void updateBall(byte data)
{
	matrix.setLed(0, ball.y, ball.x, false);

	if (ball.y <= 0 || ball.y >= 7)
		ball.yDir *= -1;

	if (ball.x == 1 or ball.x == 6) {
		byte player = (ball.x == 1) ? 0 : 1;

		if ((ball.y == players[player].y) or (ball.y == players[player].y + 1)) {
			ball.xDir *= -1;
		} else if ((ball.y == players[player].y - 1 and ball.yDir == 1) or (ball.y == players[player].y + 2 and ball.yDir == -1)) {
			ball.xDir *= -1;
			ball.yDir *= -1;
		}
	} else if (ball.x == 0 or ball.x == 7) { // Ball out of bound
		printLcd((ball.x == 0) ? 13 : 2, 1, "+1");

		delay(2000);

		if (ball.x == 0) {
			players[1].score++;
			newRound(0);
		} else {
			players[0].score++;
			newRound(1);
		}

		return;
	}

	ball.x += ball.xDir;
	ball.y += ball.yDir;

	matrix.setLed(0, ball.y, ball.x, true);

	updateBallTimer = registerTimerEvent(ball.speed * 10, updateBall, 0);
}

static void updateSpeed(byte data)
{
	if (ball.speed > MAXIMUM_SPEED) {
		ball.speed -= 5;
		updateSpeedTimer = registerTimerEvent(DELAY_BETWEEN_SPEED_INCREASE * 1000, updateSpeed, 0);
	}
}

static void printScore(void)
{

}

static void up1(byte data)
{
	if (players[0].y > 0) {
		matrix.setLed(0, players[0].y + 1, 0, false);
		matrix.setLed(0, --players[0].y, 0, true);
	}
}

static void down1(byte data)
{
	if (players[0].y < 6) {
		matrix.setLed(0, players[0].y, 0, false);
		matrix.setLed(0, ++players[0].y + 1, 0, true);
	}
}

static void up2(byte data)
{
	if (players[1].y > 0) {
		matrix.setLed(0, players[1].y + 1, 7, false);
		matrix.setLed(0, --players[1].y, 7, true);
	}
}

static void down2(byte data)
{
	if (players[1].y < 6) {
		matrix.setLed(0, players[1].y, 7, false);
		matrix.setLed(0, ++players[1].y + 1, 7, true);
	}
}

static void menu(byte data)
{
	endRound();
	showMenu();
}

static void endRound(void)
{
	cancelTimerEvent(updateBallTimer);
	cancelTimerEvent(updateSpeedTimer);
	updateBallTimer = updateSpeedTimer = -1;
}

static void newRound(byte playerStarting)
{
	endRound();

	ball.x = (playerStarting == 0) ? 2 : 5;
	ball.y = random(1, 7);
	ball.xDir = (playerStarting == 0) ? 1 : -1;
	ball.yDir = (random(0, 2)) ? 1 : -1;
	ball.speed = STARTING_SPEED / 10;

	players[0].y = players[1].y = 3;

	countdown();

	matrix.setRow(0, 3, B10000001);
	matrix.setRow(0, 4, B10000001);
	matrix.setLed(0, ball.y, ball.x, true);

	printLcd(2, 0, players[0].score);
	printLcd(7, 0, "--");
	printLcd(13, 0, players[1].score);

	delay(500);

	updateBallTimer = registerTimerEvent(ball.speed * 10, updateBall, 0);
	updateSpeedTimer = registerTimerEvent(DELAY_BETWEEN_SPEED_INCREASE * 1000, updateSpeed, 0);
}

void showPong(void)
{
	setRepeatClickHandler(PLAYER1_LEFT, DELAY_BETWEEN_PADDLE_MOVES, up1, 0);
	setRepeatClickHandler(PLAYER1_RIGHT, DELAY_BETWEEN_PADDLE_MOVES, down1, 0);
	setRepeatClickHandler(PLAYER2_LEFT, DELAY_BETWEEN_PADDLE_MOVES, up2, 0);
	setRepeatClickHandler(PLAYER2_RIGHT, DELAY_BETWEEN_PADDLE_MOVES, down2, 0);
	setSingleClickHandler(MENU, menu, 0);

	updateBallTimer = updateSpeedTimer = -1;

	players[0].score = players[1].score = 0;

	newRound(0);
}
