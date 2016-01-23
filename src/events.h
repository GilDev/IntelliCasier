#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
  PLAYER1_LEFT,
  PLAYER1_RIGHT,
  PLAYER2_LEFT,
  PLAYER2_RIGHT,
  MENU
} ButtonId;

typedef signed char TimerId;

void eventsInit(void);
void eventsUpdateLoop(void);

void setSingleClickHandler(ButtonId button, void (*callback)(byte data), byte data);
void setHoldClickHandler(ButtonId button, unsigned short delay, void (*callback)(byte data), byte data);
void setRepeatClickHandler(ButtonId button, unsigned short delay, void (*callback)(byte data), byte data);

TimerId registerTimerEvent(unsigned short delay, void (*callback)(byte data), bool repeat, byte data);
void rescheduleTimerEvent(TimerId id, unsigned short newDelay);
void cancelTimerEvent(TimerId id);
void cancelAllTimerEvents(void);

#endif
