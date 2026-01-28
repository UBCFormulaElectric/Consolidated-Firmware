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
#include "stm32h7xx_hal.h"

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
#define tim_6_prescaler 256
#define tim_6_period 1000
#define SPI_SCK_Pin GPIO_PIN_2
#define SPI_SCK_GPIO_Port GPIOE
#define IR_P_EN_Pin GPIO_PIN_3
#define IR_P_EN_GPIO_Port GPIOE
#define SHDN_EN_Pin GPIO_PIN_4
#define SHDN_EN_GPIO_Port GPIOE
#define SPI_MISO_Pin GPIO_PIN_5
#define SPI_MISO_GPIO_Port GPIOE
#define SPI_MOSI_Pin GPIO_PIN_6
#define SPI_MOSI_GPIO_Port GPIOE
#define LEDR_Pin GPIO_PIN_13
#define LEDR_GPIO_Port GPIOC
#define SPI_C_LS_Pin GPIO_PIN_14
#define SPI_C_LS_GPIO_Port GPIOC
#define MSD_SHDN_SNS_Pin GPIO_PIN_0
#define MSD_SHDN_SNS_GPIO_Port GPIOC
#define HV_P_INTLCK_SNS_Pin GPIO_PIN_1
#define HV_P_INTLCK_SNS_GPIO_Port GPIOC
#define HV_N_INTLCK_SNS_Pin GPIO_PIN_2
#define HV_N_INTLCK_SNS_GPIO_Port GPIOC
#define AUX_TSENSE_Pin GPIO_PIN_3
#define AUX_TSENSE_GPIO_Port GPIOC
#define LEDB_Pin GPIO_PIN_0
#define LEDB_GPIO_Port GPIOA
#define EMETER_THERM_SNS_Pin GPIO_PIN_1
#define EMETER_THERM_SNS_GPIO_Port GPIOA
#define SHDN_DIAG_EN_Pin GPIO_PIN_2
#define SHDN_DIAG_EN_GPIO_Port GPIOA
#define nEVSE_I_LIM_PWM_Pin GPIO_PIN_3
#define nEVSE_I_LIM_PWM_GPIO_Port GPIOA
#define TS_ISENSE_400A_Pin GPIO_PIN_4
#define TS_ISENSE_400A_GPIO_Port GPIOA
#define TS_ISENSE_50A_Pin GPIO_PIN_5
#define TS_ISENSE_50A_GPIO_Port GPIOA
#define FAN_ISNS_Pin GPIO_PIN_6
#define FAN_ISNS_GPIO_Port GPIOA
#define SHDN_SNS_Pin GPIO_PIN_7
#define SHDN_SNS_GPIO_Port GPIOA
#define nIMD_LATCH_Pin GPIO_PIN_4
#define nIMD_LATCH_GPIO_Port GPIOC
#define DIAG_Pin GPIO_PIN_5
#define DIAG_GPIO_Port GPIOC
#define TS_VSENSE_N_Pin GPIO_PIN_0
#define TS_VSENSE_N_GPIO_Port GPIOB
#define TS_VSENSE_P_Pin GPIO_PIN_1
#define TS_VSENSE_P_GPIO_Port GPIOB
#define BSPD_OK_Pin GPIO_PIN_2
#define BSPD_OK_GPIO_Port GPIOB
#define nBMS_LATCH_Pin GPIO_PIN_7
#define nBMS_LATCH_GPIO_Port GPIOE
#define IMD_OK_Pin GPIO_PIN_8
#define IMD_OK_GPIO_Port GPIOE
#define nBSPD_LATCH_Pin GPIO_PIN_9
#define nBSPD_LATCH_GPIO_Port GPIOE
#define PRE_CHARGE_EN_Pin GPIO_PIN_11
#define PRE_CHARGE_EN_GPIO_Port GPIOE
#define SHDN_OL_ON_Pin GPIO_PIN_13
#define SHDN_OL_ON_GPIO_Port GPIOE
#define FAN_EN_Pin GPIO_PIN_14
#define FAN_EN_GPIO_Port GPIOE
#define FAN_PGOOD_Pin GPIO_PIN_15
#define FAN_PGOOD_GPIO_Port GPIOE
#define PGOOD_5V_Pin GPIO_PIN_10
#define PGOOD_5V_GPIO_Port GPIOB
#define ACCEL_BRAKE_OK_Pin GPIO_PIN_12
#define ACCEL_BRAKE_OK_GPIO_Port GPIOB
#define nBRAKE_PRESS_3V3_Pin GPIO_PIN_13
#define nBRAKE_PRESS_3V3_GPIO_Port GPIOB
#define nHIGH_CURRENT_BSPD_Pin GPIO_PIN_14
#define nHIGH_CURRENT_BSPD_GPIO_Port GPIOB
#define BSPD_TEST_EN_Pin GPIO_PIN_9
#define BSPD_TEST_EN_GPIO_Port GPIOD
#define TS_ISENSE_OCSC_OK_3V3_Pin GPIO_PIN_13
#define TS_ISENSE_OCSC_OK_3V3_GPIO_Port GPIOD
#define SPI_CS_HS_Pin GPIO_PIN_14
#define SPI_CS_HS_GPIO_Port GPIOD
#define SD_CD_Pin GPIO_PIN_7
#define SD_CD_GPIO_Port GPIOC
#define SD_D0_Pin GPIO_PIN_8
#define SD_D0_GPIO_Port GPIOC
#define SD_D1_Pin GPIO_PIN_9
#define SD_D1_GPIO_Port GPIOC
#define IMD_M_HS_3V3_Pin GPIO_PIN_8
#define IMD_M_HS_3V3_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SD_D2_Pin GPIO_PIN_10
#define SD_D2_GPIO_Port GPIOC
#define SD_D3_Pin GPIO_PIN_11
#define SD_D3_GPIO_Port GPIOC
#define SD_CK_Pin GPIO_PIN_12
#define SD_CK_GPIO_Port GPIOC
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define SD_CMD_Pin GPIO_PIN_2
#define SD_CMD_GPIO_Port GPIOD
#define TSENSE_SEL0_Pin GPIO_PIN_5
#define TSENSE_SEL0_GPIO_Port GPIOD
#define TSENSE_SEL1_Pin GPIO_PIN_6
#define TSENSE_SEL1_GPIO_Port GPIOD
#define TSENSE_SEL2_Pin GPIO_PIN_7
#define TSENSE_SEL2_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define CAN1_CHRG_RX_Pin GPIO_PIN_5
#define CAN1_CHRG_RX_GPIO_Port GPIOB
#define CAN1_CHRG_TX_Pin GPIO_PIN_6
#define CAN1_CHRG_TX_GPIO_Port GPIOB
#define SHDN_FAULT_Pin GPIO_PIN_8
#define SHDN_FAULT_GPIO_Port GPIOB
#define PGOOD_3V3_Pin GPIO_PIN_9
#define PGOOD_3V3_GPIO_Port GPIOB
#define BMS_OK_Pin GPIO_PIN_0
#define BMS_OK_GPIO_Port GPIOE
#define nEVSE_I_LIM_GPIO_Pin GPIO_PIN_1
#define nEVSE_I_LIM_GPIO_GPIO_Port GPIOE

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
