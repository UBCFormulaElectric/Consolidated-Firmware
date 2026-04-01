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
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern ADC_HandleTypeDef   hadc1;
    extern ADC_HandleTypeDef   hadc2;
    extern FDCAN_HandleTypeDef hfdcan1;
    extern FDCAN_HandleTypeDef hfdcan2;
    extern FDCAN_HandleTypeDef hfdcan3;
    extern I2C_HandleTypeDef   hi2c2;
    extern I2C_HandleTypeDef   hi2c3;
    extern I2C_HandleTypeDef   hi2c4;
    extern I2C_HandleTypeDef   hi2c5;
    extern IWDG_HandleTypeDef  hiwdg1;
    extern TIM_HandleTypeDef   htim3;
    extern UART_HandleTypeDef  huart8;
    extern PCD_HandleTypeDef   hpcd_USB_OTG_HS;
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
#define SB_SHDN_3V3_OUT_Pin GPIO_PIN_0
#define SB_SHDN_3V3_OUT_GPIO_Port GPIOC
#define INTERIA_3V3_OUT_Pin GPIO_PIN_1
#define INTERIA_3V3_OUT_GPIO_Port GPIOC
#define F_PUMP_I_SNS_Pin GPIO_PIN_0
#define F_PUMP_I_SNS_GPIO_Port GPIOA
#define RR_PUMP_I_SNS_Pin GPIO_PIN_1
#define RR_PUMP_I_SNS_GPIO_Port GPIOA
#define DAM_I_SNS_Pin GPIO_PIN_2
#define DAM_I_SNS_GPIO_Port GPIOA
#define FRONT_I_SNS_Pin GPIO_PIN_3
#define FRONT_I_SNS_GPIO_Port GPIOA
#define RSM_I_SNS_Pin GPIO_PIN_4
#define RSM_I_SNS_GPIO_Port GPIOA
#define L_RAD_FAN_I_SNS_Pin GPIO_PIN_5
#define L_RAD_FAN_I_SNS_GPIO_Port GPIOA
#define R_RAD_FAN_I_SNS_Pin GPIO_PIN_6
#define R_RAD_FAN_I_SNS_GPIO_Port GPIOA
#define FR_STBY_INV_Pin GPIO_PIN_7
#define FR_STBY_INV_GPIO_Port GPIOA
#define RL_PUMP_I_SNS_Pin GPIO_PIN_4
#define RL_PUMP_I_SNS_GPIO_Port GPIOC
#define F_INV_I_SNS_Pin GPIO_PIN_5
#define F_INV_I_SNS_GPIO_Port GPIOC
#define R_INV_I_SNS_Pin GPIO_PIN_0
#define R_INV_I_SNS_GPIO_Port GPIOB
#define BMS_I_SNS_Pin GPIO_PIN_1
#define BMS_I_SNS_GPIO_Port GPIOB
#define TSMS_3V3_OUT_Pin GPIO_PIN_2
#define TSMS_3V3_OUT_GPIO_Port GPIOB
#define RR_ILCK_3V3_OUT_Pin GPIO_PIN_7
#define RR_ILCK_3V3_OUT_GPIO_Port GPIOE
#define R_INV_EN_Pin GPIO_PIN_8
#define R_INV_EN_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_9
#define LED_GPIO_Port GPIOE
#define BOOT_LED_Pin GPIO_PIN_10
#define BOOT_LED_GPIO_Port GPIOE
#define BMS_EN_Pin GPIO_PIN_11
#define BMS_EN_GPIO_Port GPIOE
#define FR_STBY_FRONT_Pin GPIO_PIN_12
#define FR_STBY_FRONT_GPIO_Port GPIOE
#define PGOOD_Pin GPIO_PIN_13
#define PGOOD_GPIO_Port GPIOE
#define BAT_CHRG_MODE_Pin GPIO_PIN_14
#define BAT_CHRG_MODE_GPIO_Port GPIOE
#define BAT_CHRG_nSHDN_Pin GPIO_PIN_15
#define BAT_CHRG_nSHDN_GPIO_Port GPIOE
#define BAT_MTR_SCL_Pin GPIO_PIN_10
#define BAT_MTR_SCL_GPIO_Port GPIOB
#define BAT_MTR_SDA_Pin GPIO_PIN_11
#define BAT_MTR_SDA_GPIO_Port GPIOB
#define BAT_MTR_nALERT_Pin GPIO_PIN_12
#define BAT_MTR_nALERT_GPIO_Port GPIOB
#define BAT_MTR_nALERT_EXTI_IRQn EXTI15_10_IRQn
#define FR_STBY_REAR_Pin GPIO_PIN_13
#define FR_STBY_REAR_GPIO_Port GPIOB
#define FRONT_EN_Pin GPIO_PIN_14
#define FRONT_EN_GPIO_Port GPIOB
#define RSM_EN_Pin GPIO_PIN_15
#define RSM_EN_GPIO_Port GPIOB
#define IMU_INT1_Pin GPIO_PIN_9
#define IMU_INT1_GPIO_Port GPIOD
#define IMU_INT1_EXTI_IRQn EXTI9_5_IRQn
#define IMU_INT2_Pin GPIO_PIN_10
#define IMU_INT2_GPIO_Port GPIOD
#define IMU_INT2_EXTI_IRQn EXTI15_10_IRQn
#define RAD_FAN_FR_STBY_Pin GPIO_PIN_6
#define RAD_FAN_FR_STBY_GPIO_Port GPIOC
#define PWR_MTR_nALERT_Pin GPIO_PIN_7
#define PWR_MTR_nALERT_GPIO_Port GPIOC
#define PWR_MTR_nALERT_EXTI_IRQn EXTI9_5_IRQn
#define RL_PUMP_PGOOD_Pin GPIO_PIN_8
#define RL_PUMP_PGOOD_GPIO_Port GPIOC
#define IMU_SDA_Pin GPIO_PIN_9
#define IMU_SDA_GPIO_Port GPIOC
#define IMU_SCL_Pin GPIO_PIN_8
#define IMU_SCL_GPIO_Port GPIOA
#define F_INV_EN_Pin GPIO_PIN_9
#define F_INV_EN_GPIO_Port GPIOA
#define DAM_EN_Pin GPIO_PIN_10
#define DAM_EN_GPIO_Port GPIOA
#define SWDIO_1_Pin GPIO_PIN_13
#define SWDIO_1_GPIO_Port GPIOA
#define SWCLK_1_Pin GPIO_PIN_14
#define SWCLK_1_GPIO_Port GPIOA
#define PWR_MTR_SDA_Pin GPIO_PIN_10
#define PWR_MTR_SDA_GPIO_Port GPIOC
#define PWR_MTR_SCL_Pin GPIO_PIN_11
#define PWR_MTR_SCL_GPIO_Port GPIOC
#define RR_PUMP_EN_Pin GPIO_PIN_12
#define RR_PUMP_EN_GPIO_Port GPIOC
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define RR_PUMP_PGOOD_Pin GPIO_PIN_2
#define RR_PUMP_PGOOD_GPIO_Port GPIOD
#define PCM_EN_Pin GPIO_PIN_3
#define PCM_EN_GPIO_Port GPIOD
#define F_PUMP_PGOOD_Pin GPIO_PIN_4
#define F_PUMP_PGOOD_GPIO_Port GPIOD
#define F_PUMP_EN_Pin GPIO_PIN_5
#define F_PUMP_EN_GPIO_Port GPIOD
#define R_RAD_FAN_EN_Pin GPIO_PIN_6
#define R_RAD_FAN_EN_GPIO_Port GPIOD
#define RL_PUMP_EN_Pin GPIO_PIN_7
#define RL_PUMP_EN_GPIO_Port GPIOD
#define SWO1_Pin GPIO_PIN_3
#define SWO1_GPIO_Port GPIOB
#define CAN3_RX_Pin GPIO_PIN_5
#define CAN3_RX_GPIO_Port GPIOB
#define CAN3_TX_Pin GPIO_PIN_6
#define CAN3_TX_GPIO_Port GPIOB
#define L_RAD_FAN_EN_Pin GPIO_PIN_7
#define L_RAD_FAN_EN_GPIO_Port GPIOB
#define R_PUMP_SCL_Pin GPIO_PIN_8
#define R_PUMP_SCL_GPIO_Port GPIOB
#define R_PUMP_SDA_Pin GPIO_PIN_9
#define R_PUMP_SDA_GPIO_Port GPIOB
#define FROM_GPS_RX_Pin GPIO_PIN_0
#define FROM_GPS_RX_GPIO_Port GPIOE
#define TO_GPS_TX_Pin GPIO_PIN_1
#define TO_GPS_TX_GPIO_Port GPIOE

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
