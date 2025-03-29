/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern ADC_HandleTypeDef   hadc1;
    extern ADC_HandleTypeDef   hadc3;
    extern DMA_HandleTypeDef   hdma_adc1;
    extern FDCAN_HandleTypeDef hfdcan1;
    extern I2C_HandleTypeDef   hi2c1;
    extern I2C_HandleTypeDef   hi2c2;
    extern IWDG_HandleTypeDef  hiwdg1;
    extern SD_HandleTypeDef    hsd1;
    extern TIM_HandleTypeDef   htim3;
    extern UART_HandleTypeDef  huart7;
    extern UART_HandleTypeDef  huart1;
    extern UART_HandleTypeDef  huart2;
    extern UART_HandleTypeDef  huart3;
    extern DMA_HandleTypeDef   hdma_usart2_rx;

    extern osThreadId_t Task1HzHandle;
    extern osThreadId_t Task100HzHandle;
    extern osThreadId_t Task1kHzHandle;
    extern osThreadId_t TaskCanRxHandle;
    extern osThreadId_t TaskCanTxHandle;
    extern osThreadId_t TaskLoggingHandle;
    extern osThreadId_t TaskTelemHandle;

    extern const osThreadAttr_t Task100Hz_attributes;
    extern const osThreadAttr_t TaskCanTx_attributes;
    extern const osThreadAttr_t TaskCanRx_attributes;
    extern const osThreadAttr_t Task1kHz_attributes;
    extern const osThreadAttr_t Task1Hz_attributes;
    extern const osThreadAttr_t TaskLogging_attributes;
    extern const osThreadAttr_t TaskTelem_attributes;

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
#define TIM3_PRESCALER 8
#define ADC_FREQUENCY 1000
#define IWDG_PRESCALER 4
#define LSI_FREQUENCY 32000
#define TIMx_FREQUENCY 512000000
#define SBG_ELLIPSE_GPS_BAUD_RATE 460800
#define INV_R_PWR_EN_Pin GPIO_PIN_2
#define INV_R_PWR_EN_GPIO_Port GPIOE
#define INV_R_PROGRAM_Pin GPIO_PIN_3
#define INV_R_PROGRAM_GPIO_Port GPIOE
#define INV_L_PROGRAM_Pin GPIO_PIN_4
#define INV_L_PROGRAM_GPIO_Port GPIOE
#define SHDN_PWR_EN_Pin GPIO_PIN_6
#define SHDN_PWR_EN_GPIO_Port GPIOE
#define FR_STBY1_Pin GPIO_PIN_13
#define FR_STBY1_GPIO_Port GPIOC
#define LV_PWR_EN_Pin GPIO_PIN_14
#define LV_PWR_EN_GPIO_Port GPIOC
#define INV_R_PWR_I_SNS_Pin GPIO_PIN_0
#define INV_R_PWR_I_SNS_GPIO_Port GPIOC
#define INV_L_PWR_I_SNS_Pin GPIO_PIN_1
#define INV_L_PWR_I_SNS_GPIO_Port GPIOC
#define AUX_PWR_I_SNS_Pin GPIO_PIN_2
#define AUX_PWR_I_SNS_GPIO_Port GPIOC
#define PUMP_PWR_I_SNS_Pin GPIO_PIN_3
#define PUMP_PWR_I_SNS_GPIO_Port GPIOC
#define BUZZER_PWR_EN_Pin GPIO_PIN_0
#define BUZZER_PWR_EN_GPIO_Port GPIOA
#define BAT_I_SNS_nFLT_Pin GPIO_PIN_1
#define BAT_I_SNS_nFLT_GPIO_Port GPIOA
#define BAT_I_SNS_Pin GPIO_PIN_2
#define BAT_I_SNS_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOA
#define SHDN_PWR_I_SNS_Pin GPIO_PIN_4
#define SHDN_PWR_I_SNS_GPIO_Port GPIOA
#define VBAT_SENSE_Pin GPIO_PIN_5
#define VBAT_SENSE_GPIO_Port GPIOA
#define _24V_ACC_SENSE_Pin GPIO_PIN_6
#define _24V_ACC_SENSE_GPIO_Port GPIOA
#define _22V_BOOST_SENSE_Pin GPIO_PIN_7
#define _22V_BOOST_SENSE_GPIO_Port GPIOA
#define LV_PWR_I_SNS_Pin GPIO_PIN_4
#define LV_PWR_I_SNS_GPIO_Port GPIOC
#define R_SHDN_SNS_Pin GPIO_PIN_5
#define R_SHDN_SNS_GPIO_Port GPIOC
#define TSMS_SHDN_SNS_Pin GPIO_PIN_0
#define TSMS_SHDN_SNS_GPIO_Port GPIOB
#define ACC_I_SENSE_Pin GPIO_PIN_1
#define ACC_I_SENSE_GPIO_Port GPIOB
#define SB_ILCK_SHDN_SNS_Pin GPIO_PIN_2
#define SB_ILCK_SHDN_SNS_GPIO_Port GPIOB
#define L_SHDN_SNS_Pin GPIO_PIN_9
#define L_SHDN_SNS_GPIO_Port GPIOE
#define IMU_INT1_Pin GPIO_PIN_14
#define IMU_INT1_GPIO_Port GPIOE
#define IMU_INT2_Pin GPIO_PIN_15
#define IMU_INT2_GPIO_Port GPIOE
#define nPCM_EN_Pin GPIO_PIN_12
#define nPCM_EN_GPIO_Port GPIOB
#define ACC_I_SENSE_nFLT_Pin GPIO_PIN_14
#define ACC_I_SENSE_nFLT_GPIO_Port GPIOB
#define PUMP_PWR_EN_Pin GPIO_PIN_10
#define PUMP_PWR_EN_GPIO_Port GPIOD
#define NCHIMERA_Pin GPIO_PIN_11
#define NCHIMERA_GPIO_Port GPIOD
#define _900K_GPIO_Pin GPIO_PIN_13
#define _900K_GPIO_GPIO_Port GPIOD
#define nCHRG_FAULT_Pin GPIO_PIN_14
#define nCHRG_FAULT_GPIO_Port GPIOD
#define nCHRG_Pin GPIO_PIN_15
#define nCHRG_GPIO_Port GPIOD
#define PGOOD_Pin GPIO_PIN_6
#define PGOOD_GPIO_Port GPIOC
#define FR_STBY2_Pin GPIO_PIN_7
#define FR_STBY2_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_8
#define SD_CD_GPIO_Port GPIOA
#define TELEM_PWR_EN_Pin GPIO_PIN_15
#define TELEM_PWR_EN_GPIO_Port GPIOA
#define AUX_PWR_EN_Pin GPIO_PIN_1
#define AUX_PWR_EN_GPIO_Port GPIOD
#define NPROGRAM_3V3_Pin GPIO_PIN_5
#define NPROGRAM_3V3_GPIO_Port GPIOB
#define INV_L_PWR_EN_Pin GPIO_PIN_0
#define INV_L_PWR_EN_GPIO_Port GPIOE
#define FR_STBY3_Pin GPIO_PIN_1
#define FR_STBY3_GPIO_Port GPIOE

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
