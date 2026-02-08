/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32h7xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_INP10
        PA0     ------> ADC1_INP16
        PA1     ------> ADC1_INP17
        PA3     ------> ADC1_INP15
        PC5     ------> ADC1_INP8
        PB0     ------> ADC1_INP9
        */
        GPIO_InitStruct.Pin  = L_RAD_FAN_I_SNS_Pin | RR_PUMP_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = FRONT_SNS_Pin | R_INV_SNS_Pin | BMS_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = RL_PUMP_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(RL_PUMP_I_SNS_GPIO_Port, &GPIO_InitStruct);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
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

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC2 GPIO Configuration
        PC1     ------> ADC2_INP11
        PA2     ------> ADC2_INP14
        PA4     ------> ADC2_INP18
        PC4     ------> ADC2_INP4
        PB1     ------> ADC2_INP5
        */
        GPIO_InitStruct.Pin  = MISC_FUSE_SNS_Pin | DAM_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = F_INV_SNS_Pin | RSM_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = R_RAD_FAN_I_SNS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(R_RAD_FAN_I_SNS_GPIO_Port, &GPIO_InitStruct);

        /* ADC2 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
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
        PC0     ------> ADC1_INP10
        PA0     ------> ADC1_INP16
        PA1     ------> ADC1_INP17
        PA3     ------> ADC1_INP15
        PC5     ------> ADC1_INP8
        PB0     ------> ADC1_INP9
        */
        HAL_GPIO_DeInit(GPIOC, L_RAD_FAN_I_SNS_Pin | RR_PUMP_I_SNS_Pin);

        HAL_GPIO_DeInit(GPIOA, FRONT_SNS_Pin | R_INV_SNS_Pin | BMS_SNS_Pin);

        HAL_GPIO_DeInit(RL_PUMP_I_SNS_GPIO_Port, RL_PUMP_I_SNS_Pin);

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
        PC1     ------> ADC2_INP11
        PA2     ------> ADC2_INP14
        PA4     ------> ADC2_INP18
        PC4     ------> ADC2_INP4
        PB1     ------> ADC2_INP5
        */
        HAL_GPIO_DeInit(GPIOC, MISC_FUSE_SNS_Pin | DAM_SNS_Pin);

        HAL_GPIO_DeInit(GPIOA, F_INV_SNS_Pin | RSM_SNS_Pin);

        HAL_GPIO_DeInit(R_RAD_FAN_I_SNS_GPIO_Port, R_RAD_FAN_I_SNS_Pin);

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

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

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
    if (hfdcan->Instance == FDCAN1)
    {
        /* USER CODE BEGIN FDCAN1_MspInit 0 */

        /* USER CODE END FDCAN1_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInitStruct.FdcanClockSelection  = RCC_FDCANCLKSOURCE_HSE;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

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
        GPIO_InitStruct.Pin       = CAN_VEH_RX_Pin | CAN_VEH_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN FDCAN1_MspInit 1 */

        /* USER CODE END FDCAN1_MspInit 1 */
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        /* USER CODE BEGIN FDCAN3_MspInit 0 */

        /* USER CODE END FDCAN3_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInitStruct.FdcanClockSelection  = RCC_FDCANCLKSOURCE_HSE;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

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
        GPIO_InitStruct.Pin       = CAN_INV_RX_Pin | CAN_INV_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_FDCAN3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
        HAL_GPIO_DeInit(GPIOD, CAN_VEH_RX_Pin | CAN_VEH_TX_Pin);

        /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

        /* USER CODE END FDCAN1_MspDeInit 1 */
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
        HAL_GPIO_DeInit(GPIOD, CAN_INV_RX_Pin | CAN_INV_TX_Pin);

        /* USER CODE BEGIN FDCAN3_MspDeInit 1 */

        /* USER CODE END FDCAN3_MspDeInit 1 */
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
    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hi2c->Instance == I2C4)
    {
        /* USER CODE BEGIN I2C4_MspInit 0 */

        /* USER CODE END I2C4_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C4;
        PeriphClkInitStruct.I2c4ClockSelection   = RCC_I2C4CLKSOURCE_D3PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C4 GPIO Configuration
        PB8     ------> I2C4_SCL
        PB9     ------> I2C4_SDA
        */
        GPIO_InitStruct.Pin       = PWR_PUMP_SCL_Pin | PWR_PUMP_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_I2C4;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C4_CLK_ENABLE();
        /* USER CODE BEGIN I2C4_MspInit 1 */

        /* USER CODE END I2C4_MspInit 1 */
    }
    else if (hi2c->Instance == I2C5)
    {
        /* USER CODE BEGIN I2C5_MspInit 0 */

        /* USER CODE END I2C5_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C5;
        PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C1235CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**I2C5 GPIO Configuration
        PC10     ------> I2C5_SDA
        PC11     ------> I2C5_SCL
        */
        GPIO_InitStruct.Pin       = BAT_MON_SDA_Pin | BAT_MON_SCL_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C5_CLK_ENABLE();
        /* USER CODE BEGIN I2C5_MspInit 1 */

        /* USER CODE END I2C5_MspInit 1 */
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
    if (hi2c->Instance == I2C4)
    {
        /* USER CODE BEGIN I2C4_MspDeInit 0 */

        /* USER CODE END I2C4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C4_CLK_DISABLE();

        /**I2C4 GPIO Configuration
        PB8     ------> I2C4_SCL
        PB9     ------> I2C4_SDA
        */
        HAL_GPIO_DeInit(PWR_PUMP_SCL_GPIO_Port, PWR_PUMP_SCL_Pin);

        HAL_GPIO_DeInit(PWR_PUMP_SDA_GPIO_Port, PWR_PUMP_SDA_Pin);

        /* USER CODE BEGIN I2C4_MspDeInit 1 */

        /* USER CODE END I2C4_MspDeInit 1 */
    }
    else if (hi2c->Instance == I2C5)
    {
        /* USER CODE BEGIN I2C5_MspDeInit 0 */

        /* USER CODE END I2C5_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C5_CLK_DISABLE();

        /**I2C5 GPIO Configuration
        PC10     ------> I2C5_SDA
        PC11     ------> I2C5_SCL
        */
        HAL_GPIO_DeInit(BAT_MON_SDA_GPIO_Port, BAT_MON_SDA_Pin);

        HAL_GPIO_DeInit(BAT_MON_SCL_GPIO_Port, BAT_MON_SCL_Pin);

        /* USER CODE BEGIN I2C5_MspDeInit 1 */

        /* USER CODE END I2C5_MspDeInit 1 */
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
    if (hspi->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin       = IMU_SPC_Pin | IMU_MISO_Pin | IMU_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }
    else if (hspi->Instance == SPI2)
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
        GPIO_InitStruct.Pin       = RPI_SCLK_Pin | RPI_MISO_Pin | RPI_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
    if (hspi->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOA, IMU_SPC_Pin | IMU_MISO_Pin | IMU_MOSI_Pin);

        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
    else if (hspi->Instance == SPI2)
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
        HAL_GPIO_DeInit(GPIOB, RPI_SCLK_Pin | RPI_MISO_Pin | RPI_MOSI_Pin);

        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
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
    if (huart->Instance == UART8)
    {
        /* USER CODE BEGIN UART8_MspInit 0 */

        /* USER CODE END UART8_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_UART8;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_UART8_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**UART8 GPIO Configuration
        PE0     ------> UART8_RX
        PE1     ------> UART8_TX
        */
        GPIO_InitStruct.Pin       = FROM_GPS_TX_Pin | TO_CPU_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* USER CODE BEGIN UART8_MspInit 1 */

        /* USER CODE END UART8_MspInit 1 */
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
    if (huart->Instance == UART8)
    {
        /* USER CODE BEGIN UART8_MspDeInit 0 */

        /* USER CODE END UART8_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART8_CLK_DISABLE();

        /**UART8 GPIO Configuration
        PE0     ------> UART8_RX
        PE1     ------> UART8_TX
        */
        HAL_GPIO_DeInit(GPIOE, FROM_GPS_TX_Pin | TO_CPU_TX_Pin);

        /* USER CODE BEGIN UART8_MspDeInit 1 */

        /* USER CODE END UART8_MspDeInit 1 */
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
        PeriphClkInitStruct.PLL3.PLL3M           = 1;
        PeriphClkInitStruct.PLL3.PLL3N           = 24;
        PeriphClkInitStruct.PLL3.PLL3P           = 2;
        PeriphClkInitStruct.PLL3.PLL3Q           = 4;
        PeriphClkInitStruct.PLL3.PLL3R           = 4;
        PeriphClkInitStruct.PLL3.PLL3RGE         = RCC_PLL3VCIRANGE_3;
        PeriphClkInitStruct.PLL3.PLL3VCOSEL      = RCC_PLL3VCOWIDE;
        PeriphClkInitStruct.PLL3.PLL3FRACN       = 0;
        PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_PLL3;
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
