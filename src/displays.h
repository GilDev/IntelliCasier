#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

#define printLcd(x, y, text) lcd.setCursor(x, y); lcd.print(text);

extern LedControl matrix;
extern LiquidCrystal_I2C lcd;

void displaysInit(void);
void clearDisplays(void);

void newScroll(char *text);
void stopScrolling(void);

#endif