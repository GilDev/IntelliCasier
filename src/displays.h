#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

extern LedControl matrix;
extern LiquidCrystal_I2C lcd;

/**
 * LED matrix & LCD display initialization
 */
void displaysInit(void);

/**
 * Clear LED matrix & LCD display
 */
void clearDisplays(void);

/**
 * Scroll a text on the LED matrix
 * Supported characters: [A-Za-z0-9]
 * @param text Text to scroll
 */
void newMatrixScroll(const char *text);

/**
 * Stop scrolling text on the LED matrix
 */
void stopMatrixScroll(void);

/**
 * Print an image on the LED matrix
 * @param img Array of chars representing an image to print
 */
void drawImage(byte *img);

/**
 * Print text on the LCD display
 * @param x    Horizontal coordinate
 * @param y    Vertical coordinate
 * @param text Text to print
 */
#define printLcd(x, y, text) {lcd.setCursor(x, y); lcd.print(text);}

 /**
  * Clear text on an LCD display's line
  * @param line Line to erase
  */
void clearLcdLine(byte line);

/**
 * Scroll a text on an LCD display's line
 * @param text Text to scroll
 * @param line Line to scroll the text on
 * @param speed Time between individual scrolls in milliseconds
 */
void newLcdScroll(const char *text, byte line, unsigned short speed);

/**
 * Stop scrolling text on an LCD display's line
 * @param line Line to stop scrolling on
 */
void stopLcdScroll(byte line);

#endif
