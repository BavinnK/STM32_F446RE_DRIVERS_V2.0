#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "stm32f446xx.h"
typedef enum{
	GPIO_MODE_INPUT=0,
	GPIO_MODE_OUTPUT=1,
	GPIO_MODE_ALTERNATE=2,
	GPIO_MODE_ANALOG=3
}gpio_mode;

typedef enum{
	GPIO_SPEED_LOW_SPEED=0,
	GPIO_SPEED_MED_SPEED=1,
	GPIO_SPEED_FAST_SPEED=2,
	GPIO_SPEED_HIGH_SPEED=3
}gpio_speed;

typedef enum{
	GPIO_OTYPE_PUSH_PULL=0,
	GPIO_OTYPE_OPEN_DRAIN=1
}gpio_otype;

typedef enum{
	GPIO_PUPDR_DISABLE=0,
	GPIO_PUPDR_PULLUP=1,
	GPIO_PUPDR_PULLDOWN=2
}gpio_pupdr;

typedef struct{
	uint8_t pin;
	gpio_mode mode;
	gpio_speed speed;
	gpio_otype otype;
	gpio_pupdr pupdr;
}gpio_config_t;

typedef enum{
	GPIO_SET_LOW=0,
	GPIO_SET_HIGH=1
}gpio_set_level;

void GPIO_init(GPIO_TypeDef* port,gpio_config_t *gpio_config);
void GPIO_set_level(GPIO_TypeDef *port,uint8_t pin,gpio_set_level level);

#endif
