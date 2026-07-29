#ifndef TIM_H
#define TIM_H

#include "stdint.h"
#include "stm32f446xx.h"

typedef enum {
	UP_COUNTER=0,
	DOWN_COUNTER
}direction;
typedef struct{
	uint16_t TIM_PCS;
	uint16_t TIM_ARR;
	direction TIM_DIR;
}tim_config_t;

void TIMx_init(TIM_TypeDef *tim,tim_config_t *config);
void TIMx_stop(TIM_TypeDef *tim);
void TIMx_start(TIM_TypeDef *tim);
void TIMx_counter_reset(TIM_TypeDef *tim);
uint16_t TIMx_get_counter(TIM_TypeDef *tim);
void TIMx_set_counter(TIM_TypeDef *tim, uint16_t counter);
#endif
