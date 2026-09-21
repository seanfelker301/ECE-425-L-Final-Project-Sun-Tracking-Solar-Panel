// Header file for controlling the base board LCD

#include "TM4C123GH6PM.h"
#include "delayMs.h"

// Configures the necessary GPIO pins for the LCD
void LCD_GPIO_Config(void);

// Initializes the LCD to be ready to receive commands and data
void LCD_init(void);

// Reads bits 7-4 of data and loads them as a command (control = 0) or data (control = 1)
void LCD_nibble_write(char data, unsigned char control);

// Sends a command to the LCD
void LCD_command(unsigned char command);

// Prints the character of 'data' on the current cursor position
void LCD_data(char data);

// n millisecond delay for a 50MHz clock
void delayMs(int n);