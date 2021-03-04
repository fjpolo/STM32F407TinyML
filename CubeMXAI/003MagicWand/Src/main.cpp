/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

using namespace ei;

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
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef huart6;

// paste the raw features here
static float features[] = {-0.2000, 0.2000, 10.4000, 0.0000, 0.2000, 10.1000, 0.3000, 0.4000, 9.9000, 0.4000, 0.4000, 9.7000, 0.1000, 0.4000, 9.7000, -0.2000, 0.4000, 9.8000, 0.0000, 0.5000, 9.7000, 0.1000, 0.4000, 9.8000, 0.1000, 0.4000, 9.9000, -0.1000, 0.4000, 9.9000, 0.2000, 0.3000, 9.8000, 0.2000, 0.3000, 9.8000, 0.0000, 0.2000, 9.8000, -0.1000, 0.3000, 9.8000, -0.1000, 0.3000, 10.1000, 0.2000, 0.4000, 9.8000, 0.6000, 0.4000, 9.6000, 0.9000, 0.5000, 9.4000, 0.9000, 0.6000, 9.5000, 0.5000, 0.5000, 9.9000, 0.5000, 0.5000, 9.7000, 0.3000, 0.7000, 9.4000, -0.3000, 0.4000, 9.2000, -1.4000, 0.6000, 9.6000, -2.5000, 0.3000, 10.1000, -3.4000, 0.5000, 10.0000, -3.7000, 0.0000, 10.1000, -4.1000, -0.2000, 10.6000, -5.4000, -1.0000, 12.2000, -5.8000, -1.0000, 12.0000, -5.2000, -0.9000, 11.8000, -3.9000, -1.2000, 11.1000, -2.8000, -1.1000, 10.8000, -0.9000, -1.0000, 10.3000, 0.4000, -0.8000, 9.7000, 1.8000, -0.5000, 9.7000, 1.8000, -0.5000, 9.7000, 3.3000, -0.3000, 9.2000, 4.2000, 0.4000, 8.7000, 4.5000, 0.5000, 8.4000, 4.6000, 0.3000, 9.0000, 5.4000, 0.4000, 8.8000, 6.5000, 0.1000, 8.7000, 7.8000, -0.1000, 8.4000, 8.2000, 0.1000, 8.4000, 7.7000, 0.6000, 8.2000, 6.2000, 0.5000, 8.0000, 4.6000, 0.6000, 8.8000, 4.2000, 0.6000, 9.8000, 3.8000, 0.1000, 10.4000, 3.1000, -0.2000, 10.1000, 2.1000, -0.5000, 9.1000, 0.3000, -0.7000, 8.2000, -1.7000, -0.7000, 9.0000, -2.9000, -0.9000, 10.6000, -3.9000, -0.9000, 11.6000, -5.6000, -0.2000, 10.3000, -7.3000, -0.1000, 9.9000, -8.4000, -0.5000, 11.1000, -9.3000, -0.9000, 12.9000, -9.2000, -1.6000, 13.4000, -9.2000, -1.6000, 13.4000, -9.1000, -2.1000, 13.2000, -8.6000, -2.5000, 12.5000, -8.8000, -2.8000, 11.8000, -8.7000, -2.8000, 11.3000, -5.9000, -2.5000, 11.3000, -3.4000, -2.2000, 10.8000, -0.7000, -1.9000, 9.9000, 1.0000, -1.6000, 9.5000, 2.2000, -0.8000, 9.1000, 3.3000, -0.3000, 8.6000, 4.7000, 0.0000, 8.5000, 6.2000, 0.3000, 8.1000, 7.4000, 0.5000, 8.2000, 9.3000, 0.9000, 8.9000, 8.6000, 1.1000, 8.5000, 8.4000, 1.1000, 8.3000, 8.4000, 1.1000, 8.3000, 9.7000, 1.3000, 7.6000, 9.1000, 1.3000, 7.7000, 7.8000, 1.0000, 7.7000, 7.1000, 0.9000, 7.8000, 6.6000, 0.7000, 8.6000, 5.3000, -0.2000, 10.6000, 4.6000, -0.8000, 10.0000, 4.6000, -0.8000, 10.0000, 3.0000, -1.2000, 8.8000, 1.0000, -1.5000, 8.8000, -0.7000, -1.6000, 9.5000, -2.0000, -1.9000, 10.0000, -3.1000, -2.2000, 9.9000, -5.2000, -1.7000, 10.6000, -7.7000, -1.8000, 11.6000, -10.8000, -1.7000, 11.3000, -10.3000, -1.9000, 11.5000, -9.4000, -2.3000, 12.7000, -8.6000, -2.7000, 13.8000, -8.6000, -2.9000, 14.2000, -8.1000, -3.2000, 13.5000, -7.3000, -2.9000, 11.8000, -6.4000, -2.5000, 10.7000, -6.1000, -2.1000, 9.1000, -4.6000, -2.0000, 9.5000, -3.4000, -1.9000, 9.4000, -0.7000, -1.9000, 9.3000, 1.8000, -1.9000, 9.4000, 3.0000, -1.4000, 10.0000, 3.6000, -0.8000, 9.3000, 5.0000, -0.2000, 9.1000, 7.7000, 0.1000, 9.4000, 7.7000, 0.1000, 9.4000, 10.3000, 0.6000, 9.4000, 10.4000, 1.5000, 9.8000, 9.4000, 1.4000, 9.0000, 8.7000, 1.4000, 8.8000, 8.3000, 1.4000, 8.1000, 8.8000, 1.1000, 7.7000, 7.9000, 1.2000, 7.7000, 7.0000, 0.7000, 7.6000, 6.4000, 0.4000, 8.0000, 5.2000, 0.1000, 8.2000, 1.4000, 0.0000, 7.8000, -0.1000, -0.6000, 8.8000, -1.2000, -1.1000, 9.3000};



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void reverse(char* str, int len);
static int intToStr(int16_t x, char str[]);
static void vprint(const char *fmt, va_list argp){
    char string[200];
    if(0 < vsprintf(string, fmt, argp)){ // build string
        HAL_UART_Transmit(&huart6, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    }
}

static void ei_printf(const char *format, ...){
    va_list myargs;
    va_start(myargs, format);
    vprint(format, myargs);
    va_end(myargs);
}

static int get_feature_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();
  MX_TIM2_Init();
  MX_CRC_Init();
  MX_TIM14_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */
	
	/**/
	signal_t signal;
	signal.total_length = sizeof(features) / sizeof(features[0]);
//	signal.get_data = &get_feature_data;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		ei_impulse_result_t result;
    EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);
    ei_printf("run_classifier returned: %d\n", res);

    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);

    // print the predictions
    ei_printf("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("%.5f", result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf(", ");
#else
        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            ei_printf(", ");
        }
#endif
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("%.3f", result.anomaly);
#endif
    ei_printf("]\n\n\n");

		/*Delay*/
    HAL_Delay(5000);
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
