
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

static uint32_t HAL_RCC_ADC12_CLK_ENABLED = 0;

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
        HAL_RCC_ADC12_CLK_ENABLED++;
        if (HAL_RCC_ADC12_CLK_ENABLED == 1)
        {
            __HAL_RCC_ADC12_CLK_ENABLE();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA0     ------> ADC1_INP16
        PA1     ------> ADC1_INP17
        PA2     ------> ADC1_INP14
        PA3     ------> ADC1_INP15
        PC4     ------> ADC1_INP4
        PB1     ------> ADC1_INP5
        */
        GPIO_InitStruct.Pin  = F_PUMP_I_SNS_Pin | RR_PUMP_I_SNS_Pin | DAM_I_SNS_Pin | FRONT_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = RL_PUMP_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(RL_PUMP_I_SNS_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = BMS_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(BMS_I_SNS_GPIO_Port, &GPIO_InitStruct);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
    else if (hadc->Instance == ADC2)
    {
        /* USER CODE BEGIN ADC2_MspInit 0 */

        /* USER CODE END ADC2_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_ADC12_CLK_ENABLED++;
        if (HAL_RCC_ADC12_CLK_ENABLED == 1)
        {
            __HAL_RCC_ADC12_CLK_ENABLE();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC2 GPIO Configuration
        PA4     ------> ADC2_INP18
        PA5     ------> ADC2_INP19
        PA6     ------> ADC2_INP3
        PC5     ------> ADC2_INP8
        PB0     ------> ADC2_INP9
        */
        GPIO_InitStruct.Pin  = RSM_I_SNS_Pin | L_RAD_FAN_I_SNS_Pin | R_RAD_FAN_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = F_INV_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(F_INV_I_SNS_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = R_INV_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(R_INV_I_SNS_GPIO_Port, &GPIO_InitStruct);

        /* ADC2 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC2_MspInit 1 */

        /* USER CODE END ADC2_MspInit 1 */
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
        HAL_RCC_ADC12_CLK_ENABLED--;
        if (HAL_RCC_ADC12_CLK_ENABLED == 0)
        {
            __HAL_RCC_ADC12_CLK_DISABLE();
        }

        /**ADC1 GPIO Configuration
        PA0     ------> ADC1_INP16
        PA1     ------> ADC1_INP17
        PA2     ------> ADC1_INP14
        PA3     ------> ADC1_INP15
        PC4     ------> ADC1_INP4
        PB1     ------> ADC1_INP5
        */
        HAL_GPIO_DeInit(GPIOA, F_PUMP_I_SNS_Pin | RR_PUMP_I_SNS_Pin | DAM_I_SNS_Pin | FRONT_I_SNS_Pin);

        HAL_GPIO_DeInit(RL_PUMP_I_SNS_GPIO_Port, RL_PUMP_I_SNS_Pin);

        HAL_GPIO_DeInit(BMS_I_SNS_GPIO_Port, BMS_I_SNS_Pin);

        /* ADC1 interrupt DeInit */
        /* USER CODE BEGIN ADC1:ADC_IRQn disable */
        /**
         * Uncomment the line below to disable the "ADC_IRQn" interrupt
         * Be aware, disabling shared interrupt may affect other IPs
         */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC1:ADC_IRQn disable */

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
    else if (hadc->Instance == ADC2)
    {
        /* USER CODE BEGIN ADC2_MspDeInit 0 */

        /* USER CODE END ADC2_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_ADC12_CLK_ENABLED--;
        if (HAL_RCC_ADC12_CLK_ENABLED == 0)
        {
            __HAL_RCC_ADC12_CLK_DISABLE();
        }

        /**ADC2 GPIO Configuration
        PA4     ------> ADC2_INP18
        PA5     ------> ADC2_INP19
        PA6     ------> ADC2_INP3
        PC5     ------> ADC2_INP8
        PB0     ------> ADC2_INP9
        */
        HAL_GPIO_DeInit(GPIOA, RSM_I_SNS_Pin | L_RAD_FAN_I_SNS_Pin | R_RAD_FAN_I_SNS_Pin);

        HAL_GPIO_DeInit(F_INV_I_SNS_GPIO_Port, F_INV_I_SNS_Pin);

        HAL_GPIO_DeInit(R_INV_I_SNS_GPIO_Port, R_INV_I_SNS_Pin);

        /* ADC2 interrupt DeInit */
        /* USER CODE BEGIN ADC2:ADC_IRQn disable */
        /**
         * Uncomment the line below to disable the "ADC_IRQn" interrupt
         * Be aware, disabling shared interrupt may affect other IPs
         */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC2:ADC_IRQn disable */

        /* USER CODE BEGIN ADC2_MspDeInit 1 */

        /* USER CODE END ADC2_MspDeInit 1 */
    }
}

/**
 * @brief TIM_OC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_oc: TIM_OC handle pointer
 * @retval None
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim_oc)
{
    if (htim_oc->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspInit 0 */

        /* USER CODE END TIM3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
        /* USER CODE BEGIN TIM3_MspInit 1 */

        /* USER CODE END TIM3_MspInit 1 */
    }
}

/**
 * @brief TIM_OC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_oc: TIM_OC handle pointer
 * @retval None
 */
void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim_oc)
{
    if (htim_oc->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */

        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();
        /* USER CODE BEGIN TIM3_MspDeInit 1 */

        /* USER CODE END TIM3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
