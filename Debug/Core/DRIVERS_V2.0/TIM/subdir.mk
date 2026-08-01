################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/DRIVERS_V2.0/TIM/TIM.c \
../Core/DRIVERS_V2.0/TIM/TIM_IC.c \
../Core/DRIVERS_V2.0/TIM/TIM_OC.c 

OBJS += \
./Core/DRIVERS_V2.0/TIM/TIM.o \
./Core/DRIVERS_V2.0/TIM/TIM_IC.o \
./Core/DRIVERS_V2.0/TIM/TIM_OC.o 

C_DEPS += \
./Core/DRIVERS_V2.0/TIM/TIM.d \
./Core/DRIVERS_V2.0/TIM/TIM_IC.d \
./Core/DRIVERS_V2.0/TIM/TIM_OC.d 


# Each subdirectory must supply rules for building sources it contributes
Core/DRIVERS_V2.0/TIM/%.o Core/DRIVERS_V2.0/TIM/%.su Core/DRIVERS_V2.0/TIM/%.cyclo: ../Core/DRIVERS_V2.0/TIM/%.c Core/DRIVERS_V2.0/TIM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/TIM" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/ADC" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/DMA" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/UART" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/DELAY" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/GPIO" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0" -I"C:/Users/pc/Desktop/STM_PROJECTS/BAREMETAL_DRIVERS_V2.0/Core/DRIVERS_V2.0/CLOCK" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-DRIVERS_V2-2e-0-2f-TIM

clean-Core-2f-DRIVERS_V2-2e-0-2f-TIM:
	-$(RM) ./Core/DRIVERS_V2.0/TIM/TIM.cyclo ./Core/DRIVERS_V2.0/TIM/TIM.d ./Core/DRIVERS_V2.0/TIM/TIM.o ./Core/DRIVERS_V2.0/TIM/TIM.su ./Core/DRIVERS_V2.0/TIM/TIM_IC.cyclo ./Core/DRIVERS_V2.0/TIM/TIM_IC.d ./Core/DRIVERS_V2.0/TIM/TIM_IC.o ./Core/DRIVERS_V2.0/TIM/TIM_IC.su ./Core/DRIVERS_V2.0/TIM/TIM_OC.cyclo ./Core/DRIVERS_V2.0/TIM/TIM_OC.d ./Core/DRIVERS_V2.0/TIM/TIM_OC.o ./Core/DRIVERS_V2.0/TIM/TIM_OC.su

.PHONY: clean-Core-2f-DRIVERS_V2-2e-0-2f-TIM

