#include "DHT11.h"

SemaphoreHandle_t dhtSemaphore;

static volatile uint16_t last_time=0;
static volatile uint16_t current_time=0;
static volatile uint16_t index=0,edgecounter=0;
static volatile uint8_t *global_buffer=NULL;
static GPIO_TypeDef *global_port;
static uint8_t global_pin;
uint16_t *bi=NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


static inline uint8_t pcs_set(TIM_TypeDef *tim){
	if(tim==TIM1 || tim==TIM8 || tim==TIM9 || tim==TIM10 || tim==TIM11) return 179;
	else return 89;
}

static inline void set_int(TIM_TypeDef *tim, uint8_t channel){
	if(channel==1) tim->DIER|=(1<<1);
	else if(channel==2) tim->DIER|=(1<<2);
	else if(channel==3) tim->DIER|=(1<<3);
	else if(channel==4) tim->DIER|=(1<<4);
}

static inline void en_nvic(TIM_TypeDef *tim){
	if(tim==TIM1) NVIC_EnableIRQ(TIM1_CC_IRQn);
	else if(tim==TIM2) NVIC_EnableIRQ(TIM2_IRQn);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void DHT11_init(TIM_TypeDef *tim, uint8_t channel){

	dhtSemaphore=xSemaphoreCreateBinary();

	tim_ic_config_t tim_config={
			.pcs=pcs_set(tim),
			.capture_operation=IC_POLARITY_RISING,
			.input_capture_filter=0,
			.input_capture_pcs=PCS_NO,
			.arr=0xffff
	};

	TIMx_IC_init(tim, &tim_config, channel);
	TIMx_stop(tim);

	en_nvic(tim);
	set_int(tim, channel);
}


void DHT11_read(GPIO_TypeDef *port, uint8_t pin, TIM_TypeDef *tim, uint8_t af_number, uint8_t *buffer, uint16_t *b){
	gpio_config_t gpio;
	bi=b;
	gpio.mode=GPIOx_MODE_OUTPUT;
	gpio.otype=GPIOx_OTYPE_OPEN_DRAIN;
	gpio.pin=pin;
	gpio.pupdr=GPIOx_PUPDR_DISABLE;
	gpio.speed=GPIOx_SPEED_FAST_SPEED;

	global_pin=pin ,global_port=port;

	last_time=0,current_time=0,index=0;
	global_buffer=buffer;
	edgecounter=0;

	GPIO_init(port, &gpio);

	GPIO_set_level(port, pin, 0);

	vTaskDelay(pdMS_TO_TICKS(18));

	GPIO_set_level(port, pin, 1);
	gpio.mode=GPIOx_MODE_ALTERNATE;

	GPIO_init(port, &gpio);

	if(pin<8){
		port->AFR[0]&=~(0b1111<<(pin*4));
		port->AFR[0]|=(af_number<<(pin*4));
	}
	else{
		port->AFR[1]&=~(0b1111<<((pin*4)-32));
		port->AFR[1]|=(af_number<<((pin*4)-32));
	}

	tim->CNT=0;
	TIMx_start(tim);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// END FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION HANDLER
///////////////////////////////////////////////////////////////////////////////////////////////////
volatile uint16_t duration=0;

void TIM2_IRQHandler(void){
	TIM2->SR=0;
	edgecounter++;

	BaseType_t xTaskWoken=pdFALSE;

	if(edgecounter <= 1) return;


	current_time=TIM2->CCR4;
	duration=current_time-last_time;
	last_time=current_time;
	bi[index]=duration;


	if(duration>120){
		global_buffer[index/8]<<=1;
		global_buffer[index/8]|=1;
		index++;
	}
	else{
		global_buffer[index/8]<<=1;
		global_buffer[index/8]|=0;
		index++;
	}

	if(index==40){
		TIMx_stop(TIM2);
		xSemaphoreGiveFromISR(dhtSemaphore, &xTaskWoken);
		portYIELD_FROM_ISR(xTaskWoken);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END  FUNCTION HANDLER
///////////////////////////////////////////////////////////////////////////////////////////////////
