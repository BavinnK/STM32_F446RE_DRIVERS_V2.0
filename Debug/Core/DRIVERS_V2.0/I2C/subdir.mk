################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.c \
../Core/DRIVERS_V2.0/I2C/I2C_POLLING.c 

OBJS += \
./Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.o \
./Core/DRIVERS_V2.0/I2C/I2C_POLLING.o 

C_DEPS += \
./Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.d \
./Core/DRIVERS_V2.0/I2C/I2C_POLLING.d 


# Each subdirectory must supply rules for building sources it contributes
Core/DRIVERS_V2.0/I2C/%.o Core/DRIVERS_V2.0/I2C/%.su Core/DRIVERS_V2.0/I2C/%.cyclo: ../Core/DRIVERS_V2.0/I2C/%.c Core/DRIVERS_V2.0/I2C/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/I2C" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/EXTI" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/TIM" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/ADC" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/DMA" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/UART" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/DELAY" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/GPIO" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/CLOCK" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-DRIVERS_V2-2e-0-2f-I2C

clean-Core-2f-DRIVERS_V2-2e-0-2f-I2C:
	-$(RM) ./Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.cyclo ./Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.d ./Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.o ./Core/DRIVERS_V2.0/I2C/I2C_INTERRUPT.su ./Core/DRIVERS_V2.0/I2C/I2C_POLLING.cyclo ./Core/DRIVERS_V2.0/I2C/I2C_POLLING.d ./Core/DRIVERS_V2.0/I2C/I2C_POLLING.o ./Core/DRIVERS_V2.0/I2C/I2C_POLLING.su

.PHONY: clean-Core-2f-DRIVERS_V2-2e-0-2f-I2C

