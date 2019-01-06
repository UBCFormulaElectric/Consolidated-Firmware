/**
 * @file  CurrentSense.h
 * @brief Current Sensing Library for 12V outputs
 */

#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal.h"
#include "arm_math.h"
#include "Gpio.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
/** @brief AUX Current Sense first reading is consistently incorrect (reads > 0
 *         when there is no current), take second reading as workaround */
#define NUM_READINGS_PER_ADC_DMA_TRANSFER 2


/** @defgroup PROFET2
 *  The constants related to current sensing on PROFET2 (BTS7008-2EPA)
 *  @{
 */
/** @brief Sense resistance for the IS output of PROFET2 */
#define SENSE_RESISTANCE (float32_t)(330.0f)

/** @brief The typical current scaling factor is 5500, but we also manually
 *         calibrate the value at 5A trip current for AUX outputs */
#define CURRENT_SCALING_AUX (float32_t)(6000.0f)

/** @brief The typical current scaling factor is 5500, but we also manually
 *         calibrate the value at 5A trip current for non-AUX outputs */
#define CURRENT_SCALING (float32_t)(7000.0f)

/** @brief Multiple the ADC voltage reading by this (in 3.3V scale) to get
 *         e-fuse current for AUX outputs */
#define AMP_PER_VOLT_AUX (float32_t)(CURRENT_SCALING_AUX / SENSE_RESISTANCE)

/** @brief Multiple the ADC voltage reading by this (in 3.3V scale) to get
 *         e-fuse current for non-AUX outputs */
#define AMP_PER_VOLT (float32_t)(CURRENT_SCALING / SENSE_RESISTANCE)

/** @} PROFET2 */


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

/** @defgroup IIR_LPF
 *  The constants needed for a infinite impulse response (IIR) low-pass filter,
 *  taken from:
 *  https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
 *  @{
 */
/** @brief Sampling time interval */
#define CURRENT_IIR_LPF_SAMPLING_PERIOD    (float32_t)(1.0f / ADC_TRIGGER_FREQUENCY)

/** @brief 10Hz cutoff to account for false tripping from inrush (See
 *         SoftwareTools for data) */
#define IIR_LPF_CUTOFF_FREQUENCY   (float32_t)(10.0f)

/** @brief RC time constant */
#define CURRENT_IIR_LPF_RC \
        (float32_t)(1.0f / \
                   (2.0f * PI * IIR_LPF_CUTOFF_FREQUENCY))
/** @} IIR_LPF */

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on
typedef enum
{
    SENSE_0,
    SENSE_1
} SenseChannel_Enum;

typedef struct
{
    GPIO_PinPort_Struct pin_mapping;
    float32_t current;
    float32_t ampere_per_volt;
} efuse_struct;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
// clang-format on
// Index-based conversion for each e-fuse

// TODO (Issue #191): Can this not be a static const? Or can it be in .c file
// instead at least 3 retries for all outputs except FANS/COOLING which have 10
// retries to account for inrush, and 1 retry for VICOR poweroff
static const uint8_t MAX_FAULTS[NUM_ADC_CHANNELS * NUM_CHANNELS_PER_PROFET2] = {
    3, 10, 3, 10, 3, 3, 3, 1, 3, 3, 3, 10, 3, 3, 3, 1
};

extern const efuse_struct efuse_lut[NUM_EFUSES];

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Convert ADC readings for current sense from PROFET2
 * @Note  If sense_channel = SENSE_0, we read current for AUXILIARY_1, COOLING,
 *        AIR_SHDN, ACC_SEGMENT_FAN, and LEFT_INVERTER.
 *        If sense_channel = SENSE_1, we read current for AUXILIARY_2, PDM_FAN,
 *        CAN_GLV, ACC_ENCLOSURE_FAN, RIGHT_INVERTER
 */
void CurrentSense_ConvertCurrentAdcReadings(void);

/**
 * @brief Toggle the SENSE output between 0 and 1 for all PROFET2s
 */
void CurrentSense_ToggleCurrentSenseChannel(void);

/**
 * @return Return the currently selected SENSE channel
 */
SenseChannel_Enum CurrentSense_GetCurrentSenseChannel(void);

/**
 * @brief Select the SENSE output 0/1 for all PROFET2s
 * @param channel The SENSE output channel to select
 */
void CurrentSense_SelectCurrentSenseChannel(SenseChannel_Enum channel);

#endif /* CURRENT_SENSE_H */
