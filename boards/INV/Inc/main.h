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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Io_SharedErrorHandlerOverride.h"
    /* USER CODE END Includes */

    /* Exported types
     * ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants
     * --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* Exported macro
     * ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

    /* Exported functions prototypes
     * ---------------------------------------------*/
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
#define LSI_FREQUENCY 32000
#define ADC1_NUM_CONVERSIONS 3
#define ADC2_NUM_CONVERSIONS 4
#define TASKCTRLLOOP_STACK_SIZE 512
#define FLASH_SPI_CLK_Pin GPIO_PIN_2
#define FLASH_SPI_CLK_GPIO_Port GPIOE
#define FLASH_nWP_Pin GPIO_PIN_3
#define FLASH_nWP_GPIO_Port GPIOE
#define FLASH_SPI_CS_Pin GPIO_PIN_4
#define FLASH_SPI_CS_GPIO_Port GPIOE
#define FLASH_SPI_MISO_Pin GPIO_PIN_5
#define FLASH_SPI_MISO_GPIO_Port GPIOE
#define FLASH_SPI_MOSI_Pin GPIO_PIN_6
#define FLASH_SPI_MOSI_GPIO_Port GPIOE
#define nDIAG_PHB_HS_Pin GPIO_PIN_13
#define nDIAG_PHB_HS_GPIO_Port GPIOC
#define nDIAG_PHC_LS_Pin GPIO_PIN_14
#define nDIAG_PHC_LS_GPIO_Port GPIOC
#define nDIAG_PHC_HS_Pin GPIO_PIN_15
#define nDIAG_PHC_HS_GPIO_Port GPIOC
#define nSHDN_GDRV_MCU_Pin GPIO_PIN_0
#define nSHDN_GDRV_MCU_GPIO_Port GPIOC
#define nSHDN_GDRV_Pin GPIO_PIN_1
#define nSHDN_GDRV_GPIO_Port GPIOC
#define PHA_LIN_IN_Pin GPIO_PIN_2
#define PHA_LIN_IN_GPIO_Port GPIOC
#define PHA_HIN_IN_Pin GPIO_PIN_3
#define PHA_HIN_IN_GPIO_Port GPIOC
#define MOD_TEMP_ADC_Pin GPIO_PIN_0
#define MOD_TEMP_ADC_GPIO_Port GPIOA
#define PHA_CUR_ADC_Pin GPIO_PIN_1
#define PHA_CUR_ADC_GPIO_Port GPIOA
#define VBUS_SENSE_ADC_Pin GPIO_PIN_2
#define VBUS_SENSE_ADC_GPIO_Port GPIOA
#define PHB_CUR_ADC_Pin GPIO_PIN_3
#define PHB_CUR_ADC_GPIO_Port GPIOA
#define MOTOR_TEMP_ADC_Pin GPIO_PIN_6
#define MOTOR_TEMP_ADC_GPIO_Port GPIOA
#define PHB_HIN_Pin GPIO_PIN_7
#define PHB_HIN_GPIO_Port GPIOA
#define GPIOA_1_Pin GPIO_PIN_4
#define GPIOA_1_GPIO_Port GPIOC
#define PHC_CUR_ADC_Pin GPIO_PIN_5
#define PHC_CUR_ADC_GPIO_Port GPIOC
#define PHA_HIN_Pin GPIO_PIN_0
#define PHA_HIN_GPIO_Port GPIOB
#define PHC_HIN_Pin GPIO_PIN_1
#define PHC_HIN_GPIO_Port GPIOB
#define ENDAT_DATA_SEND_Pin GPIO_PIN_7
#define ENDAT_DATA_SEND_GPIO_Port GPIOE
#define ENDAT_CLK_TX_Pin GPIO_PIN_9
#define ENDAT_CLK_TX_GPIO_Port GPIOE
#define nPHC_OC_ALARM_Pin GPIO_PIN_10
#define nPHC_OC_ALARM_GPIO_Port GPIOB
#define GDRV_SPI_CS_Pin GPIO_PIN_12
#define GDRV_SPI_CS_GPIO_Port GPIOB
#define GDRV_SPI_CLK_Pin GPIO_PIN_13
#define GDRV_SPI_CLK_GPIO_Port GPIOB
#define GDRV_SPI_MISO_Pin GPIO_PIN_14
#define GDRV_SPI_MISO_GPIO_Port GPIOB
#define GDRV_SPI_MOSI_Pin GPIO_PIN_15
#define GDRV_SPI_MOSI_GPIO_Port GPIOB
#define nDIAG_PHA_LS_Pin GPIO_PIN_11
#define nDIAG_PHA_LS_GPIO_Port GPIOD
#define nDIAG_PHA_HS_Pin GPIO_PIN_12
#define nDIAG_PHA_HS_GPIO_Port GPIOD
#define nDIAG_PHB_LS_Pin GPIO_PIN_13
#define nDIAG_PHB_LS_GPIO_Port GPIOD
#define PHB_LIN_Pin GPIO_PIN_6
#define PHB_LIN_GPIO_Port GPIOC
#define PHA_LIN_Pin GPIO_PIN_7
#define PHA_LIN_GPIO_Port GPIOC
#define PHC_LIN_Pin GPIO_PIN_8
#define PHC_LIN_GPIO_Port GPIOC
#define PHC_HIN_IN_Pin GPIO_PIN_9
#define PHC_HIN_IN_GPIO_Port GPIOC
#define nMOD_OT_ALARM_Pin GPIO_PIN_9
#define nMOD_OT_ALARM_GPIO_Port GPIOA
#define nMOTOR_OT_ALARM_Pin GPIO_PIN_10
#define nMOTOR_OT_ALARM_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define ENDAT_CLK_EN_Pin GPIO_PIN_15
#define ENDAT_CLK_EN_GPIO_Port GPIOA
#define CHANGE_THIS_REVC_Pin GPIO_PIN_10
#define CHANGE_THIS_REVC_GPIO_Port GPIOC
#define ENDAT_DATA_RX_Pin GPIO_PIN_11
#define ENDAT_DATA_RX_GPIO_Port GPIOC
#define ENDAT_DATA_TX_Pin GPIO_PIN_12
#define ENDAT_DATA_TX_GPIO_Port GPIOC
#define CAN_RX_Pin GPIO_PIN_0
#define CAN_RX_GPIO_Port GPIOD
#define CAN_TX_Pin GPIO_PIN_1
#define CAN_TX_GPIO_Port GPIOD
#define GPIOD_1_Pin GPIO_PIN_4
#define GPIOD_1_GPIO_Port GPIOD
#define GPIOD_2_Pin GPIO_PIN_5
#define GPIOD_2_GPIO_Port GPIOD
#define nPHA_OC_ALARM_Pin GPIO_PIN_8
#define nPHA_OC_ALARM_GPIO_Port GPIOB
#define nPHB_OC_ALARM_Pin GPIO_PIN_9
#define nPHB_OC_ALARM_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
