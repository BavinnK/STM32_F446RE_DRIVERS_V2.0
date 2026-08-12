#include "I2C_INTERRUPT.h"
#include <stddef.h>

#define I2C_WRITE 0
#define I2C_READ  1

static uint16_t slave_addr_global=0;
static uint16_t register_addr_global=0;
static uint8_t *buffer_global=NULL;
static I2C_TypeDef *i2c_global;
static uint8_t length_global=0;


///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

static void i2c_pin_clk_config(I2C_TypeDef *i2c){
	if(i2c==I2C1){
		RCC->APB1ENR|=(1<<21);

		gpio_config_t i2c1_sda_PB7_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=7
		};
		gpio_config_t i2c1_scl_PB8_config={
			.mode=GPIOx_MODE_ALTERNATE,
			.otype=GPIOx_OTYPE_OPEN_DRAIN,
			.pupdr=GPIOx_PUPDR_PULLUP,
			.speed=GPIOx_SPEED_HIGH_SPEED,
			.pin=8
		};
		GPIO_init(GPIOB, &i2c1_sda_PB7_config);
		GPIO_init(GPIOB, &i2c1_scl_PB8_config);

		GPIOB->AFR[0]&=~(0b1111<<(7*4));
		GPIOB->AFR[0]|=(4<<(7*4));

		GPIOB->AFR[1]&=~(0b1111<<((8*4)-32));
		GPIOB->AFR[1]|=(4<<((8*4)-32));
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

inline static void en_nvic(I2C_TypeDef *i2c){
	if(i2c==I2C1) 	   NVIC_EnableIRQ(I2C1_EV_IRQn);
	else if(i2c==I2C2) NVIC_EnableIRQ(I2C2_EV_IRQn);
	else if(i2c==I2C3) NVIC_EnableIRQ(I2C3_EV_IRQn);
	else while(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void I2Cx_Interrupt_init(i2c_interrupt_config_t *config){
	i2c_pin_clk_config(config->i2c);
	config->i2c->CR1&=~(1<<0);
	config->i2c->CR2|=(1<<10)|(1<<9)|(1<<8);

	config->i2c->CR2&=~(0b111111<<0);
	config->i2c->CR2|=(45<<0);
	config->i2c->CCR=(45000000)/(2*config->speed);
	config->i2c->TRISE=(45+1);



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

///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION HANDLERS
///////////////////////////////////////////////////////////////////////////////////////////////////

void I2C1_EV_IRQHandler(void){

}

void I2C2_EV_IRQHandler(void){

}

void I2C3_EV_IRQHandler(void){

}
///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTION HANDLERS
///////////////////////////////////////////////////////////////////////////////////////////////////
