#include "I2C_INTERRUPT.h"
#include <stddef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES & GLOBAL VARIABLES & ENUMS
///////////////////////////////////////////////////////////////////////////////////////////////////

#define I2C_WRITE 0
#define I2C_READ  1
#define write	  0
#define read	  1
static volatile uint8_t 	state;
static volatile uint8_t 	status=1;
static volatile uint16_t 	slave_addr_global=0;
static volatile uint16_t 	register_addr_global=0;
static volatile uint8_t 	*buffer_global=NULL;
static I2C_TypeDef 			*i2c_global;
static volatile uint8_t 	length_global=0;
static volatile uint8_t 	index=0;
volatile uint8_t 			i2c_done = 0;

typedef enum {
	I2C_STOP,
	I2C_START,
	I2C_IDLE,
	I2C_SLAVE_ADDR,
	I2C_FAIL,
	I2C_REG_ADDR,
	I2C_REPEATED_START,
	I2C_SEND,
	I2C_ACK,
	I2C_NACK

}i2c_states;

i2c_states i2c_state=I2C_STOP;
///////////////////////////////////////////////////////////////////////////////////////////////////
// END DEFINES & GLOBAL VARIABLES & ENUMS
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

		GPIOB->AFR[1]&=~(0b1111<<((9*4)-32));
		GPIOB->AFR[1]|=(4<<((9*4)-32));

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

	i2c->CR1|=(1<<8);
}

uint8_t I2Cx_Interrupt_Read(I2C_TypeDef *i2c, uint16_t slave_addr, uint16_t register_addr, uint8_t *buffer, uint8_t length){
	i2c_global=i2c;
	buffer_global=buffer;
	slave_addr_global=slave_addr;
	register_addr_global=register_addr;
	length_global=length;
	i2c_state=I2C_START;
	index=0;
	i2c->CR1|=(1<<8);
	if(state==I2C_FAIL) {
		i2c_state=I2C_START;
		I2C1->CR1|=(1<<8);
		return 1;
	}
	else return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION HANDLERS
///////////////////////////////////////////////////////////////////////////////////////////////////

void I2C1_EV_IRQHandler(void){
	if(I2C1->SR1&(1<<0)){
		if(i2c_state==I2C_START){
			I2C1->DR=(slave_addr_global<<1)|I2C_WRITE;
			i2c_state=I2C_SLAVE_ADDR;
		}
		else if(i2c_state==I2C_REPEATED_START){
			I2C1->DR=(slave_addr_global<<1)|I2C_READ;
			i2c_state=I2C_SEND;
		}
	}

	else if(I2C1->SR1&(1<<1) || (I2C1->SR1&(1<<10))){
		if(I2C1->SR1&(1<<10)){
			i2c_state=I2C_FAIL;
			I2C1->SR1&=~(1<<10);
			I2C1->CR1|=(1<<9);
			return;
		}
		else{
			if(i2c_state==I2C_SLAVE_ADDR){
				(void) I2C1->SR1; (void) I2C1->SR2;
				I2C1->DR=register_addr_global;
				i2c_state=I2C_REG_ADDR;
			}
			else if(i2c_state==I2C_SEND){
				if(length_global==1){
					I2C1->CR1&=~(1<<10);
					(void) I2C1->SR1; (void) I2C1->SR2;
					I2C1->CR1|=(1<<9);
					i2c_state=I2C_STOP;
				}

			}
		}
	}
	else if(I2C1->SR1&(1<<7)){
		i2c_state=I2C_REPEATED_START;
		I2C1->CR1|=(1<<8);
	}
	else if(I2C1->SR1&(1<<6)){
		buffer_global[0]=I2C1->DR;
		i2c_state=I2C_IDLE;
	}

	if(i2c_state==I2C_IDLE){
		return;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTION HANDLERS
///////////////////////////////////////////////////////////////////////////////////////////////////
