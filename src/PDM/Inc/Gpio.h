/**
 * @file  Gpio.h
 * @brief GPIO Library
 */

#ifndef GPIO_H
#define GPIO_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal.h"
#include "Can.h"
#include "main.h"
#include "SharedAdc.h"
#include "SharedGpio.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
/** @brief 12V Sense, VBAT Sense, and Flywire Sense */
#define NUM_VOLTAGE_SENSE_PINS 3

/** @brief Number of PROFET 2's */
#define NUM_PROFET2S NUM_ADC_CHANNELS - NUM_VOLTAGE_SENSE_PINS

/** @brief PROFET 2 is a dual-channel IC */
#define NUM_CHANNELS_PER_PROFET2 2

/** @brief Number of e-fuses */
#define NUM_EFUSES NUM_PROFET2S * NUM_CHANNELS_PER_PROFET2

/**
 * @brief We have 8 ADC channels enabled, but 5 of those are connected to
 *        PROFET 2's. Each PROFET 2 has two e-fuse channels, which means we
 *        are really getting two unique ADC readings per PROFET.
 */
#define NUM_UNIQUE_ADC_READINGS NUM_EFUSES + NUM_VOLTAGE_SENSE_PINS

// Pin definitions
#define CHARGER_FAULT_STATE 									GPIO_PIN_RESET // Low = fault, high = no fault
#define CHARGER_CHARGING_STATE									GPIO_PIN_RESET // Low = charging, high = not charging

// Battery Cell Balancing
#define CELL_BALANCE_OVERVOLTAGE_FAULT_STATE 					GPIO_PIN_SET // High = fault, low = no fault

// Boost Converter
#define BOOST_PGOOD_FAULT_STATE 								GPIO_PIN_RESET // Low = fault, high = no fault

#define INIT_EFUSE(index, pin, port) = { [index].pin = pin, [index].port = port},

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

typedef enum
{
    DSEL_LOW = GPIO_PIN_RESET,
    DSEL_HIGH = GPIO_PIN_SET
} DselState_Enum;

typedef enum
{
    SENSE_0,
    SENSE_1
} SenseChannel_Enum;

/** Efuse State */
typedef enum
{
    /** @brief Operating as expected */
    NORMAL_STATE,
    /** @brief Exceeded current limit but not maximum number of retries */
    RETRY_STATE,
    /** @brief Exceed maximum number of retries and is permanently stuck in
               error state */
    ERROR_STATE
} Efuse_State_Enum;

typedef enum
{
    EFUSE_ON  = GPIO_PIN_SET,
    EFUSE_OFF = !EFUSE_ON
} EfuseOnOff_GPIO_PinState;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern const GPIO_PinPort_Struct efuse_pin_mapping[NUM_EFUSES];

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Initialize GPIO
 */
void GPIO_Init(void);

/**
 * @brief  Enable all e-fuses (that have not faulted) and their
 *         corresponding current sense diagnostics. Only to be called after
 *         PreCharge is completed.
 * @param  fault_states Array with (NumReadings x ChannelCount) elements
 *         which tracks outputs that need to be renabled or are permanently
 *         faulted
 */
void GPIO_ConfigurePreChargeComplete(volatile uint8_t *fault_states);

/**
 * @brief  Enable CAN_GLV/AIR SHDN (if they are not faulted) and their
 *         corresponding current sense diagnostics. Disable all other outputs.
 * @param  fault_states Array with (NumReadings x ChannelCount) elements which
 *         tracks outputs that need to be renabled or are permanently faulted
 */
void GPIO_ConfigurePowerUp(volatile uint8_t *fault_states);

/**
 * @brief  Helper function to turn e-fuse on or off
 * @param  index Index of e-fuse
 * @param  state Turn e-fuse on or off
 */
void Gpio_ConfigureEfuseOnOff(
    EfuseCurrentIndex_Enum   index,
    EfuseOnOff_GPIO_PinState state);

#endif /* GPIO_H */
