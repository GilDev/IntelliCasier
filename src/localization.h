#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "config.h"

extern const char *strings[];
extern const byte stringsSizes[]; // stringsSizes[i] == strlen(strings[i]) 

enum Strings {
	CONFIRM,
	PLAY,
	BACK,
	GAMES_1_PLAYER_MATRIX,
	GAMES_1_PLAYER_LCD,
	GAMES_2_PLAYERS_MATRIX,
	GAMES_2_PLAYERS_LCD,
	ABOUT,
	RACE,
	SNAKE,
	PONG,
	TRON,
	READY,
	SET,
	GO,
	PRESS_BUTTON,
	BY_GILDEV,
	ARCADE_LOCKER,
	OPTION_1,
	OPTION_2,
	OPTION_3,
	OPTION_4
};

#endif
