/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32f4xx_hal_msp.c
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
    __HAL_RCC_PWR_CLK_ENABLE();

    /* System interrupt init*/
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

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
        __HAL_RCC_CAN1_CLK_ENABLE();

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

        /* CAN1 interrupt Init */
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
        /* USER CODE BEGIN CAN1_MspInit 1 */

        /* USER CODE END CAN1_MspInit 1 */
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

        /* CAN1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
        /* USER CODE BEGIN CAN1_MspDeInit 1 */

        /* USER CODE END CAN1_MspDeInit 1 */
    }
}

/**
 * @brief CRC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
    if (hcrc->Instance == CRC)
    {
        /* USER CODE BEGIN CRC_MspInit 0 */

        /* USER CODE END CRC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CRC_CLK_ENABLE();
        /* USER CODE BEGIN CRC_MspInit 1 */

        /* USER CODE END CRC_MspInit 1 */
    }
}

/**
 * @brief CRC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
    if (hcrc->Instance == CRC)
    {
        /* USER CODE BEGIN CRC_MspDeInit 0 */

        /* USER CODE END CRC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CRC_CLK_DISABLE();
        /* USER CODE BEGIN CRC_MspDeInit 1 */

        /* USER CODE END CRC_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
