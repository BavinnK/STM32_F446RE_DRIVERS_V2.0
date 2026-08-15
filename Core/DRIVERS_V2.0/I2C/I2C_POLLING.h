#ifndef I2C_POLLING_H
#define I2C_POLLING_H

#include "stdint.h"
#include "stm32f446xx.h"
#include "GPIO.h"

typedef struct{
	uint32_t speed;

}i2c__polling_config_t;

void I2Cx_Polling_init(I2C_TypeDef *i2c, i2c__polling_config_t *config);
void I2Cx_Polling_start(I2C_TypeDef *i2c);
void I2Cx_Polling_write(I2C_TypeDef* i2c,uint8_t slave_addr, uint8_t register_addr, uint8_t *buffer, uint16_t length);
void I2Cx_Polling_read(I2C_TypeDef *i2c,uint16_t slave_addr, uint16_t register_addr, uint8_t *buffer, uint8_t length);
void I2Cx_Polling_stop(I2C_TypeDef *i2c);

#endif
