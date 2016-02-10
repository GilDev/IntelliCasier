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
	"Pong",
	"Tron",
	"Ready?",
	"Set",
	"Go!",
	"Press a button!",
	"Made by GilDev",
	"Arcade Locker"
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
	4,
	4,
	6,
	3,
	3,
	15,
	14,
	13
};

#endif
