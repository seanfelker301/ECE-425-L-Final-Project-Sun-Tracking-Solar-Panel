// Header File for the controlling the servo connected to Port F3 - J5 on the base board

#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "delayMs.h"
#define DELAY_TIME 5 
#define INCREMENT 35  // Increment by 35 for about 1 degree


/*Definitions for the initial compare values for the upper limit, lower limit, and current
	for the pulse width modulation signal controlling the servo position
*/
extern const unsigned int upperLimit;
extern const unsigned int lowerLimit;
extern volatile unsigned int compareValue;
extern volatile bool format;

// Configures the push buttons SW2, SW3, and SW5 as interrupts to control the
void buttonConfig(void);

// Configures Port F3 to output a PWM signal to control the servo
void servoConfig(void);

// Configures Port B3, B2, and B1 to control the base board LEDs 3-1
void LEDConfig(void);

// Rotates the servo right by incrementing the compareValue global variable
// Increment amount determined by the INCREMENT definition
void servoRight(void);

// Rotates the servo left by decrementing the compareValue global variable
// Decrement amount determined by INCREMENT definition
void servoLeft(void);

// "Resets" the servo position by rotating it back to about 0 degrees.
void servoReset(void);

// Delays the system by n milliseconds when called
void delayMs(int n);


// Interrupt handler for push buttons. SW2 rotates the servo left, SW3 rotates right, and SW4 resets the position
void GPIOD_Handler(void);


