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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_error.h"
#include "cmsis_os.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */
    extern FDCAN_HandleTypeDef hfdcan2;
    extern I2C_HandleTypeDef   hi2c1;
    extern IWDG_HandleTypeDef  hiwdg1;
    extern SD_HandleTypeDef    hsd1;
    extern TIM_HandleTypeDef   htim15;
    extern UART_HandleTypeDef  huart2;
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
#define IWDG_RESET_FREQUENCY 5
#define IWDG_PRESCALER 4
#define LSI_FREQUENCY 32000
#define nRTC_INT1_Pin GPIO_PIN_5
#define nRTC_INT1_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOC
#define BOOT_LED_Pin GPIO_PIN_1
#define BOOT_LED_GPIO_Port GPIOC
#define SD_FAIL_Pin GPIO_PIN_3
#define SD_FAIL_GPIO_Port GPIOC
#define FROM_900M_CTS_Pin GPIO_PIN_0
#define FROM_900M_CTS_GPIO_Port GPIOA
#define TO_900M_RTS_Pin GPIO_PIN_1
#define TO_900M_RTS_GPIO_Port GPIOA
#define TO_900M_TX_Pin GPIO_PIN_2
#define TO_900M_TX_GPIO_Port GPIOA
#define FROM_900M_RX_Pin GPIO_PIN_3
#define FROM_900M_RX_GPIO_Port GPIOA
#define _900M_GPIO_Pin GPIO_PIN_4
#define _900M_GPIO_GPIO_Port GPIOA
#define TELEM_PWR_EN_Pin GPIO_PIN_9
#define TELEM_PWR_EN_GPIO_Port GPIOE
#define BUZZER_PWR_EN_Pin GPIO_PIN_15
#define BUZZER_PWR_EN_GPIO_Port GPIOE
#define nTSIM_GRN_EN_Pin GPIO_PIN_13
#define nTSIM_GRN_EN_GPIO_Port GPIOD
#define TSIM_RED_EN_Pin GPIO_PIN_15
#define TSIM_RED_EN_GPIO_Port GPIOD
#define SPLITTER_SHDN_SENSE_Pin GPIO_PIN_6
#define SPLITTER_SHDN_SENSE_GPIO_Port GPIOC
#define L_SHDN_SENSE_Pin GPIO_PIN_7
#define L_SHDN_SENSE_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_8
#define SD_CD_GPIO_Port GPIOA
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define RTC_I2C_SCL_Pin GPIO_PIN_6
#define RTC_I2C_SCL_GPIO_Port GPIOB
#define RTC_I2C_SDA_Pin GPIO_PIN_7
#define RTC_I2C_SDA_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
#endif
void DAM_StartAllTasks(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
