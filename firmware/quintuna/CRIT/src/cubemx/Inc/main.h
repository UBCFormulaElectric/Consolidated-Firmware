/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "hw_error.h"
#include "cmsis_os.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern CAN_HandleTypeDef  hcan2;
    extern IWDG_HandleTypeDef hiwdg;
    extern TIM_HandleTypeDef  htim4;
    extern TIM_HandleTypeDef  htim12;

    extern osThreadId_t Task1HzHandle;
    extern osThreadId_t Task100HzHandle;
    extern osThreadId_t Task1kHzHandle;
    extern osThreadId_t TaskCanRxHandle;
    extern osThreadId_t TaskCanTxHandle;

    extern const osThreadAttr_t Task1Hz_attributes;
    extern const osThreadAttr_t Task100Hz_attributes;
    extern const osThreadAttr_t Task1kHz_attributes;
    extern const osThreadAttr_t TaskCanRx_attributes;
    extern const osThreadAttr_t TaskCanTx_attributes;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWDG_RESET_FREQUENCY 5
#define SYS_TIMER_CLOCK 96000000
#define PWM_AUTO_RELOAD 4095
#define IWDG_PRESCALER 4
#define PWM_PRESCALER 650
#define LSI_FREQUENCY 32000
#define BOOT_Pin GPIO_PIN_4
#define BOOT_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define TELEM_SIG_Pin GPIO_PIN_6
#define TELEM_SIG_GPIO_Port GPIOA
#define PUSH_DRIVE_SIG_Pin GPIO_PIN_7
#define PUSH_DRIVE_SIG_GPIO_Port GPIOA
#define ROT_S_Pin GPIO_PIN_4
#define ROT_S_GPIO_Port GPIOC
#define ROT_S_EXTI_IRQn EXTI4_IRQn
#define ROT_B_Pin GPIO_PIN_5
#define ROT_B_GPIO_Port GPIOC
#define ROT_B_EXTI_IRQn EXTI9_5_IRQn
#define ROT_A_Pin GPIO_PIN_0
#define ROT_A_GPIO_Port GPIOB
#define ROT_A_EXTI_IRQn EXTI0_IRQn
#define LAUNCH_CONTROL_SIG_Pin GPIO_PIN_1
#define LAUNCH_CONTROL_SIG_GPIO_Port GPIOB
#define VANILLA_SIG_Pin GPIO_PIN_2
#define VANILLA_SIG_GPIO_Port GPIOB
#define REGEN_SIG_Pin GPIO_PIN_10
#define REGEN_SIG_GPIO_Port GPIOB
#define LED_DIMMING_Pin GPIO_PIN_14
#define LED_DIMMING_GPIO_Port GPIOB
#define LED_SERIN_Pin GPIO_PIN_15
#define LED_SERIN_GPIO_Port GPIOB
#define LED_RCK_Pin GPIO_PIN_6
#define LED_RCK_GPIO_Port GPIOC
#define LED_SRCK_Pin GPIO_PIN_7
#define LED_SRCK_GPIO_Port GPIOC
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SEVEN_SEG_SRCK_Pin GPIO_PIN_10
#define SEVEN_SEG_SRCK_GPIO_Port GPIOC
#define SEVEN_SEG_SERIN_Pin GPIO_PIN_12
#define SEVEN_SEG_SERIN_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SEVEN_SEG_RCK_Pin GPIO_PIN_4
#define SEVEN_SEG_RCK_GPIO_Port GPIOB
#define SEVEN_SEG_DIMMING_Pin GPIO_PIN_6
#define SEVEN_SEG_DIMMING_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

    void CRIT_StartAllTasks(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
