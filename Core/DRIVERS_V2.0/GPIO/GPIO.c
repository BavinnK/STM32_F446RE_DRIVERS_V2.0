#include "GPIO.h"
inline static void port_setup(GPIO_TypeDef* port){
	if(port==GPIOA){
		RCC->AHB1ENR|=(1);
	}
	else if(port==GPIOB){
		RCC->AHB1ENR|=(2);
	}
	else if(port==GPIOC){
		RCC->AHB1ENR|=(4);
	}
}

void GPIO_init(GPIO_TypeDef* port,gpio_config_t *gpio_config){


	port->MODER&=~(3<<(gpio_config->pin*2));								//setting up MODER register, first clear then set
	port->MODER|=((gpio_config->mode)<<(gpio_config->pin*2));

	port->OTYPER&=~(1<<(gpio_config->pin));									//setting up OTYPE register, first clear then set
	port->OTYPER|=((gpio_config->otype)<<(gpio_config->pin));

	port->OSPEEDR&=~(3<<(gpio_config->pin*2));								//setting up SPEED register, first clear then set
	port->OSPEEDR|=((gpio_config->speed)<<(gpio_config->pin*2));

	port->PUPDR&=~(3<<(gpio_config->pin*2));								//setting up PUPDR register, first clear then set
	port->PUPDR|=((gpio_config->pupdr)<<(gpio_config->pin*2));

};
