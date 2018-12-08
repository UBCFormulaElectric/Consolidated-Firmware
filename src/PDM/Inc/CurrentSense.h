/**
 * @file  CurrentSense.h
 * @brief Current Sensing Library for 12V outputs
 */

#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "Gpio.h"
#include "stm32f3xx_hal.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off
#define NUM_READINGS 2 // AUX Current Sense first reading is consistently incorrect (reads > 0 when there is no current), take second reading as workaround

// ADC value to voltage
#define VDDA_VOLTAGE  (float)3.3f
#define VBAT_VOLTAGE (float)8.4f // 4.2V peak * 2 in series
#define GLV_VOLTAGE  (float) 12.0f
#define ADC_12_BIT_POINTS (float) 4095.0f // 12-bit ADC

// Voltage to current conversion constants
#define SENSE_RESISTANCE (float) 330.0f
// Note: Current scaling typ. value is 5500 (see BTS-7008 datasheet). The below values were determined (experimentally) to be accurate at the 5A trip current.
#define CURRENT_SCALING_AUX (float) 6000.0f // Current scaling ratio for AUX output efuses
#define CURRENT_SCALING (float) 7000.0f // Current scaling ratio for other efuses

// We only want to power CAN and AIR SHDN outputs with the on-board 18650s.
// To notify the MCU to disable all other outputs when 12V_ACC is off, we sample the EN2 net (the output of a voltage divider for 12V_ACC).
// This is nomially 0.7V when 12V_ACC is 12V. PDM 2018 was reworked to connect EN2 to PA6 (ADC in) to detect when 12V_ACC is below 10.5V (threshold for switching back to
// 12V_AUX is 10.2V, the 0.3V provide a safety margin). At this point all other outputs should be shut off for safety.
// Conversion currents for 12V_ACC: 12V --> R1 -(EN2)-> R2 --> GND
#define R1 (float) 160000
#define R2 (float) 10000
#define EN2_TO_12VACC (float) (R1 + R2)/R2 // convert EN2 net voltage to 12V_ACC value

// Voltage and current limits
#define CURRENT_LIMIT (float) 1.0f
#define VBAT_OVERVOLTAGE (float) 8.6f // BQ29209 Overvoltage is defined as 4.3V * 2 in series
#define OVERVOLTAGE_GLV_THRES (float) 13.0f
#define UNDERVOLTAGE_GLV_THRES (float) 11.0f
#define UNDERVOLTAGE_VICOR_THRES (float) 10.5f // Diode-OR controller switches back to 12V_AUX when 12V_ACC < 10.2V, set undervoltage limit to 10.5V for a safety margin

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
* Global Variables
*******************************************************************************/
// clang-format on
// TODO (Issue #191): Perhaps VOLTAGE_TO_CURRENT and MAX_FAULTS can be combined into a struct?
// TODO (Issue #191): Can this not be a static const? Or can it be in .c file instead at least	
static const float VOLTAGE_TO_CURRENT[ADC_CHANNEL_COUNT * NUM_CHANNELS] =
{
    CURRENT_SCALING_AUX/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    0,
    0,
    0,
    CURRENT_SCALING_AUX/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    CURRENT_SCALING/SENSE_RESISTANCE,
    0,
    0,
    0
};
// Index-based conversion for each e-fuse

// TODO (Issue #191): Can this not be a static const? Or can it be in .c file instead at least
// 3 retries for all outputs except FANS/COOLING which have 10 retries to account for inrush, and 1 retry for VICOR poweroff
static const uint8_t MAX_FAULTS[ADC_CHANNEL_COUNT * NUM_CHANNELS] =
{
    3,
    10,
    3,
    10,
    3,
    3,
    3,
    1,
    3,
    3,
    3,
    10,
    3,
    3,
    3,
    1
};

/******************************************************************************
* Function Prototypes
*******************************************************************************/
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
void CurrentSense_ConvertFilteredADCToCurrentValues(volatile float *converted_readings);

#endif /* CURRENT_SENSE_H */
