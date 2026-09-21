/* Source code to configure the analog to digital converter to read
	 voltages for two light dependent resistors
	 12 bit ADC with 3.3V reference voltage
*/

#include "ADC_Config.h"

void ADC_Config(void){
	SYSCTL->RCGCADC |= 1; 		// Enable clock to ADC module 0
	SYSCTL->RCGCGPIO |= 0x10; // Enable clock to GPIO port E
	
	// Configure PE1 and PE2 for ADC module 0
	GPIOE->AFSEL |= 0x06; 		// Alternate function
	GPIOE->DEN &= ~0x06; 			// Disable digital
	GPIOE->DIR &= ~0x06;      // input
	GPIOE->AMSEL |= 0x06;			// Enable analog
	
	ADC0->ACTSS &= ~1;				// Disable sample sequencer for ADC0 during configuration 
	ADC0->EMUX &= ~0x000F;		// Enable software trigger for sample sequencer 0
	ADC0->SSMUX0 &= ~0xFFFFFFFF; 	// Clear input channel selection for SS0
	ADC0->SSMUX0 |= 0x00000021;  	// AIN 2 (PE1) for first reading, AIN1 (PE2) for second reading
	ADC0->SSCTL0 |= 0x00000600;	 	// End sampling and enable interrupt after the 2nd sample
	ADC0->ACTSS |= 1;					// Enable the sample sequencer
}
