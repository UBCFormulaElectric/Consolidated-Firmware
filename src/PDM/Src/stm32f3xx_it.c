/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f3xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
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
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CurrentSense.h"
#include "Gpio.h"
#include "SharedCan.h"
#include "ErrorHandling.h"
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

// TODO: Redo heartbeat handling
#ifndef DEBUG

// SysTick/heartbeat variables
extern const int         HEARTBEAT_TICK_PERIOD;      // Period in ms
extern const int         HEARTBEAT_BROADCAST_PERIOD; // Period in ms
extern volatile uint16_t HeartbeatCount[PCB_COUNT];
static volatile uint32_t HeartbeatTimeoutTicks      = 0;
static volatile uint32_t PDMHeartbeatBroadcastTicks = 0;

#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim17;
/* USER CODE BEGIN EV */
extern IWDG_HandleTypeDef hiwdg;
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

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

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
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
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

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    // CANNOT USE DELAY FUNCTION HERE AS THE ABOVE CODE INCREMENTS THE DELAY
    // COUNTER!
    HAL_IWDG_Refresh(&hiwdg);

#ifndef DEBUG

    HeartbeatTimeoutTicks++;
    PDMHeartbeatBroadcastTicks++;

    // Check for Heartbeat Timeouts here
    if (HeartbeatTimeoutTicks >= HEARTBEAT_TICK_PERIOD)
    {
        HeartbeatTimeoutTicks = 0;

        // BMS checks PDM, FSM, DCM
        // PDM, FSM, DCM check only BMS
        if (HeartbeatCount[BATTERY_MANAGEMENT_SYSTEM] == 0)
        {
            // We have not received the BMS's heartbeat CAN Message. Assume the
            // board is not working.
            ErrorHandling_HandleHeartbeatTimeout();
        }

        // Reset the ticks for this module
        HeartbeatCount[POWER_DISTRIBUTION_MODULE] = 0;
    }

    // Broadcast PDM heartbeat for other boards to monitor
    if (PDMHeartbeatBroadcastTicks >= HEARTBEAT_BROADCAST_PERIOD)
    {
        PDMHeartbeatBroadcastTicks = 0;
        SharedCan_BroadcastHeartbeat();
    }

#endif

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles EXTI line2 and Touch Sense controller.
 */
void EXTI2_TSC_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */
    /* USER CODE END EXTI2_TSC_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

    /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel1 global interrupt.
 */
void DMA1_Channel1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

    /* USER CODE END DMA1_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc1);
    /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

    /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
 * @brief This function handles CAN TX and USB high priority interrupts.
 */
void USB_HP_CAN_TX_IRQHandler(void)
{
    /* USER CODE BEGIN USB_HP_CAN_TX_IRQn 0 */

    /* USER CODE END USB_HP_CAN_TX_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan);
    /* USER CODE BEGIN USB_HP_CAN_TX_IRQn 1 */

    /* USER CODE END USB_HP_CAN_TX_IRQn 1 */
}

/**
 * @brief This function handles CAN RX0 and USB low priority interrupts.
 */
void USB_LP_CAN_RX0_IRQHandler(void)
{
    /* USER CODE BEGIN USB_LP_CAN_RX0_IRQn 0 */

    /* USER CODE END USB_LP_CAN_RX0_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan);
    /* USER CODE BEGIN USB_LP_CAN_RX0_IRQn 1 */

    /* USER CODE END USB_LP_CAN_RX0_IRQn 1 */
}

/**
 * @brief This function handles CAN RX1 interrupt.
 */
void CAN_RX1_IRQHandler(void)
{
    /* USER CODE BEGIN CAN_RX1_IRQn 0 */

    /* USER CODE END CAN_RX1_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan);
    /* USER CODE BEGIN CAN_RX1_IRQn 1 */

    /* USER CODE END CAN_RX1_IRQn 1 */
}

/**
 * @brief This function handles TIM1 trigger, commutation and TIM17 interrupts.
 */
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
    /* USER CODE BEGIN TIM1_TRG_COM_TIM17_IRQn 0 */

    /* USER CODE END TIM1_TRG_COM_TIM17_IRQn 0 */
    HAL_TIM_IRQHandler(&htim17);
    /* USER CODE BEGIN TIM1_TRG_COM_TIM17_IRQn 1 */

    /* USER CODE END TIM1_TRG_COM_TIM17_IRQn 1 */
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
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI15_10_IRQn 0 */

    /* USER CODE END EXTI15_10_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    /* USER CODE BEGIN EXTI15_10_IRQn 1 */

    /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
