#ifndef I2C_INTERRUPT_H
#define I2C_INTERRUPT_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "GPIO.h"

typedef struct{
	uint32_t speed;
	I2C_TypeDef *i2c;

}i2c_interrupt_config_t;

void I2Cx_Interrupt_init(i2c_interrupt_config_t *config);



#endif
