#include "ADC.h"

static inline void adc_on(ADC_TypeDef *adc){
	if(adc==ADC1) RCC->APB2ENR|=(1<<8);
	else if(adc==ADC2) RCC->APB2ENR|=(1<<9);
	else if(adc==ADC3) RCC->APB2ENR|=(1<<10);
}

void ADCx_init(ADC_TypeDef *adc,GPIO_TypeDef *port,uint8_t pin){
	adc_on(adc);
	gpio_config_t gpio_setup={
		.pin=pin,
		.mode=GPIOx_MODE_ANALOG,
		.pupdr=GPIOx_PUPDR_DISABLE
	};

	GPIO_init(port, &gpio_setup);
}

void ADCx_chn_config(ADC_TypeDef *adc,uint8_t channel,uint8_t rank){

}
