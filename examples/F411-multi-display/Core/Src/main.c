/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "lcdDisplay.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LCD1		0
#define LCD2		1
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

/* USER CODE BEGIN PV */
lcd_t Lcd[2] = {0};
uint8_t Lcd2Bright = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void _lcd1_config(){
	// Small 16x2 LCD, wired for 4bit communication
	Lcd[LCD1].columns = 16;
	Lcd[LCD1].rows = 2;
	Lcd[LCD1].interface = LCD_INTERFACE_4BIT;
	Lcd[LCD1].font = LCD_FONT_5X8;
	// GPIOs of the Display
	Lcd[LCD1].gpios[LCD_RS].GPIO = 	(uint32_t)LCD1_RS_GPIO_Port;
	Lcd[LCD1].gpios[LCD_RS].pin = 	LCD1_RS_Pin;
	Lcd[LCD1].gpios[LCD_E].GPIO = 	(uint32_t)LCD1_E_GPIO_Port;
	Lcd[LCD1].gpios[LCD_E].pin = 	LCD1_E_Pin;
	Lcd[LCD1].gpios[LCD_D4].GPIO = 	(uint32_t)LCD1_D4_GPIO_Port;
	Lcd[LCD1].gpios[LCD_D4].pin = 	LCD1_D4_Pin;
	Lcd[LCD1].gpios[LCD_D5].GPIO =	(uint32_t)LCD1_D5_GPIO_Port;
	Lcd[LCD1].gpios[LCD_D5].pin = 	LCD1_D5_Pin;
	Lcd[LCD1].gpios[LCD_D6].GPIO = 	(uint32_t)LCD1_D6_GPIO_Port;
	Lcd[LCD1].gpios[LCD_D6].pin = 	LCD1_D6_Pin;
	Lcd[LCD1].gpios[LCD_D7].GPIO = 	(uint32_t)LCD1_D7_GPIO_Port;
	Lcd[LCD1].gpios[LCD_D7].pin = 	LCD1_D7_Pin;

	// Gpio for drive the LCD backlight
	Lcd[LCD1].backlightGpio.GPIO = (uint32_t)LCD1_BL_GPIO_Port;
	Lcd[LCD1].backlightGpio.pin = LCD1_BL_Pin;

	// Initialize the display and turnon the Backlight
	lcd_init(&Lcd[LCD1]);
	lcd_backlight_set(&Lcd[LCD1], LCD_BACKLIGHT_ON);
}

void _lcd2_config(){
	// Small 16x2 LCD, wired for 4bit communication
	Lcd[LCD2].columns = 16;
	Lcd[LCD2].rows = 2;
	Lcd[LCD2].interface = LCD_INTERFACE_4BIT;
	Lcd[LCD2].font = LCD_FONT_5X8;
	// GPIOs of the Display
	Lcd[LCD2].gpios[LCD_RS].GPIO = 	(uint32_t)LCD2_RS_GPIO_Port;
	Lcd[LCD2].gpios[LCD_RS].pin = 	LCD2_RS_Pin;
	Lcd[LCD2].gpios[LCD_E].GPIO = 	(uint32_t)LCD2_E_GPIO_Port;
	Lcd[LCD2].gpios[LCD_E].pin = 	LCD2_E_Pin;
	Lcd[LCD2].gpios[LCD_D4].GPIO = 	(uint32_t)LCD2_D4_GPIO_Port;
	Lcd[LCD2].gpios[LCD_D4].pin = 	LCD2_D4_Pin;
	Lcd[LCD2].gpios[LCD_D5].GPIO =	(uint32_t)LCD2_D5_GPIO_Port;
	Lcd[LCD2].gpios[LCD_D5].pin = 	LCD2_D5_Pin;
	Lcd[LCD2].gpios[LCD_D6].GPIO = 	(uint32_t)LCD2_D6_GPIO_Port;
	Lcd[LCD2].gpios[LCD_D6].pin = 	LCD2_D6_Pin;
	Lcd[LCD2].gpios[LCD_D7].GPIO = 	(uint32_t)LCD2_D7_GPIO_Port;
	Lcd[LCD2].gpios[LCD_D7].pin = 	LCD2_D7_Pin;

	// Timer handler and channel for PWM operation
	Lcd[LCD2].backlightPwm.Channel = TIM_CHANNEL_1;
	Lcd[LCD2].backlightPwm.Peripheral = (uint32_t)&htim4;

	// Initialize display and turnon the Cursor
	lcd_init(&Lcd[LCD2]);
	lcd_cursor_on(&Lcd[LCD2]);

	// Turnon Timer 5 to fade-in the LCD
	HAL_TIM_Base_Start_IT(&htim5);
}

void _lcd_custom_char(){
	uint8_t arrow1[8] = { 0x00, 0x04, 0x06, 0x1F, 0x1F, 0x06, 0x04, 0x00 };
	uint8_t arrow2[8] = { 0x00, 0x04, 0x0C, 0x1F, 0x1F, 0x0C, 0x04, 0x00 };

	lcd_create_custom_char(&Lcd[LCD1], LCD_CUSTOM_1, arrow1);
	lcd_create_custom_char(&Lcd[LCD1], LCD_CUSTOM_2, arrow2);

	lcd_create_custom_char(&Lcd[LCD2], LCD_CUSTOM_1, arrow1);
	lcd_create_custom_char(&Lcd[LCD2], LCD_CUSTOM_2, arrow2);
}

void _lcd1_lineStart(){
	lcd_put_pos(&Lcd[LCD1], 0, 0);
	lcd_send_char(&Lcd[LCD1], LCD_CUSTOM_1);
	lcd_send_string(&Lcd[LCD1], " This is LCD1 ");
	lcd_send_char(&Lcd[LCD1], LCD_CUSTOM_2);
}

void _lcd2_lineStart(){
	lcd_put_pos(&Lcd[LCD2], 0, 0);
	lcd_send_char(&Lcd[LCD2], LCD_CUSTOM_1);
	lcd_send_string(&Lcd[LCD2], " This is LCD2 ");
	lcd_send_char(&Lcd[LCD2], LCD_CUSTOM_2);
}

/* Callbacks */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance = TIM5){
		if (Lcd2Bright < 100){
			Lcd2Bright++;
			lcd_backlight_set_bright(&Lcd[LCD2], Lcd2Bright);
			if (Lcd2Bright == 100){
				HAL_TIM_Base_Stop_IT(&htim5);
			}
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char _auxStr[21];
	int _cnt;

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
  MX_TIM4_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(150);

  /* Start and configure the Lcd[LCD1] and Lcd[LCD2] */
  _lcd1_config();
  _lcd2_config();
  /* Configures the custom char on the two Lcds */
  _lcd_custom_char();

  /* Start the Text on First line of the LCDs */
  _lcd1_lineStart();
  _lcd2_lineStart();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  _cnt = 0;
  while (1)
  {
	  /* Text for LCD1 */
	  sprintf(_auxStr, "C: 0x%04X", _cnt);
	  lcd_send_string_pos(&Lcd[LCD1], _auxStr, 1, 0);

	  /* Text for LCD2 */
	  sprintf(_auxStr, "C: 0x%04X", 0xFFFF-_cnt);
	  lcd_send_string_pos(&Lcd[LCD2], _auxStr, 1, 0);

	  /* Increase counter and wait for 500ms */
	  _cnt++;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 15;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 9999;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD1_D7_Pin|LCD1_D6_Pin|LCD1_D5_Pin|LCD1_D4_Pin
                          |LCD1_E_Pin|LCD1_RS_Pin|LCD2_RS_Pin|LCD2_E_Pin
                          |LCD2_D4_Pin|LCD2_D5_Pin|LCD2_D6_Pin|LCD2_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD1_BL_GPIO_Port, LCD1_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD1_D7_Pin LCD1_D6_Pin LCD1_D5_Pin LCD1_D4_Pin
                           LCD1_E_Pin LCD1_RS_Pin LCD2_RS_Pin LCD2_E_Pin
                           LCD2_D4_Pin LCD2_D5_Pin LCD2_D6_Pin LCD2_D7_Pin */
  GPIO_InitStruct.Pin = LCD1_D7_Pin|LCD1_D6_Pin|LCD1_D5_Pin|LCD1_D4_Pin
                          |LCD1_E_Pin|LCD1_RS_Pin|LCD2_RS_Pin|LCD2_E_Pin
                          |LCD2_D4_Pin|LCD2_D5_Pin|LCD2_D6_Pin|LCD2_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD1_BL_Pin */
  GPIO_InitStruct.Pin = LCD1_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD1_BL_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
