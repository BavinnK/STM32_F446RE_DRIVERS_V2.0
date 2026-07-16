#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "stm32f446xx.h"
typedef enum{
	GPIOx_MODE_INPUT=0,
	GPIOx_MODE_OUTPUT=1,
	GPIOx_MODE_ALTERNATE=2,
	GPIOx_MODE_ANALOG=3
}gpio_mode;

typedef enum{
	GPIOx_SPEED_LOW_SPEED=0,
	GPIOx_SPEED_MED_SPEED=1,
	GPIOx_SPEED_FAST_SPEED=2,
	GPIOx_SPEED_HIGH_SPEED=3
}gpio_speed;

typedef enum{
	GPIOx_OTYPE_PUSH_PULL=0,
	GPIOx_OTYPE_OPEN_DRAIN=1
}gpio_otype;

typedef enum{
	GPIOx_PUPDR_DISABLE=0,
	GPIOx_PUPDR_PULLUP=1,
	GPIOx_PUPDR_PULLDOWN=2
}gpio_pupdr;

typedef struct{
	uint8_t pin;
	gpio_mode mode;
	gpio_speed speed;
	gpio_otype otype;
	gpio_pupdr pupdr;
}gpio_config_t;

typedef enum{
	GPIOx_SET_LOW=0,
	GPIOx_SET_HIGH=1
}gpio_set_level;

void GPIO_init(GPIO_TypeDef* port,gpio_config_t *gpio_config);
void GPIO_set_level(GPIO_TypeDef *port,uint8_t pin,gpio_set_level level);
uint8_t GPIO_read_pin(GPIO_TypeDef *port,uint8_t pin);

#endif
