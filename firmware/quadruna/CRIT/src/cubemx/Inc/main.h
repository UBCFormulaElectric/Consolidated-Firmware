/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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
#define TASK_100HZ_STACK_SIZE 512
#define TASK_CANRX_STACK_SIZE 512
#define TASK_CANTX_STACK_SIZE 512
#define TIM12_PWM_MINIMUM_FREQUENCY 1
#define IDWG_RESET_FREQUENCY 5
#define TIMx_FREQUENCY 96000000
#define TIM12_PRESCALER (TIMx_FREQUENCY / TIM12_AUTO_RELOAD_REG / TIM12_PWM_MINIMUM_FREQUENCY)
#define TASK_1HZ_STACK_SIZE 512
#define TASK_1KHZ_STACK_SIZE 512
#define TIM12_AUTO_RELOAD_REG 0xFFFF
#define IWDG_PRESCALER 4
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define LSI_FREQUENCY 32000
#define TIM3_PRESCALER 8
#define ADC_FREQUENCY 1000
#define NDRIVE_MODE_1b_Pin GPIO_PIN_13
#define NDRIVE_MODE_1b_GPIO_Port GPIOC
#define NCHIMERA_Pin GPIO_PIN_15
#define NCHIMERA_GPIO_Port GPIOC
#define NDRIVE_MODE_0b_Pin GPIO_PIN_0
#define NDRIVE_MODE_0b_GPIO_Port GPIOC
#define NDRIVE_MODE_3b_Pin GPIO_PIN_1
#define NDRIVE_MODE_3b_GPIO_Port GPIOC
#define NDRIVE_MODE_2b_Pin GPIO_PIN_2
#define NDRIVE_MODE_2b_GPIO_Port GPIOC
#define SHDN_G_Pin GPIO_PIN_3
#define SHDN_G_GPIO_Port GPIOC
#define SHDN_R_Pin GPIO_PIN_0
#define SHDN_R_GPIO_Port GPIOA
#define BMS_B_Pin GPIO_PIN_1
#define BMS_B_GPIO_Port GPIOA
#define UART_TX_Pin GPIO_PIN_2
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin GPIO_PIN_3
#define UART_RX_GPIO_Port GPIOA
#define BMS_G_Pin GPIO_PIN_4
#define BMS_G_GPIO_Port GPIOA
#define BSPD_R_Pin GPIO_PIN_5
#define BSPD_R_GPIO_Port GPIOA
#define BMS_R_Pin GPIO_PIN_7
#define BMS_R_GPIO_Port GPIOA
#define REGEN_3V3_Pin GPIO_PIN_4
#define REGEN_3V3_GPIO_Port GPIOC
#define VC_G_Pin GPIO_PIN_5
#define VC_G_GPIO_Port GPIOC
#define VC_R_Pin GPIO_PIN_0
#define VC_R_GPIO_Port GPIOB
#define AUX_DB_B_Pin GPIO_PIN_1
#define AUX_DB_B_GPIO_Port GPIOB
#define AUX_DB_G_Pin GPIO_PIN_2
#define AUX_DB_G_GPIO_Port GPIOB
#define AUX_DB_R_Pin GPIO_PIN_10
#define AUX_DB_R_GPIO_Port GPIOB
#define CRIT_DB_B_Pin GPIO_PIN_12
#define CRIT_DB_B_GPIO_Port GPIOB
#define CRIT_DB_G_Pin GPIO_PIN_13
#define CRIT_DB_G_GPIO_Port GPIOB
#define CRIT_DB_R_Pin GPIO_PIN_14
#define CRIT_DB_R_GPIO_Port GPIOB
#define RSM_B_Pin GPIO_PIN_15
#define RSM_B_GPIO_Port GPIOB
#define RSM_G_Pin GPIO_PIN_6
#define RSM_G_GPIO_Port GPIOC
#define RSM_R_Pin GPIO_PIN_7
#define RSM_R_GPIO_Port GPIOC
#define FSM_B_Pin GPIO_PIN_8
#define FSM_B_GPIO_Port GPIOC
#define FSM_G_Pin GPIO_PIN_9
#define FSM_G_GPIO_Port GPIOC
#define FSM_R_Pin GPIO_PIN_8
#define FSM_R_GPIO_Port GPIOA
#define NPROGRAM_3V3_Pin GPIO_PIN_9
#define NPROGRAM_3V3_GPIO_Port GPIOA
#define VC_B_Pin GPIO_PIN_10
#define VC_B_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define INERTIA_SEN_Pin GPIO_PIN_10
#define INERTIA_SEN_GPIO_Port GPIOC
#define SHDN_SEN_Pin GPIO_PIN_11
#define SHDN_SEN_GPIO_Port GPIOC
#define IMD_R_Pin GPIO_PIN_12
#define IMD_R_GPIO_Port GPIOC
#define AMS_R_Pin GPIO_PIN_2
#define AMS_R_GPIO_Port GPIOD
#define START_LED_Pin GPIO_PIN_4
#define START_LED_GPIO_Port GPIOB
#define START_SIG_Pin GPIO_PIN_5
#define START_SIG_GPIO_Port GPIOB
#define TORQUE_VECTORING_LED_Pin GPIO_PIN_6
#define TORQUE_VECTORING_LED_GPIO_Port GPIOB
#define REGEN_SIG_Pin GPIO_PIN_7
#define REGEN_SIG_GPIO_Port GPIOB
#define TORQUE_VECTORING_SIG_Pin GPIO_PIN_8
#define TORQUE_VECTORING_SIG_GPIO_Port GPIOB
#define REGEN_LED_Pin GPIO_PIN_9
#define REGEN_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
