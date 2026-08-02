#include "ADC.h"
#include <stddef.h>

const static ADC_TypeDef *adc_int;

static void (*ADC1_callback) (uint16_t)=NULL;
static void (*ADC2_callback) (uint16_t)=NULL;
static void (*ADC3_callback) (uint16_t)=NULL;
///////////////////////////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION
///////////////////////////////////////////////////////////////////////////////////////////////////

static inline void adc_on(ADC_TypeDef *adc){
	if(adc==ADC1) RCC->APB2ENR|=(1<<8);
	else if(adc==ADC2) RCC->APB2ENR|=(1<<9);
	else if(adc==ADC3) RCC->APB2ENR|=(1<<10);
}

static inline void channel_adc(ADC_TypeDef *adc_port,uint8_t chn,uint8_t sample){
	if(chn>=0&&chn<=9){
		adc_port->SMPR2&=~((0b111<<(chn*3)));
		adc_port->SMPR2|=(sample<<(chn*3));

	}
	else if(chn>=10&&chn<=15){
		adc_port->SMPR1&=~((0b111<<((chn-10)*3)));
		adc_port->SMPR1|=(sample<<((chn-10)*3));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// END INLINE FUNCTION
///////////////////////////////////////////////////////////////////////////////////////////////////

void ADCx_init(ADC_TypeDef *adc, GPIO_TypeDef *port, uint8_t pin, uint8_t adc_channel, adc_config_t *config){
	gpio_config_t gpio_setup={
		.pin=pin,
		.mode=GPIOx_MODE_ANALOG,
		.pupdr=GPIOx_PUPDR_DISABLE
	};

	GPIO_init(port, &gpio_setup);
	adc_on(adc);

	adc->CR1&=~(3<<24);
	adc->CR1|=(config->resolution<<24);

	if(config->mode == ADC_DMA){
		adc->CR2|=(1<<8)|(1<<9)|(1<<1);				//EN ADC
		adc->CR2|=(1<<0)|(1<<30);
	}
	else if(config->mode == ADC_INTERRUPT){
		adc->CR1|=(1<<5);
		NVIC_EnableIRQ(ADC_IRQn);
		adc_int=adc;
	}

	channel_adc(adc, adc_channel, config->sample);
	adc->CR2|=(1<<0);				//EN ADC
}

void ADCx_chn_config(ADC_TypeDef *adc,uint8_t channel,uint8_t rank){
	uint16_t shift;
	if(rank>0 && rank<=6){
		shift=(rank-1)*5;
		adc->SQR3&=~(0b11111<<shift);
		adc->SQR3|=(channel<<shift);
	}
	else if(rank>6 && rank<13){
		shift=(rank-7)*5;
		adc->SQR2&=~(0b11111<<shift);
		adc->SQR2|=(channel<<shift);
	}
	else if(rank>=13 && rank<=16){
		shift=(rank-13)*5;
		adc->SQR1&=~(0b11111<<shift);
		adc->SQR1|=(channel<<shift);
	}
}

uint16_t ADCx_read(ADC_TypeDef *adc){ //used when user picked POLLING
	adc->CR2&=~(1<<30);
	adc->CR2|=(1<<30);
	while(!(adc->SR&(1<<1)));
	return adc->DR;
}

void ADCx_sequence_length(ADC_TypeDef *adc,uint8_t len){
	adc->SQR1&=~(0b1111<<20);
	adc->SQR1|=((len-1)<<20);
}


void ADCx_start(ADC_TypeDef *adc){
	adc->CR2|=(1<<0)|(1<<30);
}


void ADC_IRQHandler(void){
	if(ADC1->SR & (1<<1)){
		uint16_t adc1_val=ADC1->DR;
		if(ADC1_callback != NULL) ADC1_callback(adc1_val);
	}

	if(ADC2->SR & (1<<1)){
		uint16_t adc2_val=ADC2->DR;
		if(ADC2_callback != NULL) ADC2_callback(adc2_val);
	}

	if(ADC3->SR & (1<<1)){
		uint16_t adc3_val=ADC3->DR;
		if(ADC3_callback != NULL) ADC3_callback(adc3_val);
	}

}

