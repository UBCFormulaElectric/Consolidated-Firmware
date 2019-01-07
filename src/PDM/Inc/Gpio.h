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

typedef enum
{
    DSEL_LOW  = GPIO_PIN_RESET,
    DSEL_HIGH = !DSEL_LOW
} DselOnOff_GPIO_PinState;

typedef enum
{
    DEN_OFF = GPIO_PIN_RESET,
    DEN_ON  = !DEN_OFF
} DenOnOff_GPIO_PinState;

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
    SENSE_0,
    SENSE_1
} SenseChannel_Enum;

typedef enum
{
    EFUSE_ON  = GPIO_PIN_SET,
    EFUSE_OFF = !EFUSE_ON
} EfuseOnOff_GPIO_PinState;

typedef enum
{
    AUXILIARY_1 = SENSE_0,
    AUXILIARY_2 = SENSE_1,
} Aux1Aux2Index_Enum;
typedef enum
{
    COOLING = SENSE_0,
    PDM_FAN = SENSE_1,
} CoolingPdmFanIndex_Enum;

typedef enum
{
    AIR_SHDN = SENSE_0,
    CAN_GLV  = SENSE_1,
} AirShdnCanGlvIndex_Enum;

typedef enum
{
    ACC_SEGMENT_FAN   = SENSE_0,
    ACC_ENCLOSURE_FAN = SENSE_1,
} AccSegmentFanAccEnclosureFanIndex_Enum;

typedef enum
{
    LEFT_INVERTER  = SENSE_0,
    RIGHT_INVERTER = SENSE_1,
} LeftInverterRightInverterIndex_Enum;

typedef enum
{
    AUX1_AUX2,
    COOLING_PDMFAN,
    AIRSHDN_CANGLV,
    ACCSEGMENTFAN_ACCENCLOSUREFAN,
    LEFTINVERTER_RIGHTINVERTER,
} Profet2Index_Enum;

typedef struct
{
    GPIO_PinPort_Struct input_channel;
    volatile float32_t  current;
    float32_t           ampere_per_volt;
} efuse_struct;

typedef struct
{
    efuse_struct        efuse[NUM_CHANNELS_PER_PROFET2];
    GPIO_PinPort_Struct dsel_pin_mapping;
    GPIO_PinPort_Struct den_pin_mapping;
} Profet2_Struct;

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
 * @param  fault_states Array with (NumReadings x ChannelCount) elements
 *         which tracks outputs that need to be renabled or are permanently
 *         faulted
 */
void GPIO_ConfigureFor12VAcc(volatile uint8_t *fault_states);

/**
 * @brief  Enable CAN_GLV/AIR SHDN (if they are not faulted) and their
 *         corresponding current sense diagnostics. Disable all other outputs.
 * @param  fault_states Array with (NumReadings x ChannelCount) elements which
 *         tracks outputs that need to be renabled or are permanently faulted
 */
void GPIO_ConfigureFor12VAux(volatile uint8_t *fault_states);

/**
 * @brief  Helper function to turn e-fuse on or off
 * @param  efuse Efuse to configure
 * @param  state Turn e-fuse on or off
 */
void Gpio_ConfigureSingleEfuse(
    efuse_struct *           efuse,
    EfuseOnOff_GPIO_PinState state);

/**
 * @brief  Helper function to turn the DEN pin of PROFET2 on or off
 * @param  profet2 PROFET2 to configure
 * @param  state Turn DEN pin on or off
 */
void Gpio_ConfigureSingleDen(
    Profet2_Struct *       profet2,
    DenOnOff_GPIO_PinState state);

/**
 * @brief Helper function to turn all DSEL pins on or off
 * @param state Turn DSEL pins on or off
 */
void Gpio_ConfigureAllDsels(DselOnOff_GPIO_PinState state);

/**
 * @brief Return array of structs containing information for each PROFET
 */
Profet2_Struct *const Gpio_GetProfet2s(void);

#endif /* GPIO_H */
