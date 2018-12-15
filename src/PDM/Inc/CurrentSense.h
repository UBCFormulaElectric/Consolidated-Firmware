/**
 * @file  CurrentSense.h
 * @brief Current Sensing Library for 12V outputs
 */

#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Gpio.h"
#include "stm32f3xx_hal.h"
#include "arm_math.h"

/******************************************************************************
 * Preprocessor Constants
 *****************************************************************************/
// clang-format off
/** @brief AUX Current Sense first reading is consistently incorrect (reads > 0 when
           there is no current), take second reading as workaround */
#define NUM_READINGS 2

#define VDDA_VOLTAGE  (float32_t)(3.3f)

/**  @brief 2 Li-Ion cells in series, each with 4.2 peak voltage */
#define VBAT_VOLTAGE (float32_t)(8.4f)

#define GLV_VOLTAGE  (float32_t)(12.0f)

/** Issue (#224): Maybe move this to shared folder? */
/** @brief Bit resolution for a 12 bit ADC */
#define ADC_12_BIT_POINTS (float32_t)(4095.0f)

// Voltage to current conversion constants
#define SENSE_RESISTANCE (float32_t)(330.0f)

/** @brief Current scaling typical value is 5500 (see BTS-7008 datasheet).
 *         The below values were calibrated manually to be accurate at
 *         the 5A trip current. */
/** @brief Current scaling ratio for AUX output efuses */
#define CURRENT_SCALING_AUX (float32_t)(6000.0f)

/** @brief Current scaling ratio for other efuses */
#define CURRENT_SCALING (float32_t)(7000.0f)

/**
 * @brief We only want to power CAN and AIR SHDN outputs with the on-board
 *        18650s. To notify the MCU to disable all other outputs when 12V_ACC
 *        is off, we sample the EN2 net (the output of a voltage divider for
 *        12V_ACC). This is nomially 0.7V when 12V_ACC is 12V. PDM 2018 was
 *        reworked to connect EN2 to PA6 (ADC in) to detect when 12V_ACC is
 *        below 10.5V (threshold for switching back to 12V_AUX is 10.2V, the
 *        0.3V provide a safety margin). At this point all other outputs
 *        should be shut off for safety. Conversion currents for 12V_ACC:
 *        12V --> R1 -(EN2)-> R2 --> GND
 */
#define R1 (float32_t)(160000.0f)
#define R2 (float32_t)(10000.0f)

/** @brief Convert EN2 net voltage to 12V_ACC value */
#define EN2_TO_12VACC_RATIO (float32_t)((R1 + R2) / R2)

/** @brief Software current limit for efuse outputs */
#define EFUSE_CURRENT_LIMIT (float32_t)(1.0f)

/** @brief Cell balance IC (BQ29209) defines overvoltage as 4.3V * 2 in series */
#define VBAT_OVERVOLTAGE       (float32_t)(8.6f)
#define OVERVOLTAGE_GLV_THRES  (float32_t)(13.0f)
#define UNDERVOLTAGE_GLV_THRES (float32_t)(11.0f)

/** @brief  Diode-OR controller switches back to 12V_AUX when 12V_ACC < 10.2V,
 *          so we set undervoltage limit to 10.5V for a safety margin of 0.3V*/
#define UNDERVOLTAGE_VICOR_THRES (float32_t)(10.5f)

/** @brief LPF constants (calculated using this article:
// https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization) */
/** TODO (Issue #223): Update this to use Cube USER Constants */
/** @brief ADC sampling frequency - 72MHz/14400 (TIM2 prescaler value) = 5000*/
#define ADC_TRIGGER_FREQUENCY (float32_t)(72000000.0f / 14400.0f)
#define DELTA                 (float32_t)(1.0f / ADC_TRIGGER_FREQUENCY)

/** @brief 10Hz cutoff to account for false tripping from inrush (See
 *         SoftwareTools for data) */
#define CUTOFF_FREQUENCY      (float32_t)(10.0f)

#define RC                    (float32_t)(1.0f / (2.0f * 3.14159265f * CUTOFF_FREQUENCY))
#define LPF_ALPHA             (float32_t)(DELTA / (RC + DELTA))

/******************************************************************************
* Preprocessor Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 *****************************************************************************/
// clang-format on
// TODO (Issue #191): Perhaps VOLTAGE_TO_CURRENT and MAX_FAULTS can be combined
// into a struct?
// TODO (Issue #191): Can this not be a static const? Or can it be in .c file
// instead at least
static const float32_t
    VOLTAGE_TO_CURRENT[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = {
        CURRENT_SCALING_AUX / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        0,
        0,
        0,
        CURRENT_SCALING_AUX / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        CURRENT_SCALING / SENSE_RESISTANCE,
        0,
        0,
        0};
// Index-based conversion for each e-fuse

// TODO (Issue #191): Can this not be a static const? Or can it be in .c file
// instead at least 3 retries for all outputs except FANS/COOLING which have 10
// retries to account for inrush, and 1 retry for VICOR poweroff
static const uint8_t MAX_FAULTS[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = {
    3, 10, 3, 10, 3, 3, 3, 1, 3, 3, 3, 10, 3, 3, 3, 1};

/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
/**
 * @brief Low pass filters ADC readings with a cutoff frequency of
 *        CUTOFF_FREQUENCY
 * @param adc_readings Pointer to array containing the unfiltered ADC readings
 */
void CurrentSense_LowPassFilterADCReadings(volatile uint32_t *adc_readings);

/**
 * @brief Converts filtered ADC readings to currents or voltages
 * @param converted_readings Pointer to array containing converted ADC readings
 */
void CurrentSense_ConvertFilteredADCToCurrentValues(
    volatile float32_t *converted_readings);

#endif /* CURRENT_SENSE_H */
