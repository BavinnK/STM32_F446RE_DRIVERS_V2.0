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
	DISABLE=0,
	ENABLE=1
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

typedef struct{
	uint16_t pcs;
	uint16_t arr;
	center_aligned_modes center_align_mode;
	direction dir;
	fast_en fast_enable;
	tim_modes timer_mode;

}tim_oc_config_t;

#endif
