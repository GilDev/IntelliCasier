#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
  PLAYER1_LEFT,
  PLAYER1_RIGHT,
  PLAYER2_LEFT,
  PLAYER2_RIGHT,
  MENU
} ButtonId;

typedef byte TimerId;

void eventsInit(void);
void eventsUpdateLoop(void);

void setSingleClickHandler(ButtonId id, void (*callback)(void));
void setHoldClickHandler(ButtonId button, unsigned short delay, void (*callback)(void));
void setRepeatClickHandler(ButtonId button, unsigned short delay, void (*callback)(void));

TimerId registerTimerEvent(unsigned short delay, void (*callback)(void));
void rescheduleTimerEvent(TimerId id, unsigned short newDelay);
void cancelTimerEvent(TimerId id);

#endif
