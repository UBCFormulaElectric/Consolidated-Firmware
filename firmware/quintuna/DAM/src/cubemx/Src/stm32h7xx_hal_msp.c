
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32h7xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* System interrupt init*/
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
 * @brief FDCAN MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hfdcan: FDCAN handle pointer
 * @retval None
 */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hfdcan->Instance == FDCAN2)
    {
        /* USER CODE BEGIN FDCAN2_MspInit 0 */

        /* USER CODE END FDCAN2_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInitStruct.FdcanClockSelection  = RCC_FDCANCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_FDCAN_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**FDCAN2 GPIO Configuration
        PB12     ------> FDCAN2_RX
        PB13     ------> FDCAN2_TX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* FDCAN2 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
        /* USER CODE BEGIN FDCAN2_MspInit 1 */

        /* USER CODE END FDCAN2_MspInit 1 */
    }
}

/**
 * @brief FDCAN MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hfdcan: FDCAN handle pointer
 * @retval None
 */
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan->Instance == FDCAN2)
    {
        /* USER CODE BEGIN FDCAN2_MspDeInit 0 */

        /* USER CODE END FDCAN2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_FDCAN_CLK_DISABLE();

        /**FDCAN2 GPIO Configuration
        PB12     ------> FDCAN2_RX
        PB13     ------> FDCAN2_TX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13);

        /* FDCAN2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
        /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

        /* USER CODE END FDCAN2_MspDeInit 1 */
    }
}

/**
 * @brief RTC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hrtc->Instance == RTC)
    {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_RTC_ENABLE();
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
    }
}

/**
 * @brief RTC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    if (hrtc->Instance == RTC)
    {
        /* USER CODE BEGIN RTC_MspDeInit 0 */

        /* USER CODE END RTC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE();
        /* USER CODE BEGIN RTC_MspDeInit 1 */

        /* USER CODE END RTC_MspDeInit 1 */
    }
}

/**
 * @brief MMC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hmmc: MMC handle pointer
 * @retval None
 */
void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hmmc->Instance == SDMMC1)
    {
        /* USER CODE BEGIN SDMMC1_MspInit 0 */

        /* USER CODE END SDMMC1_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
        PeriphClkInitStruct.SdmmcClockSelection  = RCC_SDMMCCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_SDMMC1_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**SDMMC1 GPIO Configuration
        PC8     ------> SDMMC1_D0
        PC9     ------> SDMMC1_D1
        PC10     ------> SDMMC1_D2
        PC11     ------> SDMMC1_D3
        PC12     ------> SDMMC1_CK
        PD2     ------> SDMMC1_CMD
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN SDMMC1_MspInit 1 */

        /* USER CODE END SDMMC1_MspInit 1 */
    }
}

/**
 * @brief MMC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hmmc: MMC handle pointer
 * @retval None
 */
void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{
    if (hmmc->Instance == SDMMC1)
    {
        /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

        /* USER CODE END SDMMC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SDMMC1_CLK_DISABLE();

        /**SDMMC1 GPIO Configuration
        PC8     ------> SDMMC1_D0
        PC9     ------> SDMMC1_D1
        PC10     ------> SDMMC1_D2
        PC11     ------> SDMMC1_D3
        PC12     ------> SDMMC1_CK
        PD2     ------> SDMMC1_CMD
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

        /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

        /* USER CODE END SDMMC1_MspDeInit 1 */
    }
}

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (huart->Instance == USART2)
    {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA0     ------> USART2_CTS
        PA1     ------> USART2_RTS
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin       = FROM_900M_CTS_Pin | TO_900M_RTS_Pin | TO_900M_TX_Pin | FROM_900M_RX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
}

/**
 * @brief UART MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA0     ------> USART2_CTS
        PA1     ------> USART2_RTS
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, FROM_900M_CTS_Pin | TO_900M_RTS_Pin | TO_900M_TX_Pin | FROM_900M_RX_Pin);

        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
