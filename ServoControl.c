// Source code for controlling the servo connected to the PF3 PWM signal

#include "ServoControl.h"

const unsigned int upperLimit = 60950;
const unsigned int lowerLimit = 54650;
volatile unsigned int compareValue = 57800;
volatile bool format = 1;

// Port D interrupt configuration for push buttons
void buttonConfig(void){
	SYSCTL->RCGCGPIO |= 0x08; // Enable clock for port D
	GPIOD->DIR &= ~0x0E; // Input
	GPIOD->DEN |= 0x0E; // Digital enable
	
	GPIOD->IS |= 0x0C; // PD3 and PD2 level triggered
	GPIOD->IS &= ~0x02; // PD1 edge triggered
	GPIOD->IBE &= ~0x0E; // Controlled by IEV register
	GPIOD->IEV |= 0x0E; // PD3 and PD2 positive level, PD1 rising edge
	GPIOD->ICR |= 0x0E; // clear prior interrupts
	GPIOD->IM |= 0x0E; // unmask interrupt
	
	NVIC->IPR[3] = 1<<5; // interrupt priority level is 1
	NVIC->ISER[0] |= 0x08; // Enable IRQ3
	
}


// Configuration for servo using PWM module 1
void servoConfig(void){

	unsigned int loadvalue = 62500;
	
	SYSCTL->RCGCPWM |= 0X02; // Enable the clock to PWM module 1
	SYSCTL->RCGCGPIO |= 0x20; // Enable the clock to GPIOF
	
	SYSCTL->RCC |= 0x00100000; // Enable clock divider for PWM
	SYSCTL->RCC &= ~0x00E0000; // Clear the current clock divider
	SYSCTL->RCC |= 0x00060000; // Set clock divide to 16: 50MHz/16 = 3.125MHz
	
	delayMs(1); 							 // Wait for PWM clock to stabilize
	
	PWM1->_3_CTL = 0;						// Disable PWM1_3 during configuration
	PWM1->_3_GENB = 0x0000080C; // Output high when load and low when match with compare value
	PWM1->_3_LOAD = loadvalue - 1;  // Load value for 50 Hz
	PWM1->_3_CMPB = compareValue - 1;  // Compare value for initial 7.5% duty cycle
	PWM1->_3_CTL = 1;						// Enable PWM1_3, set counter mode to count down
	PWM1->ENABLE |= 0x80;				// Enable PWM1
	
	GPIOF->DIR |= 0x08;					// Set PF3 pin as output (Green)
	GPIOF->DEN |= 0x08; 				// Set PF3 pin as digital
	GPIOF->AFSEL |= 0x08;				// Set PF3 as alternate function
	GPIOF->PCTL &= ~0x0000F000; // Clear PF3's current alternate function
	GPIOF->PCTL |= 0x00005000;  // Set PF3 alternate function to PWM
}

// LED configuration
void LEDConfig(void){
	SYSCTL->RCGCGPIO |= 0x02;
	GPIOB->DEN |= 0x0E;
	GPIOB->DIR |= 0x0E;
}

void servoRight(void){
	if(compareValue < upperLimit){						// Limit to +/- 90 degrees
		compareValue = compareValue + INCREMENT;
		PWM1->_3_CMPB = compareValue;
		delayMs(DELAY_TIME);
	}
	
}

void servoLeft(void){
	if(compareValue > lowerLimit){						// Limit to +/- 90 degrees
		compareValue = compareValue - INCREMENT;
		PWM1->_3_CMPB = compareValue;
		delayMs(DELAY_TIME);
	}

}

void servoReset(void){
	unsigned int middle = (upperLimit+lowerLimit) / 2;
	if(compareValue > middle){
		while(compareValue != middle){
			compareValue -= 10;
			PWM1->_3_CMPB = compareValue;
			delayMs(1);
		}
	}
	else if(compareValue < middle){
		while(compareValue != middle){
			compareValue += 10;
			PWM1->_3_CMPB = compareValue;
			delayMs(1);
		}
	}
}


void GPIOD_Handler(void){

	if(GPIOD->MIS & 0x08){ // SW4 pressed
		GPIOB->DATA |= 0x08;
		servoLeft();
		GPIOD->ICR |= 0x08; // Clear the interrupt 
		GPIOB->DATA &= ~0x08;
	}
	
	else if(GPIOD->MIS & 0x04){ // SW3 pressed
		GPIOB->DATA |= 0x04;
		servoRight();
		GPIOD->ICR |= 0x04; // Clear the interrupt flag
		GPIOB->DATA &= ~0x04;
	}
	else if(GPIOD->MIS & 0x02){ // SW2 pressed
		servoReset();
		GPIOD->ICR |= 0x02; // Clear the interrupt flag
	}
	else if(GPIOD->MIS & 0x01){ // SW1 pressed
		format = !format;
		delayMs(20);
		GPIOD->ICR |= 0x01;
	}
}

