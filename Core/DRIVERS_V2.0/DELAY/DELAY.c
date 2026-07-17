#include "DELAY.h"

void delay_us(uint32_t us){

	SysTick->LOAD=(us*180)-1;		//1 cycle takes 5.56ns, cycle=freq*time means to get 1us we need a 180 cycles, then minus one cuz it counts from zero
	SysTick->VAL=0;

	SysTick->CTRL=(3);				//enable systick timer, and use system freq

	while(!(SysTick->CTRL&(1<<16)));// we use polling for microsecond delay

	SysTick->CTRL=0;				//turn off systick timer


}
