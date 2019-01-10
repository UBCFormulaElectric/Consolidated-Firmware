/**
 * @file  CanDefinitions.h
 * @brief CAN message header definitions. You can find more information
 *        about the CAN bus protocol here:
 *        http://www.ti.com/lit/an/sloa101b/sloa101b.pdf
 */

#ifndef CAN_DEFINITIONS_H
#define CAN_DEFINITIONS_H
/******************************************************************************
 * Includes
 ******************************************************************************/
// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017 (Shared CAN Library doesn't yet support this)
#include "stm32f0xx_hal.h"
#else
#error \
    "No valid architecture selected - unable to determine what HAL library to use"
#endif

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format  on
/** @brief CAN Standard ID Enum*/
typedef enum
{
    BMS_ERROR_STDID                                 = (0x00U),
    BMS_HEARTBEAT_STDID                             = (0x01U),
    BMS_CAN_TX_FIFO_OVERFLOW_STDID                  = (0x02U),
    BMS_STARTUP_STDID                               = (0x03U),
    BMS_RESERVED2_STDID                             = (0x04U),
    BMS_RESERVED3_STDID                             = (0x05U),
    BMS_ACCUMULATOR_STATUS_STDID                    = (0x06U),
    BMS_DISCHARGE_POWER_LIMIT_STDID                 = (0x07U),
    BMS_REGEN_POWER_LIMIT_STDID                     = (0x08U),
    BMS_STATE_OF_CHARGE_STDID                       = (0x09U),
    BMS_TURN_ON_ALL_FETS_STDID                      = (0x0AU),
    BMS_CELL_VOLTAGE_CAN_STDID                      = (0x0BU),
    BMS_BALANCE_MOSFET_STDID                        = (0x0CU),
    BMS_LATCH_RESET_STDID                           = (0x0DU),
    BMS_MOTOR_REENABLE_STDID                        = (0x0EU),
    BMS_AIR_SHUTDOWN_ERROR_STDID                    = (0x0FU),
    DCM_ERROR_STDID                                 = (0x20U),
    DCM_HEARTBEAT_STDID                             = (0x21U),
    DCM_CAN_TX_FIFO_OVERFLOW_STDID                  = (0x22U),
    DCM_STARTUP_STDID                               = (0x23U),
    DCM_RESERVED2_STDID                             = (0x24U),
    DCM_RESERVED3_STDID                             = (0x25U),
    DCM_LEFT_SLIP_RATIO_STDID                       = (0x26U),
    DCM_RIGHT_SLIP_RATIO_STDID                      = (0x27U),
    DCM_LEFT_MOTOR_OUTPUT_STDID                     = (0x28U),
    DCM_RIGHT_MOTOR_OUTPUT_STDID                    = (0x29U),
    DCM_LEFT_SLIP_ERROR_STDID                       = (0x2AU),
    DCM_RIGHT_SLIP_ERROR_STDID                      = (0x2BU),
    FSM_ERROR_STDID                                 = (0x40U),
    FSM_HEARTBEAT_STDID                             = (0x41U),
    FSM_CAN_TX_FIFO_OVERFLOW_STDID                  = (0x42U),
    FSM_STARTUP_STDID                               = (0x43U),
    FSM_RESERVED2_STDID                             = (0x44U),
    FSM_RESERVED3_STDID                             = (0x45U),
    FSM_ACCELERATOR_PEDAL_POSITION_STDID            = (0x46U),
    FSM_STEERING_ANGLE_STDID                        = (0x47U),
    FSM_LEFT_WHEEL_SPEED_STDID                      = (0x48U),
    FSM_RIGHT_WHEEL_SPEED_STDID                     = (0x49U),
    PDM_ERROR_STDID                                 = (0x60U),
    PDM_HEARTBEAT_STDID                             = (0x61U),
    PDM_CAN_TX_FIFO_OVERFLOW_STDID                  = (0x62U),
    PDM_STARTUP_STDID                               = (0x63U),
    PDM_RESERVED2_STDID                             = (0x64U),
    PDM_RESERVED3_STDID                             = (0x65U),
    PDM_AUX1_AUX2_CURRENT_STDID                     = (0x66U),
    PDM_COOLING_PDMFAN_CURRENT_STDID                = (0x67U),
    PDM_AIRSHDN_CANGLV_CURRENT_STDID                = (0x68U),
    PDM_ACCUMULATOR_FAN_CURRENT_STDID               = (0x69U),
    PDM_INVERTER_IO_CURRENT_STDID                   = (0x6AU),
    PDM_12V_VBAT_STDID                              = (0x6BU),
    PDM_FLYWIRE_STDID                               = (0x6CU),
    SHARED_MOTOR_SHUTDOWN_ERROR_STDID               = (0x80U),
    LEFT_TORQUE_REQUEST_TX_STDID                    = (0x190U),
    RIGHT_TORQUE_REQUEST_TX_STDID                   = (0x191U),
    LEFT_TORQUE_REQUEST_RX_STDID                    = (0x210U),
    RIGHT_TORQUE_REQUEST_RX_STDID                   = (0x211U),
    DEMO_TOGGLE_GPIO1_STDID                         = (0x400U),
    DEMO_TOGGLE_GPIO2_STDID                         = (0x401U),
    DEMO_2_FLOAT_NUCLEO_TX_STDID                    = (0x402U),
    DEMO_2_FLOAT_NUCLEO_RX_STDID                    = (0x403U),
    DEMO_2_UINT32_NUCLEO_TX_STDID                   = (0x404U),
    DEMO_2_UINT32_NUCLEO_RX_STDID                   = (0x405U),
    DEMO_4_UINT16_NUCLEO_TX_STDID                   = (0x406U),
    DEMO_4_UINT16_NUCLEO_RX_STDID                   = (0x407U),
    DEMO_8_UINT8_NUCLEO_TX_STDID                    = (0x408U),
    DEMO_8_UINT8_NUCLEO_RX_STDID                    = (0x409U),
    DEMO_2_INT32_NUCLEO_TX_STDID                    = (0x40AU),
    DEMO_2_INT32_NUCLEO_RX_STDID                    = (0x40BU)
} CanStandardId_Enum;


/** @brief CAN Data Length Code Enum */
typedef enum
{
    BMS_ERROR_DLC                                   = (4U),
    BMS_HEARTBEAT_DLC                               = (1U),
    BMS_CAN_TX_FIFO_OVERFLOW_DLC                    = (4U),
    BMS_STARTUP_DLC                                 = (1U),
    BMS_RESERVED2_DLC                               = (8U),
    BMS_RESERVED3_DLC                               = (8U),
    BMS_ACCUMULATOR_STATUS_DLC                      = (3U),
    BMS_DISCHARGE_POWER_LIMIT_DLC                   = (4U),
    BMS_REGEN_POWER_LIMIT_DLC                       = (4U),
    BMS_STATE_OF_CHARGE_DLC                         = (4U),
    BMS_TURN_ON_ALL_FETS_DLC                        = (1U),
    BMS_CELL_VOLTAGE_CAN_DLC                        = (4U),
    BMS_BALANCE_MOSFET_DLC                          = (5U),
    BMS_LATCH_RESET_DLC                             = (1U),
    BMS_MOTOR_REENABLE_DLC                          = (1U),
    BMS_AIR_SHUTDOWN_ERROR_DLC                      = (6U),
    DCM_ERROR_DLC                                   = (4U),
    DCM_HEARTBEAT_DLC                               = (1U),
    DCM_CAN_TX_FIFO_OVERFLOW_DLC                    = (4U),
    DCM_STARTUP_DLC                                 = (1U),
    DCM_RESERVED2_DLC                               = (8U),
    DCM_RESERVED3_DLC                               = (8U),
    DCM_LEFT_SLIP_RATIO_DLC                         = (4U),
    DCM_RIGHT_SLIP_RATIO_DLC                        = (4U),
    DCM_LEFT_MOTOR_OUTPUT_DLC                       = (4U),
    DCM_RIGHT_MOTOR_OUTPUT_DLC                      = (4U),
    DCM_LEFT_SLIP_ERROR_DLC                         = (4U),
    DCM_RIGHT_SLIP_ERROR_DLC                        = (4U),
    FSM_ERROR_DLC                                   = (4U),
    FSM_HEARTBEAT_DLC                               = (1U),
    FSM_CAN_TX_FIFO_OVERFLOW_DLC                    = (4U),
    FSM_STARTUP_DLC                                 = (1U),
    FSM_RESERVED2_DLC                               = (8U),
    FSM_RESERVED3_DLC                               = (8U),
    FSM_ACCELERATOR_PEDAL_POSITION_DLC              = (3U),
    FSM_STEERING_ANGLE_DLC                          = (4U),
    FSM_LEFT_WHEEL_SPEED_DLC                        = (4U),
    FSM_RIGHT_WHEEL_SPEED_DLC                       = (4U),
    PDM_ERROR_DLC                                   = (4U),
    PDM_HEARTBEAT_DLC                               = (1U),
    PDM_CAN_TX_FIFO_OVERFLOW_DLC                    = (4U),
    PDM_STARTUP_DLC                                 = (1U),
    PDM_RESERVED2_DLC                               = (8U),
    PDM_RESERVED3_DLC                               = (8U),
    PDM_AUX1_AUX2_CURRENT_DLC                       = (8U),
    PDM_COOLING_PDMFAN_CURRENT_DLC                  = (8U),
    PDM_AIRSHDN_CANGLV_CURRENT_DLC                  = (8U),
    PDM_ACCUMULATOR_FAN_CURRENT_DLC                 = (8U),
    PDM_INVERTER_IO_CURRENT_DLC                     = (8U),
    PDM_12V_VBAT_DLC                                = (8U),
    PDM_FLYWIRE_DLC                                 = (4U),
    SHARED_MOTOR_SHUTDOWN_ERROR_DLC                 = (6U),
    LEFT_TORQUE_REQUEST_TX_DLC                      = (3U),
    RIGHT_TORQUE_REQUEST_TX_DLC                     = (3U),
    LEFT_TORQUE_REQUEST_RX_DLC                      = (3U),
    RIGHT_TORQUE_REQUEST_RX_DLC                     = (3U),
    DEMO_TOGGLE_GPIO1_DLC                           = (1U),
    DEMO_TOGGLE_GPIO2_DLC                           = (1U),
    DEMO_2_FLOAT_NUCLEO_TX_DLC                      = (8U),
    DEMO_2_FLOAT_NUCLEO_RX_DLC                      = (8U),
    DEMO_2_UINT32_NUCLEO_TX_DLC                     = (8U),
    DEMO_2_UINT32_NUCLEO_RX_DLC                     = (8U),
    DEMO_4_UINT16_NUCLEO_TX_DLC                     = (8U),
    DEMO_4_UINT16_NUCLEO_RX_DLC                     = (8U),
    DEMO_8_UINT8_NUCLEO_TX_DLC                      = (8U),
    DEMO_8_UINT8_NUCLEO_RX_DLC                      = (8U),
    DEMO_2_INT32_NUCLEO_TX_DLC                      = (8U),
    DEMO_2_INT32_NUCLEO_RX_DLC                      = (8U)
} CanDataLengthCode_Enum;

/** @brief PCB Names */
typedef enum
{
    BATTERY_MANAGEMENT_SYSTEM,
    DRIVE_CONTROL_MODULE,
    POWER_DISTRIBUTION_MODULE,
    FRONT_SENSOR_MODULE,
    PCB_COUNT
} Pcb_Enum;

/**
 * @brief FSM Errors
 * PAPPS_OUT_OF_RANGE: PAPPS value out of acceptable range
 * SAPPS_OUT_OF_RANGE: SAPPS value out of acceptable range
 * MAX3097E_ALARMD: The ALARMD pin on either PAPPS or SAPPS indicates a fault
 * PEDAL_TRAVEL_IMPLAUSIBILITY: Differene between PAPPS and SAPPS exceeds
 *                              threshold
 * PLAUSIBILITY: Brake pedal and accelerator pedal pressed at the same time
 * MAX_TORQUE_STUCK: Max torque is requested for an excessivey long duration
 * STEERING_OUT_OF_RANGE: Steering angle out of acceptable range
 * WHEELSPEED_OUT_OF_RANGE: Wheel speed out of acceptable range
 * BMS_HEARTBEAT_TIMEOUT: Missed too many BMS heartbeat
 */
typedef enum
{
    PAPPS_OUT_OF_RANGE,
    SAPPS_OUT_OF_RANGE,
    MAX3097E_ALARMD,
    PEDAL_TRAVEL_IMPLAUSIBILITY,
    PLAUSIBILITY,
    MAX_TORQUE_STUCK,
    STEERING_OUT_OF_RANGE,
    WHEELSPEED_OUT_OF_RANGE,
    BMS_HEARTBEAT_TIMEOUT,
    FSM_ERRORS_COUNT
} FsmError_Enum;

/** @brief PDM Errors */
typedef enum
{
    SYSTICK_INITIALISATION_ERROR,
    MISSING_HEARTBEAT,
    EFUSE_FAULT,
    CHARGER_FAULT,
    CELL_BALANCE_OVERVOLTAGE_FAULT,
    BOOST_PGOOD_FAULT,
    _12V_FAULT_OV,
    _12V_FAULT_UV,
    VBAT_FAULT,
    PDM_ERRORS_COUNT
} PdmError_Enum;

/** @brief DCM Errors */
typedef enum
{
    DCM_SENSOR_RANGE_ERROR,
    BMS_HEARTBEAT_TIMEOUT_ERROR_ON_DCM,
    DCM_ERRORS_COUNT
} DcmError_Enum;

typedef struct
{
    uint32_t std_id;
    uint32_t dlc;
} CanInfo_Struct;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

#endif /* CAN_DEFINITIONS_H */
