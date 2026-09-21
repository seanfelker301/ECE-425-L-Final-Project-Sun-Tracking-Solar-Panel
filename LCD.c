// Source code for configuring and interacting with the base board LCD 

#include "LCD.h"


// Configures the needed GPIO pins to run the LCD
void LCD_GPIO_Config(void){
	
	SYSCTL->RCGCGPIO |= 0x01; // Enable clock to Ports A, E, C
	SYSCTL->RCGCGPIO |= 0x10;
	SYSCTL->RCGCGPIO |= 0x04;
	// Configure GPIO A pins 5-2 as digital output
	GPIOA->AMSEL &= ~0x3C;
	GPIOA->DATA &= ~0x3C;
	GPIOA->DIR |= 0x3C;
	GPIOA->DEN |= 0x3C;
	
	// Configure PE0 to control the RS pin of the LCD. Digital output
	GPIOE->AMSEL &= ~0x01;
	GPIOE->DIR |= 0x01;
	GPIOE->DEN |= 0x01;
	GPIOE->DATA |= 0x01; // 1 = display data, 0 = commands
	
	// Configure PC6 for LCD enable pin. Digital output
	GPIOC->AMSEL &= ~0x40;
	GPIOC->DIR |= 0x40;
	GPIOC->DEN |= 0x40;
	GPIOC->DATA &= ~0x40; // 0 = idle LCD state, 1 = ready to read data
	
	// Configure PD3 as a digital input
	SYSCTL->RCGCGPIO |= 0x08; // Clock to port D
	GPIOD->DIR &= ~0x08; 			// PD3
	GPIOD->DEN |= 0x08;
	GPIOD->AMSEL &= ~0x08;
	
}

// Initializes and configures the LCD
void LCD_init(void){
	
	// Initialization sequence
	delayMs(20);
	LCD_nibble_write(0x30, 0); 
	delayMs(5);
	LCD_nibble_write(0x30, 0);
	delayMs(1);
	LCD_nibble_write(0x30, 0);
	delayMs(1);
	
	// Configuration
	LCD_nibble_write(0x20, 0);	// 4 bit data mode
	delayMs(1);
	LCD_command(0x28);					// 4 bit data, 2 line, 5x7 font
	LCD_command(0x06);					// move cursor right
	LCD_command(0x01);					// clear screen, move cursor to home
	LCD_command(0x0F);					// turn on LCD
	
}


// Reads bits 7-4 of data and loads them as a command (control = 0) or data (control = 1)
void LCD_nibble_write(char data, unsigned char control){
	GPIOA->DIR |= 0x3C; 		// set PA5-PA2 as outputs for LCD display
	GPIOA->DATA &= ~0x3C;		// clear the line
	GPIOA->DATA |= (data & 0xF0) >> 2; // extract the upper 4 bits
	// set RS bit
	if (control & 1){
		GPIOE->DATA |= 1; 		// Check if we are sending data
	}
	else{
		GPIOE->DATA &= ~1; 		// check if we are sending a command
	}
	
	/* sending a high to low transition pulse on LCD enable pin PC6 */
	GPIOC->DATA |= 1 << 6;
	delayMs(0);
	GPIOC->DATA &= ~(1 << 6);
	GPIOA->DIR &= ~0x3C; 		// clear the line
}

// Sends a command to the LCD
void LCD_command(unsigned char command){
	LCD_nibble_write(command & 0xF0, 0); // upper nibble (4 bits)
	LCD_nibble_write(command << 4, 0); // lower nibble
	if(command < 3){
		delayMs(2);						// maximum delay of 2ms for commands 1 and 2
	}
	else{
		delayMs(1);						// maximum delay of 1ms for all other commands
	}
}

// Prints a character on the current cursor position
void LCD_data(char data){
	LCD_nibble_write(data & 0xF0, 1); // upper 4 bits
	LCD_nibble_write(data << 4, 1);		// lower 4 bits
	delayMs(1);
}


