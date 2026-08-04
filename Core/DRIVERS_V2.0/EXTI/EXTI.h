#ifndef EXTI_H
#define EXTI_H

#include "stdint.h"
#include "stm32f446xx.h"

typedef enum{
	EXTI_FALLING=0,
	EXTI_RISING=1,
	EXTI_BOTH=2
}exti_edge_t;

typedef struct{
	GPIO_TypeDef *port;
	uint8_t pin;
	exti_edge_t edge;

}exti_config_t;

#endif
