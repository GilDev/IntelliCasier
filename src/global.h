#ifndef GLOBAL_H
#define GLOBAL_H

#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

extern LedControl matrix;
extern LiquidCrystal_I2C lcd;
extern bool displayingScreensaver;
extern unsigned short screensaverDelay;

void clearDisplays(void);

#endif
