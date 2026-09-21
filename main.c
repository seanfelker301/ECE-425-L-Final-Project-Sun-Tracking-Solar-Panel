// ECE 425L: Light Detector Project
//

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "TM4C123GH6PM.h"
#include "ServoControl.h"
#include "ADC_Config.h"
#include "LCD.h"
#include "delayMs.h"


int main(void){
	
	// Servo control functions
	buttonConfig();
	LEDConfig();
	servoConfig();
	
	// LCD functions
	LCD_GPIO_Config();
	LCD_init();
	
	// ADC
	ADC_Config();
	
	// Variables to hold the digital and analog LDR readings
	int LDR1_dig; int LDR2_dig;
	double LDR1_analog; double LDR2_analog;
	double diff1; double diff2;
	double diffLimit = 0.15;
	char reading_str[9];
	int nelms;
	
	while(1){
		ADC0->PSSI |= 1;							 	// Begin ADC conversion
		while((ADC0->RIS & 1) == 0){	// Wait for ADC conversion to be completed
		}
		// Store the sample results
		LDR1_dig = ADC0->SSFIFO0;
		LDR2_dig = ADC0->SSFIFO0;
		ADC0->ISC = 1; 													// Clear the raw interrupt flag
		
		// Calculate the analog values
		LDR1_analog = (LDR1_dig * 3.3) / 4095;
		LDR2_analog = (LDR2_dig * 3.3) / 4095;
		
		// Determine the difference to rotate the servo
		diff1 = fabs(LDR1_analog - LDR2_analog);
		diff2 = fabs(LDR2_analog - LDR1_analog);
		if((diff1 <= diffLimit) || (diff2 <= diffLimit)){ // Difference is not high enough -> do nothing
		}
		else if(LDR1_analog > LDR2_analog){
			servoLeft(); 
		}
		else if(LDR1_analog < LDR2_analog){
			servoRight();
		}
		

		
			
		
		delayMs(100);
	}
	return 0;
}


