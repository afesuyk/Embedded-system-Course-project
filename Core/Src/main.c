/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdio.h"
#include "lcd_hd44780_i2c.h"
#include "DS3231.h"
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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId printLcdTaskHandle;
osThreadId checkDataSensorHandle;
osThreadId myBtnTaskHandle;
osThreadId myPrintTimeHandle;
/* USER CODE BEGIN PV */
bool isPressed = false;
uint8_t numTask = 0;
char buffer[15];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
void StartDefaultTask(void const * argument);
void StartLcdTask(void const * argument);
void StartDataSensor(void const * argument);
void StartBtnTask(void const * argument);
void StartTimeTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//uint8_t decToBcd(int val)
//{
//  return (uint8_t)( (val/10*16) + (val%10) );
//}
//
//int bcdToDec(uint8_t val)
//{
//  return (int)( (val/16*10) + (val%16) );
//}
//
//typedef struct {
//	uint8_t seconds;
//	uint8_t minutes;
//	uint8_t hour;
//	uint8_t dayofweek;
//	uint8_t dayofmonth;
//	uint8_t month;
//	uint8_t year;
//} TIME;
//
//TIME time;
//
//
//void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
//{
//	uint8_t set_time[7];
//	set_time[0] = decToBcd(sec);
//	set_time[1] = decToBcd(min);
//	set_time[2] = decToBcd(hour);
//	set_time[3] = decToBcd(dow);
//	set_time[4] = decToBcd(dom);
//	set_time[5] = decToBcd(month);
//	set_time[6] = decToBcd(year);
//
//	HAL_I2C_Mem_Write(&hi2c3, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
//}
//
//void Get_Time (void)
//{
//	uint8_t get_time[7];
//	HAL_I2C_Mem_Read(&hi2c3, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
//	time.seconds = bcdToDec(get_time[0]);
//	time.minutes = bcdToDec(get_time[1]);
//	time.hour = bcdToDec(get_time[2]);
//	time.dayofweek = bcdToDec(get_time[3]);
//	time.dayofmonth = bcdToDec(get_time[4]);
//	time.month = bcdToDec(get_time[5]);
//	time.year = bcdToDec(get_time[6]);
//}
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */

  /* Use if you want to change start time and date */
  /* Set_Time(20, 27, 16, 6, 30, 5, 20, &hi2c3); */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of printLcdTask */
  osThreadDef(printLcdTask, StartLcdTask, osPriorityNormal, 0, 128);
  printLcdTaskHandle = osThreadCreate(osThread(printLcdTask), NULL);

  /* definition and creation of checkDataSensor */
  osThreadDef(checkDataSensor, StartDataSensor, osPriorityNormal, 0, 128);
  checkDataSensorHandle = osThreadCreate(osThread(checkDataSensor), NULL);

  /* definition and creation of myBtnTask */
  osThreadDef(myBtnTask, StartBtnTask, osPriorityNormal, 0, 128);
  myBtnTaskHandle = osThreadCreate(osThread(myBtnTask), NULL);

  /* definition and creation of myPrintTime */
  osThreadDef(myPrintTime, StartTimeTask, osPriorityNormal, 0, 128);
  myPrintTimeHandle = osThreadCreate(osThread(myPrintTime), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  vTaskSuspend(printLcdTaskHandle);
  vTaskSuspend(myPrintTimeHandle);
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  for(;;)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    vTaskDelay(1000);
  }

  osThreadTerminate(NULL);
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartLcdTask */
/**
* @brief Function implementing the printLcdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLcdTask */
void StartLcdTask(void const * argument)
{
  /* USER CODE BEGIN StartLcdTask */
	lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
	lcdDisplayClear();

	for(;;)
	{
      lcdSetCursorPosition(0, 0);
	  lcdPrintStr((uint8_t*)"Have a nice day!", 16);

      lcdSetCursorPosition(0, 1);
	  lcdPrintStr((uint8_t*)"Just a line...", 14);

	  HAL_Delay(1000);
	  lcdDisplayClear();

	  vTaskDelay(100);
	}

	osThreadTerminate(NULL);
  /* USER CODE END StartLcdTask */
}

/* USER CODE BEGIN Header_StartDataSensor */
/**
* @brief Function implementing the checkDataSensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDataSensor */
void StartDataSensor(void const * argument)
{
  /* USER CODE BEGIN StartDataSensor */
  lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
  lcdDisplayClear();

  for(;;)
  {
	lcdSetCursorPosition(0, 0);
	lcdPrintStr((uint8_t*)"Our sensor data:", 16);

	lcdSetCursorPosition(0, 1);
	lcdPrintStr((uint8_t*) "Humidity = ", 11);

	lcdSetCursorPosition(0, 2);
	lcdPrintStr((uint8_t*) "Pressure = ", 11);

	HAL_Delay(1000);
	lcdDisplayClear();

    vTaskDelay(100);
  }

  osThreadTerminate(NULL);
  /* USER CODE END StartDataSensor */
}

/* USER CODE BEGIN Header_StartBtnTask */
/**
* @brief Function implementing the myBtnTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBtnTask */
void StartBtnTask(void const * argument)
{
  /* USER CODE BEGIN StartBtnTask */
  /* Infinite loop */
  for(;;)
  {
    if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
    	vTaskDelay(30);

    	if(numTask == 3) {
    		numTask = 0;
    	}

    	if(numTask == 0) {
    		vTaskSuspend(printLcdTaskHandle);
    		vTaskSuspend(myPrintTimeHandle);
   		    vTaskResume(checkDataSensorHandle);
   	    	numTask++;
    	}
    	else if(numTask == 1) {
    		vTaskSuspend(checkDataSensorHandle);
    		vTaskSuspend(myPrintTimeHandle);
    		vTaskResume(printLcdTaskHandle);
        	numTask++;
    	}
    	else if(numTask == 2) {
    		vTaskSuspend(checkDataSensorHandle);
    		vTaskSuspend(printLcdTaskHandle);
    		vTaskResume(myPrintTimeHandle);
        	numTask++;
    	}
    }
    vTaskDelay(100);
  }

  osThreadTerminate(NULL);
  /* USER CODE END StartBtnTask */
}

/* USER CODE BEGIN Header_StartTimeTask */
/**
* @brief Function implementing the myPrintTime thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTimeTask */
void StartTimeTask(void const * argument)
{
  /* USER CODE BEGIN StartTimeTask */
  lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
  lcdDisplayClear();

  for(;;)
  {
	Get_Time(&hi2c3);

	lcdSetCursorPosition(0, 0);
	lcdPrintStr((uint8_t*)"Your time: ", 11);

	sprintf(buffer, "%02d:%02d:%02d", time.hour, time.minutes, time.seconds);
	lcdSetCursorPosition(11, 0);
	lcdPrintStr((uint8_t*)buffer,8);

	lcdSetCursorPosition(0, 2);
	lcdPrintStr((uint8_t*)"Date: ", 6);

	sprintf(buffer, "%02d-%02d-20%02d", time.dayofmonth, time.month, time.year);
	lcdSetCursorPosition(6, 2);
	lcdPrintStr((uint8_t*)buffer, 10);

	HAL_Delay(1000);
	lcdDisplayClear();
//    vTaskDelay(100);
  }

  osThreadTerminate(NULL);
  /* USER CODE END StartTimeTask */
}

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