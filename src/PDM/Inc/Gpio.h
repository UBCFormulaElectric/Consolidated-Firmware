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
#include "arm_math.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
/** @brief 12V Sense, VBAT Sense, and Flywire Sense */
#define NUM_VOLTAGE_SENSE_PINS (uint32_t)(3)

/** @brief Number of PROFET 2's */
#define NUM_PROFET2S (uint32_t)(NUM_ADC_CHANNELS - NUM_VOLTAGE_SENSE_PINS)

/** @brief PROFET 2 is a dual-channel IC */
#define NUM_CHANNELS_PER_PROFET2 (uint32_t)(2)

/** @brief Number of e-fuses */
#define NUM_EFUSES (uint32_t)(NUM_PROFET2S * NUM_CHANNELS_PER_PROFET2)

/**
 * @brief We have 8 ADC channels enabled, but 5 of those are connected to
 *        PROFET 2's. Each PROFET 2 has two e-fuse channels, which means we
 *        are really getting two unique ADC readings per PROFET.
 */
#define NUM_UNIQUE_ADC_READINGS (uint32_t)(NUM_EFUSES + NUM_VOLTAGE_SENSE_PINS)

// Pin definitions
#define CHARGER_FAULT_STATE 									GPIO_PIN_RESET // Low = fault, high = no fault
#define CHARGER_CHARGING_STATE									GPIO_PIN_RESET // Low = charging, high = not charging

// Battery Cell Balancing
#define CELL_BALANCE_OVERVOLTAGE_FAULT_STATE 					GPIO_PIN_SET // High = fault, low = no fault

// Boost Converter
#define BOOST_PGOOD_FAULT_STATE 								GPIO_PIN_RESET // Low = fault, high = no fault

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on
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

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Initialize GPIO
 */
void GPIO_Init(void);

/**
 * @brief  Enable all e-fuses (that have not faulted) and their corresponding
 *         current sense diagnostics. Only to be called after 12V_ACC is
 * available
 */
void GPIO_ConfigureFor12VAcc(void);

/**
 * @brief  Enable CAN_GLV/AIR SHDN (if they are not faulted) and their
 *         corresponding current sense diagnostics. Disable all other outputs.
 */
void GPIO_ConfigureFor12VAux(void);

#endif /* GPIO_H */
