/**
 * @file  Gpio.h
 * @brief GPIO Library
 */


#ifndef GPIO_H
#define GPIO_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f3xx_hal.h"
#include "Can.h"
#include "main.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off

// ADC Variables
#define ADC_CHANNEL_COUNT 8
#define NUM_CHANNELS 2
#define VOLTAGE_SENSE_PINS 3
#define ADC_EFUSE_READINGS ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS
// There are (ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS)* NUM_CHANNELS Efuse 
// readings as well as VOLTAGE_SENSE_PINS voltage readings
#define ADC_TOTAL_READINGS_SIZE (NUM_CHANNELS * ADC_EFUSE_READINGS) \
                                + VOLTAGE_SENSE_PINS

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

// DSEL State
#define DSEL_LOW GPIO_PIN_RESET
#define DSEL_HIGH GPIO_PIN_SET

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

// Battery Charger
typedef enum {
  // Low = active fault, high = no fault
  CHARGER_NO_FAULT = GPIO_PIN_SET,
  CHARGER_ACTIVE_FAULT = !CHARGER_NO_FAULT
} ChargerFault_GPIO_PinState;

typedef enum {
  // Low = charging, high = not charging
  NOT_CHARGING = GPIO_PIN_SET,
  CHARGING = !NOT_CHARGING
} ChargingState_GPIO_PinState;

// Battery Cell Balancing
typedef enum {
  // High = active fault, low = no fault
  CELL_BALANCE_OV_ACTIVE_FAULT = GPIO_PIN_SET,
  CELL_BALANCE_OV_NO_FAULT = !CELL_BALANCE_OV_ACTIVE_FAULT
} CellBalanceOvervoltageFault_GPIO_PinState;

// Boost Converter
typedef enum {
  // Low = active fault, high = no fault
  BOOST_PGOOD_NO_FAULT = GPIO_PIN_SET,
  BOOST_PGOOD_ACTIVE_FAULT = !BOOST_PGOOD_NO_FAULT
} BoostPGoodFault_GPIO_PinState;

/** Efuse State */
typedef enum {
    // Operating as expected
    STATIC_EFUSE = 0, 
    // Exceeded current limit but not max number of retries, in retry mode
    RENABLE_EFUSE = 1, 
    // Exceeded max number of retries, permanently in error state
    ERROR_EFUSE = 2 
} Efuse_State_Enum;

// Efuse Indexing, corresponding to: 0 to (ADC_TOTAL_READINGS_SIZE - 1)
// Indices 0-4 correspond to DSEL_LOW Efuses and 5-7 are voltage readings
// Indices 8-12 correspond to DSEL_HIGH Efuses
// Note: Indices 13-15 are omitted; they are redundant with indices 5-7
typedef enum {
    AUX_1_INDEX = 0,
    COOLING_INDEX,
    AIR_SHDN_INDEX,
    ACC_SEG_FAN_INDEX,
    L_INV_INDEX,
    _12V_SUPPLY_INDEX,
    VBAT_SUPPLY_INDEX,
    VICOR_SUPPLY_INDEX,
    AUX_2_INDEX = 8,
    PDM_FAN_INDEX,
    CAN_INDEX,
    ACC_ENC_FAN_INDEX,
    R_INV_INDEX
} ADC_Index_Enum;

/** TODO (Issue #191): What is this struct for */
typedef struct {
    uint16_t pin[ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS];
    GPIO_TypeDef* port[ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS];
} output_pinout;

/******************************************************************************
* Global Variables
*******************************************************************************/
extern volatile GPIO_PinState dsel_state;

// E-fuse output pin mapping
static const output_pinout OUTPUT_0_PINOUT = {{EFUSE_AUX_1_IN_Pin,
                                               EFUSE_COOLING_IN_Pin,
                                               EFUSE_AIR_SHDN_IN_Pin,
                                               EFUSE_ACC_SEG_FAN_IN_Pin,
                                               EFUSE_LEFT_INVERTER_IN_Pin},
                                              {EFUSE_AUX_1_IN_GPIO_Port,
                                               EFUSE_COOLING_IN_GPIO_Port,
                                               EFUSE_AIR_SHDN_IN_GPIO_Port,
                                               EFUSE_ACC_SEG_FAN_IN_GPIO_Port,
                                               EFUSE_LEFT_INVERTER_IN_GPIO_Port}};

static const output_pinout OUTPUT_1_PINOUT = {{EFUSE_AUX_2_IN_Pin,
                                               EFUSE_PDM_FAN_IN_Pin,
                                               EFUSE_CAN_IN_Pin,
                                               EFUSE_ACC_ENC_FAN_IN_Pin,
                                               EFUSE_RIGHT_INVERTER_IN_Pin},
                                              {EFUSE_AUX_2_IN_GPIO_Port,
                                               EFUSE_PDM_FAN_IN_GPIO_Port,
                                               EFUSE_CAN_IN_GPIO_Port,
                                               EFUSE_ACC_ENC_FAN_IN_GPIO_Port,
                                               EFUSE_RIGHT_INVERTER_IN_GPIO_Port}};


/******************************************************************************
* Function Prototypes
*******************************************************************************/
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
void GPIO_ConfigurePreChargeComplete(volatile uint8_t* fault_states);

/**
 * @brief  Enable CAN/AIR SHDN (if they are not faulted) and their 
 *         corresponding current sense diagnostics. Disable all other outputs.
 * @param  fault_states Array with (NumReadings x ChannelCount) elements which 
 *         tracks outputs that need to be renabled or are permanently faulted
 */
void GPIO_ConfigurePowerUp(volatile uint8_t* fault_states);

/**
 *  @brief  Select E-Fuse output for current sense (DSEL toggle)
 *  @param  dsel_value value to set DSEL pin to (DSEL_HIGH or DSEL_LOW)
 */
void GPIO_EFuseSelectDSEL(GPIO_PinState dsel_value);

/**
 * @brief Check for faults on startup from charging IC, cell balancing IC,
 *        and boost converter and transmit a CAN message if error occured.
 */
void GPIO_CheckFaultsStartup(void);

#endif /* GPIO_H */
