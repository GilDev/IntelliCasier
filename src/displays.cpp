#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "displays.h"
#include "events.h"

// MATRIX
#define actualLetterId ((scrollingText[c] >= 'a') ? scrollingText[c] - 'a' + 26 : (scrollingText[c] <= '9') ? scrollingText[c] - '0' + 26 * 2 : scrollingText[c] - 'A')

LedControl matrix = LedControl(MAX_CHIP_DIN_PIN, MAX_CHIP_CLK_PIN, MAX_CHIP_LOAD_PIN, 1);

static char *scrollingText;
static byte c;     // Character pointer
static byte x;     // Character column pointer
static byte printingEmptySpace = false; // True when printing an empty space
static byte i;

static byte frameBuffer[8];
static TimerId scrollingTimerId;
static byte timerActivated = false;

static const PROGMEM byte font[][9] = { // [A-Z][a-z][0-9]. Ninth value specify character width
// A
{
B010,
B101,
B101,
B111,
B101,
B101,
B000,
B000,
3
},
// B
{
B110,
B101,
B110,
B101,
B101,
B110,
B000,
B000,
3
},
// C
{
B011,
B100,
B100,
B100,
B100,
B011,
B000,
B000,
3
},
// D
{
B110,
B101,
B101,
B101,
B101,
B110,
B000,
B000,
3
},
// E
{
B111,
B100,
B110,
B100,
B100,
B111,
B000,
B000,
3
},
// F
{
B111,
B100,
B110,
B100,
B100,
B100,
B000,
B000,
3
},
// G
{
B0110,
B1000,
B1000,
B1011,
B1001,
B0110,
B0000,
B0000,
4
},
// H
{
B101,
B101,
B111,
B101,
B101,
B101,
B000,
B000,
3
},
// I
{
B111,
B010,
B010,
B010,
B010,
B111,
B000,
B000,
3
},
// J
{
B0111,
B0010,
B0010,
B0010,
B1010,
B0100,
B0000,
B0000,
4
},
// K
{
B101,
B101,
B110,
B101,
B101,
B101,
B000,
B000,
3
},
// L
{
B100,
B100,
B100,
B100,
B100,
B111,
B000,
B000,
3
},
// M
{
B10001,
B11011,
B10101,
B10001,
B10001,
B10001,
B00000,
B00000,
5
},
// N
{
B10001,
B11001,
B10101,
B10011,
B10001,
B10001,
B00000,
B00000,
5
},
// O
{
B0110,
B1001,
B1001,
B1001,
B1001,
B0110,
B0000,
B0000,
4
},
// P
{
B110,
B101,
B110,
B100,
B100,
B100,
B000,
B000,
3
},
// Q
{
B01100,
B10010,
B10010,
B10010,
B10110,
B01110,
B00001,
B00000,
5
},
// R
{
B110,
B101,
B110,
B101,
B101,
B101,
B000,
B000,
3
},
// S
{
B011,
B100,
B010,
B001,
B001,
B110,
B000,
B000,
3
},
// T
{
B111,
B010,
B010,
B010,
B010,
B010,
B000,
B000,
3
},
// U
{
B101,
B101,
B101,
B101,
B101,
B111,
B000,
B000,
3
},
// V
{
B10001,
B10001,
B10001,
B01010,
B01010,
B00100,
B00000,
B00000,
5
},
// W
{
B10001,
B10001,
B10001,
B10001,
B10101,
B01010,
B00000,
B00000,
5
},
// X
{
B101,
B101,
B010,
B101,
B101,
B101,
B000,
B000,
3
},
// Y
{
B101,
B101,
B010,
B010,
B010,
B010,
B000,
B000,
3
},
// Z
{
B111,
B001,
B010,
B010,
B100,
B111,
B000,
B000,
3
},
// a
{
B0000,
B0110,
B0001,
B0111,
B1001,
B0111,
B000,
B000,
4
},
// b
{
B000,
B100,
B100,
B110,
B101,
B110,
B000,
B000,
3
},
// c
{
B000,
B000,
B011,
B100,
B100,
B011,
B000,
B000,
3
},
// d
{
B000,
B001,
B001,
B011,
B101,
B011,
B000,
B000,
3
},
// e
{
B0000,
B0110,
B1001,
B1110,
B1000,
B0110,
B0000,
B0000,
4
},
// f
{
B000,
B011,
B100,
B110,
B100,
B100,
B000,
B000,
3
},
// g
{
B0000,
B0000,
B0110,
B1001,
B1001,
B0111,
B0001,
B0110,
4
},
// h
{
B000,
B100,
B100,
B110,
B101,
B101,
B000,
B000,
3
},
// i
{
B0,
B1,
B0,
B1,
B1,
B1,
B0,
B0,
1
},
// j
{
B000,
B001,
B000,
B001,
B001,
B001,
B101,
B010,
3
},
// k
{
B000,
B100,
B100,
B110,
B111,
B101,
B000,
B000,
3
},
// l
{
B0,
B1,
B1,
B1,
B1,
B1,
B0,
B0,
1
},
// m
{
B00000,
B00000,
B11010,
B10101,
B10101,
B10101,
B00000,
B00000,
5
},
// n
{
B000,
B000,
B110,
B101,
B101,
B101,
B000,
B000,
3
},
// o
{
B0000,
B0000,
B0110,
B1001,
B1001,
B0110,
B0000,
B0000,
4
},
// p
{
B0000,
B0000,
B0110,
B1001,
B1001,
B1110,
B1000,
B1000,
4
},
// q
{
B0000,
B0000,
B0110,
B1001,
B1001,
B0111,
B0001,
B0001,
4
},
// r
{
B000,
B000,
B011,
B100,
B100,
B100,
B000,
B000,
3
},
// s
{
B000,
B011,
B100,
B010,
B001,
B110,
B000,
B000,
3
},
// t
{
B000,
B010,
B111,
B010,
B010,
B011,
B000,
B000,
3
},
// u
{
B000,
B000,
B101,
B101,
B101,
B011,
B000,
B000,
3
},
// v
{
B000,
B000,
B101,
B101,
B101,
B010,
B000,
B000,
3
},
// w
{
B00000,
B00000,
B10001,
B10001,
B10101,
B01010,
B00000,
B00000,
5
},
// x
{
B000,
B000,
B101,
B010,
B101,
B101,
B000,
B000,
3
},
// y
{
B000,
B000,
B101,
B101,
B101,
B011,
B001,
B010,
3
},
// z
{
B00,
B00,
B11,
B01,
B10,
B11,
B00,
B00,
2
},
// 0
{
B0000,
B0110,
B1001,
B1001,
B1001,
B0110,
B0000,
B0000,
4
},
// 1
{
B000,
B010,
B110,
B010,
B010,
B111,
B000,
B000,
3
},
// 2
{
B000,
B110,
B001,
B010,
B100,
B111,
B000,
B000,
3
},
// 3
{
B000,
B110,
B001,
B110,
B001,
B110,
B000,
B000,
3
},
// 4
{
B000,
B101,
B101,
B111,
B001,
B001,
B000,
B000,
3
},
// 5
{
B000,
B111,
B100,
B010,
B001,
B110,
B000,
B000,
3
},
// 6
{
B0000,
B0110,
B1000,
B1111,
B1001,
B0110,
B0000,
B0000,
4
},
// 7
{
B0000,
B1110,
B0010,
B0111,
B0010,
B0010,
B0000,
B0000,
4
},
// 8
{
B0000,
B0110,
B1001,
B0110,
B1001,
B0110,
B0000,
B0000,
4
},
// 9
{
B0000,
B0110,
B1001,
B0111,
B0001,
B0110,
B0000,
B0000,
4
}};

static void scroll(void)
{
	if (!printingEmptySpace && (x >= pgm_read_byte(&font[actualLetterId][8]) || scrollingText[c] == ' ' || scrollingText[c] == '\0')) {
		switch (scrollingText[++c]) {
			case ' ':
				x = EMPTY_SPACE_SIZE;
				c++;
				break;
			case '\0':
				x = EMPTY_SPACE_ROLL_SIZE;
				c = 0;
				break;
			default:
				x = 1;
		}

		printingEmptySpace = true;
	}

	if (printingEmptySpace) {
		for (i = 0; i < 8; i++) {
			frameBuffer[i] <<= 1;
			matrix.setRow(0, i, frameBuffer[i]);
		}

		if (--x == 0)
			printingEmptySpace = false;
	} else {
		for (i = 0; i < 8; i++) {
			frameBuffer[i] = (frameBuffer[i] << 1) | (pgm_read_byte(&font[actualLetterId][i]) >> pgm_read_byte(&font[actualLetterId][8]) - 1 - x) & 1;
			matrix.setRow(0, i, frameBuffer[i]);
		}
		x++;
	}
}

void newScroll(char *text)
{
	stopScrolling();

	matrix.clearDisplay(0);
	scrollingText = text;
	for (i = 0; i < 8; i++)
		frameBuffer[i] = 0;
	c = x = printingEmptySpace = 0;
	scrollingTimerId = registerTimerEvent(TEXT_SCROLLING_SPEED, scroll);
	timerActivated = true;
}

void stopScrolling(void)
{
	if (timerActivated) {
		cancelTimerEvent(scrollingTimerId);
		timerActivated = false;
	}
}

void drawImage(byte *img)
{
	stopScrolling();

	byte i;
	for (i = 0; i < 8; i++)
		matrix.setRow(0, i, img[i]);
}

static void matrixInit(void)
{
	matrix.shutdown(0, false);
	matrix.setIntensity(0, DEFAULT_MATRIX_INTENSITY);
}

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

static byte downArrow[] = {
	B00100,
	B00100,
	B00100,
	B00100,
	B00100,
	B11111,
	B01110,
	B00100
};

static void lcdInit(void)
{
	Wire.begin();
	lcd.createChar(0, downArrow);
	lcd.begin(16, 2);
}

// COMMON
void displaysInit(void)
{
	matrixInit();
	lcdInit();
}

void clearDisplays(void)
{
	// LED Matrix
	matrix.clearDisplay(0);
	stopScrolling();

	// LCD display
	lcd.backlight();
	lcd.noCursor();
	lcd.noBlink();
	lcd.noAutoscroll();
	lcd.clear();
}

void clearLcdLine(byte line)
{
	printLcd(0, line, F("                "));
}
