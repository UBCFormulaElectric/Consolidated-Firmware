/**
 * @brief Driver for interfacing with the 22XS4200 E-Fuse via SPI
 * @Settings Efuse max SPI transfer rate: 8MHz
 *           SPI Clock Polarity: 0 (SCK low-level idle state)
 *           SPI Clock Phase: 2nd edge (slave samples MOSI on SCK falling edge)
 *           Slave Select: Active Low (Must be toggled between SPI messages)
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// Current Sense Ratios
#define HIGH_CURRENT_SENSE_RATIO (1.0f / 1500.0f)
#define LOW_CURRENT_SENSE_RATIO (1.0f / 500.0f)

// Current Sense Channels
#define AUX1_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define AUX2_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH1
#define EM_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define PUMP_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH1
#define DRIVE_INVERTER_L_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define DRIVE_INVERTER_R_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH1
#define AIR_SHDN_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define GLV_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH1

//! Serial input register addresses

// STATR_s input address
#define SI_STATR_0_ADDR 0x00U
#define SI_STATR_1_ADDR 0x08U

#define SOA0_MASK 0x01U
#define SOA1_MASK 0x02U
#define SOA2_MASK 0x04U
#define SOA3_MASK 0x08U

#define SOA0_SHIFT 0x00U
#define SOA1_SHIFT 0x01U
#define SOA2_SHIFT 0x02U
#define SOA3_SHIFT 0x0DU

// PWMR_s input address
#define SI_PWMR_0_ADDR 0x01U
#define SI_PWMR_1_ADDR 0x09U

#define PWM0_S_MASK 0x01U
#define PWM1_S_MASK 0x02U
#define PWM2_S_MASK 0x04U
#define PWM3_S_MASK 0x08U
#define PWM4_S_MASK 0x10U
#define PWM5_S_MASK 0x20U
#define PWM6_S_MASK 0x40U
#define PWM7_S_MASK 0x80U
#define ON_S_MASK 0x100U

#define PWM0_S_SHIFT 0x00U
#define PWM1_S_SHIFT 0x01U
#define PWM2_S_SHIFT 0x02U
#define PWM3_S_SHIFT 0x03U
#define PWM4_S_SHIFT 0x04U
#define PWM5_S_SHIFT 0x05U
#define PWM6_S_SHIFT 0x06U
#define PWM7_S_SHIFT 0x07U
#define ON_S_SHIFT 0x08U

// CONFR_s input address
#define SI_CONFR_0_ADDR 0x02U
#define SI_CONFR_1_ADDR 0x0AU

#define DELAY0_S_MASK 0x01U
#define DELAY1_S_MASK 0x02U
#define DELAY2_S_MASK 0x04U
#define SR0_S_MASK 0x08U
#define SR1_S_MASK 0x10U
#define DIR_DIS_S_MASK 0x20U
#define OLOFF_DIS_S_MASK 0x40U
#define OLON_DIS_S_MASK 0x80U
#define OS_DIS_S_MASK 0x100U

#define DELAY0_S_SHIFT 0x00U
#define DELAY1_S_SHIFT 0x01U
#define DELAY2_S_SHIFT 0x02U
#define SR0_S_SHIFT 0x03U
#define SR1_S_SHIFT 0x04U
#define DIR_DIS_S_SHIFT 0x05U
#define OLOFF_DIS_S_SHIFT 0x06U
#define OLON_DIS_S_SHIFT 0x07U
#define OS_DIS_S_SHIFT 0x08U

// OCR_s input address
#define SI_OCR_0_ADDR 0x04U
#define SI_OCR_1_ADDR 0x0CU

#define OCL_S_MASK 0x01U
#define OCM_S_MASK 0x02U
#define OCH_S_MASK 0x04U
#define T_OCM_S_MASK 0x08U
#define T_OCH_S_MASK 0x10U
#define CSNS_RATIO_S_MASK 0x20U
#define CLOCK_INT_S_MASK 0x40U
#define PR_S_MASK 0x80U
#define HOCR_S_MASK 0x100U

#define OCL_S_SHIFT 0x00U
#define OCM_S_SHIFT 0x01U
#define OCH_S_SHIFT 0x02U
#define T_OCM_S_SHIFT 0x03U
#define T_OCH_S_SHIFT 0x04U
#define CSNS_RATIO_S_SHIFT 0x05U
#define CLOCK_INT_S_SHIFT 0x06U
#define PR_S_SHIFT 0x07U
#define HOCR_S_SHIFT 0x08U

// RETRY_s input address
#define SI_RETRY_0_ADDR 0x05U
#define SI_RETRY_1_ADDR 0x0DU

#define RETRY_S_MASK 0x01U
#define RETRY_UNLIMITED_S_MASK 0x02U
#define AUTO_PERIOD_0_S_MASK 0x04U
#define AUTO_PERIOD_1_S_MASK 0x08U
#define OFP_S_MASK 0x100U

#define RETRY_S_SHIFT 0x00U
#define RETRY_UNLIMITED_S_SHIFT 0x01U
#define AUTO_PERIOD_0_S_SHIFT 0x02U
#define AUTO_PERIOD_1_S_SHIFT 0x03U
#define OFP_S_SHIFT 0x08U

// GCR_s input address
#define SI_GCR_ADDR 0x06U

#define OV_DIS_MASK 0x01U
#define CSNS0_EN_MASK 0x02U
#define CSNS1_EN_MASK 0x04U
#define VDD_FAIL_EN_MASK 0x08U
#define WD_DIS_MASK 0x10U
#define T_H_EN_MASK 0x20U
#define PARALLEL_MASK 0x40U
#define PWM_EN_0_MASK 0x80U
#define PWM_EN_1_MASK 0x100U

#define OV_DIS_SHIFT 0x00U
#define CSNS0_EN_SHIFT 0x01U
#define CSNS1_EN_SHIFT 0x02U
#define VDD_FAIL_EN_SHIFT 0x03U
#define WD_DIS_SHIFT 0x04U
#define T_H_EN_SHIFT 0x05U
#define PARALLEL_SHIFT 0x06U
#define PWM_EN_0_SHIFT 0x07U
#define PWM_EN_1_SHIFT 0x08U

// CALR_s input address
#define SI_CALR_0_ADDR 0x07U
#define SI_CALR_1_ADDR 0x0FU
#define CALR_MASK 0x15BU

//! Serial output register addresses

// STATR output address
#define SO_STATR_ADDR 0x00U

#define SO_OUT0_MASK 0x01U
#define SO_OUT1_MASK 0x02U
#define SO_FAULT0_MASK 0x04U
#define SO_FAULT1_MASK 0x08U
#define SO_R_FULL0_MASK 0x10U
#define SO_R_FULL1_MASK 0x20U
#define SO_POR_MASK 0x40U
#define SO_UV_MASK 0x80U
#define SO_OV_MASK 0x100U

#define SO_OUT0_SHIFT 0x00U
#define SO_OUT1_SHIFT 0x01U
#define SO_FAULT0_SHIFT 0x02U
#define SO_FAULT1_SHIFT 0x03U
#define SO_R_FULL0_SHIFT 0x04U
#define SO_R_FULL1_SHIFT 0x05U
#define SO_POR_SHIFT 0x06U
#define SO_UV_SHIFT 0x07U
#define SO_OV_SHIFT 0x08U

// FAULTR_r output address
#define SO_FAULTR_0_ADDR 0x01U
#define SO_FAULTR_1_ADDR 0x09U

#define SO_OC_S_MASK 0x01U
#define SO_SC_S_MASK 0x02U
#define SO_OT_S_MASK 0x04U
#define SO_OS_S_MASK 0x08U
#define SO_OLOFF_MASK 0x10U
#define SO_OLON_MASK 0x20U
#define SO_OTW_MASK 0x100U

#define SO_OC_S_SHIFT 0x01U
#define SO_SC_S_SHIFT 0x02U
#define SO_OT_S_SHIFT 0x03U
#define SO_OS_S_SHIFT 0x04U
#define SO_OLOFF_SHIFT 0x05U
#define SO_OLON_SHIFT 0x06U
#define SO_OTW_SHIFT 0x08U

// PWMR_s output address
#define SO_PWMR_0_ADDR 0x02U
#define SO_PWMR_1_ADDR 0x0AU

#define SO_PWM_S_MASK 0xFFU
#define SO_ON_S_MASK 0x100U

#define SO_PWM_S_SHIFT 0x00U
#define SO_ON_S_SHIFT 0x08U

// CONFR_s output address
#define SO_CONFR_0_ADDR 0x03U
#define SO_CONFR_1_ADDR 0x0BU

#define SO_DELAY_S_MASK 0x07U
#define SO_SR_S_MASK 0x18U
#define SO_DIR_DIS_S_MASK 0x20U
#define SO_OLOFF_DIS_S_MASK 0x40U
#define SO_OLON_DIS_S_MASK 0x80U
#define SO_OS_DIS_S_MASK 0x100U

#define SO_DELAY_S_SHIFT 0x00U
#define SO_SR_S_SHIFT 0x03U
#define SO_DIR_DIS_S_SHIFT 0x05U
#define SO_OLOFF_DIS_S_SHIFT 0x06U
#define SO_OLON_DIS_S_SHIFT 0x07U
#define SO_OS_DIS_S_SHIFT 0x08U

// OCR_s output address
#define SO_OCR_0_ADDR 0x04U
#define SO_OCR_1_ADDR 0x0CU

#define SO_OCL_S_MASK 0x01U
#define SO_OCM_S_MASK 0x02U
#define SO_OCH_S_MASK 0x04U
#define SO_T_COM_S_MASK 0x08U
#define SO_T_OCH_S_MASK 0x10U
#define SO_CSNS_RATIO_S_MASK 0x20U
#define SO_CLOCK_INT_S_MASK 0x40U
#define SO_PR_S_MASK 0x80U
#define SO_HOCR_S_MASK 0x100U

#define SO_OCL_S_SHIFT 0x00U
#define SO_OCM_S_SHIFT 0x01U
#define SO_OCH_S_SHIFT 0x02U
#define SO_T_COM_S_SHIFT 0x03U
#define SO_T_COH_S_SHIFT 0x04U
#define SO_CSNS_RATIO_S_SHIFT 0x05U
#define SO_CLOCK_INT_S_SHIFT 0x06U
#define SO_PR_S_SHIFT 0x07U
#define SO_HOCR_S_SHIFT 0x08U

// RETRYR_S output address
#define SO_RETRYR_0_ADDR 0x05U
#define SO_RETRYR_1_ADDR 0x0DU

#define SO_RETRY_S_MASK 0x01U
#define SO_RETRY_UNLIMITED_S_MASK 0x02U
#define SO_AUTO_PERIOD_S_MASK 0x0CU
#define SO_R_MASK 0xF0U
#define SO_OFP_MASK 0x100U

#define SO_RETRY_S_SHIFT 0x00U
#define SO_RETRY_UNLIMITED_S_SHIFT 0x01U
#define SO_AUTO_PERIOD_S_SHIFT 0x02U
#define SO_R_SHIFT 0x04U
#define SO_OFP_SHIFT 0x08U

// GCR output address
#define SO_GCR_ADDR 0x06U

#define SO_OV_DIS_MASK 0x01U
#define SO_CSNS_EN_MASK 0x06U
#define SO_VDD_FAIL_EN_MASK 0x08U
#define SO_WD_DIS_MASK 0x10U
#define SO_T_H_EN_MASK 0x20U
#define SO_PARALLEL_MASK 0x40U
#define SO_PWM_EN_0_MASK 0x80U
#define SO_PWM_EN_1_MASK 0x100U

#define SO_OV_DIS_SHIFT 0x00U
#define SO_CSNS_EN_SHIFT 0x01U
#define SO_VDD_FAIL_EN_SHIFT 0x03U
#define SO_WD_DIS_SHIFT 0x04U
#define SO_T_H_EN_SHIFT 0x05U
#define SO_PARALLEL_SHIFT 0x06U
#define SO_PWM_EN_0_SHIFT 0x07U
#define SO_PWM_EN_1_SHIFT 0x08U

// DIAGR output address
#define SO_DIAGR_ADDR 0x07U

#define SO_CAL_FAIL0_MASK 0x01U
#define SO_CAL_FAIL1_MASK 0x02U
#define SO_CLOCK_FAIL_MASK 0x04U
#define SO_IN0_MASK 0x08U
#define SO_IN1_MASK 0x10U
#define SO_ID0_MASK 0x20U
#define SO_ID1_MASK 0x40U
#define SO_CONF0_MASK 0x80U
#define SO_CONF1_MASK 0x100U

#define SO_CAL_FAIL0_SHIFT 0x00U
#define SO_CAL_FAIL1_SHIFT 0x01U
#define SO_CLOCK_FAIL_SHIFT 0x02U
#define SO_IN0_SHIFT 0x03U
#define SO_IN1_SHIFT 0x04U
#define SO_ID0_SHIFT 0x05U
#define SO_ID1_SHIFT 0x06U
#define SO_CONF0_SHIFT 0x07U
#define SO_CONF1_SHIFT 0x08U

// Feature definitions in CONFR_s register
#define SWITCH_ON_DELAY_NO_DELAY 0x00U
#define SWITCH_ON_DELAY_32_PWM_CLOCKS 0x01U
#define SWITCH_ON_DELAY_64_PWM_CLOCKS 0x02U
#define SWITCH_ON_DELAY_96_PWM_CLOCKS 0x03U
#define SWITCH_ON_DELAY_128_PWM_CLOCKS 0x04U
#define SWITCH_ON_DELAY_160_PWM_CLOCKS 0x05U
#define SWITCH_ON_DELAY_192_PWM_CLOCKS 0x06U
#define SWITCH_ON_DELAY_224_PWM_CLOCKS 0x07U

#define SLEW_RATE_LOW (0x01 << SR0_S_SHIFT)
#define SLEW_RATE_MEDIUM (0x00 << SR0_S_SHIFT)
#define SLEW_RATE_HIGH (0x03 << SR0_S_SHIFT)
#define SLEW_RATE_HIGH_MAX (0x02 << SR0_S_SHIFT)

#define DIRECT_CONTROL_ENABLE (0x00 << DIR_DIS_S_SHIFT)
#define DIRECT_CONTROL_DISABLE (0x01 << DIR_DIS_S_SHIFT)

#define OPEN_LOAD_DETECTION_OFF_ENABLE (0x00 << OLOFF_DIS_S_SHIFT)
#define OPEN_LOAD_DETECTION_OFF_DISABLE (0x01 << OLOFF_DIS_S_SHIFT)

#define OPEN_LOAD_DETECTION_ON_ENABLE (0x00 << OLON_DIS_S_SHIFT)
#define OPEN_LOAD_DETECTION_ON_DISABLE (0x01 << OLON_DIS_S_SHIFT)

#define SHORT_CIRCUIT_DETECTION_ENABLE (0x00 << OS_DIS_S_SHIFT)
#define SHORT_CIRCUIT_DETECTION_DISABLE (0x01 << OS_DIS_S_SHIFT)

// Feature definitions in OCR_s register
#define LOW_CURRENT_THRESHOLD_SELECT_IOCL1 \
    ((0x00 << HOCR_S_SHIFT) | (0x01 << OCL_S_SHIFT))
#define LOW_CURRENT_THRESHOLD_SELECT_IOCL2 \
    ((0x01 << HOCR_S_SHIFT) | (0x00 << OCL_S_SHIFT))
#define LOW_CURRENT_THRESHOLD_SELECT_IOCL3 \
    ((0x01 << HOCR_S_SHIFT) | (0x01 << OCL_S_SHIFT))

#define MEDIUM_CURRENT_THRESHOLD_SELECT_IOCM1 (0x00 << OCM_S_SHIFT)
#define MEDIUM_CURRENT_THRESHOLD_SELECT_IOCM2 (0x01 << OCM_S_SHIFT)

#define HIGH_CURRENT_THRESHOLD_SELECT_IOCH1 (0x00 << OCH_S_SHIFT)
#define HICG_CURRENT_THRESHOLD_SELECT_IOCH2 (0x01 << OCH_S_SHIFT)
// bulb
#define THRESHOLD_ACTIVATION_TIMES_T_OCH1_T_OCM1_L \
    ((0x00 << T_OCH_S_SHIFT) | (0x00 << T_OCM_S_SHIFT))
#define THRESHOLD_ACTIVATION_TIMES_T_OCH1_T_OCM2_L \
    ((0x00 << T_OCH_S_SHIFT) | (0x01 << T_OCM_S_SHIFT))
#define THRESHOLD_ACTIVATION_TIMES_T_OCH2_T_OCM1_L \
    ((0x01 << T_OCH_S_SHIFT) | (0x00 << T_OCM_S_SHIFT))
#define THRESHOLD_ACTIVATION_TIMES_T_OCH2_T_OCM2_L \
    ((0x01 << T_OCH_S_SHIFT) | (0x01 << T_OCM_S_SHIFT))
// dc motor
#define THRESHOLD_ACTIVATION_TIMES_T_OCM1_M \
    ((0x00 << T_OCH_S_SHIFT) | (0x00 << T_OCM_s_SHIFT))
#define THRESHOLD_ACTIVATION_TIMES_T_OCM2_M \
    ((0x00 << T_OCH_S_SHIFT) | (0x01 << T_OCM_S_SHIFT))
#define THRESHOLD_ACTIVATION_TIMES_T_OCH1 \
    ((0x01 << T_OCH_S_SHIFT) | (0x00 << T_OCM_S_SHIFT))
#define THRESHOLD_ACTIVATION_TIMES_T_OCH2 \
    ((0x01 << T_OCH_S_SHIFT) | (0x01 << T_OCM_S_SHIFT))

#define CURRENT_SENSE_RATIO_LOW (0x01 << CSNS_RATIO_S_SHIFT)
#define CURRENT_SENSE_RATIO_HIGH (0x00 << CSNS_RATIO_S_SHIFT)

#define PWM_CLOCK_SOURCE_INTERNAL (0x01 << CLOCK_INT_S_SHIFT)
#define PWM_CLOCK_SOURCE_EXTERNAL (0x00 << CLOCK_INT_S_SHIFT)

#define PWM_EXTERNAL_CLOCK_DIVIDER_256 (0x00 << PR_S_SHIFT)
#define PWM_EXTERNAL_CLOCK_DIVIDER_512 (0x01 << PR_S_SHIFT)

// Feature definitions in RETRY_s register
#define AUTO_RETRY_PERIOD_T_AUTO_00 \
    ((0x00 << AUTO_PERIOD_1_S_SHIFT) | (0x00 << AUTO_PERIOD_0_S_SHIFT))
#define AUTO_RETRY_PERIOD_tAUTO_01 \
    ((0x00 << AUTO_PERIOD_1_S_SHIFT) | (0x01 << AUTO_PERIOD_0_S_SHIFT))
#define AUTO_RETRY_PERIOD_T_AUTO_10 \
    ((0x01 << AUTO_PERIOD_1_S_SHIFT) | (0x00 << AUTO_PERIOD_0_S_SHIFT))
#define AUTO_RETRY_PERIOD_T_AUTO_11 \
    ((0x01 << AUTO_PERIOD_1_S_SHIFT) | (0x01 << AUTO_PERIOD_0_S_SHIFT))

#define AUTO_RETRY_NUMBER_16 (0x00 << RETRY_UNLIMITED_S_SHIFT)
#define AUTO_RETRY_NUMBER_INFINITE (0x01 << RETRY_UNLIMITED_S_SHIFT)
#define AUTO_RETRY_BULB_ENABLED (0x00 << RETRY_S_SHIFT)
#define AUTO_RETRY_BULB_DISABLED (0x01 << RETRY_S_SHIFT)
#define AUTO_RETRY_DC_MOTOR_ENABLED (0x01 << RETRY_S_SHIFT)
#define AUTO_RETRY_DC_MOTOR_DISABLED (0x00 << RETRY_S_SHIFT)

#define RANDOM_CURRENT_OFFSET_ADD (0x01 << OFP_S_SHIFT)
#define RANDOM_CURRENT_OFFSET_SUBTRACT (0x00 << OFP_S_SHIFT)

// Feature definitions in GCR register
#define OVERVOLTAGE_PROTECTION_ENABLE (0x00 << OV_DIS_SHIFT)
#define OVERVOLTAGE_PROTECTION_DISABLE (0x01 << OV_DIS_SHIFT)

#define CSNS_FUNCTION_DISABLED \
    ((0x00 << CSNS1_EN_SHIFT) | (0x00 << CSNS0_EN_SHIFT))
#define CSNS_FUNCTION_CURRENT_CH0 \
    ((0x00 << CSNS1_EN_SHIFT) | (0x01 << CSNS0_EN_SHIFT))
#define CSNS_FUNCTION_CURRENT_CH1 \
    ((0x01 << CSNS1_EN_SHIFT) | (0x00 << CSNS0_EN_SHIFT))
#define CSNS_FUNCTION_TEMPERATURE \
    ((0x01 << CSNS1_EN_SHIFT) | (0x01 << CSNS0_EN_SHIFT))
#define CSNS_FUNCTION_CURRENT_SUM \
    ((0x00 << CSNS1_EN_SHIFT) | (0x01 << CSNS0_EN_SHIFT))

#define VDD_FAILURE_DETECTION_ENABLE (0x01 << VDD_FAIL_EN_SHIFT)
#define VDD_FAILURE_DETECTION_DISABLE (0x00 << VDD_FAIL_EN_SHIFT)

#define WATCHDOG_DISABLE (0x01 << WD_DIS_SHIFT)
#define WATCHDOG_ENABLE (0x00 << WD_DIS_SHIFT)

#define TRACK_AND_HOLD_CURRENT_ENABLE (0x01 << T_H_EN_SHIFT)
#define TRACK_AND_HOLD_CURRENT_DISABLE (0x00 << T_H_EN_SHIFT)

#define PARALLEL_MODE_ENABLE (0x01 << PARALLEL_SHIFT)
#define PARALLEL_MODE_DISABLE (0x00 << PARALLEL_SHIFT)

#define PWM_CHANNEL_0_ENABLE (0x01 << PWM_EN_0_SHIFT)
#define PWM_CHANNEL_0_DISABLE (0x00 << PWM_EN_0_SHIFT)

#define PWM_CHANNEL_1_ENABLE (0x01 << PWM_EN_1_SHIFT)
#define PWM_CHANNEL_1_DISABLE (0x00 << PWM_EN_1_SHIFT)

//! Configuration values used for initializing the registers

// Global Configuration
#define GCR_CONFIG                                                           \
    (PWM_CHANNEL_0_DISABLE | PWM_CHANNEL_1_DISABLE | PARALLEL_MODE_DISABLE | \
     TRACK_AND_HOLD_CURRENT_DISABLE | WATCHDOG_DISABLE |                     \
     VDD_FAILURE_DETECTION_DISABLE | CSNS_FUNCTION_DISABLED |                \
     OVERVOLTAGE_PROTECTION_ENABLE)

// Channel 0 Configurations
#define RETRY_0_CONFIG                                             \
    (RANDOM_CURRENT_OFFSET_SUBTRACT | AUTO_RETRY_NUMBER_INFINITE | \
     AUTO_RETRY_PERIOD_T_AUTO_10 | AUTO_RETRY_DC_MOTOR_DISABLED)
#define CONFR_0_CONFIG                                                  \
    (SHORT_CIRCUIT_DETECTION_DISABLE | OPEN_LOAD_DETECTION_ON_DISABLE | \
     OPEN_LOAD_DETECTION_OFF_DISABLE | DIRECT_CONTROL_ENABLE |          \
     SWITCH_ON_DELAY_NO_DELAY | SLEW_RATE_MEDIUM)
#define OCR_0_CONFIG                                              \
    (PWM_EXTERNAL_CLOCK_DIVIDER_256 | PWM_CLOCK_SOURCE_EXTERNAL | \
     THRESHOLD_ACTIVATION_TIMES_T_OCH1_T_OCM1_L |                 \
     MEDIUM_CURRENT_THRESHOLD_SELECT_IOCM1 |                      \
     LOW_CURRENT_THRESHOLD_SELECT_IOCL3 |                         \
     HIGH_CURRENT_THRESHOLD_SELECT_IOCH1 | CURRENT_SENSE_RATIO_LOW)

// Channel 1 Configurations
#define RETRY_1_CONFIG                                             \
    (RANDOM_CURRENT_OFFSET_SUBTRACT | AUTO_RETRY_NUMBER_INFINITE | \
     AUTO_RETRY_PERIOD_T_AUTO_10 | AUTO_RETRY_DC_MOTOR_DISABLED)
#define CONFR_1_CONFIG                                                  \
    (SHORT_CIRCUIT_DETECTION_DISABLE | OPEN_LOAD_DETECTION_ON_DISABLE | \
     OPEN_LOAD_DETECTION_OFF_DISABLE | DIRECT_CONTROL_ENABLE |          \
     SWITCH_ON_DELAY_NO_DELAY | SLEW_RATE_MEDIUM)
#define OCR_1_CONFIG                                              \
    (PWM_EXTERNAL_CLOCK_DIVIDER_256 | PWM_CLOCK_SOURCE_EXTERNAL | \
     THRESHOLD_ACTIVATION_TIMES_T_OCH1_T_OCM1_L |                 \
     MEDIUM_CURRENT_THRESHOLD_SELECT_IOCM1 |                      \
     LOW_CURRENT_THRESHOLD_SELECT_IOCL3 |                         \
     HIGH_CURRENT_THRESHOLD_SELECT_IOCH1 | CURRENT_SENSE_RATIO_LOW)

//! Serial Output Registers Enums

//! Enumeration for the status types
typedef enum
{
    StatusType_NoFault = (0 << 0), //!< No fault
    StatusType_OUT0    = (1 << 0), //!< Channel 0 Output state (ON/OFF)
    StatusType_OUT1    = (1 << 1), //!< Channel 0 Output state (ON/OFF)
    StatusType_FAULT0  = (1 << 2), //!< Fault on Channel 0
    StatusType_FAULT1  = (1 << 3), //!< Fault on Channel 1
    StatusType_R_FULL0 = (1 << 4), //!< Channel 0 Auto-retry Counter Full
    StatusType_R_FULL1 = (1 << 5), //!< Channel 1 Auto-retry Counter Full
    StatusType_POR     = (1 << 6), //!< Power-on Reset Has Occurred
    StatusType_UV      = (1 << 7), //!< Under Voltage Fault
    StatusType_OV      = (1 << 8)  //!< Over Voltage Fault
} StatusType_e;

//! Enumeration for the fault types
typedef enum
{
    FaultType_NoFault = (0 << 0), //!< No fault
    FaultType_OC      = (1 << 0), //!< Over Current fault
    FaultType_SC      = (1 << 1), //!< Severe Short Circuit fault
    FaultType_OT      = (1 << 2), //!< Over Temperature fault
    FaultType_OS      = (1 << 3), //!< Output Shorted to Vpwr fault
    FaultType_OLOFF   = (1 << 4), //!< Open Load in OFF State fault
    FaultType_OLON    = (1 << 5), //!< Open Load in ON State fault
    FaultType_OTW     = (1 << 8)  //!< Over Temperature Warning fault
} FaultType_e;

//! Enumeration for the PWM types
typedef enum
{
    PWM_Type_DC_0 = (1 << 0), //!< PWM Duty Cycle bit 0
    PWM_Type_DC_1 = (1 << 1), //!< PWM Duty Cycle bit 1
    PWM_Type_DC_2 = (1 << 2), //!< PWM Duty Cycle bit 2
    PWM_Type_DC_3 = (1 << 3), //!< PWM Duty Cycle bit 3
    PWM_Type_DC_4 = (1 << 4), //!< PWM Duty Cycle bit 4
    PWM_Type_DC_5 = (1 << 5), //!< PWM Duty Cycle bit 5
    PWM_Type_DC_6 = (1 << 6), //!< PWM Duty Cycle bit 6
    PWM_Type_DC_8 = (1 << 7), //!< PWM Duty Cycle bit 7
    PWM_Type_ON   = (1 << 8)  //!< PWM ON
} PWM_Type_e;

//! Enumeration for the channel configurations types
typedef enum
{
    ChannelConfigType_Delay_0    = (1 << 0), //!< Switch-on Delay Bit 0
    ChannelConfigType_Delay_1    = (1 << 1), //!< Switch-on Delay Bit 1
    ChannelConfigType_Delay_2    = (1 << 2), //!< Switch-on Delay Bit 2
    ChannelConfigType_SlewRate_0 = (1 << 3), //!< Slew Rate Bit 3
    ChannelConfigType_SlewRate_1 = (1 << 4), //!< Slew Rate Bit 4
    ChannelConfigType_DIR_Dis    = (1 << 5), //!< Direct Control Disable
    ChannelConfigType_OL_OFF_Dis =
        (1 << 6), //!< Open Load Detection in Off-state Disable
    ChannelConfigType_OL_ON_Dis =
        (1 << 7), //!< Open Load Detection in On-state Disable
    ChannelConfigType_OS_Dis =
        (1 << 8) //!< Short-circuit to Vpwr Detection Disable
} ChannelConfigType_e;

//! Enumeration for the overcurrent protection types
typedef enum
{
    OverCurrentConfigType_OCL = (1 << 0), //!< Lowest Over-current Profile
    OverCurrentConfigType_OCM = (1 << 1), //!< Over-current Medium Threshold
    OverCurrentConfigType_OCH = (1 << 2), //!< Over-current High Threshold
    OverCurrentConfigType_tOCM =
        (1 << 3), //!< Over-current Medium Threshold Window
    OverCurrentConfigType_tOCH =
        (1 << 4), //!< Over-current High Threshold Window
    OverCurrentConfigType_CurrentSense_Ratio =
        (1 << 5), //!< Low-current Sense Ratio
    OverCurrentConfigType_Clock_Internal =
        (1 << 6),                          //!< PWM Internal Clock Enable
    OverCurrentConfigType_PR   = (1 << 7), //!< PWM Frequency 2X Prescaler
    OverCurrentConfigType_HOCR = (1 << 8)  //!< Highest Over-current Profile
} OverCurrentConfigType_e;

//! Enumeration for the retry configurations types
typedef enum
{
    RetryType_AutoRetry = (1 << 0), //!< Auto Retry Disable for Lighting Profile
    RetryType_UnlimitedRetries = (1 << 1), //!< Unlimited Retries
    RetryType_Auto_Period0     = (1 << 2), //!< Auto-retry Period Bit 0
    RetryType_Auto_Period1     = (1 << 3), //!< Auto-retry Period Bit 1
    RetryType_ReservedBit_0    = (1 << 4), //!< Unused Bit 0
    RetryType_ReservedBit_1    = (1 << 5), //!< Unused Bit 0
    RetryType_ReservedBit_2    = (1 << 6), //!< Unused Bit 0
    RetryType_ReservedBit_3    = (1 << 7), //!< Unused Bit 0
    RetryType_Add_Offset_Current =
        (1 << 8) //!< Random Offset Current Added to Sensed Current
} RetryType_e;

//! Enumeration for the global configurations types
typedef enum
{
    GlobalConfigType_OvervoltageProtection_Disable =
        (1 << 0), //!< Over-voltage Protection Disable
    GlobalConfigType_CSNS0_Enable =
        (1 << 1), //!< Current Sensing on Channel 0 Enable
    GlobalConfigType_CSNS1_Enable =
        (1 << 2), //!< Current Sensing on Channel 1 Enable
    GlobalConfigType_Temperature_Sense_Enable =
        ((1 << 1) | (1 << 2)), //!< Temperature Sensing Enable
    GlobalConfigType_VDD_Fail_Enable =
        (1 << 3), //!< VDD Failure Detection Enable
    GlobalConfigType_Watchdog_Disable = (1 << 4), //!< SPI Watchdog Disable
    GlobalConfigType_TrackHold_Sense_Enable =
        (1 << 5), //!< Track and Hold Current Sensing Enable
    GlobalConfigType_Parallel_Mode_Enable =
        (1 << 6), //!< Parallel Channels Mode Enable
    GlobalConfigType_PWM_Channel0_Enable =
        (1 << 7), //!< Channel 0 PWM Function Enable
    GlobalConfigType_PWM_Channel1_Enable =
        (1 << 8) //!< Channel 1 PWM Function Enable
} GlobalConfigType_e;

//! Enumeration for the diagnostic configurations types
typedef enum
{
    DiagnosticType_Cal0_Failure =
        (1 << 0), //!< Channel 0 Internal Clock Calibration Failure
    DiagnosticType_Cal1_Failure =
        (1 << 1), //!< Channel 1 Internal Clock Calibration Failure
    DiagnosticType_ExtClock_Failure = (1 << 2), //!< External Clock Failure
    DiagnosticType_Input0_On        = (1 << 3), //!< Input 0 Pin On
    DiagnosticType_Input1_On        = (1 << 4), //!< Input 1 Pin On
    DiagnosticType_ID0              = (1 << 5), //!< Product ID Number Bit 0
    DiagnosticType_ID1              = (1 << 6), //!< Product ID Number Bit 1
    DiagnosticType_CONF0            = (1 << 7), //!< Channel 0 Config DC Motor
    DiagnosticType_CONF1            = (1 << 8)  //!< Channel 1 Config DC Motor
} DiagnosticType_e;

struct Efuse
{
    bool         watch_dog_state;
    StatusType_e status;
    struct ChannelFaults
    {
        FaultType_e channel_0_faults;
        FaultType_e channel_1_faults;
    } faults;
    struct EfuseChipSelect
    {
        GPIO_TypeDef *GPIO_Port;
        uint16_t      GPIO_Pin;
    } chip_select;
    struct EfuseCurrentSenseSync
    {
        GPIO_TypeDef *GPIO_Port;
        uint16_t      GPIO_Pin;
    } current_sense_sync;
};

/**
 * Initializes the SPI handle for the Efuses
 * @param hspi Handle to the SPI peripheral used for the Efuses
 */
void Io_Efuse_Init(SPI_HandleTypeDef *const hspi);

/**
 * Configures the Efuse's registers using the values defined in:
 * REGISTERNAME_CONFIG
 * @param e_fuse Pointer to Efuse structure being configured
 */
void Io_Efuse_ConfigureEfuse(struct Efuse *e_fuse);

/**
 * Sets current/temperature monitoring option of CSNS pin
 * @param selection The desired monitoring function which can be selected by
 * specifying the following values: CSNS_FUNCTION_DISABLED - Current sensing
 * disabled CSNS_FUNCTION_CURRENT_CH0 - Current sensing for channel 0
 *                  CSNS_FUNCTION_CURRENT_CH1 - Current sensing for channel 1
 *                  CSNS_FUNCTION_TEMPERATURE - Ffuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @param e_fuse Pointer to Efuse structure being configured
 */
void Io_Efuse_ConfigureChannelMonitoring(
    uint8_t       selection,
    struct Efuse *e_fuse);

/**
 * Gets the value of the Efuse's Status Register over SPI and updates the Efuse
 * handle's operating status
 * @param e_fuse Pointer to Efuse structure being checked
 */
void Io_Efuse_UpdateStatus(struct Efuse *e_fuse);

/**
 * Get the value of the Efuse's Fault Register over SPI and updates the Efuse
 * fault status
 * @param e_fuse Pointer to Efuse structure being checked
 */
void Io_Efuse_UpdateFaults(struct Efuse *e_fuse);

/**
 * Write data to a specific Serial Input register on the Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @param e_fuse Pointer to Efuse structure being written to
 */
void Io_Efuse_WriteRegister(
    uint8_t       register_address,
    uint16_t      register_value,
    struct Efuse *e_fuse);

/**
 * Read data from a specific Serial Output register on the Efuse
 * @param register_address Serial Output register being read from
 * @param e_fuse Pointer to Efuse structure being read from
 * @return Contents of the addressed register
 */
uint16_t Io_Efuse_ReadRegister(uint8_t register_address, struct Efuse *e_fuse);

/**
 * Calculates the parity of the SPI command to be sent to the Efuse.
 * Using the XOR sum of bits method taken from:
 * https://en.wikipedia.org/wiki/Parity_bit#Parity
 * @param spi_command Original SPI command without the parity bit
 * @return Returns true if the parity bit needs to be set
 */
bool Io_Efuse_SetParityBit(uint16_t* spi_command);

/**
 * Write SPI data to Efuse in blocking mode
 * @param *TxData Pointer to data being sent to the Efuse
 * @param *GPIO_Port Handle to Efuse's chip-select GPIO port
 * @param GPIO_Pin Efuse's chip-select GPIO pin
 * @return Status of the SPI transfer
 */
HAL_StatusTypeDef Io_Efuse_WriteToEfuse(
    uint16_t *    TxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin);

/**
 * Read SPI data from Efuse in blocking mode
 * @param *TxData Pointer to data being sent to the Efuse
 * @param *RxData Pointer to data being received from the Efuse
 * @param *GPIO_Port Handle to Efuse's chip-select GPIO port
 * @param GPIO_Pin Efuse's chip-select GPIO pin
 * @return Status of the SPI transfer
 */
HAL_StatusTypeDef Io_Efuse_ReadFromEfuse(
    uint16_t *    TxData,
    uint16_t *    RxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin);
