#ifndef ADC_DMA_H
#define ADC_DMA_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "DMA.h"
#include "GPIO.h"

void ADCx_DMA_init(ADC_TypeDef *adc,GPIO_TypeDef *port,uint8_t pin,uint8_t chn,dma_config_t *dma);
void ADCx_DMA_chn_config(ADC_TypeDef *adc,uint8_t channel,uint8_t rank);
void ADCx_DMA_sequence_length(ADC_TypeDef *adc,uint8_t len);


#endif
