/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h5xx_hal.h"

    /* Private includes ----------------------------------------------------------*/
    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */
    extern FDCAN_HandleTypeDef hfdcan1;
    extern SPI_HandleTypeDef   hspi1;
    extern SPI_HandleTypeDef   hspi2;
    extern SPI_HandleTypeDef   hspi3;
    extern TIM_HandleTypeDef   htim3;
    extern TIM_HandleTypeDef   htim4;
    extern PCD_HandleTypeDef   hpcd_USB_DRD_FS;

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

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
#define BOOT_Pin GPIO_PIN_13
#define BOOT_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOC
#define LED_DATA_Pin GPIO_PIN_1
#define LED_DATA_GPIO_Port GPIOC
#define LED_DIMMING_Pin GPIO_PIN_2
#define LED_DIMMING_GPIO_Port GPIOC
#define LED_RCK_Pin GPIO_PIN_3
#define LED_RCK_GPIO_Port GPIOA
#define _7SEG_RCK_Pin GPIO_PIN_4
#define _7SEG_RCK_GPIO_Port GPIOA
#define PWR_CHG_CLK_Pin GPIO_PIN_5
#define PWR_CHG_CLK_GPIO_Port GPIOA
#define PWR_CHG_DATA_Pin GPIO_PIN_7
#define PWR_CHG_DATA_GPIO_Port GPIOA
#define _7SEG_SERIN_Pin GPIO_PIN_2
#define _7SEG_SERIN_GPIO_Port GPIOB
#define LED_CLK_Pin GPIO_PIN_10
#define LED_CLK_GPIO_Port GPIOB
#define PUSH_DRIVE_SIG_Pin GPIO_PIN_14
#define PUSH_DRIVE_SIG_GPIO_Port GPIOB
#define TELEM_SIG_Pin GPIO_PIN_15
#define TELEM_SIG_GPIO_Port GPIOB
#define _7SEG_DIMMING_Pin GPIO_PIN_6
#define _7SEG_DIMMING_GPIO_Port GPIOC
#define REGEN_SIG_Pin GPIO_PIN_7
#define REGEN_SIG_GPIO_Port GPIOC
#define TORQUE_VECTORING_SIG_Pin GPIO_PIN_8
#define TORQUE_VECTORING_SIG_GPIO_Port GPIOC
#define LAUNCH_CONTROL_SIG_Pin GPIO_PIN_9
#define LAUNCH_CONTROL_SIG_GPIO_Port GPIOC
#define ROT_S_Pin GPIO_PIN_8
#define ROT_S_GPIO_Port GPIOA
#define ROT_B_Pin GPIO_PIN_9
#define ROT_B_GPIO_Port GPIOA
#define ROT_A_Pin GPIO_PIN_10
#define ROT_A_GPIO_Port GPIOA
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define _7SEG_SRCK_Pin GPIO_PIN_10
#define _7SEG_SRCK_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
