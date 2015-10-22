#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG
#define SERIAL_BAUD 9600

#define RTC // Only define if you use a DS3231 RTC

#define DEFAULT_MATRIX_INTENSITY 5 // Default LEDs intensity between 0-15
#define SCREENSAVER_MATRIX_INTENSITY 0 // Default LEDs intensity between 0-15
#define DELAY_BEFORE_SCREENSAVER_IN_MENU 10 // Cancel game. In seconds
#define DELAY_BEFORE_SCREENSAVER_IN_GAME 30 // In seconds
#define DEBOUNCE_TIME 10            // Delay where no input can change after changing state
#define BUTTON_OPEN HIGH            // Default value read when button is not pressed
#define EMPTY_SPACE_SIZE 3          // Number of blank columns to display between words
#define EMPTY_SPACE_ROLL_SIZE 8     // Number of blank columns to display when rolling
#define TEXT_SCROLLING_SPEED  200   // Time between a column shift occurs when scrolling text in milliseconds


#define MAX_CHIP_DIN_PIN  2
#define MAX_CHIP_CLK_PIN  4
#define MAX_CHIP_LOAD_PIN 3

const unsigned char buttonsPins[] = {
	5, // PLAYER1_LEFT
	6, // PLAYER1_RIGHT
	8, // PLAYER2_LEFT
	9, // PLAYER2_RIGHT
	7  // MENU
};

#endif
