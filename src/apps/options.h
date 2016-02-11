#ifndef OPTIONS_H
#define OPTIONS_H

enum Options {
	BRIGHTNESS,             // 0-15
	SCREENSAVER_BRIGHTNESS, // 0-15
	MATRIX_SCROLLING_SPEED, // In milliseconds
	PONG_PADDLE_SPEED,      // In milliseconds
	PONG_START_DELAY,       // In milliseconds
	PONG_MIN_DELAY,         // In milliseconds
	SPEED_INCREASE_DELAY    // In milliseconds
};

extern unsigned short options[];

void showOptions(void); 

#endif
