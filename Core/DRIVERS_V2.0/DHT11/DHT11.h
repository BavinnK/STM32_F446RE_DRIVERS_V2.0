#ifndef DHT11_H
#define DHT11_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "TIM.h"
#include "GPIO.h"
#include "TIM_IC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "delay.h"

extern SemaphoreHandle_t dhtSemaphore;

void DHT11_init(TIM_TypeDef *tim, uint8_t channel);
void DHT11_read(GPIO_TypeDef *port, uint8_t pin, TIM_TypeDef *tim, uint8_t af_number, uint8_t *buffer, uint16_t *b);



#endif
