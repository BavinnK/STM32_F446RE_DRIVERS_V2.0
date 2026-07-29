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
#include "main.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CLOCK.h"
#include "GPIO.h"
#include "DELAY.h"
#include "UART.h"
//#include "ADC.h"
#include "DMA.h"
#include "ADC_DMA.h"
#include "TIM.h"
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
int main(void)
{
	system_clk_180mhz();

	gpio_config_t config={
		.mode=GPIOx_MODE_OUTPUT,
		.otype=GPIOx_OTYPE_PUSH_PULL,
		.pin=5,
		.pupdr=GPIOx_PUPDR_DISABLE,
		.speed=GPIOx_SPEED_MED_SPEED
	};



	GPIO_init(GPIOA, &config);

	systick_init();
	char buff[100];
	uint16_t data=0;
	dma_config_t dma_setup={
		.stream=DMA2_Stream0,
		.channel=0,
		.m_burst=SINGLE,
		.p_burst=SINGLE,
		.priority=HIGH,
		.m_data_size=HALF_WORD,
		.p_data_size=HALF_WORD,
		.data_transfer_dir=PER_TO_MEM,
		.length=1,
		.per_addr=&ADC1->DR,
		.mem_addr=&data,
		.mode=CIRCULAR
	};

	ADCx_DMA_init(ADC1, GPIOA, 0, 0, &dma_setup,DMA2);
	ADCx_DMA_chn_config(ADC1, 0, 1);
	ADCx_DMA_sequence_length(ADC1, 1);
	ADCx_DMA_start(ADC1);

	tim_config_t config_TIM={
			.TIM_PCS=60000,
			.TIM_DIR=UP_COUNTER,
			.TIM_ARR=600
	};

	TIMx_init(TIM2, &config_TIM);
	TIMx_start(TIM2);

	UART2_init(115200);

	while (1)
	{
		/* USER CODE END WHILE */
		uint16_t cnt=TIMx_get_counter(TIM2);
		sprintf(buff,"TIM COUNTER: %d\n\r",cnt);
		UART2_write_string(buff);
		delay_ms(10);




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
