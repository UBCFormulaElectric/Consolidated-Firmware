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

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/** @brief Number of microcontroller pins that are configured to be ADC inputs */
#define NUM_ADC_CHANNELS 8

/** @brief 12V Sense, VBAT Sense, and Flywire Sense */
#define NUM_VOLTAGE_SENSE_PINS 3

/** @brief Number of PROFET 2's */
#define NUM_PROFET2S NUM_ADC_CHANNELS - NUM_VOLTAGE_SENSE_PINS

/** @brief PROFET 2 is a dual-channel IC containing two separate e-fuses */
#define NUM_EFUSES_PER_PROFET2 2

/** @brief Number of e-fuses */
#define NUM_EFUSES NUM_PROFET2S * NUM_EFUSES_PER_PROFET2

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

// DSEL State
#define DSEL_LOW GPIO_PIN_RESET
#define DSEL_HIGH GPIO_PIN_SET

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

/**
 * @brief ADC Readings Indexing, corresponding to: 0 to (NUM_UNIQUE_ADC_READINGS - 1)
 *         Index 0 - 4:  E-fuses selected when DSEL = DSEL_LOW
 *         Index 5 - 7:  Voltage sense reading
 *         Index 8 - 12: E-fuses selected when DSEL = DSEL_HIGH
 *         Note: Indices 13 - 15 would have also represented voltage sense
 *         readings, which would be redundant so they are ommited.
 */
typedef enum
{
    AUXILIARY_1,
    COOLING,
    AIR_SHDN,
    ACC_SEGMENT_FAN,
    LEFT_INVERTER,
    _12V_SUPPLY,
    VBAT_SUPPLY,
    FLYWIRE,
    AUXILIARY_2,
    PDM_FAN,
    CAN_GLV,
    ACC_ENCLOSURE_FAN,
    RIGHT_INVERTER
} ADC_Index_Enum;

/** TODO (Issue #191): What is this struct for */
typedef struct
{
    uint16_t      pin[NUM_PROFET2S];
    GPIO_TypeDef *port[NUM_PROFET2S];
} GPIO_PinPort_Struct;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern volatile GPIO_PinState dsel_state;

// E-fuse output pin mapping
// TODO (Issue #191): The index can be a value of @ ...
static const GPIO_PinPort_Struct PROFET2_IN0 = {
    {EFUSE_AUX_1_IN_Pin, EFUSE_COOLING_IN_Pin, EFUSE_AIR_SHDN_IN_Pin,
     EFUSE_ACC_SEG_FAN_IN_Pin, EFUSE_LEFT_INVERTER_IN_Pin},
    {EFUSE_AUX_1_IN_GPIO_Port, EFUSE_COOLING_IN_GPIO_Port,
     EFUSE_AIR_SHDN_IN_GPIO_Port, EFUSE_ACC_SEG_FAN_IN_GPIO_Port,
     EFUSE_LEFT_INVERTER_IN_GPIO_Port}};

// TODO (Issue #191): The index can be a value of @ ...
static const GPIO_PinPort_Struct PROFET2_IN1 = {
    {EFUSE_AUX_2_IN_Pin, EFUSE_PDM_FAN_IN_Pin, EFUSE_CAN_IN_Pin,
     EFUSE_ACC_ENC_FAN_IN_Pin, EFUSE_RIGHT_INVERTER_IN_Pin},
    {EFUSE_AUX_2_IN_GPIO_Port, EFUSE_PDM_FAN_IN_GPIO_Port,
     EFUSE_CAN_IN_GPIO_Port, EFUSE_ACC_ENC_FAN_IN_GPIO_Port,
     EFUSE_RIGHT_INVERTER_IN_GPIO_Port}};

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
 *  @brief  Select E-Fuse output for current sense (DSEL toggle)
 *  @param  dsel_value value to set DSEL pin to (DSEL_HIGH or DSEL_LOW)
 */
void GPIO_EFuseSelectDSEL(GPIO_PinState dsel_value);

#endif /* GPIO_H */
