#include <I2C_POLLING.h>
#define I2C_WRITE 0
#define I2C_READ  1

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

void I2Cx_Polling_init(I2C_TypeDef *i2c, i2c__polling_config_t *config){
	i2c_pin_clk_config(i2c);

	i2c->CR1&=~(1<<0);


	i2c->CR2&=~(0b111111<<0);
	i2c->CR2|=45;
	i2c->CCR =225;            // 100 kHz
	i2c->TRISE= 46;

	i2c->CR1|=(1<<0);
}

void I2Cx_Polling_start(I2C_TypeDef *i2c){
	i2c->CR1|=(1<<8);
	while(!(i2c->SR1&(1<<0)));
}

void I2Cx_Polling_stop(I2C_TypeDef *i2c){
	i2c->CR1|=(1<<9);
}

void I2Cx_Polling_write(I2C_TypeDef* i2c,uint8_t slave_addr, uint8_t register_addr, uint8_t *buffer, uint16_t length){
	I2Cx_Polling_start(i2c);
	i2c->DR=(slave_addr<<1)|I2C_WRITE;
	while(!(i2c->SR1&(1<<1)));
	(void)i2c->SR1;
	(void)i2c->SR2;

	i2c->DR=register_addr;
	while(!(i2c->SR1&(1<<7)));

	for(uint16_t i=1;i<=length;i++){
		i2c->DR=*buffer;
		buffer++;
		while(!(i2c->SR1&(1<<7)));
	}

	I2Cx_Polling_stop(i2c);
}

void I2Cx_Polling_read(I2C_TypeDef *i2c,uint16_t slave_addr, uint16_t register_addr, uint8_t *buffer, uint8_t length){
	I2Cx_Polling_start(i2c);
	while(!(i2c->CR1&(1<<0)));
	i2c->DR=(slave_addr<<1)|I2C_WRITE;
	while(!(i2c->SR1&(1<<1)));
	(void)i2c->SR1;
	(void)i2c->SR2;

	i2c->DR=register_addr;
	while(!(i2c->SR1&(1<<7)));

	I2Cx_Polling_start(i2c);
	while(!(i2c->CR1&(1<<0)));
	i2c->DR=(slave_addr<<1)|I2C_READ;
	while(!(i2c->SR1&(1<<1)));

	if(length==1){
		i2c->CR1&=~(1<<10);
		(void)i2c->SR1; (void)i2c->SR2;
		I2Cx_Polling_stop(i2c);
		while(!(i2c->SR1&(1<<6)));
		buffer[0]=i2c->DR;
	}
	else if(length==2){
		i2c->CR1|=(1<<11); i2c->CR1&=~(1<<10);
		(void)i2c->SR1; (void)i2c->SR2;
		while(!(i2c->SR1&(1<<2)));
		I2Cx_Polling_stop(i2c);
		buffer[0]=i2c->DR;
		buffer[1]=i2c->DR;
	}
	else if(length>=3){
		(void)i2c->SR1; (void)i2c->SR2;
		uint16_t i=0;
		length--;
		for(i=0;i<length;i++){
			i2c->CR1|=(1<<10);
			while(!(i2c->SR1&(1<<6)));
			buffer[i]=i2c->DR;
		}

		i2c->CR1&=~(1<<10);
		while(!(i2c->SR1&(1<<6)));
		buffer[i]=i2c->DR;
		I2Cx_Polling_stop(i2c);
	}
}


