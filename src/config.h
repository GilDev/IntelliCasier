#ifndef CONFIG_H
#define CONFIG_H

// Delays are inversely proportional to speed
// Delays are in milliseconds

/* ----- General settings ----- */
//#define DEBUG            // Activate serial messages & internal LED blinking every 1000 loop cycle
#define SERIAL_BAUD 9600 // Serial communication speed

#define LANG 1 // 0 = English, 1 = French

#define RTC // Define if you have a DS3231 RTC

#define USE_INTERNAL_PULLUPS true // Use the Arduino's builtin pullups on inputs
#define BUTTON_OPEN HIGH          // Default value when button is not pressed
#define DEBOUNCE_TIME 10          // Delay where no input can change after changing state

#define DELAY_BEFORE_SCREENSAVER_IN_MENU 15 // In seconds
#define DELAY_BEFORE_SCREENSAVER_IN_APP  30 // Cancel game. In seconds

#define MAX_CHIP_DIN_PIN  2
#define MAX_CHIP_CLK_PIN  4
#define MAX_CHIP_LOAD_PIN 3

static const unsigned char buttonsPins[] = {
	5, // PLAYER1_LEFT
	6, // PLAYER1_RIGHT
	8, // PLAYER2_LEFT
	9, // PLAYER2_RIGHT
	7  // MENU
};


/* ----- LED matrix ----- */
#define MATRIX_BRIGHTNESS             5   // Default LEDs intensity between 0-15
#define MATRIX_SCREENSAVER_BRIGHTNESS 0   // Default LEDs intensity between 0-15
#define MATRIX_SCROLL_DELAY           200 // Time between a column shift occurs when scrolling text
#define MATRIX_EMPTY_SPACE_SIZE       3   // Number of blank columns to display between words when scrolling
#define MATRIX_EMPTY_SPACE_ROLL_SIZE  8   // Number of blank columns to display when rolling over when scrolling


/* ----- Pong ----- */
#define PONG_PADDLE_DELAY          200  // Delay between moves when holding up/down
#define PONG_START_DELAY           500  // Time between ball position updates
#define PONG_MIN_DELAY             100  // Shortest time between ball position updates
#define PONG_SPEED_INCREASES_DELAY 5000 // Delay between ball speed increases


/* ----- Snake ----- */
#define SNAKE_MOVE_DELAY 500 // Delay between snake's moves


/* ----- Flappy Bird ----- */
#define FLAPPY_BIRD_FALL_DELAY            500
#define FLAPPY_BIRD_START_DELAY           1000
#define FLAPPY_BIRD_MIN_DELAY             100
#define FLAPPY_BIRD_SPEED_INCREASES_DELAY 5000

#endif