#include <Arduino.h>
#include <Sodaq_DS3231.h>
#include "events.h"
#include "time.h"

static void checkTime()
{
	DateTime time = rtc.now();

	if (time.hour() >= 20);
}

DateTime getTime()
{
	return rtc.now();
}

void timeInit(void)
{
	rtc.begin();
}
