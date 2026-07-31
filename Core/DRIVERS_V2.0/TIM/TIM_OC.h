#ifndef TIM_OC_H
#define TIM_OC_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "TIM.h"

typedef enum{
	EDGE_MODE=0,
	MODE1,
	MODE2,
	MODE3
}center_aligned_modes;

typedef enum{
	DISABLE_FAST=0,
	ENABLE_FAST=1
}fast_en;

typedef enum{
	FROZEN=0,
	ACTIVE_MATCH,
	INACTIVE_MATCH,
	TOGGLE,
	FORCE_INACTIVE,
	FORCE_ACTIVE,
	PWM1,
	PWM2
}tim_modes;

typedef enum{
    TIM_POLARITY_HIGH=0,
    TIM_POLARITY_LOW
}tim_polarity;

typedef struct{
	uint16_t pcs;
	uint16_t arr;
	center_aligned_modes center_align_mode;
	direction dir;
	fast_en fast_enable;
	tim_modes timer_mode;
	uint16_t ccrx_val;
	tim_polarity polarity;

}tim_oc_config_t;

void TIMx_OC_init(TIM_TypeDef *tim,uint8_t channel,tim_oc_config_t *config);


#endif
