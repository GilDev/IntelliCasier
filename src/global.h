#ifndef GLOBAL_H
#define GLOBAL_H

#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

extern LedControl matrix;

extern LiquidCrystal_I2C lcd;
#define printLcd(x, y, text) lcd.setCursor(x, y); lcd.print(text);

extern bool displayingScreensaver;
extern unsigned short screensaverDelay;

void clearDisplays(void);

#endif
