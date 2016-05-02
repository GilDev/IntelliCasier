#include <Arduino.h>
#include "../config.h"

#if (LANG == 1)
const char str0[]  PROGMEM  = "Valider";
const char str1[]  PROGMEM  = "Jouer";
const char str2[]  PROGMEM  = "Retour";
const char str3[]  PROGMEM  = "1 joueur";
const char str4[]  PROGMEM  = "Jeux 1 joueur";
const char str5[]  PROGMEM  = "2 joueurs";
const char str6[]  PROGMEM  = "Jeux 2 joueurs";
const char str7[]  PROGMEM  = "A propos";
const char str8[]  PROGMEM  = "Course";
const char str9[]  PROGMEM  = "Snake";
const char str10[] PROGMEM  = "Flappy Bird";
const char str11[] PROGMEM  = "Pong";
const char str12[] PROGMEM  = "A vos marques";
const char str13[] PROGMEM  = "Pret";
const char str14[] PROGMEM  = "Partez !";
const char str15[] PROGMEM  = "Game Over !";
const char str16[] PROGMEM  = "Joueur   gagne !";
const char str17[] PROGMEM  = "Egalite !";
const char str18[] PROGMEM  = "Appuyez sur un bouton !";
const char str19[] PROGMEM  = "Fait par GilDev";
const char str20[] PROGMEM  = "IntelliCasier";
const char str21[] PROGMEM  = "Luminosite";
const char str22[] PROGMEM  = "Luminosite economiseur ecran";
const char str23[] PROGMEM  = "Vitesse defilemement matrice (ms)";
const char str24[] PROGMEM  = "Pong : Vitesse raquette (ms)";
const char str25[] PROGMEM  = "Pong : Vitesse de base (ms)";
const char str26[] PROGMEM  = "Pong : Vitesse max (ms)";
const char str27[] PROGMEM  = "Pong : Delai entre accelerations (ms)";

const char * const strings[] PROGMEM = {
	str0,
	str1,
	str2,
	str3,
	str4,
	str5,
	str6,
	str7,
	str8,
	str9,
	str10,
	str11,
	str12,
	str13,
	str14,
	str15,
	str16,
	str17,
	str18,
	str19,
	str20,
	str21,
	str22,
	str23,
	str24,
	str25,
	str26,
	str27,
};

char buffer[38]; // Max string size + 1
char buffer2[38];

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
