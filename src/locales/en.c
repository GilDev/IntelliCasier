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
	"Tron",
	"Ready?",
	"Set",
	"Go!",
	"Press a button!",
	"Made by GilDev",
	"Arcade Locker",
	"Brightness",
	"Screensaver brightness",
	"Matrix scrolling speed (ms)",
	"Pong: Paddle speed (ms)",
	"Pong: Starting speed (ms)",
	"Pong: Max speed (ms)",
	"Pong: Time between speed change (ms)"
};

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
	4,
	6,
	3,
	3,
	15,
	14,
	13,
	10,
	22,
	27,
	23,
	25,
	20,
	36
};

#endif
