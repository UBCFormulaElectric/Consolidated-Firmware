/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_hardFaultHandler.h"
#include "SEGGER_SYSVIEW_FreeRTOS.h"
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
extern DMA_HandleTypeDef   hdma_adc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern TIM_HandleTypeDef   htim3;
extern DMA_HandleTypeDef   hdma_usart2_rx;
extern UART_HandleTypeDef  huart7;
extern TIM_HandleTypeDef   htim6;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
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
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles DMA1 stream0 global interrupt.
 */
void DMA1_Stream0_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END DMA1_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc1);
    /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
 * @brief This function handles DMA1 stream1 global interrupt.
 */
void DMA1_Stream1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END DMA1_Stream1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart2_rx);
    /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
 * @brief This function handles FDCAN1 interrupt 0.
 */
void FDCAN1_IT0_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN1_IT0_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END FDCAN1_IT0_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan1);
    /* USER CODE BEGIN FDCAN1_IT0_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END FDCAN1_IT0_IRQn 1 */
}

/**
 * @brief This function handles FDCAN1 interrupt 1.
 */
void FDCAN1_IT1_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN1_IT1_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END FDCAN1_IT1_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan1);
    /* USER CODE BEGIN FDCAN1_IT1_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END FDCAN1_IT1_IRQn 1 */
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    /* USER CODE BEGIN TIM3_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END TIM3_IRQn 1 */
}

/**
 * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
 */
void TIM6_DAC_IRQHandler(void)
{
    /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END TIM6_DAC_IRQn 0 */
    HAL_TIM_IRQHandler(&htim6);
    /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
 * @brief This function handles UART7 global interrupt.
 */
void UART7_IRQHandler(void)
{
    /* USER CODE BEGIN UART7_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END UART7_IRQn 0 */
    HAL_UART_IRQHandler(&huart7);
    /* USER CODE BEGIN UART7_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END UART7_IRQn 1 */
}

/**
 * @brief This function handles ADC3 global interrupt.
 */
void ADC3_IRQHandler(void)
{
    /* USER CODE BEGIN ADC3_IRQn 0 */
    traceISR_ENTER();
    /* USER CODE END ADC3_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc3);
    /* USER CODE BEGIN ADC3_IRQn 1 */
    traceISR_EXIT();
    /* USER CODE END ADC3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
