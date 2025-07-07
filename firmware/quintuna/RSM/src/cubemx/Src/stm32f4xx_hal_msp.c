
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
extern DMA_HandleTypeDef hdma_adc1;

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
        PC2     ------> ADC1_IN12
        PA0     ------> ADC1_IN0
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        */
        GPIO_InitStruct.Pin  = BPS_R_3V3_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(BPS_R_3V3_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = LC3_OUT_Pin | SUSP_TRAVEL_RL_3V3_Pin | SUSP_TRAVEL_RR_3V3_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance                 = DMA2_Stream0;
        hdma_adc1.Init.Channel             = DMA_CHANNEL_0;
        hdma_adc1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_adc1.Init.Mode                = DMA_CIRCULAR;
        hdma_adc1.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_adc1.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
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
        PC2     ------> ADC1_IN12
        PA0     ------> ADC1_IN0
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        */
        HAL_GPIO_DeInit(BPS_R_3V3_GPIO_Port, BPS_R_3V3_Pin);

        HAL_GPIO_DeInit(GPIOA, LC3_OUT_Pin | SUSP_TRAVEL_RL_3V3_Pin | SUSP_TRAVEL_RR_3V3_Pin);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(hadc->DMA_Handle);

        /* ADC1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
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
    if (hcan->Instance == CAN2)
    {
        /* USER CODE BEGIN CAN2_MspInit 0 */

        /* USER CODE END CAN2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();
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

        /* CAN2 interrupt Init */
        HAL_NVIC_SetPriority(CAN2_TX_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
        HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
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
    if (hcan->Instance == CAN2)
    {
        /* USER CODE BEGIN CAN2_MspDeInit 0 */

        /* USER CODE END CAN2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();
        __HAL_RCC_CAN2_CLK_DISABLE();

        /**CAN2 GPIO Configuration
        PB12     ------> CAN2_RX
        PB13     ------> CAN2_TX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13);

        /* CAN2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
        HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
        HAL_NVIC_DisableIRQ(CAN2_SCE_IRQn);
        /* USER CODE BEGIN CAN2_MspDeInit 1 */

        /* USER CODE END CAN2_MspDeInit 1 */
    }
}

/**
 * @brief I2C MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hi2c->Instance == I2C1)
    {
        /* USER CODE BEGIN I2C1_MspInit 0 */

        /* USER CODE END I2C1_MspInit 0 */

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin       = IMU_SCL_Pin | IMU_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        /* I2C1 interrupt Init */
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
        /* USER CODE BEGIN I2C1_MspInit 1 */

        /* USER CODE END I2C1_MspInit 1 */
    }
    else if (hi2c->Instance == I2C3)
    {
        /* USER CODE BEGIN I2C3_MspInit 0 */

        /* USER CODE END I2C3_MspInit 0 */

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**I2C3 GPIO Configuration
        PC9     ------> I2C3_SDA
        PA8     ------> I2C3_SCL
        */
        GPIO_InitStruct.Pin       = R_PUMP_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(R_PUMP_SDA_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = R_PUMP_SCL_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(R_PUMP_SCL_GPIO_Port, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C3_CLK_ENABLE();
        /* I2C3 interrupt Init */
        HAL_NVIC_SetPriority(I2C3_EV_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_SetPriority(I2C3_ER_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
        /* USER CODE BEGIN I2C3_MspInit 1 */

        /* USER CODE END I2C3_MspInit 1 */
    }
}

/**
 * @brief I2C MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        /* USER CODE BEGIN I2C1_MspDeInit 0 */

        /* USER CODE END I2C1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        HAL_GPIO_DeInit(IMU_SCL_GPIO_Port, IMU_SCL_Pin);

        HAL_GPIO_DeInit(IMU_SDA_GPIO_Port, IMU_SDA_Pin);

        /* I2C1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
        /* USER CODE BEGIN I2C1_MspDeInit 1 */

        /* USER CODE END I2C1_MspDeInit 1 */
    }
    else if (hi2c->Instance == I2C3)
    {
        /* USER CODE BEGIN I2C3_MspDeInit 0 */

        /* USER CODE END I2C3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C3_CLK_DISABLE();

        /**I2C3 GPIO Configuration
        PC9     ------> I2C3_SDA
        PA8     ------> I2C3_SCL
        */
        HAL_GPIO_DeInit(R_PUMP_SDA_GPIO_Port, R_PUMP_SDA_Pin);

        HAL_GPIO_DeInit(R_PUMP_SCL_GPIO_Port, R_PUMP_SCL_Pin);

        /* I2C3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
        /* USER CODE BEGIN I2C3_MspDeInit 1 */

        /* USER CODE END I2C3_MspDeInit 1 */
    }
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (htim_base->Instance == TIM2)
    {
        /* USER CODE BEGIN TIM2_MspInit 0 */

        /* USER CODE END TIM2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();
        /* TIM2 interrupt Init */
        HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspInit 1 */

        /* USER CODE END TIM2_MspInit 1 */
    }
    else if (htim_base->Instance == TIM4)
    {
        /* USER CODE BEGIN TIM4_MspInit 0 */

        /* USER CODE END TIM4_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM4_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM4 GPIO Configuration
        PB8     ------> TIM4_CH3
        */
        GPIO_InitStruct.Pin       = FLOW_METER_5V_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
        HAL_GPIO_Init(FLOW_METER_5V_GPIO_Port, &GPIO_InitStruct);

        /* TIM4 interrupt Init */
        HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
        /* USER CODE BEGIN TIM4_MspInit 1 */

        /* USER CODE END TIM4_MspInit 1 */
    }
    else if (htim_base->Instance == TIM13)
    {
        /* USER CODE BEGIN TIM13_MspInit 0 */

        /* USER CODE END TIM13_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM13_CLK_ENABLE();
        /* TIM13 interrupt Init */
        HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
        /* USER CODE BEGIN TIM13_MspInit 1 */

        /* USER CODE END TIM13_MspInit 1 */
    }
}

/**
 * @brief TIM_Base MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM2)
    {
        /* USER CODE BEGIN TIM2_MspDeInit 0 */

        /* USER CODE END TIM2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();

        /* TIM2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspDeInit 1 */

        /* USER CODE END TIM2_MspDeInit 1 */
    }
    else if (htim_base->Instance == TIM4)
    {
        /* USER CODE BEGIN TIM4_MspDeInit 0 */

        /* USER CODE END TIM4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM4_CLK_DISABLE();

        /**TIM4 GPIO Configuration
        PB8     ------> TIM4_CH3
        */
        HAL_GPIO_DeInit(FLOW_METER_5V_GPIO_Port, FLOW_METER_5V_Pin);

        /* TIM4 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM4_IRQn);
        /* USER CODE BEGIN TIM4_MspDeInit 1 */

        /* USER CODE END TIM4_MspDeInit 1 */
    }
    else if (htim_base->Instance == TIM13)
    {
        /* USER CODE BEGIN TIM13_MspDeInit 0 */

        /* USER CODE END TIM13_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM13_CLK_DISABLE();

        /* TIM13 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
        /* USER CODE BEGIN TIM13_MspDeInit 1 */

        /* USER CODE END TIM13_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
