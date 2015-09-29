#include <Arduino.h>
#include "global.h"
#include "scrolling.h"

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
