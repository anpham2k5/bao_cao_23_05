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
#include <stdio.h>
#include <string.h>

/* Các bi?n luu giá tr? tham chi?u ban d?u */
float refer1 = 0;
float refer2 = 0;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void Delay_us(uint16_t us);
uint32_t Measure_RC_Once_US(uint16_t pin_chg, uint16_t pin_read);
float Measure_RC_Time_US_Avg(uint16_t pin_chg, uint16_t pin_read, uint16_t samples);
void Pin_As_Output_Low(uint16_t pin);
void Pin_As_Input(uint16_t pin);
void UART_Send(char *s);
/* USER CODE END PFP */

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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  
  /* B?t d?u kích ho?t b? d?m Timer 2 */
  HAL_TIM_Base_Start(&htim2);

  /* Thông báo tr?ng thái l?y m?u chu?n */
  UART_Send("Dang lay mau calibratSAion ban dau (Vui long doi 2 giay)... \r\n");
/* L?y m?u g?c ban d?u (Gi?m xu?ng 200 l?n d? m?ch kh?i d?ng nhanh mà v?n chính xác c?c cao) */
  refer1 = Measure_RC_Time_US_Avg(GPIO_PIN_0, GPIO_PIN_1, 200);
  refer2 = Measure_RC_Time_US_Avg(GPIO_PIN_2, GPIO_PIN_3, 200);

  UART_Send("Calibration hoàn t?t! Bat dau do lien tuc.\r\n");

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    float rc1, rc2, gt1, gt2, gt;
    uint32_t start_time, total_duration_ms; 
    char msg[150];

    
    start_time = HAL_GetTick();
    
    rc1 = Measure_RC_Time_US_Avg(GPIO_PIN_0, GPIO_PIN_1, 100);
          
    
    rc2 = Measure_RC_Time_US_Avg(GPIO_PIN_2, GPIO_PIN_3, 100);
         
    
    total_duration_ms = HAL_GetTick() - start_time;
		gt1 = rc1 - refer1;
		gt2 = rc2 - refer2;
		gt = gt1+gt2;
		sprintf(msg, "gt = %.2f us | Tong tg do: %lu ms\r\n", 
            gt, total_duration_ms);
    UART_Send(msg);

    HAL_Delay(1000); 
  }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71; // T?n s? vào 72MHz / (71 + 1) = 1MHz -> B? d?m tang chính xác sau m?i 1 microgiây
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  
  /* T?I UU: Ð?i t? 65535 sang m?c t?i da 4294967295 d? ch?ng tràn b? d?m khi do kho?ng th?i gian dài */
  htim2.Init.Period = 65535; 
  
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_2, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Hàm t?o tr? chính xác m?c Microgiây s? d?ng ph?n c?ng Timer 2
  */
void Delay_us(uint16_t us)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(&htim2);
    while ((__HAL_TIM_GET_COUNTER(&htim2) - start) < us);
}

void Pin_As_Output_Low(uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // Tang t?c d? dáp ?ng c?a chân IO
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET);
}

void Pin_As_Input(uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint32_t Measure_RC_Once_US(uint16_t pin_chg, uint16_t pin_read)
{
    uint32_t timeout = 100000; 

    
    HAL_GPIO_WritePin(GPIOA, pin_chg, GPIO_PIN_RESET);
    Pin_As_Output_Low(pin_read);
    HAL_Delay(1); 
    
    Pin_As_Input(pin_read);
    
    
    Delay_us(10); 

    
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_GPIO_WritePin(GPIOA, pin_chg, GPIO_PIN_SET);

    
    while(HAL_GPIO_ReadPin(GPIOA, pin_read) == GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > timeout)
        {
            HAL_GPIO_WritePin(GPIOA, pin_chg, GPIO_PIN_RESET);
            return timeout;
        }
    }

    
    uint32_t result = __HAL_TIM_GET_COUNTER(&htim2);

    
    HAL_GPIO_WritePin(GPIOA, pin_chg, GPIO_PIN_RESET);
return result;
}

float Measure_RC_Time_US_Avg(uint16_t pin_chg, uint16_t pin_read, uint16_t samples)
{
    uint32_t sum = 0;
    for(uint16_t i = 0; i < samples; i++)
    {
        sum += Measure_RC_Once_US(pin_chg, pin_read);
        Delay_us(500); 
    }
    return (float)sum / samples;
}

void UART_Send(char *s)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)s, strlen(s), 200);
}
/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */