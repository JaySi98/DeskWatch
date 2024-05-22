/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "ds1307/ds1307.h"
#include "lps25hb/lps25hb.h"
#include "ds18b20/ds18b20.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_font.h"
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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

volatile uint8_t mesure = 0;
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  char* DAYS_OF_WEEK[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
  DS1307_Init(&hi2c1);
//  DS1307_Set_TimeZone(+8, 00);
//  DS1307_Set_Date(11);
//  DS1307_Set_Month(5);
//  DS1307_Set_Year(2024);
//  DS1307_Set_DayOfWeek(6);
//  DS1307_Set_Hour(14);
//  DS1307_Set_Minute(25);
//  DS1307_Set_Second(00);

  LPS25HB_Init(&hi2c1);
  const float h = 138;
  LPS25HB_Set_Calib(208);
  SSD1306_Init(&hi2c1);

  if (ds18b20_init() != HAL_OK) {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  char buffer[21];
  float temp2 = 0;

  while (1)
  {
	uint8_t date   = DS1307_Get_Date();
	uint8_t month  = DS1307_Get_Month();
	uint16_t year  = DS1307_Get_Year();
	uint8_t dow    = DS1307_Get_DayOfWeek();
	uint8_t hour   = DS1307_Get_Hour();
	uint8_t minute = DS1307_Get_Minute();
//	uint8_t second = DS1307_Get_Second();

	float temp = LPS25HB_Get_Temp();
	float p    = LPS25HB_Get_Pressure();	// cisnienie bezwzględne
	float p0   = p * exp(0.034162608734308 * h / (temp+ 273.15)); // cisnienie względne

	ds18b20_start_measure(NULL);
	HAL_TIM_Base_Start_IT(&htim10);
	if(mesure == 1)
	{
		HAL_TIM_Base_Stop_IT(&htim10);
		float temp = ds18b20_get_temp(NULL);
		if(temp < 85.0)
		{
			temp2 = ds18b20_get_temp(NULL);
		}
	}

	// line 1
	sprintf(buffer, "%04d-%02d-%02d %s %02d:%02d", year, month, date, DAYS_OF_WEEK[dow], hour, minute);
	SSD1306_SetCursor(0, 0);
    SSD1306_WriteString(buffer, Font_6x8, White);

    // line 2
	sprintf(buffer, "p0: %i hPa", (int)p0);
    SSD1306_SetCursor(0, 9);
    SSD1306_WriteString(buffer, Font_6x8, White);

    // line 3
	sprintf(buffer, "p:  %i hPa", (int)p);
    SSD1306_SetCursor(0, 18);
    SSD1306_WriteString(buffer, Font_6x8, White);

    // line 4
	sprintf(buffer, "T1:  %.1f*C", temp);
    SSD1306_SetCursor(0, 27);
    SSD1306_WriteString(buffer, Font_6x8, White);

    // line 5
	sprintf(buffer, "T2:  %.1f*C", temp2);
    SSD1306_SetCursor(0, 36);
	SSD1306_WriteString(buffer, Font_6x8, White);
	SSD1306_UpdateScreen();

	SSD1306_UpdateScreen();
	HAL_Delay(1000);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim10) {
//    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  mesure = 1;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	__disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
