#include "CLOCK.h"

void system_clk_180mhz(void){
	RCC->CR|=(1<<16);					//enable HSE CLK
	while(!(RCC->CR&(1<<17)));			//wait until HSI is ready

	RCC->PLLCFGR|=(8);					//DIVIDE HSE BY 8
	RCC->PLLCFGR|=(360<<6);				//MULTIPLY BY 360
	RCC->PLLCFGR&=~(3<<16);				//DIVIDE BY 2

	RCC->CFGR&=~((7<<10)|(7<<13));  	//since perpherials on both APB1 AND APB2 CAN'T run at 180mhz wee need to slow them down
	RCC->CFGR|=((5<<10)|(4<<13));


	RCC->APB1ENR|=(1<<28);				//180mhz is no joke, higher freq need higher power
	PWR->CR|=(3<<14);					//set voltage scale to 1
	PWR->CR|=(1<<16);					//EN overdrive
	while(!(PWR->CSR&(1<<16)));			//wait for overdrive

	PWR->CR|=(1<<17);					//overdrive switch
	while(!(PWR->CSR&(1<<17)));			//wait for overdrive switch

	FLASH->ACR|=(5)|					//180mhz is too fast for memory, so we need to add latency for the cpu, 5WaitState or 5 cycle wait for cpu
				(1<<8)|					//EN prefetch
				(1<<9)|					//EN instruction cache
				(1<<10);				//EN data cache

	RCC->CR|=(1<<24);					//enable PLL
	while(!(RCC->CR&(1<<25)));			//wait until PLL is locked

	RCC->CFGR&=~(3);					//when every configuration is done, then we switch main clk sourse from HSI to PLL
	RCC->CFGR|=(2);
	while((RCC->CFGR&(3<<2))!=(2<<2));	//wait until switching is done


}


