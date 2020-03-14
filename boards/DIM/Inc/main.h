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
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define LSI_FREQUENCY 40000
#define TASK1HZ_STACK_SIZE 128
#define TASK1KHZ_STACK_SIZE 128
#define TASKCANTX_STACK_SIZE 128
#define TASKCANRX_STACK_SIZE 128
#define BSPD_LED_Pin GPIO_PIN_13
#define BSPD_LED_GPIO_Port GPIOC
#define PDM_R_Pin GPIO_PIN_14
#define PDM_R_GPIO_Port GPIOC
#define PDM_G_Pin GPIO_PIN_15
#define PDM_G_GPIO_Port GPIOC
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOF
#define DCM_B_Pin GPIO_PIN_0
#define DCM_B_GPIO_Port GPIOA
#define DIM_G_Pin GPIO_PIN_1
#define DIM_G_GPIO_Port GPIOA
#define DIM_B_Pin GPIO_PIN_2
#define DIM_B_GPIO_Port GPIOA
#define PDM_RA3_Pin GPIO_PIN_3
#define PDM_RA3_GPIO_Port GPIOA
#define DCM_G_Pin GPIO_PIN_4
#define DCM_G_GPIO_Port GPIOA
#define PDM_B_Pin GPIO_PIN_5
#define PDM_B_GPIO_Port GPIOA
#define FSM_G_Pin GPIO_PIN_6
#define FSM_G_GPIO_Port GPIOA
#define BMS_B_Pin GPIO_PIN_7
#define BMS_B_GPIO_Port GPIOA
#define DCM_R_Pin GPIO_PIN_0
#define DCM_R_GPIO_Port GPIOB
#define BMS_G_Pin GPIO_PIN_1
#define BMS_G_GPIO_Port GPIOB
#define REGEN_Pin GPIO_PIN_2
#define REGEN_GPIO_Port GPIOB
#define FSM_R_Pin GPIO_PIN_10
#define FSM_R_GPIO_Port GPIOB
#define FSM_B_Pin GPIO_PIN_11
#define FSM_B_GPIO_Port GPIOB
#define BMS_R_Pin GPIO_PIN_12
#define BMS_R_GPIO_Port GPIOB
#define SEG_SRCK_Pin GPIO_PIN_13
#define SEG_SRCK_GPIO_Port GPIOB
#define DRIVE_MODE5_Pin GPIO_PIN_14
#define DRIVE_MODE5_GPIO_Port GPIOB
#define SEG_SEROUT_Pin GPIO_PIN_15
#define SEG_SEROUT_GPIO_Port GPIOB
#define DRIVE_MODE4_Pin GPIO_PIN_8
#define DRIVE_MODE4_GPIO_Port GPIOA
#define DRIVE_MODE3_Pin GPIO_PIN_9
#define DRIVE_MODE3_GPIO_Port GPIOA
#define DRIVE_MODE2_Pin GPIO_PIN_10
#define DRIVE_MODE2_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SEG_DIMMING_Pin GPIO_PIN_15
#define SEG_DIMMING_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SEG_RCK_Pin GPIO_PIN_4
#define SEG_RCK_GPIO_Port GPIOB
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
