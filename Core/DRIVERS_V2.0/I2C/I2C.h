#ifndef I2C_H
#define IC2_H

#include "stdint.h"
#include "stm32f446xx.h"

typedef enum{
	I2C_POLLING=0,
	I2C_INTERRUPT,
	I2C_DMA
}i2c_mode_t;

typedef struct{
	uint32_t speed;
	i2c_mode_t mode;
}i2c_config_t;

void I2Cx_init();
void I2Cx_start();
void I2Cx_write();
void I2Cx_read();
void I2Cx_stop();

#endif
