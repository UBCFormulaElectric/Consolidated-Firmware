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
extern "C"
{
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LC3_OUT_Pin GPIO_PIN_0
#define LC3_OUT_GPIO_Port GPIOC
#define CoolantPressure2_3V3_Pin GPIO_PIN_1
#define CoolantPressure2_3V3_GPIO_Port GPIOC
#define CoolantPressure1_3V3_Pin GPIO_PIN_2
#define CoolantPressure1_3V3_GPIO_Port GPIOC
#define LC4_OUT_Pin GPIO_PIN_0
#define LC4_OUT_GPIO_Port GPIOA
#define SUSP_TRAVEL_RL_3V3_Pin GPIO_PIN_1
#define SUSP_TRAVEL_RL_3V3_GPIO_Port GPIOA
#define CoolantTemp1_3V3_Pin GPIO_PIN_2
#define CoolantTemp1_3V3_GPIO_Port GPIOA
#define CoolantTemp2_3V3_Pin GPIO_PIN_3
#define CoolantTemp2_3V3_GPIO_Port GPIOA
#define SUSP_TRAVEL_RR_3V3_Pin GPIO_PIN_4
#define SUSP_TRAVEL_RR_3V3_GPIO_Port GPIOA
#define BRAKE_LIGHT_EN_3V3_Pin GPIO_PIN_5
#define BRAKE_LIGHT_EN_3V3_GPIO_Port GPIOA
#define FR_STBY_Pin GPIO_PIN_6
#define FR_STBY_GPIO_Port GPIOA
#define nCHIMERA_Pin GPIO_PIN_7
#define nCHIMERA_GPIO_Port GPIOA
#define RAD_FAN_I_SNS_Pin GPIO_PIN_4
#define RAD_FAN_I_SNS_GPIO_Port GPIOC
#define ACC_FAN_I_SNS_Pin GPIO_PIN_5
#define ACC_FAN_I_SNS_GPIO_Port GPIOC
#define RAD_FAN_EN_Pin GPIO_PIN_1
#define RAD_FAN_EN_GPIO_Port GPIOB
#define ACC_FAN_EN_Pin GPIO_PIN_10
#define ACC_FAN_EN_GPIO_Port GPIOB
#define nProgram_3V3_Pin GPIO_PIN_12
#define nProgram_3V3_GPIO_Port GPIOB
#define UART_RX_Pin GPIO_PIN_9
#define UART_RX_GPIO_Port GPIOA
#define UART_TX_Pin GPIO_PIN_10
#define UART_TX_GPIO_Port GPIOA

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
