#include "SPI_INTERRUPT.h"

typedef struct{
	uint8_t *rx_buffer;
	uint8_t *tx_buffer;

	uint16_t rx_length;
	uint16_t tx_length;

	SPI_TypeDef *spi;

	uint16_t rx_index;
	uint16_t tx_index;

	uint8_t mode;
}spi_handle_t;

spi_handle_t spi_data;


///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

static inline void spi_set(SPI_TypeDef *spi,GPIO_TypeDef *port,uint8_t CS){
	if(spi==SPI1){
		gpio_config_t config_spi_MOSI,config_spi_MISO,config_spi_SCLK,config_spi_CS;
		config_spi_MOSI.pin=7;//PA7
		config_spi_MOSI.mode=GPIOx_MODE_ALTERNATE;
		config_spi_MOSI.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_MOSI.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_MOSI.pupdr=GPIOx_PUPDR_DISABLE;

		config_spi_MISO.pin=6;//PA6
		config_spi_MISO.mode=GPIOx_MODE_ALTERNATE;
		config_spi_MISO.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_MISO.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_MISO.pupdr=GPIOx_PUPDR_DISABLE;

		config_spi_SCLK.pin=5;//PA5
		config_spi_SCLK.mode=GPIOx_MODE_ALTERNATE;
		config_spi_SCLK.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_SCLK.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_SCLK.pupdr=GPIOx_PUPDR_DISABLE;

		config_spi_CS.pin=CS;//user provided pin
		config_spi_CS.mode=GPIOx_MODE_OUTPUT;
		config_spi_CS.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_CS.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_CS.pupdr=GPIOx_PUPDR_DISABLE;

		GPIO_init(GPIOA, &config_spi_MOSI);
		GPIO_init(GPIOA, &config_spi_MISO);
		GPIO_init(GPIOA, &config_spi_SCLK);
		GPIO_init(port, &config_spi_CS);

		GPIOA->AFR[0]&=~((15<<(7*4))|(15<<(6*4))|(15<<(5*4)));
		GPIOA->AFR[0]|=(5<<(7*4))|(5<<(6*4))|(5<<(5*4));
		RCC->APB2ENR|=(1<<12);
	}
	else if(spi==SPI2){
		gpio_config_t config_spi_MOSI,config_spi_MISO,config_spi_SCLK,config_spi_CS;
		config_spi_MOSI.pin=1;//PC1
		config_spi_MOSI.mode=GPIOx_MODE_ALTERNATE;
		config_spi_MOSI.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_MOSI.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_MOSI.pupdr=GPIOx_PUPDR_DISABLE;

		config_spi_MISO.pin=2;//PC2
		config_spi_MISO.mode=GPIOx_MODE_ALTERNATE;
		config_spi_MISO.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_MISO.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_MISO.pupdr=GPIOx_PUPDR_DISABLE;

		config_spi_SCLK.pin=10;//PB10
		config_spi_SCLK.mode=GPIOx_MODE_ALTERNATE;
		config_spi_SCLK.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_SCLK.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_SCLK.pupdr=GPIOx_PUPDR_DISABLE;

		config_spi_CS.pin=CS;//user provided pin
		config_spi_CS.mode=GPIOx_MODE_OUTPUT;
		config_spi_CS.speed=GPIOx_SPEED_HIGH_SPEED;
		config_spi_CS.otype=GPIOx_OTYPE_PUSH_PULL;
		config_spi_CS.pupdr=GPIOx_PUPDR_DISABLE;

		GPIO_init(GPIOC, &config_spi_MOSI);
		GPIO_init(GPIOC, &config_spi_MISO);
		GPIO_init(GPIOB, &config_spi_SCLK);
		GPIO_init(port, &config_spi_CS);

		GPIOC->AFR[0]&=~((0b1111<<4*1)|(0b1111<<4*2));
		GPIOB->AFR[1]&=~(0b1111<<4*(10-8));
		GPIOC->AFR[0]|=((5<<4*1)|(5<<4*2));
		GPIOB->AFR[1]|=(5<<4*(10-8));

		RCC->APB1ENR|=(1<<14);//SPIx CLK EN
	}
}

inline static void spi_IT(SPI_TypeDef *spi){
	if(spi==SPI1) 		NVIC_EnableIRQ(SPI1_IRQn);
	else if(spi==SPI2) 	NVIC_EnableIRQ(SPI2_IRQn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void SPIx_INTERRUPT_init(SPI_TypeDef *spi, spi_interrupt_config_t *config){
	spi_set(spi, config->cs_port, config->cs_pin);

	spi->CR1&=~(1<<6);
	spi->CR1&=~((1<<11)|(1<<10)|(1<<7)|(7<<3)|(1<<1)|(1<<0));
	spi->CR1|=(config->data_format<<11)|(1<<9)|(1<<8)|(config->frame_format<<7)|(config->prescaler<<3)|(1<<2)|(config->clock_polarity<<1)|(config->clock_phase<<0);
	spi->CR2|=(1<<6);
	spi->CR2&=~(1<<4);
	spi_IT(spi);
	spi->CR1|=(1<<6);
}

void SPIx_INTERRUPT_transmit(SPI_TypeDef *spi, uint8_t *buffer, uint16_t length){
	spi_data.mode=0;
	spi_data.tx_length=length;
	spi_data.spi=spi;
	spi_data.tx_index=1;

	spi_data.tx_buffer=buffer+1;
	spi->CR2|=(1<<7);
	spi->DR=*buffer++;
}



void SPIx_INTERRUPT_receive(SPI_TypeDef *spi, uint8_t *buffer, uint16_t length){
	spi_data.mode=1;
	spi_data.rx_buffer=buffer;
	spi_data.rx_length=length;
	spi_data.spi=spi;
	spi_data.rx_index=0;
	spi->DR=0x00;
	spi->CR2|=(1<<7);
}

void SPIx_INTERRUPT_transfer(SPI_TypeDef *spi, uint8_t *tx_buffer, uint16_t tx_length, uint8_t *rx_buffer, uint16_t rx_length){
	spi_data.mode=2;
	spi_data.rx_buffer=rx_buffer;
	spi_data.rx_length=rx_length;
	spi_data.spi=spi;
	spi_data.rx_index=0;
	spi_data.tx_buffer=++tx_buffer;
	spi_data.tx_length=tx_length;
	spi_data.tx_index=0;
	spi->DR=*tx_buffer;

	spi->CR2|=(1<<7);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// HANDLER FUNCTIONS FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void SPI1_IRQHandler(void){
	if(spi_data.mode == 0){
		if(spi_data.spi->SR&(1<<1)){
			if(spi_data.tx_index<spi_data.tx_length){
				spi_data.spi->DR=spi_data.tx_buffer[spi_data.tx_index];
				spi_data.tx_index++;
			}
			else spi_data.spi->CR2&=~(1<<7);
		}
	}

	if(spi_data.mode == 1){
		if(spi_data.spi->SR&(1<<1)){
			if(spi_data.tx_index<spi_data.tx_length){
				spi_data.spi->DR=0x00;
				spi_data.tx_index++;
			}
			else spi_data.spi->CR2&=~(1<<7);
		}

		if(spi_data.spi->SR&(1<<0)){
				spi_data.rx_buffer[spi_data.rx_index]=spi_data.spi->DR;
				spi_data.rx_index++;
		}
	}

	if(spi_data.mode == 2){
		if(spi_data.spi->SR&(1<<1)){
			if(spi_data.tx_index<spi_data.tx_length){
				spi_data.spi->DR=spi_data.tx_buffer[spi_data.tx_index];
				spi_data.tx_index++;
			}
			else spi_data.spi->CR2&=~(1<<7);
		}

		if(spi_data.spi->SR&(1<<0)){
				spi_data.rx_buffer[spi_data.rx_index]=spi_data.spi->DR;
				spi_data.rx_index++;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// END HANDLER FUNCTIONS FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////
