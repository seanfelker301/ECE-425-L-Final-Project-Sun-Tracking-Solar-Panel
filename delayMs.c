// n milliseconds delay for a 50MHz system clock

#include "delayMs.h"

void delayMs(int n){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < 6265; j++)
		{} // Do nothing for 1 ms
	}
}