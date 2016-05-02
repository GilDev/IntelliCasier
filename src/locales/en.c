#include <Arduino.h>
#include "../config.h"

#if (LANG == 0)
const char *strings[] = {
	"Confirm",
	"Play",
	"Back",
	"1 player",
	"1 player games",
	"2 players",
	"2 player games",
	"About",
	"Race",
	"Snake",
	"Flappy Bird",
	"Pong",
	"Ready?",
	"Set",
	"Go!",
	"Game Over!",
	"Player   win!",
	"Tie!";
	"Press a button!",
	"Made by GilDev",
	"SmartLocker",
	"Brightness",
	"Screensaver brightness",
	"Matrix scrolling speed (ms)",
	"Pong: Paddle speed (ms)",
	"Pong: Starting speed (ms)",
	"Pong: Max speed (ms)",
	"Pong: Time between speed change (ms)"
};

#define BUFFER_SIZE 37 // Max string size + 1

const byte stringsSizes[] = {
	7,
	4,
	4,
	8,
	14,
	9,
	14,
	5,
	4,
	5,
	11,
	4,
	6,
	3,
	3,
	10,
	13,
	4,
	15,
	14,
	11,
	10,
	22,
	27,
	23,
	25,
	20,
	36
};

#endif
