#include "I2C_INTERRUPT.h"
#include <stddef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES & GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////////////////////////////

#define I2C_WRITE 0
#define I2C_READ  1
#define write	  0
#define read	  1
static uint8_t state;
static uint8_t status=1;
static uint16_t 		slave_addr_global=0;
static uint16_t 		register_addr_global=0;
static uint8_t 			*buffer_global=NULL;
static I2C_TypeDef 		*i2c_global;
static uint8_t 			length_global=0;
static volatile uint8_t index=0;

///////////////////////////////////////////////////////////////////////////////////////////////////
// END DEFINES & GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

static void i2c_pin_clk_config(I2C_TypeDef *i2c){
	if(i2c==I2C1){


		gpio_config_t i2c1_sda_PB9_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=9
		};
		gpio_config_t i2c1_scl_PB8_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=8
		};
		GPIO_init(GPIOB, &i2c1_sda_PB9_config);
		GPIO_init(GPIOB, &i2c1_scl_PB8_config);

		GPIOB->AFR[1]&=~(0b1111<<((9*4)-34));
		GPIOB->AFR[1]|=(4<<((9*4)-34));

		GPIOB->AFR[1]&=~(0b1111<<((8*4)-32));
		GPIOB->AFR[1]|=(4<<((8*4)-32));
		RCC->APB1ENR|=(1<<21);
	}
	else if(i2c==I2C2){
		RCC->APB1ENR|=(1<<22);

		gpio_config_t i2c2_sda_PC12_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=12
		};
		gpio_config_t i2c2_scl_PB10_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=10
		};
		GPIO_init(GPIOC, &i2c2_sda_PC12_config);
		GPIO_init(GPIOB, &i2c2_scl_PB10_config);

		GPIOC->AFR[1]&=~(0b1111<<((12*4)-32));
		GPIOC->AFR[1]|=(4<<((12*4)-32));

		GPIOB->AFR[1]&=~(0b1111<<((10*4)-32));
		GPIOB->AFR[1]|=(4<<((10*4)-32));
	}
	else if(i2c==I2C3){
		RCC->APB1ENR|=(1<<23);

		gpio_config_t i2c3_sda_PC9_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=9
		};
		gpio_config_t i2c3_scl_PA8_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=8
		};
		GPIO_init(GPIOC, &i2c3_sda_PC9_config);
		GPIO_init(GPIOA, &i2c3_scl_PA8_config);

		GPIOC->AFR[1]&=~(0b1111<<((9*4)-32));
		GPIOC->AFR[1]|=(4<<((9*4)-32));

		GPIOA->AFR[1]&=~(0b1111<<((8*4)-32));
		GPIOA->AFR[1]|=(4<<((8*4)-32));
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void I2Cx_Interrupt_init(i2c_interrupt_config_t *config){
	i2c_pin_clk_config(config->i2c);

	config->i2c->CR1=1<<15;
		//for(volatile int i=0; i<1000; i++);
	config->i2c->CR1=0;
	config->i2c->CR2|=(1<<10)|(1<<9)|(1<<8);

	config->i2c->CR2&=~(0b111111<<0);
	config->i2c->CR2|=(45<<0);
	config->i2c->CCR=225;
	config->i2c->TRISE=(45+1);

	if(config->i2c ==I2C1) 	   NVIC_EnableIRQ(I2C1_EV_IRQn);
	else if(config->i2c ==I2C2) NVIC_EnableIRQ(I2C2_EV_IRQn);
	else if(config->i2c ==I2C3) NVIC_EnableIRQ(I2C3_EV_IRQn);

	config->i2c->CR1|=(1<<0);
}

void I2Cx_Interrupt_write(I2C_TypeDef *i2c, uint16_t slave_addr, uint16_t register_addr, uint8_t *buffer, uint8_t length){
	i2c_global=i2c;
	buffer_global=buffer;
	slave_addr_global=slave_addr;
	register_addr_global=register_addr;
	length_global=length;
	state=write;
	i2c->CR1|=(1<<8);
}

void I2Cx_Interrupt_Read(I2C_TypeDef *i2c, uint16_t slave_addr, uint16_t register_addr, uint8_t *buffer, uint8_t length){
	i2c_global=i2c;
	buffer_global=buffer;
	slave_addr_global=slave_addr;
	register_addr_global=register_addr;
	length_global=length;
	state=read;
	index=0;
	i2c->CR1|=(1<<8);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION HANDLERS
///////////////////////////////////////////////////////////////////////////////////////////////////

void I2C1_EV_IRQHandler(void){
	if(state==write){
		if(i2c_global->SR1&(1<<0)){
			i2c_global->DR=(slave_addr_global<<1)|I2C_WRITE;
		}
		else if(i2c_global->SR1&(1<<10)){
			i2c_global->CR1|=(1<<9);
		}
		else if(i2c_global->SR1&(1<<1)){
			(void)i2c_global->SR1; (void)i2c_global->SR2;
			i2c_global->DR=(register_addr_global);
		}
		else if(i2c_global->SR1&(1<<7)){
			if(index<length_global){
				i2c_global->DR=*buffer_global++;
				index++;
			}
		}
		else if(i2c_global->SR1&(1<<2)){
			if(index>=length_global){
				i2c_global->CR1|=(1<<9);
				index=0;
			}
		}
	}
	if(state==read){
		if(i2c_global->SR1&(1<<0)){
			if(status==1){
				i2c_global->DR=(slave_addr_global<<1)|I2C_WRITE;
				status=3;
			}
			else if(status==2){
				i2c_global->DR=(slave_addr_global<<1)|I2C_READ;
				status=4;
			}
		}
		else if(i2c_global->SR1&(1<<10)){
			i2c_global->CR1|=(1<<9);
			status=1;
		}
		else if(i2c_global->SR1&(1<<1)){
			if(status==3){
				(void)i2c_global->SR1; (void)i2c_global->SR2;
				i2c_global->DR=(register_addr_global);
			}
			else if(status==4){
				if(length_global==1){
					i2c_global->CR1 &= ~(1<<10);
					(void)i2c_global->SR1; (void)i2c_global->SR2;
					i2c_global->CR1|=(1<<9);
				}
				else if(length_global==2){
					i2c_global->CR1|=(1<<11); i2c_global->CR1&=~(1<<10);
					(void)i2c_global->SR1; (void)i2c_global->SR2;

				}
				else if(length_global>2){
					(void)i2c_global->SR1; (void)i2c_global->SR2;

					if(index<length_global){
						i2c_global->CR1|=(1<<10);

					}
				}
			}
		}
		else if(i2c_global->SR1&(1<<2)){
			i2c_global->CR1|=(1<<9);
			buffer_global[0]=i2c_global->DR;
			buffer_global[1]=i2c_global->DR;
			status=1;
		}
		else if(i2c_global->SR1&(1<<7)){
			i2c_global->CR1|=(1<<8);
			status=2;
		}
		else if(i2c_global->SR1&(1<<6)){
			if(length_global==1){
				buffer_global[0]=i2c_global->DR;
				status=1;
			}
			else if(length_global>2){
				buffer_global[index]=i2c_global->DR;
				index++;
				if(index==(length_global-1)){
					i2c_global->CR1&=~(1<<10);
					status=5;
				}
			}
			if(status==5){
				buffer_global[index]=i2c_global->DR;
				i2c_global->CR1|=(1<<9);
				status=1;
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTION HANDLERS
///////////////////////////////////////////////////////////////////////////////////////////////////
