#include <Arduino.h>
#include "menu.h"
#include "common.h"
#include "options.h"
#include "../alarm.h"
#include "../config.h"
#include "../displays.h"
#include "../events.h"
#include "../localization.h"
#include "../screensaver.h"


static char *lessonName[] = {"Histoire-Geo", "Anglais", "Physique-Chimie", "SI", "ISN", "Allemand", "Mathematiques", "Philosophie", "EPS"};
static char *roomNames[] = {"E205", "E218", "E187", "D143", "E215", "E188", "E206", "E303", "P14", "C158", "TP", "E86", "P21", "E305"};
struct hour {
	byte lesson:4;
	byte room:4;
	byte hours:2;
};
static struct hour schedule[6][10] = {
	{
		// Monday
		{0, 0, 0},
		{15, 15, 0},
		{1, 1, 0},
		{2, 2, 0},
		{15, 15, 0},
		{3, 3, 3},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Tuesday
		{3, 3, 3},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{4, 3, 1},
		{15, 15, 0},
		{5, 4, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Wednesday
		{3, 3, 0},
		{2, 5, 1},
		{15, 15, 0},
		{6, 6, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Thursday
		{0, 7, 0},
		{6, 8, 0},
		{7, 9, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{2, 10, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Friday
		{1, 1, 0},
		{5, 4, 0},
		{8, 15, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{7, 11, 0},
		{2, 12, 0},
		{0, 13, 0},
		{2, 12, 0}
	},
	{
		// Saturday
		{6, 8, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	}
};

static TimerId blinkSelectedLessonTimer;
static byte selectedDay, selectedLesson;

static void drawSchedule(void)
{
	byte day, lesson, hour;
	for (day = 0; day < 6; day++) {
		for (lesson = 0; lesson < 8; lesson++) {
			if (schedule[day][lesson].lesson != 15)
				for (hour = 0; hour <= schedule[day][lesson].hours; hour++)
					matrix.setLed(0, lesson + hour, day + 1, true);
		}
	}

	clearLcdLine(0);
	clearLcdLine(1);
	printLcd(0, 0, lessonName[schedule[selectedDay][selectedLesson].lesson]);
	printLcd(0, 1, roomNames[schedule[selectedDay][selectedLesson].room]);
	printLcd(9, 1, selectedLesson + 8);
	lcd.print("h-");
	lcd.print(selectedLesson + 8 + schedule[selectedDay][selectedLesson].hours + 1);
	lcd.print("h");
}

static void blinkSelectedLesson(byte on)
{
	byte hour;
	for (hour = 0; hour <= schedule[selectedDay][selectedLesson].hours; hour++)
		matrix.setLed(0, selectedLesson + hour, selectedDay + 1, on);

	blinkSelectedLessonTimer = registerTimerEvent(100, blinkSelectedLesson, !on);
}

static void nextDay(byte data)
{
	if (selectedDay++ == 5)
		selectedDay = 0;
	selectedLesson = 0;

	drawSchedule();
}

static void previousDay(byte data)
{
	if (selectedDay-- == 0)
		selectedDay = 5;
	selectedLesson = 0;

	drawSchedule();
}

static void nextLesson(byte data)
{
	if (selectedLesson == 9)
		selectedLesson = 0;
	else {
		while (schedule[selectedDay][++selectedLesson].lesson == 15)
			if (selectedLesson == 9) {
				selectedLesson = 0;
				break;
			}
	}

	drawSchedule();
}

static void previousLesson(byte data)
{
	if (selectedLesson == 0)
		selectedLesson = 10;

	while (schedule[selectedDay][--selectedLesson].lesson == 15)
		;

	drawSchedule();
}

static void menu(byte data)
{
	cancelTimerEvent(&blinkSelectedLessonTimer);
	alarmOn();
	showMenu();
}

void showSchedule(void)
{
	clearDisplays();

	screensaverDelay = -1;

	alarmOff();

	selectedDay = selectedLesson = 0;

	drawSchedule();
	blinkSelectedLesson(true);

	setSingleClickHandler(PLAYER1_LEFT, previousDay, 0);
	setSingleClickHandler(PLAYER1_RIGHT, nextDay, 0);
	setSingleClickHandler(PLAYER2_LEFT, previousLesson, 0);
	setSingleClickHandler(PLAYER2_RIGHT, nextLesson, 0);
	setSingleClickHandler(MENU, menu, 0);
}
