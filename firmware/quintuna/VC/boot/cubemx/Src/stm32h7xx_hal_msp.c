
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

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

/**
 * @brief FDCAN MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hfdcan: FDCAN handle pointer
 * @retval None
 */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hfdcan->Instance == FDCAN1)
    {
        /* USER CODE BEGIN FDCAN1_MspInit 0 */

        /* USER CODE END FDCAN1_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1)
        {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**FDCAN1 GPIO Configuration
        PD0     ------> FDCAN1_RX
        PD1     ------> FDCAN1_TX
        */
        GPIO_InitStruct.Pin       = CAN1_RX_Pin | CAN1_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* FDCAN1 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
        /* USER CODE BEGIN FDCAN1_MspInit 1 */

        /* USER CODE END FDCAN1_MspInit 1 */
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        /* USER CODE BEGIN FDCAN2_MspInit 0 */

        /* USER CODE END FDCAN2_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1)
        {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**FDCAN2 GPIO Configuration
        PB5     ------> FDCAN2_RX
        PB6     ------> FDCAN2_TX
        */
        GPIO_InitStruct.Pin       = CAN3_RX_Pin | CAN3_TX_Pin;
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
    else if (hfdcan->Instance == FDCAN3)
    {
        /* USER CODE BEGIN FDCAN3_MspInit 0 */

        /* USER CODE END FDCAN3_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1)
        {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**FDCAN3 GPIO Configuration
        PD12     ------> FDCAN3_RX
        PD13     ------> FDCAN3_TX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_FDCAN3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* FDCAN3 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);
        /* USER CODE BEGIN FDCAN3_MspInit 1 */

        /* USER CODE END FDCAN3_MspInit 1 */
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
    if (hfdcan->Instance == FDCAN1)
    {
        /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

        /* USER CODE END FDCAN1_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0)
        {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        /**FDCAN1 GPIO Configuration
        PD0     ------> FDCAN1_RX
        PD1     ------> FDCAN1_TX
        */
        HAL_GPIO_DeInit(GPIOD, CAN1_RX_Pin | CAN1_TX_Pin);

        /* FDCAN1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
        /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

        /* USER CODE END FDCAN1_MspDeInit 1 */
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        /* USER CODE BEGIN FDCAN2_MspDeInit 0 */

        /* USER CODE END FDCAN2_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0)
        {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        /**FDCAN2 GPIO Configuration
        PB5     ------> FDCAN2_RX
        PB6     ------> FDCAN2_TX
        */
        HAL_GPIO_DeInit(GPIOB, CAN3_RX_Pin | CAN3_TX_Pin);

        /* FDCAN2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
        /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

        /* USER CODE END FDCAN2_MspDeInit 1 */
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        /* USER CODE BEGIN FDCAN3_MspDeInit 0 */

        /* USER CODE END FDCAN3_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0)
        {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        /**FDCAN3 GPIO Configuration
        PD12     ------> FDCAN3_RX
        PD13     ------> FDCAN3_TX
        */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12 | GPIO_PIN_13);

        /* FDCAN3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN3_IT1_IRQn);
        /* USER CODE BEGIN FDCAN3_MspDeInit 1 */

        /* USER CODE END FDCAN3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
