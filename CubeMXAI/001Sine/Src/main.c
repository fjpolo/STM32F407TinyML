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
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "sine_model.h"
#include "sine_model_data.h"
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  /* USER CODE BEGIN 1 */
	char buf[50];
	uint32_t buf_len;
	ai_error ai_error;
	ai_i32 n_batch;
	uint32_t timestamp;
	float y_val;
	const uint8_t x_val_count = 100;
	uint8_t x_val_it = 0;
	const float x_val[] = {
													 0.        , 0.06283185, 0.12566371, 0.18849556, 0.25132741,
													 0.31415927, 0.37699112, 0.43982297, 0.50265482, 0.56548668,
													 0.62831853, 0.69115038, 0.75398224, 0.81681409, 0.87964594,
													 0.9424778 , 1.00530965, 1.0681415 , 1.13097336, 1.19380521,
													 1.25663706, 1.31946891, 1.38230077, 1.44513262, 1.50796447,
													 1.57079633, 1.63362818, 1.69646003, 1.75929189, 1.82212374,
													 1.88495559, 1.94778745, 2.0106193 , 2.07345115, 2.136283  ,
													 2.19911486, 2.26194671, 2.32477856, 2.38761042, 2.45044227,
													 2.51327412, 2.57610598, 2.63893783, 2.70176968, 2.76460154,
													 2.82743339, 2.89026524, 2.95309709, 3.01592895, 3.0787608 ,
													 3.14159265, 3.20442451, 3.26725636, 3.33008821, 3.39292007,
													 3.45575192, 3.51858377, 3.58141563, 3.64424748, 3.70707933,
													 3.76991118, 3.83274304, 3.89557489, 3.95840674, 4.0212386 ,
													 4.08407045, 4.1469023 , 4.20973416, 4.27256601, 4.33539786,
													 4.39822972, 4.46106157, 4.52389342, 4.58672527, 4.64955713,
													 4.71238898, 4.77522083, 4.83805269, 4.90088454, 4.96371639,
													 5.02654825, 5.0893801 , 5.15221195, 5.2150438 , 5.27787566,
													 5.34070751, 5.40353936, 5.46637122, 5.52920307, 5.59203492,
													 5.65486678, 5.71769863, 5.78053048, 5.84336234, 5.90619419,
													 5.96902604, 6.03185789, 6.09468975, 6.1575216 , 6.22035345
												};
	
	/*Chunk of memory used to hold intermediate values for neural network*/
	AI_ALIGNED(4) ai_u8 activations[AI_SINE_MODEL_DATA_ACTIVATIONS_SIZE];
	
	/*Buffers used to store input and output tensors*/
	AI_ALIGNED(4) ai_i8 in_data[AI_SINE_MODEL_IN_1_SIZE_BYTES];
	AI_ALIGNED(4) ai_i8 out_data[AI_SINE_MODEL_OUT_1_SIZE_BYTES];
	
	/*Pointer to model*/
	ai_handle sine_model = AI_HANDLE_NULL;
	
	/**
	 *  Initialize wrapper structs that hold pointers to data 
	 * and info about the data ( tensor height, width, channels)
	 */
	 ai_buffer ai_input[AI_SINE_MODEL_IN_NUM] = AI_SINE_MODEL_IN;
	 ai_buffer ai_output[AI_SINE_MODEL_OUT_NUM] = AI_SINE_MODEL_OUT;
	 
	 /*Set working memory and get weights/biases from model*/
	 ai_network_params ai_params = {
			AI_SINE_MODEL_DATA_WEIGHTS(ai_sine_model_data_weights_get()),
		  AI_SINE_MODEL_DATA_ACTIVATIONS(activations)
	 };
	 
	 /*Set pointer wrappers structs to our data buffers*/
	 ai_input[0].n_batches = 1;
	 ai_input[0].data = AI_HANDLE_PTR(in_data);
	 ai_output[0].n_batches = 1;
	 ai_output[0].data = AI_HANDLE_PTR(out_data);
	 
	 
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
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */
	
	/*Inint Timer14*/
	HAL_TIM_Base_Start(&htim14);
	
	/*Say hi!*/
	buf_len = sprintf(buf, "\r\n\r\nSTM32F407 Deep Learning test!\r\n");
	HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
	
	/*Create instance of NN*/
	ai_error = ai_sine_model_create(&sine_model, AI_SINE_MODEL_DATA_CONFIG);
	if(ai_error.type!= AI_ERROR_NONE){
		buf_len = sprintf(buf, "ERROR: Could not create NN instance!\r\n");
		HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
	}
	
	/*Initialise NN*/
	if(!ai_sine_model_init(sine_model, &ai_params)){
		buf_len = sprintf(buf, "ERROR: Could not initialise NN!\r\n");
		HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		/*Fill input buffer*/
		for(uint32_t i=0;i<AI_SINE_MODEL_IN_1_SIZE;i++){
//			((ai_float *)in_data)[i] = (ai_float)2.355f;
			((ai_float *)in_data)[i] = (ai_float)x_val[x_val_it];
			x_val_it++;
			x_val_it %= x_val_count;
			
		}
		
		/*Get current timestamp*/
		timestamp = htim14.Instance->CNT;
		
		/*Inference*/
		n_batch = ai_sine_model_run(sine_model, &ai_input[0], &ai_output[0]);
		if(n_batch != 1){
			buf_len = sprintf(buf, "ERROR: Could not run inference!\r\n");
			HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
		}
		
		/*Read output*/
		y_val = ((float *)out_data)[0];
		
		/*Print output and inference time [uS]*/
		buf_len = sprintf(buf,
		"In: %.2f | Out: %f | %lu[uS]\r\n",
											x_val[x_val_it-1],
											y_val,
											(unsigned long)(htim14.Instance->CNT - timestamp)
										 );
		HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
		
		/*Delay*/
		HAL_Delay(500);
		
    /* USER CODE END WHILE */

  MX_X_CUBE_AI_Process();
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
