/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#define LSI_FREQUENCY 32000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define TEST_PIN_Pin GPIO_PIN_13
#define TEST_PIN_GPIO_Port GPIOC
#define BMS_BLUE_Pin GPIO_PIN_0
#define BMS_BLUE_GPIO_Port GPIOC
#define BMS_GREEN_Pin GPIO_PIN_1
#define BMS_GREEN_GPIO_Port GPIOC
#define BMS_RED_Pin GPIO_PIN_2
#define BMS_RED_GPIO_Port GPIOC
#define DCM_BLUE_Pin GPIO_PIN_3
#define DCM_BLUE_GPIO_Port GPIOC
#define SEVENSEGS_SEROUT_Pin GPIO_PIN_0
#define SEVENSEGS_SEROUT_GPIO_Port GPIOA
#define SEVENSEGS_SRCK_Pin GPIO_PIN_1
#define SEVENSEGS_SRCK_GPIO_Port GPIOA
#define SEVENSEGS_RCK_Pin GPIO_PIN_2
#define SEVENSEGS_RCK_GPIO_Port GPIOA
#define SEVENSEGS_DIMMING_Pin GPIO_PIN_3
#define SEVENSEGS_DIMMING_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOA
#define REGEN_Pin GPIO_PIN_5
#define REGEN_GPIO_Port GPIOA
#define FSM_BLUE_Pin GPIO_PIN_6
#define FSM_BLUE_GPIO_Port GPIOA
#define FSM_GREEN_Pin GPIO_PIN_7
#define FSM_GREEN_GPIO_Port GPIOA
#define DCM_GREEN_Pin GPIO_PIN_4
#define DCM_GREEN_GPIO_Port GPIOC
#define DCM_RED_Pin GPIO_PIN_5
#define DCM_RED_GPIO_Port GPIOC
#define BSPD_LED_Pin GPIO_PIN_0
#define BSPD_LED_GPIO_Port GPIOB
#define IMD_LED_Pin GPIO_PIN_1
#define IMD_LED_GPIO_Port GPIOB
#define SHDN_LED_Pin GPIO_PIN_2
#define SHDN_LED_GPIO_Port GPIOB
#define AUX_LED_Pin GPIO_PIN_10
#define AUX_LED_GPIO_Port GPIOB
#define AUX_IN_Pin GPIO_PIN_12
#define AUX_IN_GPIO_Port GPIOB
#define IGNTN_LED_Pin GPIO_PIN_13
#define IGNTN_LED_GPIO_Port GPIOB
#define IGNTN_IN_Pin GPIO_PIN_14
#define IGNTN_IN_GPIO_Port GPIOB
#define PDM_BLUE_Pin GPIO_PIN_9
#define PDM_BLUE_GPIO_Port GPIOC
#define FSM_RED_Pin GPIO_PIN_8
#define FSM_RED_GPIO_Port GPIOA
#define PDM_GREEN_Pin GPIO_PIN_10
#define PDM_GREEN_GPIO_Port GPIOC
#define PDM_RED_Pin GPIO_PIN_11
#define PDM_RED_GPIO_Port GPIOC
#define DRIVE_MODE_0_Pin GPIO_PIN_6
#define DRIVE_MODE_0_GPIO_Port GPIOB
#define DRIVE_MODE_1_Pin GPIO_PIN_7
#define DRIVE_MODE_1_GPIO_Port GPIOB
#define DRIVE_MODE_2_Pin GPIO_PIN_8
#define DRIVE_MODE_2_GPIO_Port GPIOB
#define DRIVE_MODE_3_Pin GPIO_PIN_9
#define DRIVE_MODE_3_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
