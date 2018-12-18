/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : stm32f3xx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2018 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *notice, this list of conditions and the following disclaimer in the
 *documentation and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGE.
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
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PC2     ------> ADC1_IN8
        PC3     ------> ADC1_IN9
        PA0     ------> ADC1_IN1
        PA1     ------> ADC1_IN2
        PA2     ------> ADC1_IN3
        PA3     ------> ADC1_IN4
        PA4     ------> ADC1_IN5
        PA6     ------> ADC1_IN10
        */
        GPIO_InitStruct.Pin  = GLV_SENSE_Pin | VBAT_SENSE_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = EFUSE_IS_1_Pin | EFUSE_IS_2_Pin | EFUSE_IS_3_Pin |
                              EFUSE_IS_4_Pin | EFUSE_IS_5_Pin | FLYWIRE_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance                 = DMA1_Channel1;
        hdma_adc1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_adc1.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
        hdma_adc1.Init.Mode                = DMA_CIRCULAR;
        hdma_adc1.Init.Priority            = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

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
        PC2     ------> ADC1_IN8
        PC3     ------> ADC1_IN9
        PA0     ------> ADC1_IN1
        PA1     ------> ADC1_IN2
        PA2     ------> ADC1_IN3
        PA3     ------> ADC1_IN4
        PA4     ------> ADC1_IN5
        PA6     ------> ADC1_IN10
        */
        HAL_GPIO_DeInit(GPIOC, GLV_SENSE_Pin | VBAT_SENSE_Pin);

        HAL_GPIO_DeInit(
            GPIOA, EFUSE_IS_1_Pin | EFUSE_IS_2_Pin | EFUSE_IS_3_Pin |
                       EFUSE_IS_4_Pin | EFUSE_IS_5_Pin | FLYWIRE_Pin);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(hadc->DMA_Handle);
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
    GPIO_InitTypeDef GPIO_InitStruct = {0};
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
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* CAN interrupt Init */
        HAL_NVIC_SetPriority(USB_HP_CAN_TX_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USB_HP_CAN_TX_IRQn);
        HAL_NVIC_SetPriority(USB_LP_CAN_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USB_LP_CAN_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN_RX1_IRQn, 0, 0);
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
        /* TIM2 interrupt Init */
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspInit 1 */

        /* USER CODE END TIM2_MspInit 1 */
    }
    else if (htim_base->Instance == TIM17)
    {
        /* USER CODE BEGIN TIM17_MspInit 0 */

        /* USER CODE END TIM17_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM17_CLK_ENABLE();
        /* TIM17 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
        /* USER CODE BEGIN TIM17_MspInit 1 */

        /* USER CODE END TIM17_MspInit 1 */
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
    else if (htim_base->Instance == TIM17)
    {
        /* USER CODE BEGIN TIM17_MspDeInit 0 */

        /* USER CODE END TIM6_MspDeInit 1 */
    }
    else if (htim_base->Instance == TIM17)
    {
        /* USER CODE BEGIN TIM17_MspDeInit 0 */

        /* USER CODE END TIM17_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM17_CLK_DISABLE();

        /* TIM17 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM17_IRQn);
        /* USER CODE BEGIN TIM17_MspDeInit 1 */

        /* USER CODE END TIM17_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
