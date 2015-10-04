#include <Arduino.h>
#include "config.h"
#include "events.h"
#include "screensaver.h"

#define NUMBER_OF_TIMER_EVENTS 10

// BUTTONS
enum { // Click types
	SINGLE,
	HOLD,
	REPEAT
};

static struct {
	byte type:2;                  // Click type
	byte oldState:1;              // Used with SINGLE and HOLD click types to know when an input change occurs
	byte clicked:1;               // True when HOLD trigger has occured
	unsigned short delay;         // Used with HOLD and REPEAT click types
	unsigned long lastActivation; // Used with SINGLE click type to avoid bouncing inputs
	void (*callback)();           // Callback to call when input triggers
} buttons[5];

// TIMER EVENTS
static struct {
	byte activated;
	unsigned int delay;
	unsigned long activationTime;
	void (*callback)();
} timers[NUMBER_OF_TIMER_EVENTS];

static byte i; // Change type if NUMBER_OF_TIMER_EVENTS > 256

unsigned long screensaverTimer = millis();

void eventsInit(void)
{
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++)
		timers[i].activated = false;
}

void eventsUpdateLoop(void)
{
	unsigned long actualTime;

	// INPUTS
	for (i = 0; i < 5; i++) {
		actualTime = millis();

		bool state = digitalRead(buttonsPins[i]);

		// SCREENSAVER
		if (state == !BUTTON_OPEN) // When an action is made, reset screensaver launch delay
			screensaverTimer = actualTime;
		else if (!displayingScreensaver && actualTime - screensaverTimer > screensaverDelay) {
			showScreensaver();
		}

		// Single click
		if (buttons[i].type == SINGLE) {
			if (state != buttons[i].oldState) {
				if (actualTime - buttons[i].lastActivation > DEBOUNCE_TIME) {
					buttons[i].lastActivation = actualTime;
					if (state == !BUTTON_OPEN) {
						#ifdef SERIAL_DEBUG
							Serial.print("Single click: ");
							Serial.println(i);
						#endif
						(*buttons[i].callback)();
					}
				}
			}
			buttons[i].oldState = state;
		// Hold click
		} else if (buttons[i].type == HOLD) {
			if (state == !BUTTON_OPEN) {
				if (state != buttons[i].oldState) {
					buttons[i].lastActivation = actualTime;
				} else {
					if (!buttons[i].clicked && actualTime - buttons[i].lastActivation > buttons[i].delay) {
						#ifdef SERIAL_DEBUG
							Serial.print("Hold click: ");
							Serial.println(i);
						#endif
						buttons[i].clicked = true;
						(*buttons[i].callback)();
					}
				}
			} else {
				buttons[i].clicked = false;
			}
			buttons[i].oldState = state;
		// Repeat click
		} else {
			if (state == !BUTTON_OPEN) {
				if (actualTime - buttons[i].lastActivation > buttons[i].delay) {
					#ifdef SERIAL_DEBUG
						Serial.print("Repeat click: ");
						Serial.println(i);
					#endif
					buttons[i].lastActivation = actualTime;
					(*buttons[i].callback)();
				}
			}
		}
	}

	// TIMERS
	actualTime = millis();
	// This loop could be optimized with a queue and a counter
	// of active timers instead of testing all timers
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++) { 
		if (timers[i].activated && actualTime >= timers[i].activationTime) {
			timers[i].activationTime = actualTime + timers[i].delay;
			(*timers[i].callback)();
			#ifdef SERIAL_DEBUG
				Serial.print("Timer: ");
				Serial.println(i);
			#endif
		}
	}
}

void setSingleClickHandler(ButtonId button, void (*callback)(void))
{
	buttons[button].type = SINGLE;
	buttons[button].oldState = BUTTON_OPEN;
	buttons[button].lastActivation = 0;
	buttons[button].callback = callback;
}

void setHoldClickHandler(ButtonId button, unsigned short delay, void (*callback)(void))
{
	buttons[button].type = HOLD;
	buttons[button].oldState = BUTTON_OPEN;
	buttons[button].delay = delay;
	buttons[button].clicked = false;
	buttons[button].callback = callback;
}

void setRepeatClickHandler(ButtonId button, unsigned short delay, void (*callback)(void))
{
	buttons[button].type = REPEAT;
	buttons[button].delay = delay;
	buttons[button].lastActivation = 0;
	buttons[button].callback = callback;
}


TimerId registerTimerEvent(unsigned short delay, void (*callback)(void))
{
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++)
		if (!timers[i].activated) {
			timers[i].activated = true;
			timers[i].delay = delay;
			timers[i].activationTime = millis() + delay;
			timers[i].callback = callback;
			return i;
		}
}

void rescheduleTimerEvent(TimerId id, unsigned short newDelay)
{
	timers[id].activationTime = newDelay;
}

void cancelTimerEvent(TimerId id)
{
	timers[id].activated = false;
}
