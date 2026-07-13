#include "CLOCK.h"

void system_clk_180mhz(void){
	RCC->CR|=(1<<16);					//enable HSI CLK
	while(!(RCC->CR&(1<<17)));			//wait until HSI is ready

	RCC->CR|=(1<<24);					//enable PLL
	while(!(RCC->CR&(1<<25)));			//wait until PLL is locked


	RCC->PLLCFGR|=(8);					//DIVIDE HSE BY 8
	RCC->PLLCFGR|=(360<<6);				//MULTIPLY BY 360
	RCC->PLLCFGR&=~(3<<16);				//DIVIDE BY 2

	RCC->CFGR&=~((7<<10)|(7<<13));
	RCC->CFGR|=((5<<10)|(4<<13));


	RCC->APB1ENR|=(1<<28);
	PWR->CR|=(3<<14);					//set voltage scale to 1
	PWR->CR|=(1<<16);
	while(!(PWR->CSR&(1<<16)));

	PWR->CR|=(1<<17);
	while(!(PWR->CSR&(1<<17)));

	FLASH->ACR|=(5)|(1<<8)|(1<<9)|(1<<10);

	RCC->CR|=(2);


}


