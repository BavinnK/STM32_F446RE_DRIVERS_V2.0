#include "EXTI.h"
#include "stddef.h"

static void (*exti_callback_0)(void)=NULL;
static void (*exti_callback_1)(void)=NULL;
static void (*exti_callback_2)(void)=NULL;
static void (*exti_callback_3)(void)=NULL;
static void (*exti_callback_4)(void)=NULL;
static void (*exti_callback_5)(void)=NULL;
static void (*exti_callback_6)(void)=NULL;
static void (*exti_callback_7)(void)=NULL;
static void (*exti_callback_8)(void)=NULL;
static void (*exti_callback_9)(void)=NULL;
static void (*exti_callback_10)(void)=NULL;
static void (*exti_callback_11)(void)=NULL;
static void (*exti_callback_12)(void)=NULL;
static void (*exti_callback_13)(void)=NULL;
static void (*exti_callback_14)(void)=NULL;
static void (*exti_callback_15)(void)=NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


inline static uint8_t port_shift(GPIO_TypeDef *port){
	if(port==GPIOA)			return 0;
	else if(port==GPIOB) 	return 1;
	else if(port==GPIOC)	return 2;
	else if(port==GPIOD) 	return 3;
	else while(1);
}

inline static void exti_edge_setup(uint8_t pin, exti_edge_t mode){
	EXTI->FTSR&=~(1<<pin);
	EXTI->RTSR&=~(1<<pin);


	if(mode==EXTI_FALLING) 		 EXTI->FTSR|=(1<<pin);
	else if(mode==EXTI_RISING) 	 EXTI->RTSR|=(1<<pin);
	else if(mode==EXTI_BOTH) 	{EXTI->RTSR|=(1<<pin); EXTI->FTSR|=(1<<pin);}

}

inline static void en_NVIC(uint8_t pin){
    if	   (pin==0) 	NVIC_EnableIRQ(EXTI0_IRQn);
	else if(pin==1) 	NVIC_EnableIRQ(EXTI1_IRQn);
	else if(pin==2) 	NVIC_EnableIRQ(EXTI2_IRQn);
	else if(pin==3) 	NVIC_EnableIRQ(EXTI3_IRQn);
	else if(pin==4) 	NVIC_EnableIRQ(EXTI4_IRQn);
	else if(pin==5) 	NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if(pin==6) 	NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if(pin==7) 	NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if(pin==8) 	NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if(pin==9) 	NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if(pin==10) 	NVIC_EnableIRQ(EXTI15_10_IRQn);
	else if(pin==11) 	NVIC_EnableIRQ(EXTI15_10_IRQn);
	else if(pin==12) 	NVIC_EnableIRQ(EXTI15_10_IRQn);
	else if(pin==13) 	NVIC_EnableIRQ(EXTI15_10_IRQn);
	else if(pin==14) 	NVIC_EnableIRQ(EXTI15_10_IRQn);
	else if(pin==15) 	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void EXTIx_init(exti_config_t *config){
	RCC->APB2ENR|=(1<<14);
	uint8_t port_shift_code=port_shift(config->port);
	uint8_t reg=config->pin/4;
	uint8_t exti_shift_code=(config->pin %4) *4;

	SYSCFG->EXTICR[reg]&=~(0b1111<<exti_shift_code);
	SYSCFG->EXTICR[reg]|=(port_shift_code<<exti_shift_code);

	EXTI->FTSR&=~(1<<config->pin);

	exti_edge_setup(config->pin, config->edge);

	EXTI->IMR&=~(1<<config->pin);
	EXTI->IMR|=(1<<config->pin);

	en_NVIC(config->pin);
}

void EXTIx_register_callbacks(void (*callback)(void), uint8_t pin){
	if	   (pin==0) 	exti_callback_0=callback;
	else if(pin==1) 	exti_callback_1=callback;
	else if(pin==2) 	exti_callback_2=callback;
	else if(pin==3) 	exti_callback_3=callback;
	else if(pin==4) 	exti_callback_4=callback;
	else if(pin==5) 	exti_callback_5=callback;
	else if(pin==6) 	exti_callback_6=callback;
	else if(pin==7) 	exti_callback_7=callback;
	else if(pin==8) 	exti_callback_8=callback;
	else if(pin==9) 	exti_callback_9=callback;
	else if(pin==10) 	exti_callback_10=callback;
	else if(pin==11) 	exti_callback_11=callback;
	else if(pin==12) 	exti_callback_12=callback;
	else if(pin==13) 	exti_callback_13=callback;
	else if(pin==14) 	exti_callback_14=callback;
	else if(pin==15) 	exti_callback_15=callback;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END  FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT HANDLER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void EXTI0_IRQHandler(void){
	if(EXTI->PR&(1<<0)){
		EXTI->PR=(1<<0);

		if(exti_callback_0 != NULL){
			exti_callback_0();
		}
	}
}

void EXTI1_IRQHandler(void){
	if(EXTI->PR&(1<<1)){
		EXTI->PR=(1<<1);

		if(exti_callback_1 != NULL){
			exti_callback_1();
		}
	}
}



void EXTI2_IRQHandler(void){
	if(EXTI->PR&(1<<2)){
		EXTI->PR=(1<<2);

		if(exti_callback_2 != NULL){
			exti_callback_2();
		}
	}
}


void EXTI3_IRQHandler(void){
	if(EXTI->PR&(1<<3)){
		EXTI->PR=(1<<3);

		if(exti_callback_3 != NULL){
			exti_callback_3();
		}
	}
}


void EXTI4_IRQHandler(void){
	if(EXTI->PR&(1<<4)){
		EXTI->PR=(1<<4);

		if(exti_callback_4 != NULL){
			exti_callback_4();
		}
	}
}


void EXTI9_5_IRQHandler(void){
	if(EXTI->PR&(1<<5)){
		EXTI->PR=(1<<5);

		if(exti_callback_5 != NULL){
			exti_callback_5();
		}
	}
}



void EXTI15_10_IRQHandler(void){

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END INTERRUPT HANDLER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////
