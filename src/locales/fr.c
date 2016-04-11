#include <Arduino.h>
#include "../config.h"

#if (LANG == 1)
const char *strings[] = {
	"Valider",
	"Jouer",
	"Retour",
	"1 joueur",
	"Jeux 1 joueur",
	"2 joueurs",
	"Jeux 2 joueurs",
	"A propos",
	"Course",
	"Snake",
	"Flappy Bird",
	"Pong",
	"A vos marques",
	"Pret",
	"Partez !",
	"Game Over !",
	"Joueur   gagne !",
	"Egalite !",
	"Appuyez sur un bouton !",
	"Fait par GilDev",
	"IntelliCasier",
	"Luminosite",
	"Luminosite economiseur ecran",
	"Vitesse defilemement matrice (ms)",
	"Pong : Vitesse raquette (ms)",
	"Pong : Vitesse de base (ms)",
	"Pong : Vitesse max (ms)",
	"Pong : Delai entre accelerations (ms)"
};

const byte stringsSizes[] = {
	7,
	5,
	6,
	8,
	13,
	9,
	14,
	8,
	6,
	5,
	11,
	4,
	13,
	4,
	8,
	11,
	16,
	9,
	23,
	15,
	13,
	10,
	28,
	33,
	28,
	27,
	23,
	37
};

#endif
