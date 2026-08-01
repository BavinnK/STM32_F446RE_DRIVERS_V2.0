#ifndef TIM_IC_H
#define TIM_IC_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "TIM.h"
#include "TIM_OC.h"

typedef struct{
	uint16_t pcs;
	uint16_t arr;
	center_aligned_modes center_align_mode;
	direction dir;
}tim_ic_config_t;



#endif
