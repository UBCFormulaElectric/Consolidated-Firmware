/**
 * @file  CANDefinitions.h
 * @brief CAN message header definitions
 */

#ifndef CAN_DEFINITIONS_H
#define CAN_DEFINITIONS_H

/** @defgroup CAN_Standard_IDs CAN Standard IDs */
#define BMS_ERROR_STDID                                 (uint32_t)(0x00)
#define BMS_HEARTBEAT_STDID                             (uint32_t)(0x01)
#define BMS_CAN_TX_OVERFLOW_STDID                       (uint32_t)(0x02)
#define BMS_RESERVED1_STDID                             (uint32_t)(0x03)
#define BMS_RESERVED2_STDID                             (uint32_t)(0x04)
#define BMS_RESERVED3_STDID                             (uint32_t)(0x05)
#define BMS_ACCUMULATOR_STATUS_STDID                    (uint32_t)(0x06)
#define BMS_ACCUMULATOR_DISCHARGE_POWER_LIMIT_STDID     (uint32_t)(0x07)
#define BMS_ACCUMULATOR_REGEN_POWER_LIMIT_STDID         (uint32_t)(0x08)
#define BMS_ACCUMULATOR_SOC_STDID                       (uint32_t)(0x09)
#define BMS_TURN_ON_ALL_FETS_STDID                      (uint32_t)(0x0A)
#define BMS_CELL_VOLTAGE_CAN_STDID                      (uint32_t)(0x0B)
#define BMS_BALANCE_MOSFET_STDID                        (uint32_t)(0x0C)
#define BMS_LATCH_RESET_STDID                           (uint32_t)(0x0D)
#define BMS_MOTOR_REENABLE_STDID                        (uint32_t)(0x0E)
#define BMS_AIR_SHUTDOWN_ERROR_STDID                    (uint32_t)(0x0F)
#define DCM_ERROR_STDID                                 (uint32_t)(0x20)
#define DCM_HEARTBEAT_STDID                             (uint32_t)(0x21)
#define DCM_CAN_TX_OVERFLOW_STDID                       (uint32_t)(0x22)
#define DCM_RESERVED1_STDID                             (uint32_t)(0x23)
#define DCM_RESERVED2_STDID                             (uint32_t)(0x24)
#define DCM_RESERVED3_STDID                             (uint32_t)(0x25)
#define DCM_LEFT_SLIP_RATIO_STDID                       (uint32_t)(0x26)
#define DCM_RIGHT_SLIP_RATIO_STDID                      (uint32_t)(0x27)
#define DCM_LEFT_MOTOR_OUTPUT_STDID                     (uint32_t)(0x28)
#define DCM_RIGHT_MOTOR_OUTPUT_STDID                    (uint32_t)(0x29)
#define DCM_LEFT_SLIP_ERROR_STDID                       (uint32_t)(0x2A)
#define DCM_RIGHT_SLIP_ERROR_STDID                      (uint32_t)(0x2B)
#define FSM_ERROR_STDID                                 (uint32_t)(0x40)
#define FSM_HEARTBEAT_STDID                             (uint32_t)(0x41)
#define FSM_CAN_TX_OVERFLOW_STDID                       (uint32_t)(0x42)
#define FSM_RESERVED1_STDID                             (uint32_t)(0x43)
#define FSM_RESERVED2_STDID                             (uint32_t)(0x44)
#define FSM_RESERVED3_STDID                             (uint32_t)(0x45)
#define FSM_BRAKE_APPS_STDID                            (uint32_t)(0x46)
#define FSM_STEERING_ANGLE_STDID                        (uint32_t)(0x47)
#define FSM_LEFT_WHEEL_SPEED_STDID                      (uint32_t)(0x48)
#define FSM_RIGHT_WHEEL_SPEED_STDID                     (uint32_t)(0x49)
#define PDM_ERROR_STDID                                 (uint32_t)(0x60)
#define PDM_HEARTBEAT_STDID                             (uint32_t)(0x61)
#define PDM_CAN_TX_OVERFLOW_STDID                       (uint32_t)(0x62)
#define PDM_RESERVED1_STDID                             (uint32_t)(0x63)
#define PDM_RESERVED2_STDID                             (uint32_t)(0x64)
#define PDM_RESERVED3_STDID                             (uint32_t)(0x65)
#define PDM_STARTUP_STATUS_STDID                        (uint32_t)(0x66)
#define SHARED_MOTOR_SHUTDOWN_ERROR_STDID               (uint32_t)(0x80)
#define LEFT_TORQUE_REQUEST_TX_STDID                    (uint32_t)(0x190)
#define RIGHT_TORQUE_REQUEST_TX_STDID                   (uint32_t)(0x191)
#define LEFT_TORQUE_REQUEST_RX_STDID                    (uint32_t)(0x210)
#define RIGHT_TORQUE_REQUEST_RX_STDID                   (uint32_t)(0x211)
#define DEMO_TOGGLE_GPIO1_STDID                         (uint32_t)(0x400)
#define DEMO_TOGGLE_GPIO2_STDID                         (uint32_t)(0x401)
#define DEMO_2_FLOAT_NUCLEO_TX_STDID                    (uint32_t)(0x402)
#define DEMO_2_FLOAT_NUCLEO_RX_STDID                    (uint32_t)(0x403)
#define DEMO_2_UINT32_NUCLEO_TX_STDID                   (uint32_t)(0x404)
#define DEMO_2_UINT32_NUCLEO_RX_STDID                   (uint32_t)(0x405)
#define DEMO_4_UINT16_NUCLEO_TX_STDID                   (uint32_t)(0x406)
#define DEMO_4_UINT16_NUCLEO_RX_STDID                   (uint32_t)(0x407)
#define DEMO_8_UINT8_NUCLEO_TX_STDID                    (uint32_t)(0x408)
#define DEMO_8_UINT8_NUCLEO_RX_STDID                    (uint32_t)(0x409)
#define DEMO_2_INT32_NUCLEO_TX_STDID                    (uint32_t)(0x40A)
#define DEMO_2_INT32_NUCLEO_RX_STDID                    (uint32_t)(0x40B)

/** @defgroup CAN_Data_Length_Codes CAN Data Length Codes */
#define BMS_ERROR_DLC                                   (uint32_t)(8)
#define BMS_HEARTBEAT_DLC                               (uint32_t)(1)
#define BMS_CAN_TX_OVERFLOW_DLC                         (uint32_t)(8)
#define BMS_RESERVED1_DLC                               (uint32_t)(8)
#define BMS_RESERVED2_DLC                               (uint32_t)(8)
#define BMS_RESERVED3_DLC                               (uint32_t)(8)
#define BMS_ACCUMULATOR_STATUS_DLC                      (uint32_t)(3)
#define BMS_ACCUMULATOR_DISCHARGE_POWER_LIMIT_DLC       (uint32_t)(4)
#define BMS_ACCUMULATOR_REGEN_POWER_LIMIT_DLC           (uint32_t)(4)
#define BMS_ACCUMULATOR_SOC_DLC                         (uint32_t)(4)
#define BMS_TURN_ON_ALL_FETS_DLC                        (uint32_t)(1)
#define BMS_CELL_VOLTAGE_CAN_DLC                        (uint32_t)(4)
#define BMS_BALANCE_MOSFET_DLC                          (uint32_t)(5)
#define BMS_LATCH_RESET_DLC                             (uint32_t)(1)
#define BMS_MOTOR_REENABLE_DLC                          (uint32_t)(1)
#define BMS_AIR_SHUTDOWN_ERROR_DLC                      (uint32_t)(6)
#define DCM_ERROR_DLC                                   (uint32_t)(8)
#define DCM_HEARTBEAT_DLC                               (uint32_t)(1)
#define DCM_CAN_TX_OVERFLOW_DLC                         (uint32_t)(8)
#define DCM_RESERVED1_DLC                               (uint32_t)(8)
#define DCM_RESERVED2_DLC                               (uint32_t)(8)
#define DCM_RESERVED3_DLC                               (uint32_t)(8)
#define DCM_LEFT_SLIP_RATIO_DLC                         (uint32_t)(4)
#define DCM_RIGHT_SLIP_RATIO_DLC                        (uint32_t)(4)
#define DCM_LEFT_MOTOR_OUTPUT_DLC                       (uint32_t)(4)
#define DCM_RIGHT_MOTOR_OUTPUT_DLC                      (uint32_t)(4)
#define DCM_LEFT_SLIP_ERROR_DLC                         (uint32_t)(4)
#define DCM_RIGHT_SLIP_ERROR_DLC                        (uint32_t)(4)
#define FSM_ERROR_DLC                                   (uint32_t)(8)
#define FSM_HEARTBEAT_DLC                               (uint32_t)(1)
#define FSM_CAN_TX_OVERFLOW_DLC                         (uint32_t)(8)
#define FSM_RESERVED1_DLC                               (uint32_t)(8)
#define FSM_RESERVED2_DLC                               (uint32_t)(8)
#define FSM_RESERVED3_DLC                               (uint32_t)(8)
#define FSM_BRAKE_APPS_DLC                              (uint32_t)(4)
#define FSM_STEERING_ANGLE_DLC                          (uint32_t)(4)
#define FSM_LEFT_WHEEL_SPEED_DLC                        (uint32_t)(4)
#define FSM_RIGHT_WHEEL_SPEED_DLC                       (uint32_t)(4)
#define PDM_ERROR_DLC                                   (uint32_t)(8)
#define PDM_HEARTBEAT_DLC                               (uint32_t)(1)
#define PDM_CAN_TX_OVERFLOW_DLC                         (uint32_t)(8)
#define PDM_RESERVED1_DLC                               (uint32_t)(8)
#define PDM_RESERVED2_DLC                               (uint32_t)(8)
#define PDM_RESERVED3_DLC                               (uint32_t)(8)
#define PDM_STARTUP_STATUS_DLC                          (uint32_t)(1)
#define SHARED_MOTOR_SHUTDOWN_ERROR_DLC                 (uint32_t)(6)
#define LEFT_TORQUE_REQUEST_TX_DLC                      (uint32_t)(3)
#define RIGHT_TORQUE_REQUEST_TX_DLC                     (uint32_t)(3)
#define LEFT_TORQUE_REQUEST_RX_DLC                      (uint32_t)(3)
#define RIGHT_TORQUE_REQUEST_RX_DLC                     (uint32_t)(3)
#define DEMO_TOGGLE_GPIO1_DLC                           (uint32_t)(1)
#define DEMO_TOGGLE_GPIO2_DLC                           (uint32_t)(1)
#define DEMO_2_FLOAT_NUCLEO_TX_DLC                      (uint32_t)(8)
#define DEMO_2_FLOAT_NUCLEO_RX_DLC                      (uint32_t)(8)
#define DEMO_2_UINT32_NUCLEO_TX_DLC                     (uint32_t)(8)
#define DEMO_2_UINT32_NUCLEO_RX_DLC                     (uint32_t)(8)
#define DEMO_4_UINT16_NUCLEO_TX_DLC                     (uint32_t)(8)
#define DEMO_4_UINT16_NUCLEO_RX_DLC                     (uint32_t)(8)
#define DEMO_8_UINT8_NUCLEO_TX_DLC                      (uint32_t)(8)
#define DEMO_8_UINT8_NUCLEO_RX_DLC                      (uint32_t)(8)
#define DEMO_2_INT32_NUCLEO_TX_DLC                      (uint32_t)(8)
#define DEMO_2_INT32_NUCLEO_RX_DLC                      (uint32_t)(8)

/** @brief PCB Names */
typedef enum
{
    BATTERY_MANAGEMENT_SYSTEM = 0,
    DRIVE_CONTROL_MODULE,
    POWER_DISTRIBUTION_MODULE,
    FRONT_SENSOR_MODULE,
    PCB_COUNT
} Pcb_Enum;

/** @brief FSM Errors */
typedef enum
{
    // Begin counting faults at 1 for boolean comparison
    FSM_APPS_NORMAL_OPERATION = 0,
    FSM_APPS_OPEN_CIRCUIT_SHORT_CIRCUIT_ERROR,
    FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR,
    FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR,
    FSM_APPS_MAX_TORQUE_ERROR,
    FSM_STEERING_BOUND_ERROR,
    FSM_WHEEL_SPEED_BOUND_ERROR,
    FSM_BMS_HEARTBEAT_TIMEOUT_ERROR
} FsmError_Enum;

/** @brief PDM Errors */
typedef enum
{
    SYSTICK_INITIALISATION_ERROR = 0,
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
    DCM_SENSOR_RANGE_ERROR = 0,
    BMS_HEARTBEAT_TIMEOUT_ERROR_ON_DCM
} DcmError_Enum;

#endif
