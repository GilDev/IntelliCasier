#ifndef COMMON_H
#define COMMON_H

/**
 * Shows a countdown on the LED matrix & LCD display
 * Warning : remove click handlers
 * @param endCallback  Function to call when the countdown ends
 * @param exitCallback Function to call when pression menu
 */
void countdown(void (*endCallback)(void), void (*exitCallback)(byte data));

/**
 * Shows an animation clearing the screen from top to bottom
 */
void wipeMatrix(void);

/**
 * 
 */

#endif
