#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include <Sodaq_DS3231.h> // Initialize the `rtc` variable, please don't use it. Also define DateTime type.

DateTime getTime();
void timeInit();

#endif
