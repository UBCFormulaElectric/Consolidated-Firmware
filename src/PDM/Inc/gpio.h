/**
 *   @file       GPIO.h
 *   @brief      PDM 2018 Code
 *   @author     UBC Formula Electric
**/

#ifndef GPIO_H
#define GPIO_H

#include "stm32f3xx_hal.h"
#include "CAN.h"

// clang-format off
// ADC Variables

#define ADC_CHANNEL_COUNT 8
#define NUM_CHANNELS 2
#define VOLTAGE_SENSE_PINS 3
#define ADC_EFUSE_READINGS ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS
// There are (ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS)* NUM_CHANNELS Efuse readings as well as VOLTAGE_SENSE_PINS voltage readings
#define ADC_TOTAL_READINGS_SIZE NUM_CHANNELS*ADC_EFUSE_READINGS + VOLTAGE_SENSE_PINS

// Pin definitions

// E-Fuse AUX 1/2
#define EFUSE_AUX_1_IN_PIN 										GPIO_PIN_0
#define EFUSE_AUX_1_IN_PORT 									GPIOB

#define EFUSE_AUX_2_IN_PIN 										GPIO_PIN_7
#define EFUSE_AUX_2_IN_PORT 									GPIOA

#define EFUSE_AUX_DSEL_PIN 										GPIO_PIN_4
#define EFUSE_AUX_DSEL_PORT 									GPIOC

#define EFUSE_AUX_DEN_PIN 										GPIO_PIN_5
#define EFUSE_AUX_DEN_PORT 										GPIOC

// E-Fuse PDM Fan/Cooling
#define EFUSE_PDM_FAN_IN_PIN 									GPIO_PIN_1
#define EFUSE_PDM_FAN_IN_PORT 									GPIOB

#define EFUSE_COOLING_IN_PIN 									GPIO_PIN_11
#define EFUSE_COOLING_IN_PORT 									GPIOB

#define EFUSE_FAN_COOLING_DSEL_PIN 								GPIO_PIN_2
#define EFUSE_FAN_COOLING_DSEL_PORT 							GPIOB

#define EFUSE_FAN_COOLING_DEN_PIN 								GPIO_PIN_10
#define EFUSE_FAN_COOLING_DEN_PORT 								GPIOB

// E-Fuse CAN/AIR SHDN
#define EFUSE_CAN_IN_PIN 										GPIO_PIN_12
#define EFUSE_CAN_IN_PORT 										GPIOB

#define EFUSE_AIR_SHDN_IN_PIN 									GPIO_PIN_15
#define EFUSE_AIR_SHDN_IN_PORT 									GPIOB

#define EFUSE_CAN_AIR_SHDN_DSEL_PIN 							GPIO_PIN_13
#define EFUSE_CAN_AIR_SHDN_DSEL_PORT 							GPIOB

#define EFUSE_CAN_AIR_SHDN_DEN_PIN 								GPIO_PIN_14
#define EFUSE_CAN_AIR_SHDN_DEN_PORT 							GPIOB

// E-Fuse Accumulator Fans
#define EFUSE_ACC_SEG_FAN_IN_PIN 								GPIO_PIN_9
#define EFUSE_ACC_SEG_FAN_IN_PORT 								GPIOC

#define EFUSE_ACC_ENC_FAN_IN_PIN 								GPIO_PIN_6
#define EFUSE_ACC_ENC_FAN_IN_PORT 								GPIOC

#define EFUSE_ACC_FAN_DSEL_PIN 									GPIO_PIN_7
#define EFUSE_ACC_FAN_DSEL_PORT 								GPIOC

#define EFUSE_ACC_FAN_DEN_PIN 									GPIO_PIN_8
#define EFUSE_ACC_FAN_DEN_PORT 									GPIOC

// E-Fuse Inverter
#define EFUSE_LEFT_INVERTER_IN_PIN 								GPIO_PIN_15
#define EFUSE_LEFT_INVERTER_IN_PORT 							GPIOA

#define EFUSE_RIGHT_INVERTER_IN_PIN 							GPIO_PIN_8
#define EFUSE_RIGHT_INVERTER_IN_PORT 							GPIOA

#define EFUSE_INVERTER_DSEL_PIN 								GPIO_PIN_9
#define EFUSE_INVERTER_DSEL_PORT 								GPIOA

#define EFUSE_INVERTER_DEN_PIN 									GPIO_PIN_10
#define EFUSE_INVERTER_DEN_PORT 								GPIOA

// Battery Charger
#define CHARGER_FAULT_PIN 										GPIO_PIN_10
#define CHARGER_INDICATOR_PIN 									GPIO_PIN_11
#define CHARGER_PORT 											GPIOC
#define CHARGER_FAULT_STATE 									GPIO_PIN_RESET // Low = fault, high = no fault
#define CHARGER_CHARGING_STATE									GPIO_PIN_RESET // Low = charging, high = not charging

// Battery Cell Balancing
#define CELL_BALANCE_OVERVOLTAGE_PIN 							GPIO_PIN_2
#define CELL_BALANCE_OVERVOLTAGE_PORT 							GPIOD
#define CELL_BALANCE_OVERVOLTAGE_FAULT_STATE 					GPIO_PIN_SET // High = fault, low = no fault

// Boost Converter
#define BOOST_PGOOD_PIN 										GPIO_PIN_12
#define BOOST_PGOOD_PORT 										GPIOC
#define BOOST_PGOOD_FAULT_STATE 								GPIO_PIN_RESET // Low = fault, high = no fault

// clang-format on

// E-fuse output pin mapping
typedef struct {
    uint16_t pin[ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS];
    GPIO_TypeDef* port[ADC_CHANNEL_COUNT - VOLTAGE_SENSE_PINS];
} output_pinout;

static const output_pinout OUTPUT_0_PINOUT = {{EFUSE_AUX_1_IN_PIN,
                                               EFUSE_COOLING_IN_PIN,
                                               EFUSE_AIR_SHDN_IN_PIN,
                                               EFUSE_ACC_SEG_FAN_IN_PIN,
                                               EFUSE_LEFT_INVERTER_IN_PIN},
                                              {EFUSE_AUX_1_IN_PORT,
                                               EFUSE_COOLING_IN_PORT,
                                               EFUSE_AIR_SHDN_IN_PORT,
                                               EFUSE_ACC_SEG_FAN_IN_PORT,
                                               EFUSE_LEFT_INVERTER_IN_PORT}};

static const output_pinout OUTPUT_1_PINOUT = {{EFUSE_AUX_2_IN_PIN,
                                               EFUSE_PDM_FAN_IN_PIN,
                                               EFUSE_CAN_IN_PIN,
                                               EFUSE_ACC_ENC_FAN_IN_PIN,
                                               EFUSE_RIGHT_INVERTER_IN_PIN},
                                              {EFUSE_AUX_2_IN_PORT,
                                               EFUSE_PDM_FAN_IN_PORT,
                                               EFUSE_CAN_IN_PORT,
                                               EFUSE_ACC_ENC_FAN_IN_PORT,
                                               EFUSE_RIGHT_INVERTER_IN_PORT}};

// Efuse State
typedef enum {
    STATIC_EFUSE = 0, // Operating as expected
    RENABLE_EFUSE =
    1, // Exceeded current limit but not max number of retries, in retry mode
    ERROR_EFUSE =
    2 // Exceeded max number of retries, permanently in error state
} Efuse_State_Enum;

// Efuse Indexing, corresponding to: 0 to ADC_TOTAL_READINGS_SIZE-1
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

// DSEL State
#define DSEL_LOW GPIO_PIN_RESET
#define DSEL_HIGH GPIO_PIN_SET

extern __IO GPIO_PinState DSEL_State;

// Function declarations

/**
 * 	@brief Initialize GPIO 
 *	@param None
 *	@return None
**/
void GPIO_Init(void);
/**
 *   @brief      Enable all e-fuses (that have not faulted) and their corresponding current sense
 * diagnostics. Only to be called after PreCharge is completed.
 *  @param      fault_states 			Array with NumReadings*ChannelCount
 * elements which tracks outputs that need to be renabled or are permanently faulted
 *  @return     None
**/
void GPIO_ConfigurePreChargeComplete(__IO uint8_t* fault_states);

/**
 *   @brief      Enable CAN/AIR SHDN (if they are not faulted) and their corresponding current sense
 * diagnostics. Disable all other outputs.
 *  @param      fault_states		Array with NumReadings*ChannelCount
 * elements which tracks outputs that need to be renabled or are permanently faulted
 *  @return     None
**/
void GPIO_ConfigurePowerUp(__IO uint8_t* fault_states);

/**
 *   @brief      Select E-Fuse output for current sense (DSEL toggle)
 *  @param      DSEL_value			value to set DSEL pin to (DSEL_HIGH or DSEL_LOW)
 *  @return     None
**/
void GPIO_EFuseSelectDSEL(GPIO_PinState DSEL_value);

/**
 *   @brief      Check for faults on startup from charging IC, cell balancing IC,
 * and boost converter and transmit a CAN message if error occured.
 *  @return     None
**/
void GPIO_CheckFaultsStartup(void);
#endif
