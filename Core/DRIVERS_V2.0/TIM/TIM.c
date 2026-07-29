#include "TIM.h"

inline static void tim_on(TIM_TypeDef *tim){
	if(tim==TIM1) RCC->APB2ENR|=(1<<0);
	else if(tim==TIM2) RCC->APB1ENR|=(1<<0);
	else if(tim==TIM3) RCC->APB1ENR|=(1<<1);
	else if(tim==TIM4) RCC->APB1ENR|=(1<<2);
	else if(tim==TIM5) RCC->APB1ENR|=(1<<3);
	else if(tim==TIM6) RCC->APB1ENR|=(1<<4);
	else if(tim==TIM7) RCC->APB1ENR|=(1<<5);
	else if(tim==TIM8) RCC->APB2ENR|=(1<<1);
}

void TIMx_init(TIM_TypeDef *tim,tim_config_t *config){
	tim_on(tim);
	tim->CR1&=~((1<<0)|(1<<4));
	tim->CR1|=(config->TIM_DIR<<4);
	tim->ARR=config->TIM_ARR;
	tim->PSC=config->TIM_PCS;
	tim->CNT=0;
	tim->EGR|=(1<<0);
}

void TIMx_stop(TIM_TypeDef *tim){
	tim->CR1&=~(1<<0);
}

void TIMx_start(TIM_TypeDef *tim){
	tim->CR1|=(1<<0);
}

void TIMx_counter_reset(TIM_TypeDef *tim){
	tim->CNT=0;
}

uint16_t TIMx_get_counter(TIM_TypeDef *tim){
	return tim->CNT;
}

void TIMx_set_counter(TIM_TypeDef *tim, uint16_t counter){
	tim->CNT=counter;
}




