/**
 * @file  CurrentSense.h
 * @brief Current Sensing Library for 12V outputs
 */

#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Gpio.h"
#include "stm32f3xx_hal.h"
#include "arm_math.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
/** @brief AUX Current Sense first reading is consistently incorrect (reads > 0
 *         when there is no current), take second reading as workaround */
#define NUM_READINGS_PER_ADC_DMA_TRANSFER 2

/** @defgroup ADC
 *  The constants related to converting ADC readings into useful voltage values
 *  @{
 */
/** @brief Nominal voltage for VDDA, or ADC power supply */
#define VDDA_VOLTAGE (float32_t)(3.3f)

/** @brief The VBAT voltage when the corresponding ADC input saturates at 3.3V.
 *         VBAT refers to the two onboard Li-Ion 18650 batteries in series */
#define VBAT_VOLTAGE (float32_t)(8.4f)

/** @brief The GLV voltage when the corresponding ADC input saturates at 3.3V.
 *         GLV stands for Grounded Low voltage, and is the voltage level used 
 *         to power the vehicle's low voltage systems */
#define GLV_VOLTAGE  (float32_t)(12.0f)

/** Issue (#224): Maybe move this to shared folder? */
/** @brief Bit resolution for a 12 bit ADC */
#define ADC_12_BIT_POINTS (float32_t)(4095.0f)

/** @} ADC */

/** @defgroup PROFET2
 *  The constants related to current sensing on PROFET2 (BTS7008-2EPA)
 *  @{
 */
/** @brief Sense resistance for the IS output of PROFET2 */
#define SENSE_RESISTANCE (float32_t)(330.0f)

/** @brief The typical current scaling factor is 5500, but we also manually
 *         calibrate the value at 5A trip curren for AUX output */
#define CURRENT_SCALING_AUX (float32_t)(6000.0f)

/** @brief The typical current scaling factor is 5500, but we also manually
 *         calibrate the value at 5A trip curren for non-AUX outputs */
#define CURRENT_SCALING (float32_t)(7000.0f)

/** @} PROFET2 */

/** @defgroup 12V_ACC_VOLTAGE_DIVIDER
 *  The constants needed for converting ADC reading of ADC1_IN10 into value for 12V_ACC
 *  @{
 */
/**
 * @note The 12V for low voltage systems is diode-OR'd with 12V_AUX (On-board
 *       Boost Controller) and 12V_ACC (Vicor 12V Output). Loosely speaking,
 *       the logic for the diode-OR controller is as follows:
 *
 *       if (12V_ACC < 10.2V) {
 *           Use 12V_AUX
 *       } else {
 *           Use 12V_ACC
 *       }
 *
 *       If 12V_ACC < 10.2V, the MCU should only enable the CAN_GLV and AIR_SHDN
 *       outputs, and all other outputs should be disabled. This is because
 *       the current rating of 12V_AUX isn't enough to power all the outputs.
 *       In order for the MCU to disable the appropriate outputs when 12V_AUX is being
 *       used, we sample 12V_ACC using a voltage divider whose output is connected
 *       to PA6 (ADC1_IN10). This way, the MCU can now tell if 12V_AUX is being used
 *       or not. The voltage divider is drawn below:
 *
 *       12V_ACC
 *          |
 *         R28
 *          |
 *          |--- ADC1_IN10
 *          |
 *         R29
 *          |
 *         GND
 *
 *       To convert the raw ADC value of ADC1_IN10 to what 12V_ACC really is,
 *       we use the voltage divider formula:
 *
 *       12V_ACC = ADC1_IN10 * ( (R28 + R29) / R29 )
 */
/** @brief One of the two resistors used in voltage divider for 12V_ACC */
#define R28 (float32_t)(160000.0f)

/** @brief One of the two resistors used in voltage divider for 12V_ACC */
#define R29 (float32_t)(10000.0f)

/** @brief Multiply the raw ADC value of ADC1_IN10 to convert it to 12V_ACC value */
#define ADC1_IN10_TO_12V_ACC_RATIO (float32_t)((R28 + R29) / R29)

/** @} 12V_ACC_VOLTAGE_DIVIDER */

/** @defgroup VOLTAGE_CURRENT_LIMITS
 *  Various voltage and current threshold for fault conditions
 *  @{
 */

/** @brief Software current limit for efuse outputs in Amperes (A) */
#define EFUSE_CURRENT_LIMIT (float32_t)(1.0f)

/** @brief Cell balance IC (BQ29209) defines overvoltage as 4.3V * 2 in series */
#define VBAT_OVERVOLTAGE       (float32_t)(8.6f)

/** @brief Nominal GLV voltage is 12V, and we choose overvoltage threshold to be 1V above */
#define OVERVOLTAGE_GLV_THRES  (float32_t)(13.0f)

/** @brief Nominal GLV voltage is 12V, and we choose undervoltage threshold to be 1V below */
#define UNDERVOLTAGE_GLV_THRES (float32_t)(11.0f)

/** @brief Diode-OR controller switches back to 12V_AUX when 12V_ACC < 10.2V,
 *         but we set undervoltage limit to 10.5V for a safety margin of 0.3V*/
#define UNDERVOLTAGE_VICOR_THRES (float32_t)(10.5f)

/** @} VOLTAGE_CURRENT_LIMITS */

/** @defgroup LPF
 *  The constants needed for apply a low pass filter, taken from:
 *  https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
 *  @{
 */

/** TODO (Issue #223): Update this to use Cube USER Constants */
/** @brief ADC sampling frequency - 72MHz/14400 (TIM2 prescaler value) = 5000*/
#define ADC_TRIGGER_FREQUENCY  (float32_t)(72000000.0f / 14400.0f)

/** @brief Sampling time interval */
#define DELTA              (float32_t)(1.0f / ADC_TRIGGER_FREQUENCY)

/** @brief 10Hz cutoff to account for false tripping from inrush (See
 *         SoftwareTools for data) */
#define CUTOFF_FREQUENCY   (float32_t)(10.0f)

/** @brief RC time constant */
#define RC                 (float32_t)(1.0f / (2.0f * PI * CUTOFF_FREQUENCY))

/** @brief ALPHA constant */
#define ALPHA              (float32_t)(DELTA / (RC + DELTA))

/** @} LPF */

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/
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
    0
};
// Index-based conversion for each e-fuse

// TODO (Issue #191): Can this not be a static const? Or can it be in .c file
// instead at least 3 retries for all outputs except FANS/COOLING which have 10
// retries to account for inrush, and 1 retry for VICOR poweroff
static const uint8_t MAX_FAULTS[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = {
    3, 10, 3, 10, 3, 3, 3, 1, 3, 3, 3, 10, 3, 3, 3, 1
};

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
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
