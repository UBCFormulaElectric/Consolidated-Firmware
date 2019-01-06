/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

    /* Private includes
     * ----------------------------------------------------------*/
    /* USER CODE BEGIN Includes */
    /* USER CODE END Includes */

    /* Exported types
     * ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants
     * --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* Exported macro
     * ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes
     * ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM2_PRESCALER 14399
#define APB1_TIMER_CLOCK 72000000
#define TIM2_CLK_DIVISION 1
#define ADC_TRIGGER_FREQUENCY 1000
#define APB2_TIMER_CLOCK 72000000
#define TIM17_CLK_DIVISION 1
#define CONTROL_LOOP_FREQUENCY 100
#define TIM17_PRESCALER 14399
#define TIM17_REPETITION 0
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOF
#define GLV_SENSE_Pin GPIO_PIN_2
#define GLV_SENSE_GPIO_Port GPIOC
#define VBAT_SENSE_Pin GPIO_PIN_3
#define VBAT_SENSE_GPIO_Port GPIOC
#define EFUSE_IS_1_Pin GPIO_PIN_0
#define EFUSE_IS_1_GPIO_Port GPIOA
#define EFUSE_IS_2_Pin GPIO_PIN_1
#define EFUSE_IS_2_GPIO_Port GPIOA
#define EFUSE_IS_3_Pin GPIO_PIN_2
#define EFUSE_IS_3_GPIO_Port GPIOA
#define EFUSE_IS_4_Pin GPIO_PIN_3
#define EFUSE_IS_4_GPIO_Port GPIOA
#define EFUSE_IS_5_Pin GPIO_PIN_4
#define EFUSE_IS_5_GPIO_Port GPIOA
#define FLYWIRE_Pin GPIO_PIN_6
#define FLYWIRE_GPIO_Port GPIOA
#define EFUSE_AUX_2_IN_Pin GPIO_PIN_7
#define EFUSE_AUX_2_IN_GPIO_Port GPIOA
#define EFUSE_DSEL_1_Pin GPIO_PIN_4
#define EFUSE_DSEL_1_GPIO_Port GPIOC
#define EFUSE_DEN_1_Pin GPIO_PIN_5
#define EFUSE_DEN_1_GPIO_Port GPIOC
#define EFUSE_AUX_1_IN_Pin GPIO_PIN_0
#define EFUSE_AUX_1_IN_GPIO_Port GPIOB
#define EFUSE_PDM_FAN_IN_Pin GPIO_PIN_1
#define EFUSE_PDM_FAN_IN_GPIO_Port GPIOB
#define EFUSE_DSEL_2_Pin GPIO_PIN_2
#define EFUSE_DSEL_2_GPIO_Port GPIOB
#define EFUSE_DEN_2_Pin GPIO_PIN_10
#define EFUSE_DEN_2_GPIO_Port GPIOB
#define EFUSE_COOLING_IN_Pin GPIO_PIN_11
#define EFUSE_COOLING_IN_GPIO_Port GPIOB
#define EFUSE_CAN_IN_Pin GPIO_PIN_12
#define EFUSE_CAN_IN_GPIO_Port GPIOB
#define EFUSE_DSEL_3_Pin GPIO_PIN_13
#define EFUSE_DSEL_3_GPIO_Port GPIOB
#define EFUSE_DEN_3_Pin GPIO_PIN_14
#define EFUSE_DEN_3_GPIO_Port GPIOB
#define EFUSE_AIR_SHDN_IN_Pin GPIO_PIN_15
#define EFUSE_AIR_SHDN_IN_GPIO_Port GPIOB
#define EFUSE_ACC_ENC_FAN_IN_Pin GPIO_PIN_6
#define EFUSE_ACC_ENC_FAN_IN_GPIO_Port GPIOC
#define EFUSE_DSEL_4_Pin GPIO_PIN_7
#define EFUSE_DSEL_4_GPIO_Port GPIOC
#define EFUSE_DEN_4_Pin GPIO_PIN_8
#define EFUSE_DEN_4_GPIO_Port GPIOC
#define EFUSE_ACC_SEG_FAN_IN_Pin GPIO_PIN_9
#define EFUSE_ACC_SEG_FAN_IN_GPIO_Port GPIOC
#define EFUSE_RIGHT_INVERTER_IN_Pin GPIO_PIN_8
#define EFUSE_RIGHT_INVERTER_IN_GPIO_Port GPIOA
#define EFUSE_DSEL_5_Pin GPIO_PIN_9
#define EFUSE_DSEL_5_GPIO_Port GPIOA
#define EFUSE_DEN_5_Pin GPIO_PIN_10
#define EFUSE_DEN_5_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define EFUSE_LEFT_INVERTER_IN_Pin GPIO_PIN_15
#define EFUSE_LEFT_INVERTER_IN_GPIO_Port GPIOA
#define CHARGER_FAULT_Pin GPIO_PIN_10
#define CHARGER_FAULT_GPIO_Port GPIOC
#define CHARGER_FAULT_EXTI_IRQn EXTI15_10_IRQn
#define CHARGER_INDICATOR_Pin GPIO_PIN_11
#define CHARGER_INDICATOR_GPIO_Port GPIOC
#define CHARGER_INDICATOR_EXTI_IRQn EXTI15_10_IRQn
#define BOOST_PGOOD_Pin GPIO_PIN_12
#define BOOST_PGOOD_GPIO_Port GPIOC
#define BOOST_PGOOD_EXTI_IRQn EXTI15_10_IRQn
#define CELL_BALANCE_OVERVOLTAGE_Pin GPIO_PIN_2
#define CELL_BALANCE_OVERVOLTAGE_GPIO_Port GPIOD
#define CELL_BALANCE_OVERVOLTAGE_EXTI_IRQn EXTI2_TSC_IRQn
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */
    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
