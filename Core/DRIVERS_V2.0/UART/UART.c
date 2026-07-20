#include "UART.h"

volatile uint8_t data=0,busy_flag=0;
static inline uint32_t set_baud(uint32_t baud){
	/*
	 * the reason i did not put any equation to get the BRR parts which are
	 * mantissa and fraction was so i dont waste cpu cycle and use floating points
	 * which abit hard on teh CPU i was like i will calculate it myself and give it to BRR register
	 * the equation is
	 * USARTDIV=(fclk/(16*baud))
	 * the fclk is the frequency of ABP1 BUS which in my case it's 45 Mhz
	 */
	if(baud==9600) 			return 0x1250;
	else if(baud==19200) 	return 0x0928;
	else if(baud==38400) 	return 0x0494;
	else if(baud==57600) 	return 0x030D;
	else if(baud==115200) 	return 0x0187;
	else if(baud==230400) 	return 0x003C;
	else if(baud==460800) 	return 0x0062;
	else if(baud==921600) 	return 0x0031;
	else 					return 0;
}

void UART2_init(uint32_t baudrate){
	gpio_config_t TX_setup={
		.pin=2,
		.mode=GPIOx_MODE_ALTERNATE,
		.otype=GPIOx_OTYPE_PUSH_PULL,
		.speed=GPIOx_SPEED_HIGH_SPEED,
		.pupdr=GPIOx_PUPDR_DISABLE
	};
	gpio_config_t RX_setup={
		.pin=3,
		.mode=GPIOx_MODE_ALTERNATE,
		.otype=GPIOx_OTYPE_PUSH_PULL,
		.speed=GPIOx_SPEED_HIGH_SPEED,
		.pupdr=GPIOx_PUPDR_DISABLE
	};

	GPIO_init(GPIOA, &TX_setup);
	GPIO_init(GPIOA, &RX_setup);
	GPIOA->AFR[0]&=~(0b11111111<<8);
	GPIOA->AFR[0]|=(0b0111<<12)|(0b0111<<8);

	RCC->APB1ENR|=(1<<17);			//EN USART2 CLK

	USART2->CR1|=(1<<13)|			//EN USART
			   (1<<5)|				//EN RX INT
			   (1<<3)|				//EN TRANSMITTER
			   (1<<2);				//EN RECEIVER

	USART2->CR2&=~((1<<12)|(1<<13));	//set stop bits to 1

	uint32_t baud=set_baud(baudrate);

	USART2->BRR=baud;

	NVIC_EnableIRQ(USART2_IRQn);
}

void UART2_write_byte(char byte){
	while(busy_flag==1);
	busy_flag=1;
	data=byte;
	USART2->CR1|=(1<<7);

}

void USART2_IRQHandler(void){
	if(USART2->SR&(1<<7)){
		USART2->DR=(char)data;
		USART2->CR1&=~(1<<7);
		busy_flag=0;
	}


}
