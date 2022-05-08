/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Io_SharedErrorHandlerOverride.h"
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
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define LSI_FREQUENCY 40000
#define TASK1HZ_STACK_SIZE 512
#define TASK100HZ_STACK_SIZE 512
#define TASK1KHZ_STACK_SIZE 512
#define TASKCANRX_STACK_SIZE 512
#define TASKCANTX_STACK_SIZE 512
#define TIM4_AUTO_RELOAD_REG 0xFFFF
#define TIMx_FREQUENCY 72000000
#define TIM4_PWM_MINIMUM_FREQUENCY 1
#define TIM4_PRESCALER (TIMx_FREQUENCY / TIM4_AUTO_RELOAD_REG / TIM4_PWM_MINIMUM_FREQUENCY)
#define TIM16_AUTO_RELOAD_REG 0xFFFF
#define TIM17_AUTO_RELOAD_REG 0xFFFF
#define TIM16_PWM_MINIMUM_FREQUENCY 1
#define TIM17_PWM_MINIMUM_FREQUENCY 1
#define TIM16_PRESCALER (TIMx_FREQUENCY / TIM16_AUTO_RELOAD_REG / TIM16_PWM_MINIMUM_FREQUENCY)
#define TIM17_PRESCALER (TIMx_FREQUENCY / TIM17_AUTO_RELOAD_REG / TIM17_PWM_MINIMUM_FREQUENCY)
#define TIM3_PRESCALER 72
#define ADC_FREQUENCY 1000
#define TIM1_AUTO_RELOAD_REG 10000
#define TIM2_AUTO_RELOAD_REG 7200
#define SECONDARY_APPS_A_Pin GPIO_PIN_0
#define SECONDARY_APPS_A_GPIO_Port GPIOA
#define SECONDARY_APPS_B_Pin GPIO_PIN_1
#define SECONDARY_APPS_B_GPIO_Port GPIOA
#define SECONDARY_APPS_ALARM_Pin GPIO_PIN_3
#define SECONDARY_APPS_ALARM_GPIO_Port GPIOA
#define STEERING_ANGLE_BUFF_Pin GPIO_PIN_4
#define STEERING_ANGLE_BUFF_GPIO_Port GPIOA
#define STATUS_R_Pin GPIO_PIN_5
#define STATUS_R_GPIO_Port GPIOA
#define BRAKE_PRESS_BUFF_Pin GPIO_PIN_6
#define BRAKE_PRESS_BUFF_GPIO_Port GPIOA
#define STATUS_G_Pin GPIO_PIN_7
#define STATUS_G_GPIO_Port GPIOA
#define STATUS_B_Pin GPIO_PIN_0
#define STATUS_B_GPIO_Port GPIOB
#define PRIMARY_APPS_A_Pin GPIO_PIN_8
#define PRIMARY_APPS_A_GPIO_Port GPIOA
#define PRIMARY_APPS_B_Pin GPIO_PIN_9
#define PRIMARY_APPS_B_GPIO_Port GPIOA
#define FLOW1_BUFF_Pin GPIO_PIN_11
#define FLOW1_BUFF_GPIO_Port GPIOA
#define FLOW2_BUFF_Pin GPIO_PIN_12
#define FLOW2_BUFF_GPIO_Port GPIOA
#define BSPD_BRAKE_STATUS_Pin GPIO_PIN_15
#define BSPD_BRAKE_STATUS_GPIO_Port GPIOA
#define FL_WHEEL_SPEED_Pin GPIO_PIN_4
#define FL_WHEEL_SPEED_GPIO_Port GPIOB
#define FR_WHEEL_SPEED_Pin GPIO_PIN_5
#define FR_WHEEL_SPEED_GPIO_Port GPIOB
#define BRAKE_OC_SC_OK_Pin GPIO_PIN_6
#define BRAKE_OC_SC_OK_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
