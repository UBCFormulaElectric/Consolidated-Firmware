/**
 * @file  VoltageSense.h
 * @brief Library for reading the ADC inputs related to voltage sensing
 */
#ifndef VOLTAGE_SENSE_H
#define VOLTAGE_SENSE_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
/** @defgroup VOLTAGE_ADC
 *  The constants related to converting ADC readings into useful voltage values
 *  @{
 */

/** @brief The VBAT voltage when the corresponding ADC input saturates at 3.3V.
 *         VBAT refers to the two onboard Li-Ion 18650 batteries in series */
#define VBAT_VOLTAGE (float32_t)(8.4f)

/** @brief The GLV voltage when the corresponding ADC input saturates at 3.3V.
 *         GLV stands for Grounded Low voltage, and is the voltage level used
 *         to power the vehicle's low voltage systems */
#define GLV_VOLTAGE  (float32_t)(12.0f)

/** @} VOLTAGE_ADC */

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
#define ADC1_IN10_TO_12V_ACC_RATIO (float32_t)((R28 + R29) / R29) * VDDA_VOLTAGE

/** @} 12V_ACC_VOLTAGE_DIVIDER */

/** @defgroup IIR_LPF
 *  The constants needed for a infinite impulse response (IIR) low-pass filter,
 *  taken from:
 *  https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
 *  @{
 */
/** @brief Sampling time interval */
#define VOLTAGE_IIR_LPF_SAMPLING_PERIOD    (float32_t)(1.0f / ADC_TRIGGER_FREQUENCY)

/** @brief 1000Hz cutoff frequency for general low-pass filtering */
#define VOLTAGE_IIR_LPF_CUTOFF_FREQUENCY   (float32_t)(10.0f)

/** @brief RC time constant */
#define VOLTAGE_IIR_LPF_RC \
        (float32_t)(1.0f / \
                   (2.0f * PI * CURRENT_IIR_LPF_CUTOFF_FREQUENCY))
/** @} IIR_LPF */

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
// TODO: Replace NUM_VOLTAGE_SENSE_PINS with an enum member
typedef enum
{
    _12V_SUPPLY,
    VBAT_SUPPLY,
    FLYWIRE,
} VoltageSenseIndex_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void VoltageSense_ConvertVoltageAdcReadings(void);

#endif /* VOLTAGE_SENSE_H */
