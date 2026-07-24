#include "ADC_DMA.h"

const uint8_t seq_len=0;
static inline void adc_on(ADC_TypeDef *adc){
	if(adc==ADC1) RCC->APB2ENR|=(1<<8);
	else if(adc==ADC2) RCC->APB2ENR|=(1<<9);
	else if(adc==ADC3) RCC->APB2ENR|=(1<<10);
}

static inline void channel_adc(ADC_TypeDef *adc_port,uint8_t chn){
	if(chn>=0&&chn<=9){
		adc_port->SMPR2&=~((0b111<<(chn*3)));
		adc_port->SMPR2|=(0b110<<(chn*3));				//sample time is hardcoded to 144

	}
	else if(chn>=10&&chn<=15){
		adc_port->SMPR1&=~((0b111<<((chn-10)*3)));
		adc_port->SMPR1|=(0b110<<((chn-10)*3));
	}
}

void ADCx_DMA_init(ADC_TypeDef *adc,GPIO_TypeDef *port,uint8_t pin,uint8_t chn,dma_config_t *dma){
	gpio_config_t gpio_setup={
		.pin=pin,
		.mode=GPIOx_MODE_ANALOG,
		.pupdr=GPIOx_PUPDR_DISABLE
	};

	GPIO_init(port, &gpio_setup);
	adc_on(adc);

	adc->CR2|=(1<<8)|(1<<9)|(1<<1);				//EN ADC


	channel_adc(adc, chn);

	adc->CR2|=(1<<0);
}

void ADCx_DMA_chn_config(ADC_TypeDef *adc,uint8_t channel,uint8_t rank){
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

void ADCx_DMA_sequence_length(ADC_TypeDef *adc,uint8_t len){
	adc->SQR1&=~(0b1111<<20);
	adc->SQR1|=(len<<20);
}
