#include <assert.h>
#include "Io_Efuse.h"
#include "Io_CurrentSense.h"

#pragma GCC diagnostic ignored "-Wconversion"

#define EXIT_OK_OR_RETURN(CODE) \
    if (CODE)                   \
    return (CODE)

// Register data masks
#define EFUSE_ADDR_MASK 0xFU
#define EFUSE_ADDR_SHIFT 0x0AU
#define EFUSE_SI_DATA_MASK 0x1FFU
#define EFUSE_SO_DATA_MASK 0xFF // Ignore Normal-Mode status bit (bit 9)

#define WATCHDOG_BIT (1U << 15U)
#define PARITY_BIT (1U << 14U)

// Current sense channels
#define AUX1_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define AUX2_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH1
#define AIR_SHUTDOWN_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define LV_POWER_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH1
#define DRIVE_INVERTER_BACK_LEFT_CURRENT_SENSE_CHANNEL CSNS_FUNCTION_CURRENT_CH0
#define DRIVE_INVERTER_BACK_RIGHT_CURRENT_SENSE_CHANNEL \
    CSNS_FUNCTION_CURRENT_CH1
#define DRIVE_INVERTER_FRONT_LEFT_CURRENT_SENSE_CHANNEL \
    CSNS_FUNCTION_CURRENT_CH0
#define DRIVE_INVERTER_FRONT_RIGHT_CURRENT_SENSE_CHANNEL \
    CSNS_FUNCTION_CURRENT_CH1

// Serial input register addresses

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

// Serial output register addresses

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
#define HIGH_CURRENT_THRESHOLD_SELECT_IOCH2 (0x01 << OCH_S_SHIFT)
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

// Configuration values used for initializing the registers

// Global Configuration
#define GCR_CONFIG                                                           \
    (PWM_CHANNEL_0_DISABLE | PWM_CHANNEL_1_DISABLE | PARALLEL_MODE_DISABLE | \
     TRACK_AND_HOLD_CURRENT_DISABLE | WATCHDOG_DISABLE |                     \
     VDD_FAILURE_DETECTION_ENABLE | CSNS_FUNCTION_DISABLED |                 \
     OVERVOLTAGE_PROTECTION_ENABLE)

// Channel  Configurations
#define RETRY_CONFIG                                               \
    (RANDOM_CURRENT_OFFSET_SUBTRACT | AUTO_RETRY_NUMBER_INFINITE | \
     AUTO_RETRY_PERIOD_T_AUTO_10 | AUTO_RETRY_DC_MOTOR_DISABLED)
#define CONFR_CONFIG                                                  \
    (SHORT_CIRCUIT_DETECTION_ENABLE | OPEN_LOAD_DETECTION_ON_ENABLE | \
     OPEN_LOAD_DETECTION_OFF_ENABLE | DIRECT_CONTROL_ENABLE |         \
     SWITCH_ON_DELAY_NO_DELAY | SLEW_RATE_MEDIUM)
#define OCR_LOW_CURRENT_SENSE_CONFIG                              \
    (PWM_EXTERNAL_CLOCK_DIVIDER_256 | PWM_CLOCK_SOURCE_EXTERNAL | \
     THRESHOLD_ACTIVATION_TIMES_T_OCH1_T_OCM1_L |                 \
     MEDIUM_CURRENT_THRESHOLD_SELECT_IOCM2 |                      \
     LOW_CURRENT_THRESHOLD_SELECT_IOCL2 |                         \
     HIGH_CURRENT_THRESHOLD_SELECT_IOCH2 | CURRENT_SENSE_RATIO_LOW)
#define OCR_HIGH_CURRENT_SENSE_CONFIG                             \
    (PWM_EXTERNAL_CLOCK_DIVIDER_256 | PWM_CLOCK_SOURCE_EXTERNAL | \
     THRESHOLD_ACTIVATION_TIMES_T_OCH1_T_OCM1_L |                 \
     MEDIUM_CURRENT_THRESHOLD_SELECT_IOCM2 |                      \
     LOW_CURRENT_THRESHOLD_SELECT_IOCL3 |                         \
     HIGH_CURRENT_THRESHOLD_SELECT_IOCH2 | CURRENT_SENSE_RATIO_HIGH)

struct EfuseIo
{
    // The current state of the watchdog-in bit (bit 15). If the watchdog is
    // enabled its state must be alternated at least once within the watchdog
    // timeout period.
    bool wdin_bit_to_set;
};

/**
 * Set the current/temperature monitoring option of CSNS pin
 * @param selection The desired monitoring function which can be selected by
 * specifying the following values:
 *                  CSNS_FUNCTION_DISABLED - Current sensing disabled
 *                  AUX1_CURRENT_SENSE_CHANNEL - Current sensing for Aux 1
 *                  AUX2_CURRENT_SENSE_CHANNEL - Current sensing for Aux 2
 *                  CSNS_FUNCTION_TEMPERATURE - Efuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @return EXIT_CODE_OK if the channel configuration was successful
 *         EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
static ExitCode Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(uint8_t selection);

/**
 * Exit fail-safe mode and disable the watchdog timer
 * @return EXIT_CODE_OK if the efuse exited fail-safe mode
 *         EXIT_CODE_TIMEOUT if the one of the SPI writes timed-out
 *         EXIT_CODE_UNIMPLEMENTED if the efuse failed to exit fail-safe mode
 */
static ExitCode Io_Efuse_Aux1Aux2ExitFailSafeMode(void);

/**
 * Write data to a specific Serial Input register on the Aux1/Aux2 Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_Aux1Aux2WriteRegister(
    uint8_t  register_address,
    uint16_t register_value);

/**
 * Read data from a specific Serial Output register on the Aux1/Aux2 Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents of Serial Output register
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_Aux1Aux2ReadRegister(
    uint8_t   register_address,
    uint16_t *register_value);

/**
 * Write data to a specific Serial Input register on the Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_WriteRegister(
    uint8_t         register_address,
    uint16_t        register_value,
    GPIO_TypeDef *  chip_select_port,
    uint16_t        chip_select_pin,
    struct EfuseIo *e_fuse);

/**
 * Read data from a specific Serial Output register on the Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents
 * @param e_fuse Pointer to Efuse structure being read from
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_ReadRegister(
    uint8_t         register_address,
    uint16_t *      register_value,
    GPIO_TypeDef *  chip_select_port,
    uint16_t        chip_select_pin,
    struct EfuseIo *e_fuse);

/**
 * Calculate the parity of the SPI command to be sent to the Efuse and
 * set/clear the parity bit. Using the XOR sum of bits method taken from:
 * https://en.wikipedia.org/wiki/Parity_bit#Parity
 * @param spi_command Original SPI command without the parity bit set/cleared
 */
static void Io_Efuse_CalculateParityBit(uint16_t *spi_command);

/**
 * Write SPI data to Efuse in blocking mode
 * @param tx_data Pointer to data being sent to the Efuse
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_pin Efuse's chip-select GPIO pin
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_WriteToEfuse(
    uint16_t *    tx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin);

/**
 * Read SPI data from Efuse in blocking mode
 * @param tx_data Pointer to data being sent to the Efuse
 * @param rx_data Pointer to data being received from the Efuse
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_pin Efuse's chip-select GPIO pin
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_ReadFromEfuse(
    uint16_t *    tx_data,
    uint16_t *    rx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin);

// The structure for each efuse which contains the watchdog-in bit's current
// value
static struct EfuseIo aux1_aux2_efuse;

// The SPI handle for the SPI device the E-Fuses are connected to
static SPI_HandleTypeDef *efuse_spi_handle;

void Io_Efuse_Init(SPI_HandleTypeDef *const hspi)
{
    assert(hspi != NULL);

    efuse_spi_handle = hspi;

    aux1_aux2_efuse.wdin_bit_to_set = true;
}

void Io_Efuse_Aux1Enable(void)
{
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_SET);
}

void Io_Efuse_Aux1Disable(void)
{
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_RESET);
}

void Io_Efuse_Aux2Enable(void)
{
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_SET);
}

void Io_Efuse_Aux2Disable(void)
{
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_RESET);
}

ExitCode Io_Efuse_GetAux1_Aux2Status(enum Efuse_Status *status)
{
    return Io_Efuse_Aux1Aux2ReadRegister(SO_STATR_ADDR, (uint16_t *)status);
}

ExitCode Io_Efuse_GetAux1Faults(enum Efuse_Fault *fault)
{
    return Io_Efuse_Aux1Aux2ReadRegister(SO_FAULTR_0_ADDR, (uint16_t *)fault);
}

ExitCode Io_Efuse_GetAux2Faults(enum Efuse_Fault *fault)
{
    return Io_Efuse_Aux1Aux2ReadRegister(SO_FAULTR_1_ADDR, (uint16_t *)fault);
}

bool Io_Efuse_IsAux1Aux2InFaultMode(void)
{
    return HAL_GPIO_ReadPin(FSB_AUX1_AUX2_GPIO_Port, FSB_AUX1_AUX2_Pin);
}

bool Io_Efuse_IsAux1Aux2InFailSafeMode(void)
{
    return HAL_GPIO_ReadPin(FSOB_AUX1_AUX2_GPIO_Port, FSOB_AUX1_AUX2_Pin);
}

void Io_Efuse_DelatchAux1Aux2Faults(void)
{
    // Delatch the latchable faults by alternating the inputs high-low-high
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_SET);
}

bool Io_Efuse_GetAux1Current(float *aux1_current)
{
    if (Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
            AUX1_CURRENT_SENSE_CHANNEL) != EXIT_CODE_OK)
    {
        return false;
    }

    if (HAL_GPIO_ReadPin(
            CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        *aux1_current = Io_CurrentSense_GetAux1Current();
        return true;
    }

    return false;
}

bool Io_Efuse_GetAux2Current(float *aux2_current)
{
    if (Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
            AUX2_CURRENT_SENSE_CHANNEL) != EXIT_CODE_OK)
    {
        return false;
    }

    if (HAL_GPIO_ReadPin(
            CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        *aux2_current = Io_CurrentSense_GetAux2Current();
        return true;
    }

    return false;
}

static ExitCode Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(uint8_t selection)
{
    uint16_t reg_val = 0x0000;

    // Read original content of GCR Register
    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2ReadRegister(SI_GCR_ADDR, &reg_val));

    CLEAR_BIT(reg_val, (CSNS1_EN_MASK | CSNS0_EN_MASK));
    SET_BIT(reg_val, (selection & (CSNS1_EN_MASK | CSNS0_EN_MASK)));

    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2WriteRegister(SI_GCR_ADDR, reg_val));

    return EXIT_CODE_OK;
}

ExitCode Io_Efuse_Aux1Aux2ConfigureEfuse(void)
{
    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2ExitFailSafeMode());

    // Global config register
    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2WriteRegister(SI_GCR_ADDR, GCR_CONFIG));

    // Channel 0 config registers
    EXIT_OK_OR_RETURN(
        Io_Efuse_Aux1Aux2WriteRegister(SI_RETRY_0_ADDR, RETRY_CONFIG));
    EXIT_OK_OR_RETURN(
        Io_Efuse_Aux1Aux2WriteRegister(SI_CONFR_0_ADDR, CONFR_CONFIG));
    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2WriteRegister(
        SI_OCR_0_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG));

    // Channel 1 config registers
    EXIT_OK_OR_RETURN(
        Io_Efuse_Aux1Aux2WriteRegister(SI_RETRY_1_ADDR, RETRY_CONFIG));
    EXIT_OK_OR_RETURN(
        Io_Efuse_Aux1Aux2WriteRegister(SI_CONFR_1_ADDR, CONFR_CONFIG));
    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2WriteRegister(
        SI_OCR_1_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG));

    return EXIT_CODE_OK;
}

static ExitCode Io_Efuse_Aux1Aux2ExitFailSafeMode(void)
{
    // Set WDIN bit for next write
    // 1_1_00000_00000_0000
    aux1_aux2_efuse.wdin_bit_to_set = true;

    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2WriteRegister(SI_STATR_0_ADDR, 0x0000));
    // Disable watchdog
    EXIT_OK_OR_RETURN(Io_Efuse_Aux1Aux2WriteRegister(SI_GCR_ADDR, GCR_CONFIG));

    // Check if the the efuse is still in fail-safe mode
    if (HAL_GPIO_ReadPin(FSOB_AUX1_AUX2_GPIO_Port, FSOB_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        return EXIT_CODE_UNIMPLEMENTED;
    }

    return EXIT_CODE_OK;
}

static ExitCode Io_Efuse_Aux1Aux2WriteRegister(
    uint8_t  register_address,
    uint16_t register_value)
{
    return Io_Efuse_WriteRegister(
        register_address, register_value, CSB_AUX1_AUX2_GPIO_Port,
        CSB_AUX1_AUX2_Pin, &aux1_aux2_efuse);
}

static ExitCode Io_Efuse_Aux1Aux2ReadRegister(
    uint8_t   register_address,
    uint16_t *register_value)
{
    return Io_Efuse_ReadRegister(
        register_address, register_value, CSB_AUX1_AUX2_GPIO_Port,
        CSB_AUX1_AUX2_Pin, &aux1_aux2_efuse);
}

static ExitCode Io_Efuse_WriteRegister(
    uint8_t         register_address,
    uint16_t        register_value,
    GPIO_TypeDef *  chip_select_port,
    uint16_t        chip_select_pin,
    struct EfuseIo *e_fuse)
{
    ExitCode exit_code;
    uint16_t command = 0x0000U;

    // Place the register address into bits 10->13
    command = (uint16_t)(
        command | ((register_address & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT));
    // Place register value to be written into bits 0->8
    command = (uint16_t)(command | (register_value & EFUSE_SI_DATA_MASK));
    // Invert watchdog bit state (Note: It is safe to do so even if the watchdog
    // is disabled)
    if (e_fuse->wdin_bit_to_set)
    {
        SET_BIT(command, WATCHDOG_BIT);
    }
    else
    {
        CLEAR_BIT(command, WATCHDOG_BIT);
    }
    // Invert watchdog bit state for next write
    e_fuse->wdin_bit_to_set = !e_fuse->wdin_bit_to_set;

    // Compute and set/clear parity value
    Io_Efuse_CalculateParityBit(&command);

    exit_code =
        Io_Efuse_WriteToEfuse(&command, chip_select_port, chip_select_pin);

    return exit_code;
}

static ExitCode Io_Efuse_ReadRegister(
    uint8_t         register_address,
    uint16_t *      register_value,
    GPIO_TypeDef *  chip_select_port,
    uint16_t        chip_select_pin,
    struct EfuseIo *e_fuse)
{
    ExitCode exit_code;
    uint16_t command = 0x0000U;

    // Place the Status Register address into bits 10->13
    command =
        (uint16_t)((SI_STATR_0_ADDR & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);
    // Shift bit 3 of the address (SOA3: the channel number) to the 13th bit
    command = (uint16_t)(
        command | ((register_address & SOA3_MASK) << EFUSE_ADDR_SHIFT) |
        (register_address & (SOA2_MASK | SOA1_MASK | SOA0_MASK)));
    // Invert watchdog bit state (Note: It is safe to do so even if the watchdog
    // is disabled)
    if (e_fuse->wdin_bit_to_set)
    {
        SET_BIT(command, WATCHDOG_BIT);
    }
    else
    {
        CLEAR_BIT(command, WATCHDOG_BIT);
    }
    // Invert watchdog bit state for next write
    e_fuse->wdin_bit_to_set = !e_fuse->wdin_bit_to_set;

    // Compute and set/clear parity bit
    Io_Efuse_CalculateParityBit(&command);

    exit_code = Io_Efuse_ReadFromEfuse(
        &command, register_value, chip_select_port, chip_select_pin);
    // Only return register contents and clear bits 9->15
    CLEAR_BIT(*register_value, ~EFUSE_SO_DATA_MASK);

    return exit_code;
}

static void Io_Efuse_CalculateParityBit(uint16_t *spi_command)
{
    uint16_t spi_data = *spi_command;
    bool     parity_bit;

    for (parity_bit = 0; spi_data > 0; spi_data >>= 1)
    {
        parity_bit ^= READ_BIT(spi_data, 1U);
    }

    // Set the parity bit if it should be set, else
    // clear the bit
    if (parity_bit)
    {
        SET_BIT(*spi_command, PARITY_BIT);
    }
    else
    {
        CLEAR_BIT(*spi_command, PARITY_BIT);
    }
}

static ExitCode Io_Efuse_WriteToEfuse(
    uint16_t *    tx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin)
{
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)tx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    if (status != HAL_OK)
    {
        return EXIT_CODE_TIMEOUT;
    }

    return EXIT_CODE_OK;
}

static ExitCode Io_Efuse_ReadFromEfuse(
    uint16_t *    tx_data,
    uint16_t *    rx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin)
{
    HAL_StatusTypeDef status;

    // Send command to read from status register
    // Data is returned on the following SPI transfer
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    if (status != HAL_OK)
    {
        return EXIT_CODE_TIMEOUT;
    }

    // Receive data from E-fuse by sending dummy data
    *tx_data = 0xFFFF;

    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    if (status != HAL_OK)
    {
        return EXIT_CODE_TIMEOUT;
    }

    return EXIT_CODE_OK;
}
