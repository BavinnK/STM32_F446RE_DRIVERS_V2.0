#include "DELAY.h"

volatile uint32_t msticks=0;

void delay_us(uint32_t us){
	SysTick->LOAD=(us*180)-1;		//1 cycle takes 5.56ns, cycle=freq*time means to get 1us we need a 180 cycles, then minus one cuz it counts from zero
	SysTick->VAL=0;					//reset counter to 0

	SysTick->CTRL=(1<<0)|			//enable systick timer
				  (1<<2);			//use the cpu freq

	while(!(SysTick->CTRL&(1<<16)));// we use polling for microsecond delay

	SysTick->CTRL=0;				//turn off systick timer
}

void systick_init(void){
	SysTick->LOAD=1800000-1;		//1 cycle takes 5.56ns, cycle=freq*time means to get 1ms we need a 1800000 cycles, then minus one cuz it counts from zero
	SysTick->VAL=0;

	SysTick->CTRL=(1<<0)|			//enable systick timer
				  (1<<1)|			//enable interrupt
				  (1<<2);			//use the cpu freq
}

void delay_ms(uint32_t ms){
	uint32_t start=msticks;
	while((msticks-start)<ms);
}

void SysTick_Handler(void){
	msticks++;
}
