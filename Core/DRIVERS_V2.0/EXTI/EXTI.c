#include "EXTI.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


inline static uint8_t port_shift(GPIO_TypeDef *port){
	if(port==GPIOA)			return 0;
	else if(port==GPIOB) 	return 1;
	else if(port==GPIOC)	return 2;
	else if(port==GPIOD) 	return 3;
	else while(1);
}

inline static void exti_edge_setup(uint8_t pin, exti_edge_t mode){
	EXTI->FTSR&=~(1<<pin);
	EXTI->RTSR&=~(1<<pin);


	if(mode==EXTI_FALLING) 		 EXTI->FTSR|=(1<<pin);
	else if(mode==EXTI_RISING) 	 EXTI->RTSR|=(1<<pin);
	else if(mode==EXTI_BOTH) 	{EXTI->RTSR|=(1<<pin); EXTI->FTSR|=(1<<pin);}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


void EXTIx_init(exti_config_t *config){
	RCC->APB2ENR|=(1<<14);
	uint8_t port_shift_code=port_shift(config->port);
	uint8_t reg=config->pin/4;
	uint8_t exti_shift_code=(config->pin %4) *4;
	SYSCFG->EXTICR[reg]&=~(0b1111<<exti_shift_code);
	SYSCFG->EXTICR[reg]|=(port_shift_code<<exti_shift_code);

	EXTI->FTSR&=~(1<<config->pin);

	exti_edge_setup(config->pin, config->edge);




}
