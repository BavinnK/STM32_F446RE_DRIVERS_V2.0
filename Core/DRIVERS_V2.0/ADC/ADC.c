#include "ADC.h"

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
		adc_port->SMPR1&=~((0b111<<(chn*3)));
		adc_port->SMPR1|=(0b110<<(chn*3));
	}
}

void ADCx_init(ADC_TypeDef *adc,GPIO_TypeDef *port,uint8_t pin,uint8_t chn){
	gpio_config_t gpio_setup={
		.pin=pin,
		.mode=GPIOx_MODE_ANALOG,
		.pupdr=GPIOx_PUPDR_DISABLE
	};

	GPIO_init(port, &gpio_setup);
	adc_on(adc);

	adc->CR2|=(1<<0);				//EN ADC

	channel_adc(adc, chn);
}

void ADCx_chn_config(ADC_TypeDef *adc,uint8_t channel,uint8_t rank){

}
