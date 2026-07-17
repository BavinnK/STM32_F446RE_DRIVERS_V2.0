#ifndef DELAY_H
#define DELAY_H

#include "stdint.h"
#include "stm32f446xx.h"

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void systick_init(void);

#endif
