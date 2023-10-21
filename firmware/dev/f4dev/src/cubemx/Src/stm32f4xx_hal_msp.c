/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : stm32f4xx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
    __HAL_RCC_PWR_CLK_ENABLE();

    /* System interrupt init*/
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
 * @brief ADC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_IN10
        PC1     ------> ADC1_IN11
        PC2     ------> ADC1_IN12
        PC3     ------> ADC1_IN13
        PA0     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PC4     ------> ADC1_IN14
        PC5     ------> ADC1_IN15
        */
        GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

/**
 * @brief ADC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_IN10
        PC1     ------> ADC1_IN11
        PC2     ------> ADC1_IN12
        PC3     ------> ADC1_IN13
        PA0     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PC4     ------> ADC1_IN14
        PC5     ------> ADC1_IN15
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED = 0;

/**
 * @brief CAN MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hcan->Instance == CAN1)
    {
        /* USER CODE BEGIN CAN1_MspInit 0 */

        /* USER CODE END CAN1_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_CAN1_CLK_ENABLED++;
        if (HAL_RCC_CAN1_CLK_ENABLED == 1)
        {
            __HAL_RCC_CAN1_CLK_ENABLE();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**CAN1 GPIO Configuration
        PB8     ------> CAN1_RX
        PB9     ------> CAN1_TX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_CAN1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN CAN1_MspInit 1 */

        /* USER CODE END CAN1_MspInit 1 */
    }
    else if (hcan->Instance == CAN2)
    {
        /* USER CODE BEGIN CAN2_MspInit 0 */

        /* USER CODE END CAN2_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_CAN1_CLK_ENABLED++;
        if (HAL_RCC_CAN1_CLK_ENABLED == 1)
        {
            __HAL_RCC_CAN1_CLK_ENABLE();
        }
        __HAL_RCC_CAN2_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**CAN2 GPIO Configuration
        PB12     ------> CAN2_RX
        PB13     ------> CAN2_TX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN CAN2_MspInit 1 */

        /* USER CODE END CAN2_MspInit 1 */
    }
}

/**
 * @brief CAN MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        /* USER CODE BEGIN CAN1_MspDeInit 0 */

        /* USER CODE END CAN1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN1 GPIO Configuration
        PB8     ------> CAN1_RX
        PB9     ------> CAN1_TX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

        /* USER CODE BEGIN CAN1_MspDeInit 1 */

        /* USER CODE END CAN1_MspDeInit 1 */
    }
    else if (hcan->Instance == CAN2)
    {
        /* USER CODE BEGIN CAN2_MspDeInit 0 */

        /* USER CODE END CAN2_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_CAN1_CLK_ENABLED--;
        if (HAL_RCC_CAN1_CLK_ENABLED == 0)
        {
            __HAL_RCC_CAN1_CLK_DISABLE();
        }
        __HAL_RCC_CAN2_CLK_DISABLE();

        /**CAN2 GPIO Configuration
        PB12     ------> CAN2_RX
        PB13     ------> CAN2_TX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13);

        /* USER CODE BEGIN CAN2_MspDeInit 1 */

        /* USER CODE END CAN2_MspDeInit 1 */
    }
}

/**
 * @brief SD MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hsd: SD handle pointer
 * @retval None
 */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hsd->Instance == SDIO)
    {
        /* USER CODE BEGIN SDIO_MspInit 0 */

        /* USER CODE END SDIO_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SDIO_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**SDIO GPIO Configuration
        PB15     ------> SDIO_CK
        PC8     ------> SDIO_D0
        PC9     ------> SDIO_D1
        PC10     ------> SDIO_D2
        PC11     ------> SDIO_D3
        PD2     ------> SDIO_CMD
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_15;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN SDIO_MspInit 1 */

        /* USER CODE END SDIO_MspInit 1 */
    }
}

/**
 * @brief SD MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hsd: SD handle pointer
 * @retval None
 */
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    if (hsd->Instance == SDIO)
    {
        /* USER CODE BEGIN SDIO_MspDeInit 0 */

        /* USER CODE END SDIO_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SDIO_CLK_DISABLE();

        /**SDIO GPIO Configuration
        PB15     ------> SDIO_CK
        PC8     ------> SDIO_D0
        PC9     ------> SDIO_D1
        PC10     ------> SDIO_D2
        PC11     ------> SDIO_D3
        PD2     ------> SDIO_CMD
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

        /* USER CODE BEGIN SDIO_MspDeInit 1 */

        /* USER CODE END SDIO_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
