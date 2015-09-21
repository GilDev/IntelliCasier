#include <Arduino.h>
#include "time.h"
#include "global.h"
#include "events.h"

static void checkTime()
{
	DateTime time = rtc.now();

	if (time.hour() >= 20);
}

DateTime getTime()
{
	return rtc.now();
}

void timeInit()
{

}
