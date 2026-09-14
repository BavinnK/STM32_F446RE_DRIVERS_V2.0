/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
//#include "I2C_POLLING.h"
#include "main.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CLOCK.h"
#include "GPIO.h"
#include "DELAY.h"
#include "UART.h"
#include "ADC.h"
#include "DMA.h"
//#include "SPI/SPI_POLLING.h"
#include "TIM.h"
#include "TIM_OC.h"
#include "EXTI.h"
#include "I2C_INTERRUPT.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

uint8_t buffer=0;
int main(void)
{
	__enable_irq();
	system_clk_180mhz();

	UART2_init(115200);

	systick_init();

	 i2c_interrupt_config_t i2c_con={
			 .i2c=I2C1,
			 .speed=100000
	 };
	 I2Cx_Interrupt_init(&i2c_con);
	//I2Cx_Polling_init(I2C1, &i2c_poll);
	 //uint8_t bufff=0x00;
	 //I2Cx_Interrupt_write(I2C1, 0x68, 0x6B, &bufff,1);
	 delay_ms(1000);


	char buff[200];
//uint8_t guz=I2Cx_Interrupt_Read(I2C1, 0x68, 0x75, &buffer, 1);
	while (1)
	{

		//I2Cx_Polling_read(I2C1, 0x68, 0x75, &buffer, 1);
		uint8_t calib[22];

		if (I2Cx_Interrupt_Read(I2C1, 0x77, 0xAA, calib, 22) == 1)
		{
			int16_t AC1 = (int16_t)(((uint16_t)calib[0] << 8) | calib[1]);
			int16_t AC2 = (int16_t)(((uint16_t)calib[2] << 8) | calib[3]);
			int16_t AC3 = (int16_t)(((uint16_t)calib[4] << 8) | calib[5]);

			uint16_t AC4 = ( calib[6] << 8) | calib[7];
			uint16_t AC5 = ( calib[8] << 8) | calib[9];
			uint16_t AC6 = ( calib[10] << 8) | calib[11];

			int16_t B1 = (int16_t)(((uint16_t)calib[12] << 8) | calib[13]);
			int16_t B2 = (int16_t)(((uint16_t)calib[14] << 8) | calib[15]);
			int16_t MB = (int16_t)(((uint16_t)calib[16] << 8) | calib[17]);
			int16_t MC = (int16_t)(((uint16_t)calib[18] << 8) | calib[19]);
			int16_t MD = (int16_t)(((uint16_t)calib[20] << 8) | calib[21]);
		    sprintf(buff, "AC1: %d, AC2:%d, AC3:%d, AC4:%d, AC5:%d, AC6:%d \n\r", AC1, AC2, AC3, AC4, AC5, AC6);
		    UART2_write_string(buff);



		}
		delay_ms(400);

		//delay_ms(1000);


		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
