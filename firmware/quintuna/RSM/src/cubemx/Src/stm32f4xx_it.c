/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_hardFaultHandler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan2;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */
    hw_hardFaultHandler_handleFault();
    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles ADC1 global interrupt.
 */
void ADC_IRQHandler(void)
{
    /* USER CODE BEGIN ADC_IRQn 0 */

    /* USER CODE END ADC_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc1);
    /* USER CODE BEGIN ADC_IRQn 1 */
    traceISR_ENTER();
    traceISR_ENTER();

    /* USER CODE END ADC_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc1);
    /* USER CODE BEGIN ADC_IRQn 1 */

    traceISR_ENTER();
    traceISR_ENTER();

    /* USER CODE END ADC_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc1);
    /* USER CODE BEGIN ADC_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END ADC_IRQn 1 */
}

/**
 * @brief This function handles TIM2 global interrupt.
 */
void TIM2_IRQHandler(void)
{
    /* USER CODE BEGIN TIM2_IRQn 0 */
    // traceISR_ENTER();
    /* USER CODE END TIM2_IRQn 0 */
    HAL_TIM_IRQHandler(&htim2);
    /* USER CODE BEGIN TIM2_IRQn 1 */
    // traceISR_EXIT();
    /* USER CODE END TIM2_IRQn 1 */
}

/**
 * @brief This function handles TIM4 global interrupt.
 */
void TIM4_IRQHandler(void)
{
    /* USER CODE BEGIN TIM4_IRQn 0 */

    /* USER CODE END TIM4_IRQn 0 */
    HAL_TIM_IRQHandler(&htim4);
    /* USER CODE BEGIN TIM4_IRQn 1 */

    /* USER CODE END TIM4_IRQn 1 */
}

/**
 * @brief This function handles I2C1 event interrupt.
 */
void I2C1_EV_IRQHandler(void)
{
    /* USER CODE BEGIN I2C1_EV_IRQn 0 */

    /* USER CODE END I2C1_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_EV_IRQn 1 */

    /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
 * @brief This function handles I2C1 error interrupt.
 */
void I2C1_ER_IRQHandler(void)
{
    /* USER CODE BEGIN I2C1_ER_IRQn 0 */

    /* USER CODE END I2C1_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_ER_IRQn 1 */

    /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI15_10_IRQn 0 */

    /* USER CODE END EXTI15_10_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(IMU_INT1_Pin);
    HAL_GPIO_EXTI_IRQHandler(IMU_INT2_Pin);
    /* USER CODE BEGIN EXTI15_10_IRQn 1 */

    /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
 * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
 */
void TIM8_UP_TIM13_IRQHandler(void)
{
    /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */

    /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
    HAL_TIM_IRQHandler(&htim13);
    /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

    /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}

/**
 * @brief This function handles TIM6 global interrupt.
 */
void TIM6_IRQHandler(void)
{
    /* USER CODE BEGIN TIM6_IRQn 0 */

    /* USER CODE END TIM6_IRQn 0 */
    HAL_TIM_IRQHandler(&htim6);
    /* USER CODE BEGIN TIM6_IRQn 1 */

    /* USER CODE END TIM6_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream0 global interrupt.
 */
void DMA2_Stream0_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END DMA2_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc1);
    /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
 * @brief This function handles CAN2 TX interrupts.
 */
void CAN2_TX_IRQHandler(void)
{
    /* USER CODE BEGIN CAN2_TX_IRQn 0 */

    /* USER CODE END CAN2_TX_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan2);
    /* USER CODE BEGIN CAN2_TX_IRQn 1 */

    /* USER CODE END CAN2_TX_IRQn 1 */
}

/**
 * @brief This function handles CAN2 RX0 interrupts.
 */
void CAN2_RX0_IRQHandler(void)
{
    /* USER CODE BEGIN CAN2_RX0_IRQn 0 */

    /* USER CODE END CAN2_RX0_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan2);
    /* USER CODE BEGIN CAN2_RX0_IRQn 1 */

    /* USER CODE END CAN2_RX0_IRQn 1 */
}

/**
 * @brief This function handles CAN2 RX1 interrupt.
 */
void CAN2_RX1_IRQHandler(void)
{
    /* USER CODE BEGIN CAN2_RX1_IRQn 0 */

    /* USER CODE END CAN2_RX1_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan2);
    /* USER CODE BEGIN CAN2_RX1_IRQn 1 */

    /* USER CODE END CAN2_RX1_IRQn 1 */
}

/**
 * @brief This function handles CAN2 SCE interrupt.
 */
void CAN2_SCE_IRQHandler(void)
{
    /* USER CODE BEGIN CAN2_SCE_IRQn 0 */

    /* USER CODE END CAN2_SCE_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan2);
    /* USER CODE BEGIN CAN2_SCE_IRQn 1 */

    /* USER CODE END CAN2_SCE_IRQn 1 */
}

/**
 * @brief This function handles USB On The Go FS global interrupt.
 */
void OTG_FS_IRQHandler(void)
{
    /* USER CODE BEGIN OTG_FS_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END OTG_FS_IRQn 0 */
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
    /* USER CODE BEGIN OTG_FS_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END OTG_FS_IRQn 1 */
}

/**
 * @brief This function handles I2C3 event interrupt.
 */
void I2C3_EV_IRQHandler(void)
{
    /* USER CODE BEGIN I2C3_EV_IRQn 0 */

    /* USER CODE END I2C3_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c3);
    /* USER CODE BEGIN I2C3_EV_IRQn 1 */

    /* USER CODE END I2C3_EV_IRQn 1 */
}

/**
 * @brief This function handles I2C3 error interrupt.
 */
void I2C3_ER_IRQHandler(void)
{
    /* USER CODE BEGIN I2C3_ER_IRQn 0 */

    /* USER CODE END I2C3_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c3);
    /* USER CODE BEGIN I2C3_ER_IRQn 1 */

    /* USER CODE END I2C3_ER_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
