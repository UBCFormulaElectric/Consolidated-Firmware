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
    extern ADC_HandleTypeDef hadc1;
    extern DMA_HandleTypeDef hdma_adc1;
    extern CAN_HandleTypeDef hcan2;
    extern TIM_HandleTypeDef htim2;
    extern I2C_HandleTypeDef hi2c1;
    // extern PCD_HandleTypeDef hpcd_USB_OTG_FS; needed??

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
#define TASKS_1HZ_STACK_SIZE 512
#define TASK_1KHZ_STACK_SIZE 512
#define TASK_CANTX_STACK_SIZE 512
#define TASK_CANRX_STACK_SIZE 512
#define TIM3_PRESCALER 8
#define TIM12_PWM_MINIMUM_FREQUENCY 1
#define ADC_FREQUENCY 1000
#define TIMx_FREQUENCY 96000000
#define IWDG_RESET_FREQUENCY 5
#define TIM12_PRESCALER (TIMx_FREQUENCY/ TIM12_AUTO_RELOAD_REG/ TM12_PWM_MINIMUM_FREQUENCY)
#define TIM12_AUTO_RELOAD_REG 0xFFFF
#define IWDG_PRESCALER 4
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define LSI_FREQUENCY 32000
#define STR_ANGLE_3V3_Pin GPIO_PIN_1
#define STR_ANGLE_3V3_GPIO_Port GPIOC
#define APPS1_3V3_Pin GPIO_PIN_2
#define APPS1_3V3_GPIO_Port GPIOC
#define FR_INT_3V3_Pin GPIO_PIN_3
#define FR_INT_3V3_GPIO_Port GPIOC
#define SUSP_FL_3V3_Pin GPIO_PIN_1
#define SUSP_FL_3V3_GPIO_Port GPIOA
#define SUSP_FR_3V3_Pin GPIO_PIN_2
#define SUSP_FR_3V3_GPIO_Port GPIOA
#define BOTS_3V3_Pin GPIO_PIN_3
#define BOTS_3V3_GPIO_Port GPIOA
#define APPS2_3V3_Pin GPIO_PIN_5
#define APPS2_3V3_GPIO_Port GPIOA
#define BPS_F_3V3_Pin GPIO_PIN_7
#define BPS_F_3V3_GPIO_Port GPIOA
#define FL_INT_3V3_Pin GPIO_PIN_4
#define FL_INT_3V3_GPIO_Port GPIOC
#define COCKPIT_SHDN_3V3_Pin GPIO_PIN_1
#define COCKPIT_SHDN_3V3_GPIO_Port GPIOB
#define nSUSP_FR_OCSC_Pin GPIO_PIN_10
#define nSUSP_FR_OCSC_GPIO_Port GPIOB
#define DEBUG_LED_Pin GPIO_PIN_6
#define DEBUG_LED_GPIO_Port GPIOC
#define nSTR_ANGLE_OCSC_Pin GPIO_PIN_7
#define nSTR_ANGLE_OCSC_GPIO_Port GPIOC
#define nBPS_F_OCSC_Pin GPIO_PIN_8
#define nBPS_F_OCSC_GPIO_Port GPIOC
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define BOOT_LED_Pin GPIO_PIN_12
#define BOOT_LED_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define IMU_INT1_Pin GPIO_PIN_4
#define IMU_INT1_GPIO_Port GPIOB
#define IMU_INT2_Pin GPIO_PIN_5
#define IMU_INT2_GPIO_Port GPIOB
#define IMU_SDA_Pin GPIO_PIN_7
#define IMU_SDA_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_8
#define IMU_SCL_GPIO_Port GPIOB
#define nSUSP_FL_OCSC_Pin GPIO_PIN_9
#define nSUSP_FL_OCSC_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
