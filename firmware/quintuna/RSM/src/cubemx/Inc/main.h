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
    extern ADC_HandleTypeDef hadc1;
    extern DMA_HandleTypeDef hdma_adc1;
    extern TIM_HandleTypeDef htim3;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWDG_RESET_FREQUENCY 5
#define TIM12_PRESCALER (TIMx_FREQUENCY / TIM12_AUTO_RELOAD_REG / TIM12_PWM_MINIMUM_FREQUENCY)
#define TASK_CANRX_STACK_SIZE 512
#define TASK_CANTX_STACK_SIZE 512
#define TASK_1HZ_STACK_SIZE 512
#define TASK_1KHZ_STACK_SIZE 512
#define TIM3_PRESCALER 8
#define TIM12_PWM_MINIMUM_FREQUENCY 1
#define TIM12_AUTO_RELOAD_REG 0xFFFF
#define ADC_FREQUENCY 1000
#define TASK_100_HZ_STACK_SIZE 512
#define TIMx_FREQUENCY 96000000
#define IWDG_PRESCALER 4
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define LSI_FREQUENCY 32000
#define BOOT_Pin GPIO_PIN_0
#define BOOT_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define LC3_OUT_Pin GPIO_PIN_7
#define LC3_OUT_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
