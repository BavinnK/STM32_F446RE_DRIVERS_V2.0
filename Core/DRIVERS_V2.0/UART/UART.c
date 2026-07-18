#include "UART.h"

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
	RCC->APB1ENR|=(1<<17);			//EN USART2 CLK

	USART2->CR1|=(1<<13)|			//EN USART
			   (1<<5)|				//EN RX INT
			   (1<<3)|				//EN TRANSMITTER
			   (1<<2);				//EN RECEIVER

	USART2->CR2&=~((1<<12)|(1<<13));	//set stop bits to 1

	uint32_t baud=set_baud(baudrate);

	USART2->BRR=baud;
}
