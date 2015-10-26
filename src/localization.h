#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "config.h"

#if (LANG == 0)
	#include "locales/en.h"
#elif (LANG == 1)
	#include "locales/fr.h"
#endif

enum Strings {
	CONFIRM,
	PLAY,
	BACK,
	GAMES_1_PLAYER,
	GAMES_2_PLAYERS,
	ABOUT,
	RACE,
	SNAKE,
	PONG,
	TRON
};

#endif
