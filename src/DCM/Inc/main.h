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
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LSI_FREQUENCY 40000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define CONTROL_LOOP_FREQUENCY 100
#define TIM6_PRESCALER 15
#define APB1_TIMER_CLOCK 72000000
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOF
#define ANALOG_IN1_Pin GPIO_PIN_0
#define ANALOG_IN1_GPIO_Port GPIOA
#define ANALOG_IN2_Pin GPIO_PIN_1
#define ANALOG_IN2_GPIO_Port GPIOA
#define ANALOG_IN3_Pin GPIO_PIN_2
#define ANALOG_IN3_GPIO_Port GPIOA
#define ANALOG_OUT_Pin GPIO_PIN_4
#define ANALOG_OUT_GPIO_Port GPIOA
#define START_STOP_SWITCH_Pin GPIO_PIN_5
#define START_STOP_SWITCH_GPIO_Port GPIOA
#define START_STOP_SWITCH_EXTI_IRQn EXTI9_5_IRQn
#define TRACTION_CONTROL_SWITCH_Pin GPIO_PIN_6
#define TRACTION_CONTROL_SWITCH_GPIO_Port GPIOA
#define TRACTION_CONTROL_SWITCH_EXTI_IRQn EXTI9_5_IRQn
#define TORQUE_VECTORING_SWITCH_Pin GPIO_PIN_7
#define TORQUE_VECTORING_SWITCH_GPIO_Port GPIOA
#define TORQUE_VECTORING_SWITCH_EXTI_IRQn EXTI9_5_IRQn
#define START_STOP_LED_Pin GPIO_PIN_0
#define START_STOP_LED_GPIO_Port GPIOB
#define TRACTION_CONTROL_LED_Pin GPIO_PIN_1
#define TRACTION_CONTROL_LED_GPIO_Port GPIOB
#define TORQUE_VECTORING_LED_Pin GPIO_PIN_2
#define TORQUE_VECTORING_LED_GPIO_Port GPIOB
#define INVERTER_L_RUN_Pin GPIO_PIN_10
#define INVERTER_L_RUN_GPIO_Port GPIOB
#define INVERTER_R_RUN_Pin GPIO_PIN_11
#define INVERTER_R_RUN_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_14
#define IMU_INT_GPIO_Port GPIOB
#define IMU_INT_EXTI_IRQn EXTI15_10_IRQn
#define DCM_STATUS_Pin GPIO_PIN_15
#define DCM_STATUS_GPIO_Port GPIOB
#define STATUS_R_Pin GPIO_PIN_8
#define STATUS_R_GPIO_Port GPIOA
#define STATUS_G_Pin GPIO_PIN_9
#define STATUS_G_GPIO_Port GPIOA
#define STATUS_B_Pin GPIO_PIN_10
#define STATUS_B_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define BSPD_BRAKE_THRES_Pin GPIO_PIN_15
#define BSPD_BRAKE_THRES_GPIO_Port GPIOA
#define BSPD_BRAKE_THRES_EXTI_IRQn EXTI15_10_IRQn
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BUZZER_EN_Pin GPIO_PIN_4
#define BUZZER_EN_GPIO_Port GPIOB
#define BRAKE_LIGHT_EN_Pin GPIO_PIN_5
#define BRAKE_LIGHT_EN_GPIO_Port GPIOB
#define INVERTER_R_REGEN_Pin GPIO_PIN_8
#define INVERTER_R_REGEN_GPIO_Port GPIOB
#define INVERTER_L_REGEN_Pin GPIO_PIN_9
#define INVERTER_L_REGEN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
