/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : stm32f3xx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_adc2;

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
    if (hadc->Instance == ADC2)
    {
        /* USER CODE BEGIN ADC2_MspInit 0 */

        /* USER CODE END ADC2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC12_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC2 GPIO Configuration
        PB2     ------> ADC2_IN12
        */
        GPIO_InitStruct.Pin  = REGEN_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(REGEN_GPIO_Port, &GPIO_InitStruct);

        /* ADC2 DMA Init */
        /* ADC2 Init */
        hdma_adc2.Instance                 = DMA2_Channel1;
        hdma_adc2.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc2.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc2.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_adc2.Init.Mode                = DMA_CIRCULAR;
        hdma_adc2.Init.Priority            = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc2);

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
    if (hadc->Instance == ADC2)
    {
        /* USER CODE BEGIN ADC2_MspDeInit 0 */

        /* USER CODE END ADC2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC12_CLK_DISABLE();

        /**ADC2 GPIO Configuration
        PB2     ------> ADC2_IN12
        */
        HAL_GPIO_DeInit(REGEN_GPIO_Port, REGEN_Pin);

        /* ADC2 DMA DeInit */
        HAL_DMA_DeInit(hadc->DMA_Handle);
        /* USER CODE BEGIN ADC2_MspDeInit 1 */

        /* USER CODE END ADC2_MspDeInit 1 */
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
    if (hcan->Instance == CAN)
    {
        /* USER CODE BEGIN CAN_MspInit 0 */

        /* USER CODE END CAN_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**CAN GPIO Configuration
        PA11     ------> CAN_RX
        PA12     ------> CAN_TX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* CAN interrupt Init */
        HAL_NVIC_SetPriority(USB_HP_CAN_TX_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USB_HP_CAN_TX_IRQn);
        HAL_NVIC_SetPriority(USB_LP_CAN_RX0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USB_LP_CAN_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN_RX1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN_RX1_IRQn);
        /* USER CODE BEGIN CAN_MspInit 1 */

        /* USER CODE END CAN_MspInit 1 */
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
    if (hcan->Instance == CAN)
    {
        /* USER CODE BEGIN CAN_MspDeInit 0 */

        /* USER CODE END CAN_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN GPIO Configuration
        PA11     ------> CAN_RX
        PA12     ------> CAN_TX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /* CAN interrupt DeInit */
        HAL_NVIC_DisableIRQ(USB_HP_CAN_TX_IRQn);
        HAL_NVIC_DisableIRQ(USB_LP_CAN_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN_RX1_IRQn);
        /* USER CODE BEGIN CAN_MspDeInit 1 */

        /* USER CODE END CAN_MspDeInit 1 */
    }
}

/**
 * @brief SPI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hspi->Instance == SPI2)
    {
        /* USER CODE BEGIN SPI2_MspInit 0 */

        /* USER CODE END SPI2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI2 GPIO Configuration
        PB13     ------> SPI2_SCK
        PB15     ------> SPI2_MOSI
        */
        GPIO_InitStruct.Pin       = SEVENSEG_SRCK_3V3_Pin | SEVENSEG_SEROUT_3V3_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* SPI2 interrupt Init */
        HAL_NVIC_SetPriority(SPI2_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
        /* USER CODE BEGIN SPI2_MspInit 1 */

        /* USER CODE END SPI2_MspInit 1 */
    }
}

/**
 * @brief SPI MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2)
    {
        /* USER CODE BEGIN SPI2_MspDeInit 0 */

        /* USER CODE END SPI2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**SPI2 GPIO Configuration
        PB13     ------> SPI2_SCK
        PB15     ------> SPI2_MOSI
        */
        HAL_GPIO_DeInit(GPIOB, SEVENSEG_SRCK_3V3_Pin | SEVENSEG_SEROUT_3V3_Pin);

        /* SPI2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(SPI2_IRQn);
        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
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
    if (htim_base->Instance == TIM2)
    {
        /* USER CODE BEGIN TIM2_MspInit 0 */

        /* USER CODE END TIM2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();
        /* USER CODE BEGIN TIM2_MspInit 1 */

        /* USER CODE END TIM2_MspInit 1 */
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
        /* USER CODE BEGIN TIM2_MspDeInit 1 */

        /* USER CODE END TIM2_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
