#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "options.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"

static TimerId updateBallTimer  = -1, updateSpeedTimer = -1;

static struct {
	byte x:3;
	byte y:3;
	signed char xDir:2;
	signed char yDir:2;
	unsigned short speed; // In milliseconds
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

	if (ball.x == 1 || ball.x == 6) {
		byte player = (ball.x == 1) ? 0 : 1;

		if ((ball.y == players[player].y) || (ball.y == players[player].y + 1)) {
			ball.xDir *= -1;
		} else if (((ball.y == players[player].y - 1 && ball.yDir == 1) || (ball.y == players[player].y + 2 && ball.yDir == -1))) {
			ball.xDir *= -1;
			if (ball.y > 0 && ball.y < 7) // Avoid double y direction inversion
				ball.yDir *= -1;
		}
	} else if (ball.x == 0 || ball.x == 7) { // Ball out of bound
		printLcd((ball.x == 0) ? 13 : 2, 1, "+1");

		wipeMatrix();
		delay(500);

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

	updateBallTimer = registerTimerEvent(ball.speed, updateBall, 0);
}

static void updateSpeed(byte data)
{
	if (ball.speed > options[PONG_MIN_DELAY_O]) {
		ball.speed -= 50;
		updateSpeedTimer = registerTimerEvent(options[PONG_SPEED_INCREASES_DELAY_O], updateSpeed, 0);
	}
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
	cancelTimerEvent(&updateBallTimer);
	cancelTimerEvent(&updateSpeedTimer);
}

static void start(void)
{
	setRepeatClickHandler(PLAYER1_LEFT, options[PONG_PADDLE_DELAY_O], up1, 0);
	setRepeatClickHandler(PLAYER1_RIGHT, options[PONG_PADDLE_DELAY_O], down1, 0);
	setRepeatClickHandler(PLAYER2_LEFT, options[PONG_PADDLE_DELAY_O], up2, 0);
	setRepeatClickHandler(PLAYER2_RIGHT, options[PONG_PADDLE_DELAY_O], down2, 0);
	setSingleClickHandler(MENU, menu, 0);

	matrix.setRow(0, 3, B10000001);
	matrix.setRow(0, 4, B10000001);
	matrix.setLed(0, ball.y, ball.x, true);

	printLcd(2, 0, players[0].score);
	printLcd(7, 0, "--");
	printLcd(13, 0, players[1].score);

	delay(500);

	updateBallTimer = registerTimerEvent(ball.speed, updateBall, 0);
	updateSpeedTimer = registerTimerEvent(options[PONG_SPEED_INCREASES_DELAY_O], updateSpeed, 0);
}

static void newRound(byte playerStarting)
{
	endRound();

	ball.x = (playerStarting == 0) ? 2 : 5;
	ball.y = random(1, 7);
	ball.xDir = (playerStarting == 0) ? 1 : -1;
	ball.yDir = (random(0, 2)) ? 1 : -1;
	ball.speed = options[PONG_START_DELAY_O];

	players[0].y = players[1].y = 3;

	countdown(start, menu);
}

void showPong(void)
{
	players[0].score = players[1].score = 0;

	newRound(0);
}
