#ifndef ADC_H
#define ADC_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "GPIO.h"

void ADCx_init(ADC_TypeDef *adc,GPIO_TypeDef *port,uint8_t pin,uint8_t chn);
void ADCx_chn_config(ADC_TypeDef *adc,uint8_t channel,uint8_t rank);
uint16_t ADCx_read(ADC_TypeDef *adc);


#endif
