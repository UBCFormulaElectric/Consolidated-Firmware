/**
 * @file  Profet2.h
 * @brief Pseuo-struct 
 */
//TODO: Update header guard below to an unique identifier in this project
#ifndef PROFET2_H
#define PROFET2_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Gpio.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

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

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
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
 * @brief  Helper function to turn e-fuse on or off
 * @param  efuse Efuse to configure
 * @param  state Turn e-fuse on or off
 */
void Profet2_ConfigureSingleEfuse(
    efuse_struct *           efuse,
    EfuseOnOff_GPIO_PinState state);

/**
 * @brief  Helper function to turn the DEN pin of PROFET2 on or off
 * @param  profet2 PROFET2 to configure
 * @param  state Turn DEN pin on or off
 */
void Profet2_ConfigureSingleDen(
    Profet2_Struct *       profet2,
    DenOnOff_GPIO_PinState state);

/**
 * @brief Helper function to turn all DSEL pins on or off
 * @param state Turn DSEL pins on or off
 */
void Profet2_ConfigureAllDsels(DselOnOff_GPIO_PinState state);

/**
 * @brief Return array of structs containing information for each PROFET
 */
Profet2_Struct *const Profet2_GetProfet2s(void);

#endif /* PROFET2_H */
