/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : stm32f7xx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

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

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_IN1
        PA3     ------> ADC1_IN3
        PC5     ------> ADC1_IN15
        */
        GPIO_InitStruct.Pin  = PHA_CUR_ADC_Pin | PHB_CUR_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = PHC_CUR_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(PHC_CUR_ADC_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
    else if (hadc->Instance == ADC2)
    {
        /* USER CODE BEGIN ADC2_MspInit 0 */

        /* USER CODE END ADC2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**ADC2 GPIO Configuration
        PA0/WKUP     ------> ADC2_IN0
        PA2     ------> ADC2_IN2
        PA6     ------> ADC2_IN6
        PC4     ------> ADC2_IN14
        */
        GPIO_InitStruct.Pin =
            MOD_TEMP_ADC_Pin | VBUS_SENSE_ADC_Pin | MOTOR_TEMP_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = GPIOA_1_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA_1_GPIO_Port, &GPIO_InitStruct);

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
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_IN1
        PA3     ------> ADC1_IN3
        PC5     ------> ADC1_IN15
        */
        HAL_GPIO_DeInit(GPIOA, PHA_CUR_ADC_Pin | PHB_CUR_ADC_Pin);

        HAL_GPIO_DeInit(PHC_CUR_ADC_GPIO_Port, PHC_CUR_ADC_Pin);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
    else if (hadc->Instance == ADC2)
    {
        /* USER CODE BEGIN ADC2_MspDeInit 0 */

        /* USER CODE END ADC2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC2_CLK_DISABLE();

        /**ADC2 GPIO Configuration
        PA0/WKUP     ------> ADC2_IN0
        PA2     ------> ADC2_IN2
        PA6     ------> ADC2_IN6
        PC4     ------> ADC2_IN14
        */
        HAL_GPIO_DeInit(
            GPIOA, MOD_TEMP_ADC_Pin | VBUS_SENSE_ADC_Pin | MOTOR_TEMP_ADC_Pin);

        HAL_GPIO_DeInit(GPIOA_1_GPIO_Port, GPIOA_1_Pin);

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
    if (hcan->Instance == CAN1)
    {
        /* USER CODE BEGIN CAN1_MspInit 0 */

        /* USER CODE END CAN1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**CAN1 GPIO Configuration
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
        GPIO_InitStruct.Pin       = CAN_RX_Pin | CAN_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
        HAL_GPIO_DeInit(GPIOD, CAN_RX_Pin | CAN_TX_Pin);

        /* USER CODE BEGIN CAN1_MspDeInit 1 */

        /* USER CODE END CAN1_MspDeInit 1 */
    }
}

/**
 * @brief DAC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hdac->Instance == DAC)
    {
        /* USER CODE BEGIN DAC_MspInit 0 */

        /* USER CODE END DAC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_DAC_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        PA5     ------> DAC_OUT2
        */
        GPIO_InitStruct.Pin  = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* DAC interrupt Init */
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        /* USER CODE BEGIN DAC_MspInit 1 */

        /* USER CODE END DAC_MspInit 1 */
    }
}

/**
 * @brief DAC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
    if (hdac->Instance == DAC)
    {
        /* USER CODE BEGIN DAC_MspDeInit 0 */

        /* USER CODE END DAC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DAC_CLK_DISABLE();

        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        PA5     ------> DAC_OUT2
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_5);

        /* DAC interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        /* USER CODE BEGIN DAC_MspDeInit 1 */

        /* USER CODE END DAC_MspDeInit 1 */
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
        PB14     ------> SPI2_MISO
        PB15     ------> SPI2_MOSI
        */
        GPIO_InitStruct.Pin =
            GDRV_SPI_CLK_Pin | GDRV_SPI_MISO_Pin | GDRV_SPI_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI2_MspInit 1 */

        /* USER CODE END SPI2_MspInit 1 */
    }
    else if (hspi->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspInit 0 */

        /* USER CODE END SPI3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI3_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PA15     ------> SPI3_NSS
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */
        GPIO_InitStruct.Pin       = ENDAT_CLK_EN_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(ENDAT_CLK_EN_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin =
            ENDAT_CLK_TX_Pin | ENDAT_DATA_RX_Pin | ENDAT_DATA_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI3_MspInit 1 */

        /* USER CODE END SPI3_MspInit 1 */
    }
    else if (hspi->Instance == SPI4)
    {
        /* USER CODE BEGIN SPI4_MspInit 0 */

        /* USER CODE END SPI4_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI4_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**SPI4 GPIO Configuration
        PE2     ------> SPI4_SCK
        PE4     ------> SPI4_NSS
        PE5     ------> SPI4_MISO
        PE6     ------> SPI4_MOSI
        */
        GPIO_InitStruct.Pin = FLASH_SPI_CLK_Pin | FLASH_SPI_CS_Pin |
                              FLASH_SPI_MISO_Pin | FLASH_SPI_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI4_MspInit 1 */

        /* USER CODE END SPI4_MspInit 1 */
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
        PB14     ------> SPI2_MISO
        PB15     ------> SPI2_MOSI
        */
        HAL_GPIO_DeInit(
            GPIOB, GDRV_SPI_CLK_Pin | GDRV_SPI_MISO_Pin | GDRV_SPI_MOSI_Pin);

        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
    }
    else if (hspi->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspDeInit 0 */

        /* USER CODE END SPI3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI3_CLK_DISABLE();

        /**SPI3 GPIO Configuration
        PA15     ------> SPI3_NSS
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */
        HAL_GPIO_DeInit(ENDAT_CLK_EN_GPIO_Port, ENDAT_CLK_EN_Pin);

        HAL_GPIO_DeInit(
            GPIOC, ENDAT_CLK_TX_Pin | ENDAT_DATA_RX_Pin | ENDAT_DATA_TX_Pin);

        /* USER CODE BEGIN SPI3_MspDeInit 1 */

        /* USER CODE END SPI3_MspDeInit 1 */
    }
    else if (hspi->Instance == SPI4)
    {
        /* USER CODE BEGIN SPI4_MspDeInit 0 */

        /* USER CODE END SPI4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI4_CLK_DISABLE();

        /**SPI4 GPIO Configuration
        PE2     ------> SPI4_SCK
        PE4     ------> SPI4_NSS
        PE5     ------> SPI4_MISO
        PE6     ------> SPI4_MOSI
        */
        HAL_GPIO_DeInit(
            GPIOE, FLASH_SPI_CLK_Pin | FLASH_SPI_CS_Pin | FLASH_SPI_MISO_Pin |
                       FLASH_SPI_MOSI_Pin);

        /* USER CODE BEGIN SPI4_MspDeInit 1 */

        /* USER CODE END SPI4_MspDeInit 1 */
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
    if (htim_base->Instance == TIM8)
    {
        /* USER CODE BEGIN TIM8_MspInit 0 */

        /* USER CODE END TIM8_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM8_CLK_ENABLE();
        /* USER CODE BEGIN TIM8_MspInit 1 */

        /* USER CODE END TIM8_MspInit 1 */
    }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (htim->Instance == TIM8)
    {
        /* USER CODE BEGIN TIM8_MspPostInit 0 */

        /* USER CODE END TIM8_MspPostInit 0 */

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**TIM8 GPIO Configuration
        PA7     ------> TIM8_CH1N
        PB0     ------> TIM8_CH2N
        PB1     ------> TIM8_CH3N
        PC6     ------> TIM8_CH1
        PC7     ------> TIM8_CH2
        PC8     ------> TIM8_CH3
        */
        GPIO_InitStruct.Pin       = PHB_HIN_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
        HAL_GPIO_Init(PHB_HIN_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = PHA_HIN_Pin | PHC_HIN_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = PHB_LIN_Pin | PHA_LIN_Pin | PHC_LIN_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM8_MspPostInit 1 */

        /* USER CODE END TIM8_MspPostInit 1 */
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
    if (htim_base->Instance == TIM8)
    {
        /* USER CODE BEGIN TIM8_MspDeInit 0 */

        /* USER CODE END TIM8_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM8_CLK_DISABLE();
        /* USER CODE BEGIN TIM8_MspDeInit 1 */

        /* USER CODE END TIM8_MspDeInit 1 */
    }
}

/**
 * @brief HCD MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hhcd: HCD handle pointer
 * @retval None
 */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hhcd->Instance == USB_OTG_FS)
    {
        /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

        /* USER CODE END USB_OTG_FS_MspInit 0 */

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USB_OTG_FS GPIO Configuration
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        GPIO_InitStruct.Pin       = USB_DM_Pin | USB_DP_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
        /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

        /* USER CODE END USB_OTG_FS_MspInit 1 */
    }
}

/**
 * @brief HCD MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hhcd: HCD handle pointer
 * @retval None
 */
void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)
{
    if (hhcd->Instance == USB_OTG_FS)
    {
        /* USER CODE BEGIN USB_OTG_FS_MspDeInit 0 */

        /* USER CODE END USB_OTG_FS_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

        /**USB_OTG_FS GPIO Configuration
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        HAL_GPIO_DeInit(GPIOA, USB_DM_Pin | USB_DP_Pin);

        /* USER CODE BEGIN USB_OTG_FS_MspDeInit 1 */

        /* USER CODE END USB_OTG_FS_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
