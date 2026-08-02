#include "GPIO.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////
//END FUNCTION INLINE
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void GPIO_init(GPIO_TypeDef* port,gpio_config_t *gpio_config){

	port_setup(port);

	port->MODER&=~(3<<(gpio_config->pin*2));								//setting up MODER register, first clear then set
	port->MODER|=((gpio_config->mode)<<(gpio_config->pin*2));

	port->OTYPER&=~(1<<(gpio_config->pin));									//setting up OTYPE register, first clear then set
	port->OTYPER|=((gpio_config->otype)<<(gpio_config->pin));

	port->OSPEEDR&=~(3<<(gpio_config->pin*2));								//setting up SPEED register, first clear then set
	port->OSPEEDR|=((gpio_config->speed)<<(gpio_config->pin*2));

	port->PUPDR&=~(3<<(gpio_config->pin*2));								//setting up PUPDR register, first clear then set
	port->PUPDR|=((gpio_config->pupdr)<<(gpio_config->pin*2));
};

void GPIO_set_level(GPIO_TypeDef *port,uint8_t pin,gpio_set_level level){
	if(level==0){
		port->BSRR|=(1<<(pin+16));
	}
	else if(level==1){
		port->BSRR|=(1<<pin);
	}
}

uint8_t GPIO_read_pin(GPIO_TypeDef *port,uint8_t pin){
	if(port->IDR&(1<<pin)){
		return 1;
	}
	else return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//END FUNCTION
///////////////////////////////////////////////////////////////////////////////////////////////////
