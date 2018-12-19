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
#define EFUSE_RETRY_FREQUENCY 1000
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
#define IS_1_Pin GPIO_PIN_0
#define IS_1_GPIO_Port GPIOA
#define IS_2_Pin GPIO_PIN_1
#define IS_2_GPIO_Port GPIOA
#define IS_3_Pin GPIO_PIN_2
#define IS_3_GPIO_Port GPIOA
#define IS_4_Pin GPIO_PIN_3
#define IS_4_GPIO_Port GPIOA
#define IS_5_Pin GPIO_PIN_4
#define IS_5_GPIO_Port GPIOA
#define FLYWIRE_Pin GPIO_PIN_6
#define FLYWIRE_GPIO_Port GPIOA
#define PIN_AUX_2_Pin GPIO_PIN_7
#define PIN_AUX_2_GPIO_Port GPIOA
#define DSEL_1_Pin GPIO_PIN_4
#define DSEL_1_GPIO_Port GPIOC
#define DEN_1_Pin GPIO_PIN_5
#define DEN_1_GPIO_Port GPIOC
#define PIN_AUX_1_Pin GPIO_PIN_0
#define PIN_AUX_1_GPIO_Port GPIOB
#define PIN_PDM_FAN_Pin GPIO_PIN_1
#define PIN_PDM_FAN_GPIO_Port GPIOB
#define DSEL_2_Pin GPIO_PIN_2
#define DSEL_2_GPIO_Port GPIOB
#define DEN_2_Pin GPIO_PIN_10
#define DEN_2_GPIO_Port GPIOB
#define PIN_COOLING_Pin GPIO_PIN_11
#define PIN_COOLING_GPIO_Port GPIOB
#define PIN_CAN_Pin GPIO_PIN_12
#define PIN_CAN_GPIO_Port GPIOB
#define DSEL_3_Pin GPIO_PIN_13
#define DSEL_3_GPIO_Port GPIOB
#define DEN_3_Pin GPIO_PIN_14
#define DEN_3_GPIO_Port GPIOB
#define PIN_AIR_SHDN_Pin GPIO_PIN_15
#define PIN_AIR_SHDN_GPIO_Port GPIOB
#define PIN_ACC_FAN_2_Pin GPIO_PIN_6
#define PIN_ACC_FAN_2_GPIO_Port GPIOC
#define DSEL_4_Pin GPIO_PIN_7
#define DSEL_4_GPIO_Port GPIOC
#define DEN_4_Pin GPIO_PIN_8
#define DEN_4_GPIO_Port GPIOC
#define PIN_ACC_FAN_1_Pin GPIO_PIN_9
#define PIN_ACC_FAN_1_GPIO_Port GPIOC
#define PIN_DRIVE_INVERTER_R_Pin GPIO_PIN_8
#define PIN_DRIVE_INVERTER_R_GPIO_Port GPIOA
#define DSEL_5_Pin GPIO_PIN_9
#define DSEL_5_GPIO_Port GPIOA
#define DEN_5_Pin GPIO_PIN_10
#define DEN_5_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define PIN_DRIVE_INVERTER_L_Pin GPIO_PIN_15
#define PIN_DRIVE_INVERTER_L_GPIO_Port GPIOA
#define CHRG_FAULT_Pin GPIO_PIN_10
#define CHRG_FAULT_GPIO_Port GPIOC
#define CHRG_FAULT_EXTI_IRQn EXTI15_10_IRQn
#define PGOOD_Pin GPIO_PIN_12
#define PGOOD_GPIO_Port GPIOC
#define PGOOD_EXTI_IRQn EXTI15_10_IRQn
#define OV_FAULT_MCU_Pin GPIO_PIN_2
#define OV_FAULT_MCU_GPIO_Port GPIOD
#define OV_FAULT_MCU_EXTI_IRQn EXTI2_TSC_IRQn
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */
    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
