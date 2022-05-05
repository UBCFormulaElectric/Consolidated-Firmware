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
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
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
#define TASK1HZ_STACK_SIZE 512
#define TASK1KHZ_STACK_SIZE 512
#define TASK100HZ_STACK_SIZE 512
#define TASKCANTX_STACK_SIZE 512
#define TASKCANRX_STACK_SIZE 512
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define LSI_FREQUENCY 40000
#define TIMx_FREQUENCY 72000000
#define TIM2_PRESCALER 72
#define ADC_FREQUENCY 1000
#define BSPD_LED_Pin GPIO_PIN_13
#define BSPD_LED_GPIO_Port GPIOC
#define DIM_RED_Pin GPIO_PIN_14
#define DIM_RED_GPIO_Port GPIOC
#define PDM_GREEN_Pin GPIO_PIN_15
#define PDM_GREEN_GPIO_Port GPIOC
#define DCM_BLUE_Pin GPIO_PIN_0
#define DCM_BLUE_GPIO_Port GPIOA
#define DIM_GREEN_Pin GPIO_PIN_1
#define DIM_GREEN_GPIO_Port GPIOA
#define DIM_BLUE_Pin GPIO_PIN_2
#define DIM_BLUE_GPIO_Port GPIOA
#define PDM_RED_Pin GPIO_PIN_3
#define PDM_RED_GPIO_Port GPIOA
#define DCM_GREEN_Pin GPIO_PIN_4
#define DCM_GREEN_GPIO_Port GPIOA
#define PDM_BLUE_Pin GPIO_PIN_5
#define PDM_BLUE_GPIO_Port GPIOA
#define FSM_GREEN_Pin GPIO_PIN_6
#define FSM_GREEN_GPIO_Port GPIOA
#define BMS_BLUE_Pin GPIO_PIN_7
#define BMS_BLUE_GPIO_Port GPIOA
#define DCM_RED_Pin GPIO_PIN_0
#define DCM_RED_GPIO_Port GPIOB
#define BMS_GREEN_Pin GPIO_PIN_1
#define BMS_GREEN_GPIO_Port GPIOB
#define REGEN_Pin GPIO_PIN_2
#define REGEN_GPIO_Port GPIOB
#define FSM_RED_Pin GPIO_PIN_10
#define FSM_RED_GPIO_Port GPIOB
#define FSM_BLUE_Pin GPIO_PIN_11
#define FSM_BLUE_GPIO_Port GPIOB
#define BMS_RED_Pin GPIO_PIN_12
#define BMS_RED_GPIO_Port GPIOB
#define SEVENSEG_SRCK_3V3_Pin GPIO_PIN_13
#define SEVENSEG_SRCK_3V3_GPIO_Port GPIOB
#define DRIVE_MODE5_Pin GPIO_PIN_14
#define DRIVE_MODE5_GPIO_Port GPIOB
#define SEVENSEG_SEROUT_3V3_Pin GPIO_PIN_15
#define SEVENSEG_SEROUT_3V3_GPIO_Port GPIOB
#define DRIVE_MODE4_Pin GPIO_PIN_8
#define DRIVE_MODE4_GPIO_Port GPIOA
#define DRIVE_MODE3_Pin GPIO_PIN_9
#define DRIVE_MODE3_GPIO_Port GPIOA
#define DRIVE_MODE2_Pin GPIO_PIN_10
#define DRIVE_MODE2_GPIO_Port GPIOA
#define SEVENSEG_DIMMING_3V3_Pin GPIO_PIN_15
#define SEVENSEG_DIMMING_3V3_GPIO_Port GPIOA
#define SEVENSEG_RCK_3V3_Pin GPIO_PIN_4
#define SEVENSEG_RCK_3V3_GPIO_Port GPIOB
#define DRIVE_MODE1_Pin GPIO_PIN_5
#define DRIVE_MODE1_GPIO_Port GPIOB
#define TRAC_CTRL_Pin GPIO_PIN_6
#define TRAC_CTRL_GPIO_Port GPIOB
#define TORQ_VECT_Pin GPIO_PIN_7
#define TORQ_VECT_GPIO_Port GPIOB
#define IMD_LED_Pin GPIO_PIN_8
#define IMD_LED_GPIO_Port GPIOB
#define IGNTN_Pin GPIO_PIN_9
#define IGNTN_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
