/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
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
#include "stm32h7xx_it.h"
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
extern PCD_HandleTypeDef   hpcd_USB_OTG_HS;
extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc2;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;
extern I2C_HandleTypeDef   hi2c2;
extern I2C_HandleTypeDef   hi2c3;
extern I2C_HandleTypeDef   hi2c4;
extern I2C_HandleTypeDef   hi2c5;
extern UART_HandleTypeDef  huart8;
extern TIM_HandleTypeDef   htim2;

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
 * @brief This function handles ADC1 and ADC2 global interrupts.
 */
void ADC_IRQHandler(void)
{
    /* USER CODE BEGIN ADC_IRQn 0 */

    /* USER CODE END ADC_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc1);
    HAL_ADC_IRQHandler(&hadc2);
    /* USER CODE BEGIN ADC_IRQn 1 */

    /* USER CODE END ADC_IRQn 1 */
}

/**
 * @brief This function handles FDCAN1 interrupt 0.
 */
void FDCAN1_IT0_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN1_IT0_IRQn 0 */

    /* USER CODE END FDCAN1_IT0_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan1);
    /* USER CODE BEGIN FDCAN1_IT0_IRQn 1 */

    /* USER CODE END FDCAN1_IT0_IRQn 1 */
}

/**
 * @brief This function handles FDCAN2 interrupt 0.
 */
void FDCAN2_IT0_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN2_IT0_IRQn 0 */

    /* USER CODE END FDCAN2_IT0_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan2);
    /* USER CODE BEGIN FDCAN2_IT0_IRQn 1 */

    /* USER CODE END FDCAN2_IT0_IRQn 1 */
}

/**
 * @brief This function handles FDCAN1 interrupt 1.
 */
void FDCAN1_IT1_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN1_IT1_IRQn 0 */

    /* USER CODE END FDCAN1_IT1_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan1);
    /* USER CODE BEGIN FDCAN1_IT1_IRQn 1 */

    /* USER CODE END FDCAN1_IT1_IRQn 1 */
}

/**
 * @brief This function handles FDCAN2 interrupt 1.
 */
void FDCAN2_IT1_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN2_IT1_IRQn 0 */

    /* USER CODE END FDCAN2_IT1_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan2);
    /* USER CODE BEGIN FDCAN2_IT1_IRQn 1 */

    /* USER CODE END FDCAN2_IT1_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[9:5] interrupts.
 */
void EXTI9_5_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI9_5_IRQn 0 */

    /* USER CODE END EXTI9_5_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(PWR_MTR_nALERT_Pin);
    HAL_GPIO_EXTI_IRQHandler(IMU_INT1_Pin);
    /* USER CODE BEGIN EXTI9_5_IRQn 1 */

    /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
 * @brief This function handles TIM2 global interrupt.
 */
void TIM2_IRQHandler(void)
{
    /* USER CODE BEGIN TIM2_IRQn 0 */

    /* USER CODE END TIM2_IRQn 0 */
    HAL_TIM_IRQHandler(&htim2);
    /* USER CODE BEGIN TIM2_IRQn 1 */

    /* USER CODE END TIM2_IRQn 1 */
}

/**
 * @brief This function handles I2C2 event interrupt.
 */
void I2C2_EV_IRQHandler(void)
{
    /* USER CODE BEGIN I2C2_EV_IRQn 0 */

    /* USER CODE END I2C2_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c2);
    /* USER CODE BEGIN I2C2_EV_IRQn 1 */

    /* USER CODE END I2C2_EV_IRQn 1 */
}

/**
 * @brief This function handles I2C2 error interrupt.
 */
void I2C2_ER_IRQHandler(void)
{
    /* USER CODE BEGIN I2C2_ER_IRQn 0 */

    /* USER CODE END I2C2_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c2);
    /* USER CODE BEGIN I2C2_ER_IRQn 1 */

    /* USER CODE END I2C2_ER_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI15_10_IRQn 0 */

    /* USER CODE END EXTI15_10_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(IMU_INT2_Pin);
    HAL_GPIO_EXTI_IRQHandler(BAT_MTR_nALERT_Pin);
    /* USER CODE BEGIN EXTI15_10_IRQn 1 */

    /* USER CODE END EXTI15_10_IRQn 1 */
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

/**
 * @brief This function handles USB On The Go HS global interrupt.
 */
void OTG_HS_IRQHandler(void)
{
    /* USER CODE BEGIN OTG_HS_IRQn 0 */

    /* USER CODE END OTG_HS_IRQn 0 */
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
    /* USER CODE BEGIN OTG_HS_IRQn 1 */

    /* USER CODE END OTG_HS_IRQn 1 */
}

/**
 * @brief This function handles UART8 global interrupt.
 */
void UART8_IRQHandler(void)
{
    /* USER CODE BEGIN UART8_IRQn 0 */

    /* USER CODE END UART8_IRQn 0 */
    HAL_UART_IRQHandler(&huart8);
    /* USER CODE BEGIN UART8_IRQn 1 */

    /* USER CODE END UART8_IRQn 1 */
}

/**
 * @brief This function handles I2C4 event interrupt.
 */
void I2C4_EV_IRQHandler(void)
{
    /* USER CODE BEGIN I2C4_EV_IRQn 0 */

    /* USER CODE END I2C4_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c4);
    /* USER CODE BEGIN I2C4_EV_IRQn 1 */

    /* USER CODE END I2C4_EV_IRQn 1 */
}

/**
 * @brief This function handles I2C4 error interrupt.
 */
void I2C4_ER_IRQHandler(void)
{
    /* USER CODE BEGIN I2C4_ER_IRQn 0 */

    /* USER CODE END I2C4_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c4);
    /* USER CODE BEGIN I2C4_ER_IRQn 1 */

    /* USER CODE END I2C4_ER_IRQn 1 */
}

/**
 * @brief This function handles I2C5 event interrupt.
 */
void I2C5_EV_IRQHandler(void)
{
    /* USER CODE BEGIN I2C5_EV_IRQn 0 */

    /* USER CODE END I2C5_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c5);
    /* USER CODE BEGIN I2C5_EV_IRQn 1 */

    /* USER CODE END I2C5_EV_IRQn 1 */
}

/**
 * @brief This function handles I2C5 error interrupt.
 */
void I2C5_ER_IRQHandler(void)
{
    /* USER CODE BEGIN I2C5_ER_IRQn 0 */

    /* USER CODE END I2C5_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c5);
    /* USER CODE BEGIN I2C5_ER_IRQn 1 */

    /* USER CODE END I2C5_ER_IRQn 1 */
}

/**
 * @brief This function handles FDCAN3 interrupt 0.
 */
void FDCAN3_IT0_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN3_IT0_IRQn 0 */

    /* USER CODE END FDCAN3_IT0_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan3);
    /* USER CODE BEGIN FDCAN3_IT0_IRQn 1 */

    /* USER CODE END FDCAN3_IT0_IRQn 1 */
}

/**
 * @brief This function handles FDCAN3 interrupt 1.
 */
void FDCAN3_IT1_IRQHandler(void)
{
    /* USER CODE BEGIN FDCAN3_IT1_IRQn 0 */

    /* USER CODE END FDCAN3_IT1_IRQn 0 */
    HAL_FDCAN_IRQHandler(&hfdcan3);
    /* USER CODE BEGIN FDCAN3_IT1_IRQn 1 */

    /* USER CODE END FDCAN3_IT1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
