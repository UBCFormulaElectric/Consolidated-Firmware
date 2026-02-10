/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32h7xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_adc1;

extern DMA_HandleTypeDef hdma_adc3;

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
        __HAL_RCC_ADC12_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_INP17
        PA4     ------> ADC1_INP18
        PA5     ------> ADC1_INP19
        PA6     ------> ADC1_INP3
        PA7     ------> ADC1_INP7
        PB0     ------> ADC1_INP9
        PB1     ------> ADC1_INP5
        */
        GPIO_InitStruct.Pin =
            EMETER_THERM_SNS_Pin | TS_ISENSE_400A_Pin | TS_ISENSE_50A_Pin | FAN_ISNS_Pin | SHDN_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = TS_VSENSE_N_Pin | TS_VSENSE_P_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance                 = DMA1_Stream0;
        hdma_adc1.Init.Request             = DMA_REQUEST_ADC1;
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

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
    else if (hadc->Instance == ADC3)
    {
        /* USER CODE BEGIN ADC3_MspInit 0 */

        /* USER CODE END ADC3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC3_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**ADC3 GPIO Configuration
        PC3_C     ------> ADC3_INP1
        */
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_OPEN);

        /* ADC3 DMA Init */
        /* ADC3 Init */
        hdma_adc3.Instance                 = DMA1_Stream1;
        hdma_adc3.Init.Request             = DMA_REQUEST_ADC3;
        hdma_adc3.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc3.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_adc3.Init.Mode                = DMA_CIRCULAR;
        hdma_adc3.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_adc3.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc3);

        /* USER CODE BEGIN ADC3_MspInit 1 */

        /* USER CODE END ADC3_MspInit 1 */
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
        __HAL_RCC_ADC12_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_INP17
        PA4     ------> ADC1_INP18
        PA5     ------> ADC1_INP19
        PA6     ------> ADC1_INP3
        PA7     ------> ADC1_INP7
        PB0     ------> ADC1_INP9
        PB1     ------> ADC1_INP5
        */
        HAL_GPIO_DeInit(
            GPIOA, EMETER_THERM_SNS_Pin | TS_ISENSE_400A_Pin | TS_ISENSE_50A_Pin | FAN_ISNS_Pin | SHDN_SNS_Pin);

        HAL_GPIO_DeInit(GPIOB, TS_VSENSE_N_Pin | TS_VSENSE_P_Pin);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(hadc->DMA_Handle);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
    else if (hadc->Instance == ADC3)
    {
        /* USER CODE BEGIN ADC3_MspDeInit 0 */

        /* USER CODE END ADC3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC3_CLK_DISABLE();

        /* ADC3 DMA DeInit */
        HAL_DMA_DeInit(hadc->DMA_Handle);
        /* USER CODE BEGIN ADC3_MspDeInit 1 */

        /* USER CODE END ADC3_MspDeInit 1 */
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
        GPIO_InitStruct.Pin       = CAN1_CHRG_RX_Pin | CAN1_CHRG_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* FDCAN1 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 0);
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
        GPIO_InitStruct.Pin       = CAN1_RX_Pin | CAN1_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* FDCAN2 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 0, 0);
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
        HAL_GPIO_DeInit(GPIOD, CAN1_CHRG_RX_Pin | CAN1_CHRG_TX_Pin);

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
        HAL_GPIO_DeInit(GPIOB, CAN1_RX_Pin | CAN1_TX_Pin);

        /* FDCAN2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
        /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

        /* USER CODE END FDCAN2_MspDeInit 1 */
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
    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hsd->Instance == SDMMC1)
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
        GPIO_InitStruct.Pin       = SD_D0_Pin | SD_D1_Pin | SD_D2_Pin | SD_D3_Pin | SD_CK_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = SD_CMD_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
        HAL_GPIO_Init(SD_CMD_GPIO_Port, &GPIO_InitStruct);

        /* SDMMC1 interrupt Init */
        HAL_NVIC_SetPriority(SDMMC1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
        /* USER CODE BEGIN SDMMC1_MspInit 1 */

        /* USER CODE END SDMMC1_MspInit 1 */
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
    if (hsd->Instance == SDMMC1)
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
        HAL_GPIO_DeInit(GPIOC, SD_D0_Pin | SD_D1_Pin | SD_D2_Pin | SD_D3_Pin | SD_CK_Pin);

        HAL_GPIO_DeInit(SD_CMD_GPIO_Port, SD_CMD_Pin);

        /* SDMMC1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
        /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

        /* USER CODE END SDMMC1_MspDeInit 1 */
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
    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hspi->Instance == SPI4)
    {
        /* USER CODE BEGIN SPI4_MspInit 0 */

        /* USER CODE END SPI4_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI4;
        PeriphClkInitStruct.Spi45ClockSelection  = RCC_SPI45CLKSOURCE_HSE;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_SPI4_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**SPI4 GPIO Configuration
        PE2     ------> SPI4_SCK
        PE5     ------> SPI4_MISO
        PE6     ------> SPI4_MOSI
        */
        GPIO_InitStruct.Pin       = SPI_SCK_Pin | SPI_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = SPI_MISO_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStruct);

        /* SPI4 interrupt Init */
        HAL_NVIC_SetPriority(SPI4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SPI4_IRQn);
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
    if (hspi->Instance == SPI4)
    {
        /* USER CODE BEGIN SPI4_MspDeInit 0 */

        /* USER CODE END SPI4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI4_CLK_DISABLE();

        /**SPI4 GPIO Configuration
        PE2     ------> SPI4_SCK
        PE5     ------> SPI4_MISO
        PE6     ------> SPI4_MOSI
        */
        HAL_GPIO_DeInit(GPIOE, SPI_SCK_Pin | SPI_MISO_Pin | SPI_MOSI_Pin);

        /* SPI4 interrupt DeInit */
        HAL_NVIC_DisableIRQ(SPI4_IRQn);
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
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (htim_base->Instance == TIM1)
    {
        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        */
        GPIO_InitStruct.Pin       = IMD_M_HS_3V3_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(IMD_M_HS_3V3_GPIO_Port, &GPIO_InitStruct);

        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
        /* USER CODE BEGIN TIM1_MspInit 1 */

        /* USER CODE END TIM1_MspInit 1 */
    }
    else if (htim_base->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspInit 0 */

        /* USER CODE END TIM3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspInit 1 */

        /* USER CODE END TIM3_MspInit 1 */
    }
}

/**
 * @brief TIM_IC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_ic: TIM_IC handle pointer
 * @retval None
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim_ic)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (htim_ic->Instance == TIM5)
    {
        /* USER CODE BEGIN TIM5_MspInit 0 */

        /* USER CODE END TIM5_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM5_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM5 GPIO Configuration
        PA3     ------> TIM5_CH4
        */
        GPIO_InitStruct.Pin       = nEVSE_I_LIM_PWM_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
        HAL_GPIO_Init(nEVSE_I_LIM_PWM_GPIO_Port, &GPIO_InitStruct);

        /* TIM5 interrupt Init */
        HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
        /* USER CODE BEGIN TIM5_MspInit 1 */

        /* USER CODE END TIM5_MspInit 1 */
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
    if (htim_base->Instance == TIM1)
    {
        /* USER CODE BEGIN TIM1_MspDeInit 0 */

        /* USER CODE END TIM1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();

        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        */
        HAL_GPIO_DeInit(IMD_M_HS_3V3_GPIO_Port, IMD_M_HS_3V3_Pin);

        /* TIM1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
        /* USER CODE BEGIN TIM1_MspDeInit 1 */

        /* USER CODE END TIM1_MspDeInit 1 */
    }
    else if (htim_base->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */

        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /* TIM3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspDeInit 1 */

        /* USER CODE END TIM3_MspDeInit 1 */
    }
}

/**
 * @brief TIM_IC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_ic: TIM_IC handle pointer
 * @retval None
 */
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim_ic)
{
    if (htim_ic->Instance == TIM5)
    {
        /* USER CODE BEGIN TIM5_MspDeInit 0 */

        /* USER CODE END TIM5_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM5_CLK_DISABLE();

        /**TIM5 GPIO Configuration
        PA3     ------> TIM5_CH4
        */
        HAL_GPIO_DeInit(nEVSE_I_LIM_PWM_GPIO_Port, nEVSE_I_LIM_PWM_Pin);

        /* TIM5 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM5_IRQn);
        /* USER CODE BEGIN TIM5_MspDeInit 1 */

        /* USER CODE END TIM5_MspDeInit 1 */
    }
}

/**
 * @brief PCD MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hpcd: PCD handle pointer
 * @retval None
 */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hpcd->Instance == USB_OTG_HS)
    {
        /* USER CODE BEGIN USB_OTG_HS_MspInit 0 */

        /* USER CODE END USB_OTG_HS_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /** Enable USB Voltage detector
         */
        HAL_PWREx_EnableUSBVoltageDetector();

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
        /* USB_OTG_HS interrupt Init */
        HAL_NVIC_SetPriority(OTG_HS_EP1_OUT_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(OTG_HS_EP1_OUT_IRQn);
        HAL_NVIC_SetPriority(OTG_HS_EP1_IN_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(OTG_HS_EP1_IN_IRQn);
        HAL_NVIC_SetPriority(OTG_HS_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
        /* USER CODE BEGIN USB_OTG_HS_MspInit 1 */

        /* USER CODE END USB_OTG_HS_MspInit 1 */
    }
}

/**
 * @brief PCD MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hpcd: PCD handle pointer
 * @retval None
 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
    if (hpcd->Instance == USB_OTG_HS)
    {
        /* USER CODE BEGIN USB_OTG_HS_MspDeInit 0 */

        /* USER CODE END USB_OTG_HS_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_HS_CLK_DISABLE();

        /* USB_OTG_HS interrupt DeInit */
        HAL_NVIC_DisableIRQ(OTG_HS_EP1_OUT_IRQn);
        HAL_NVIC_DisableIRQ(OTG_HS_EP1_IN_IRQn);
        HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
        /* USER CODE BEGIN USB_OTG_HS_MspDeInit 1 */

        /* USER CODE END USB_OTG_HS_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
