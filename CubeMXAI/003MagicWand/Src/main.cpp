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
#include "bno055_stm32.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#ifdef STM32F407_AI_EventRecorder
#include "EventRecorder.h"						// https://www.keil.com/pack/doc/compiler/EventRecorder/html/er_use.html
#endif // STM32F407_AI_EventRecorder

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
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim14;
bool TimerReadBNO;

/*namespace ei*/
using namespace ei;
// paste the raw features here
float features[EI_CLASSIFIER_RAW_SAMPLE_COUNT*EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME] ={0};
uint32_t BufferIndex = 0;
//////static float features[] = {
//////														0.3000, -0.1000, 9.9000, 0.2000, -0.2000, 9.8000, 0.1000, -0.1000, 9.9000, 0.3000, 
//////														0.0000, 9.8000, 0.3000, 0.0000, 9.8000, 0.5000, -0.1000, 9.8000, 0.5000, -0.1000,
//////														9.9000, 0.3000, -0.2000, 9.9000, 0.3000, -0.2000, 9.9000, -0.2000, -0.1000, 9.8000, 
//////														-0.2000, -0.1000, 9.7000, 0.2000, 0.0000, 9.9000, 0.4000, -0.2000, 9.8000, 0.3000, 
//////														-0.2000, 9.9000, 0.5000, -0.1000, 9.8000, 0.2000, -0.1000, 9.9000, 0.1000, -0.1000, 
//////														10.1000, 0.3000, -0.1000, 9.9000, 0.3000, 0.0000, 9.8000, 0.3000, 0.0000, 9.7000, 
//////														0.3000, 0.0000, 9.8000, 0.3000, 0.0000, 9.8000, 0.2000, -0.1000, 9.8000, -0.1000, 
//////														-0.1000, 9.8000, 0.3000, -0.2000, 10.1000, 0.3000, -0.1000, 9.3000, -0.7000, 
//////														-0.2000, 9.8000, 0.9000, -1.4000, 9.7000, 0.9000, -1.4000, 9.7000, 3.8000, 0.1000, 
//////														10.1000, 4.0000, 0.5000, 9.6000, 3.9000, 0.4000, 9.9000, 3.4000, -0.6000, 9.5000, 
//////														3.4000, -0.6000, 9.5000, 1.5000, -0.5000, 9.9000, 0.4000, -0.4000, 9.9000, 0.6000, 
//////														-0.2000, 9.8000, 1.1000, -0.2000, 9.8000, 1.5000, 0.0000, 10.0000, 0.7000, 0.0000, 
//////														9.9000, 0.3000, -0.2000, 10.0000, 0.2000, -0.2000, 10.0000, -1.5000, -0.6000, 9.5000, 
//////														-3.1000, -0.2000, 9.7000, -3.7000, -0.2000, 9.7000, -5.1000, 0.0000, 9.7000, -0.2000, 
//////														0.0000, 10.0000, -3.8000, -1.1000, 9.5000, -6.1000, -0.2000, 9.7000, -6.7000, -0.1000, 
//////														9.7000, -5.9000, -0.1000, 9.8000, -5.0000, -0.3000, 9.2000, -3.1000, -0.9000, 10.3000, 
//////														-2.2000, -0.5000, 9.3000, -0.3000, -0.2000, 9.7000, -0.3000, -0.3000, 10.0000, 0.4000, 
//////														-0.3000, 9.7000, 2.1000, -0.6000, 9.7000, 2.1000, -0.6000, 9.7000, 3.2000, -0.7000, 
//////														9.7000, 4.0000, -0.4000, 10.1000, 4.6000, -0.3000, 9.8000, 5.0000, -0.5000, 9.7000, 
//////														5.5000, -0.8000, 10.0000, 7.8000, -0.6000, 10.0000, 4.3000, -1.0000, 9.7000, 5.9000, 
//////														-0.6000, 9.9000, 6.8000, -0.6000, 9.8000, 6.1000, -1.0000, 9.8000, 5.2000, -0.8000, 
//////														9.7000, 4.5000, -1.0000, 9.6000, 3.4000, -0.9000, 9.5000, 2.0000, -0.6000, 9.9000, 
//////														0.3000, -0.2000, 9.7000, 0.1000, -0.2000, 9.9000, -0.1000, -0.1000, 9.8000, -0.4000, 
//////														-0.2000, 9.8000, -1.6000, -0.2000, 9.9000, -2.1000, 0.0000, 9.7000, -2.2000, 0.1000, 
//////														9.8000, -2.0000, -0.2000, 9.7000, -1.9000, -0.3000, 9.6000, -2.6000, -0.6000, 9.8000, 
//////														-2.6000, -0.6000, 9.8000, -3.8000, -0.7000, 9.6000, -5.4000, -0.5000, 9.8000, -6.1000, 
//////														-0.4000, 9.6000, -5.8000, -0.5000, 10.0000, -2.1000, -0.6000, 9.9000, -4.0000, -1.1000, 
//////														9.7000, -5.5000, -0.4000, 9.8000, -6.0000, -0.3000, 9.7000, -4.2000, -0.2000, 9.7000, 
//////														-2.3000, -0.5000, 9.7000, -1.2000, -0.3000, 10.0000, -0.8000, -0.2000, 10.2000, -1.0000, 
//////														-0.4000, 9.7000, -1.0000, -0.5000, 9.7000, -0.2000, -0.4000, 9.7000, 0.9000, -0.2000, 9.6000, 
////////														1.4000, -0.1000, 9.8000, 2.4000, -0.6000, 9.7000, 2.8000, -0.4000, 10.0000, 
////////														2.9000, -0.3000, 9.9000, 4.2000, -0.5000, 9.7000, 5.5000, -0.6000, 9.8000, 6.4000, 
////////														-0.7000, 9.8000, 7.0000, -0.7000, 9.8000, 7.0000, -0.7000, 9.8000, 7.4000, -0.7000, 
////////														9.8000, 3.6000, -0.4000, 9.8000, 5.1000, -0.6000, 9.9000, 5.0000, -0.6000, 10.0000, 
////////														4.5000, -0.6000, 10.0000, 3.6000, -0.8000, 9.6000, 2.3000, -0.8000, 9.7000, 1.5000, 
////////														-0.4000, 9.6000, 1.2000, 0.0000, 10.1000, 1.8000, -0.2000, 10.0000, 2.2000, -0.2000, 
////////														9.7000, 1.3000, -0.3000, 9.6000, 0.1000, -0.5000, 9.5000, 0.1000, -0.5000, 9.5000, 
////////														-2.0000, 0.0000, 9.8000, -2.0000, 0.2000, 9.8000
//////														/*Duplicate first 300 to have 600 samples*/
//////														0.3000, -0.1000, 9.9000, 0.2000, -0.2000, 9.8000, 0.1000, -0.1000, 9.9000, 0.3000, 
//////														0.0000, 9.8000, 0.3000, 0.0000, 9.8000, 0.5000, -0.1000, 9.8000, 0.5000, -0.1000,
//////														9.9000, 0.3000, -0.2000, 9.9000, 0.3000, -0.2000, 9.9000, -0.2000, -0.1000, 9.8000, 
//////														-0.2000, -0.1000, 9.7000, 0.2000, 0.0000, 9.9000, 0.4000, -0.2000, 9.8000, 0.3000, 
//////														-0.2000, 9.9000, 0.5000, -0.1000, 9.8000, 0.2000, -0.1000, 9.9000, 0.1000, -0.1000, 
//////														10.1000, 0.3000, -0.1000, 9.9000, 0.3000, 0.0000, 9.8000, 0.3000, 0.0000, 9.7000, 
//////														0.3000, 0.0000, 9.8000, 0.3000, 0.0000, 9.8000, 0.2000, -0.1000, 9.8000, -0.1000, 
//////														-0.1000, 9.8000, 0.3000, -0.2000, 10.1000, 0.3000, -0.1000, 9.3000, -0.7000, 
//////														-0.2000, 9.8000, 0.9000, -1.4000, 9.7000, 0.9000, -1.4000, 9.7000, 3.8000, 0.1000, 
//////														10.1000, 4.0000, 0.5000, 9.6000, 3.9000, 0.4000, 9.9000, 3.4000, -0.6000, 9.5000, 
//////														3.4000, -0.6000, 9.5000, 1.5000, -0.5000, 9.9000, 0.4000, -0.4000, 9.9000, 0.6000, 
//////														-0.2000, 9.8000, 1.1000, -0.2000, 9.8000, 1.5000, 0.0000, 10.0000, 0.7000, 0.0000, 
//////														9.9000, 0.3000, -0.2000, 10.0000, 0.2000, -0.2000, 10.0000, -1.5000, -0.6000, 9.5000, 
//////														-3.1000, -0.2000, 9.7000, -3.7000, -0.2000, 9.7000, -5.1000, 0.0000, 9.7000, -0.2000, 
//////														0.0000, 10.0000, -3.8000, -1.1000, 9.5000, -6.1000, -0.2000, 9.7000, -6.7000, -0.1000, 
//////														9.7000, -5.9000, -0.1000, 9.8000, -5.0000, -0.3000, 9.2000, -3.1000, -0.9000, 10.3000, 
//////														-2.2000, -0.5000, 9.3000, -0.3000, -0.2000, 9.7000, -0.3000, -0.3000, 10.0000, 0.4000, 
//////														-0.3000, 9.7000, 2.1000, -0.6000, 9.7000, 2.1000, -0.6000, 9.7000, 3.2000, -0.7000, 
//////														9.7000, 4.0000, -0.4000, 10.1000, 4.6000, -0.3000, 9.8000, 5.0000, -0.5000, 9.7000, 
//////														5.5000, -0.8000, 10.0000, 7.8000, -0.6000, 10.0000, 4.3000, -1.0000, 9.7000, 5.9000, 
//////														-0.6000, 9.9000, 6.8000, -0.6000, 9.8000, 6.1000, -1.0000, 9.8000, 5.2000, -0.8000, 
//////														9.7000, 4.5000, -1.0000, 9.6000, 3.4000, -0.9000, 9.5000, 2.0000, -0.6000, 9.9000, 
//////														0.3000, -0.2000, 9.7000, 0.1000, -0.2000, 9.9000, -0.1000, -0.1000, 9.8000, -0.4000, 
//////														-0.2000, 9.8000, -1.6000, -0.2000, 9.9000, -2.1000, 0.0000, 9.7000, -2.2000, 0.1000, 
//////														9.8000, -2.0000, -0.2000, 9.7000, -1.9000, -0.3000, 9.6000, -2.6000, -0.6000, 9.8000, 
//////														-2.6000, -0.6000, 9.8000, -3.8000, -0.7000, 9.6000, -5.4000, -0.5000, 9.8000, -6.1000, 
//////														-0.4000, 9.6000, -5.8000, -0.5000, 10.0000, -2.1000, -0.6000, 9.9000, -4.0000, -1.1000, 
//////														9.7000, -5.5000, -0.4000, 9.8000, -6.0000, -0.3000, 9.7000, -4.2000, -0.2000, 9.7000, 
//////														-2.3000, -0.5000, 9.7000, -1.2000, -0.3000, 10.0000, -0.8000, -0.2000, 10.2000, -1.0000, 
//////														-0.4000, 9.7000, -1.0000, -0.5000, 9.7000, -0.2000, -0.4000, 9.7000, 0.9000, -0.2000, 9.6000 
//////};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static int intToStr(int16_t x, char str[]);
static void reverse(char* str, int len);
void vprint(const char *fmt, va_list argp){
    char string[200];
    if(0 < vsprintf(string, fmt, argp)){ // build string
        HAL_UART_Transmit(&huart6, (uint8_t*)string, strlen(string), 1000); // send message via UART
    }
}

void ei_printf(const char *format, ...){
    va_list myargs;
    va_start(myargs, format);
    vprint(format, myargs);
    va_end(myargs);
}

int get_feature_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

uint32_t FindLabelIndex(float *labelData, uint32_t labelSize){
	float max = labelData[0];
	uint32_t maxIdx = 0;
	for(uint32_t i=1; i<labelSize;i++){
		if(labelData[i] > max){ 
			max = labelData[i];
			maxIdx = i;
		}
	}
	return maxIdx;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
		bno055_vector_t BNO055Data;
		ei_impulse_result_t result;
	signal_t signal;

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
	MX_TIM10_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */
	
#ifdef STM32F407_AI_EventRecorder
	/*Init*/
	EventRecorderInitialize(EventRecordAll, 1U);
#endif // STM32F407_AI_EventRecorder


		HAL_TIM_Base_Start_IT(&htim10);
	/*Init Timer10*/
	__HAL_TIM_ENABLE(&htim10);
	
	
		/*BNO055*/
	bno055_assignI2C(&hi2c3);
  bno055_setup();
  bno055_setOperationModeNDOF();

	/*signal*/
	signal.total_length = sizeof(features) / sizeof(features[0]);
	signal.get_data = &get_feature_data;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		/**
		* BNO055
		*/
		/*Interrupt enabled*/
		if(TimerReadBNO){
				/*Record event*/
		#ifdef STM32F407_AI_EventRecorder
				EventRecordData (3+EventLevelOp, "BNO055 Started", 14);  // Event at Start
		#endif // STM32F407_AI_EventRecorder
				
				/*Read BNO055*/
				BNO055Data = bno055_getVectorAccelerometer();
//				ei_printf("W: %.2f AccX: %.2f AccY: %.2f AccZ: %.2f\r\n\n\n", BNO055Data.w, BNO055Data.x, BNO055Data.y, BNO055Data.z);
//				/*Save in features*/
//				features[BufferIndex*3]   = BNO055Data.x;
//				features[BufferIndex*3+1] = BNO055Data.y;
//				features[BufferIndex*3+2] = BNO055Data.z;
//				/*Increase buffer*/
//				BufferIndex++;
//				TimerReadBNO = false;
			
				/*Record event*/
		#ifdef STM32F407_AI_EventRecorder
				EventRecordData (3+EventLevelOp, "BNO055 Done", 11);  // Event at Start
		#endif // STM32F407_AI_EventRecorder		
		}
	
		/**
		*	Inference
		*/

		/*Buffer full?*/
		if(BufferIndex >= EI_CLASSIFIER_RAW_SAMPLE_COUNT){
				
				/*Log*/
				ei_printf("Features length: %d\n", signal.total_length);
				ei_printf("EI_CLASSIFIER_RAW_SAMPLE_COUNT: %d\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
				ei_printf("EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME: %d\n", EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME);
				ei_printf("Features length needed: %d\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT*EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME);
				ei_printf("First value | X: %.2f  Y: %.2f  Z: %.2f\n", features[0], features[1], features[2]);
			
				/*Record event*/
		#ifdef STM32F407_AI_EventRecorder
				EventRecordData (3+EventLevelOp, "Inference Started", 17);  // Event at Start
		#endif // STM32F407_AI_EventRecorder
				
				/*Inference*/
				EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);
				/*Log*/
				ei_printf("run_classifier returned: %d\n", res);
				ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n", result.timing.dsp, result.timing.classification, result.timing.anomaly);

				/*Print predictions*/
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
				
		////////		/*Anomaly detection*/
		////////		const float AnomalyThreshold = 0.7;
		////////		if(result.anomaly > AnomalyThreshold){
		////////			ei_printf("Anomaly detected!!!\n\n");
		////////		}
		////////		else{
		////////			ei_printf("No anomaly detected.-\n\n");
		////////		}
		////////		
		////////		/*Print label*/
		////////		float inferenceArray[4];
		////////		for(uint8_t i=0;i<4;i++){
		////////			inferenceArray[i] = result.classification[i].value;
		////////		}
		////////		uint32_t InferenceIndexLabel = FindLabelIndex( inferenceArray, 4 );
		////////		ei_printf("Movement: %s\n\n\n\n", ei_classifier_inferencing_categories[InferenceIndexLabel]);
		
		/*Reset Index*/
		BufferIndex = 0;
				
				/*Record event*/
		#ifdef STM32F407_AI_EventRecorder
				EventRecordData (4+EventLevelOp, "Inference Done", 14);  // Event at Finish
		#endif // STM32F407_AI_EventRecorder
	}
	/*TimerReadBNO*/
	if(TimerReadBNO){
		/*Save in features*/
		features[BufferIndex*3]   = BNO055Data.x;
		features[BufferIndex*3+1] = BNO055Data.y;
		features[BufferIndex*3+2] = BNO055Data.z;
		/*Increase buffer*/
		BufferIndex++;
		TimerReadBNO = false;
	}
		
		/*Delay*/
//    HAL_Delay(1000);	// Not needed. Inference should run every 2S when buffer[600] is filled with 200 measurements @ 100Hz
		
		
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
