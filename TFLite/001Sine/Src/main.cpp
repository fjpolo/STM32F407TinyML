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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/*TFLite*/
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
//#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/version.h"
/*Models*/
#include "sine_model.h"

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

/*TFLite globals*/
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* model_input = nullptr;
  TfLiteTensor* model_output = nullptr;

  // Create an area of memory to use for input, output, and other TensorFlow
  // arrays. You'll need to adjust this by compiling, running, and looking
  // for errors.
  constexpr int kTensorArenaSize = 2 * 1024;
  __attribute__((aligned(16)))uint8_t tensor_arena[kTensorArenaSize];
} // END namespace

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/*Custom implementation of DebugLog from TensorFlow*/
extern "C" void DebugLog(const char* s){
  HAL_UART_Transmit(&huart6, (uint8_t *)s, strlen(s), 100);
}

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
	TfLiteStatus tflite_status;
  uint32_t num_elements;
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
  /* USER CODE BEGIN 2 */

	/*Inint Timer14*/
	HAL_TIM_Base_Start(&htim14);
	
	/*Say hi!*/
	buf_len = sprintf(buf, "\r\n\r\nSTM32F407 TFLiteuC Deep Learning test!\r\n");
	HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
	
	/*Set up logging (modify tensorflow/lite/micro/debug_log.cc)*/
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;
	
	/*Say something to test error reporter*/
  error_reporter->Report("\r\nSTM32F407 TensorFlow Lite test\r\n");
	
	// Map the model into a usable data structure
  model = tflite::GetModel(sine_model);
  if (model->version() != TFLITE_SCHEMA_VERSION)
  {
    error_reporter->Report("Model version does not match Schema");
    while(1);
  }

  // Pull in only needed operations (should match NN layers). Template parameter
  // <n> is number of ops to be added. Available ops:
  // tensorflow/lite/micro/kernels/micro_ops.h
//  static tflite::MicroMutableOpResolver<1> micro_op_resolver;
//  static tflite::MicroMutableOpResolver micro_op_resolver;

//  // Add dense neural network layer operation
//  tflite_status = micro_op_resolver.AddBuiltin(
//      tflite::BuiltinOperator_FULLY_CONNECTED,
//      tflite::ops::micro::Register_FULLY_CONNECTED());
//  if (tflite_status != kTfLiteOk)
//  {
//    error_reporter->Report("Could not add FULLY CONNECTED op");
//    while(1);
//  }	
	
	// This pulls in all the operation implementations we need.
  	static tflite::ops::micro::AllOpsResolver resolver;
	
//  // Build an interpreter to run the model with.
//  static tflite::MicroInterpreter static_interpreter(
//      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
//  interpreter = &static_interpreter;  // Build an interpreter to run the model with.
//  
	static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  tflite_status = interpreter->AllocateTensors();
  if (tflite_status != kTfLiteOk)
  {
    error_reporter->Report("AllocateTensors() failed");
    while(1);
  }

  // Assign model input and output buffers (tensors) to pointers
  model_input = interpreter->input(0);
  model_output = interpreter->output(0);

  // Get number of elements in input tensor
  num_elements = model_input->bytes / sizeof(float);
  buf_len = sprintf(buf, "Number of input elements: %lu\r\n", (unsigned long)num_elements);
  HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		// Fill input buffer (use test value)
    for (uint32_t i = 0; i < num_elements; i++  )
    {
      model_input->data.f[i] = x_val[x_val_it];
			x_val_it++;
			x_val_it %= x_val_count;
    }

    // Get current timestamp
    timestamp = htim14.Instance->CNT;

    // Run inference
    tflite_status = interpreter->Invoke();
    if (tflite_status != kTfLiteOk)
    {
      error_reporter->Report("Invoke failed");
    }

    // Read output (predicted y) of neural network
    y_val = model_output->data.f[0];

    // Print output of neural network along with inference time (microseconds)
    buf_len = sprintf(buf,
											"In: %f | Out: %f | %lu[uS]\r\n",
                      x_val[x_val_it-1],
											y_val,
                      (unsigned long)(htim14.Instance->CNT - timestamp));
    HAL_UART_Transmit(&huart6, (uint8_t *)buf, buf_len, 100);

    // Wait before doing it again
    HAL_Delay(500);
		
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
