/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "App_CanRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by DCM (i.e. the RX table).
 */
typedef struct
{
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_AvailablePower_Signals BMS_AvailablePower_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    Debug_CanModes_Signals Debug_CanModes_signals;
    DIM_Vitals_Signals DIM_Vitals_signals;
    DIM_Switches_Signals DIM_Switches_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    FSM_Vitals_Signals FSM_Vitals_signals;
    FSM_Brake_Signals FSM_Brake_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    INVL_MotorPositionInfo_Signals INVL_MotorPositionInfo_signals;
    INVL_InternalStates_Signals INVL_InternalStates_signals;
    INVR_MotorPositionInfo_Signals INVR_MotorPositionInfo_signals;
    INVR_InternalStates_Signals INVR_InternalStates_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
} DCM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static DCM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(DCM_RxMsgs));
    App_CanRx_BMS_Vitals_Heartbeat_Update(CANSIG_BMS_VITALS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Vitals_CurrentState_Update(CANSIG_BMS_VITALS_CURRENT_STATE_START_VAL);
    App_CanRx_BMS_Vitals_StateOfCharge_Update(CANSIG_BMS_VITALS_STATE_OF_CHARGE_START_VAL);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_STATE_FAULT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMM_ERROR_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_FAULT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Update(CANSIG_BMS_FAULTS_BMS_FAULT_HAS_REACHED_MAX_V_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_TS_OVERCURRENT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Update(CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_ERROR_START_VAL);
    App_CanRx_Debug_CanModes_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODES_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_DIM_Vitals_Heartbeat_Update(CANSIG_DIM_VITALS_HEARTBEAT_START_VAL);
    App_CanRx_DIM_Vitals_State_Update(CANSIG_DIM_VITALS_STATE_START_VAL);
    App_CanRx_DIM_Switches_StartSwitch_Update(CANSIG_DIM_SWITCHES_START_SWITCH_START_VAL);
    App_CanRx_DIM_Switches_AuxSwitch_Update(CANSIG_DIM_SWITCHES_AUX_SWITCH_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Update(CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Vitals_Heartbeat_Update(CANSIG_FSM_VITALS_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Vitals_State_Update(CANSIG_FSM_VITALS_STATE_START_VAL);
    App_CanRx_FSM_Brake_FrontBrakePressure_Update(CANSIG_FSM_BRAKE_FRONT_BRAKE_PRESSURE_START_VAL);
    App_CanRx_FSM_Brake_RearBrakePressure_Update(CANSIG_FSM_BRAKE_REAR_BRAKE_PRESSURE_START_VAL);
    App_CanRx_FSM_Brake_BrakePedalPercentage_Update(CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_Brake_IsActuated_Update(CANSIG_FSM_BRAKE_IS_ACTUATED_START_VAL);
    App_CanRx_FSM_Brake_PressureSensorOpenShortCircuit_Update(CANSIG_FSM_BRAKE_PRESSURE_SENSOR_OPEN_SHORT_CIRCUIT_START_VAL);
    App_CanRx_FSM_Brake_PedalOpenShortCircuit_Update(CANSIG_FSM_BRAKE_PEDAL_OPEN_SHORT_CIRCUIT_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update(CANSIG_FSM_FAULTS_FSM_FAULT_STATE_FAULT_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Update(CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Update(CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Update(CANSIG_FSM_FAULTS_FSM_FAULT_APPS_HAS_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Update(CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Update(CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_START_VAL);
    App_CanRx_INVL_InternalStates_VsmState_Update(CANSIG_INVL_INTERNAL_STATES_VSM_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_PwmFrequency_Update(CANSIG_INVL_INTERNAL_STATES_PWM_FREQUENCY_START_VAL);
    App_CanRx_INVL_InternalStates_InverterState_Update(CANSIG_INVL_INTERNAL_STATES_INVERTER_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_Relay1State_Update(CANSIG_INVL_INTERNAL_STATES_RELAY1_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_Relay2State_Update(CANSIG_INVL_INTERNAL_STATES_RELAY2_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_Relay3State_Update(CANSIG_INVL_INTERNAL_STATES_RELAY3_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_Relay4State_Update(CANSIG_INVL_INTERNAL_STATES_RELAY4_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_Relay5State_Update(CANSIG_INVL_INTERNAL_STATES_RELAY5_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_Relay6State_Update(CANSIG_INVL_INTERNAL_STATES_RELAY6_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_InverterRunMode_Update(CANSIG_INVL_INTERNAL_STATES_INVERTER_RUN_MODE_START_VAL);
    App_CanRx_INVL_InternalStates_InverterActiveDischargeState_Update(CANSIG_INVL_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_InverterCommandMode_Update(CANSIG_INVL_INTERNAL_STATES_INVERTER_COMMAND_MODE_START_VAL);
    App_CanRx_INVL_InternalStates_RollingCounter_Update(CANSIG_INVL_INTERNAL_STATES_ROLLING_COUNTER_START_VAL);
    App_CanRx_INVL_InternalStates_InverterEnableState_Update(CANSIG_INVL_INTERNAL_STATES_INVERTER_ENABLE_STATE_START_VAL);
    App_CanRx_INVL_InternalStates_StartModeActive_Update(CANSIG_INVL_INTERNAL_STATES_START_MODE_ACTIVE_START_VAL);
    App_CanRx_INVL_InternalStates_InverterLockoutEnabled_Update(CANSIG_INVL_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_START_VAL);
    App_CanRx_INVL_InternalStates_DirectionCommand_Update(CANSIG_INVL_INTERNAL_STATES_DIRECTION_COMMAND_START_VAL);
    App_CanRx_INVL_InternalStates_BmsActive_Update(CANSIG_INVL_INTERNAL_STATES_BMS_ACTIVE_START_VAL);
    App_CanRx_INVL_InternalStates_BmsTorqueLimiting_Update(CANSIG_INVL_INTERNAL_STATES_BMS_TORQUE_LIMITING_START_VAL);
    App_CanRx_INVL_InternalStates_MaxSpeedLimiting_Update(CANSIG_INVL_INTERNAL_STATES_MAX_SPEED_LIMITING_START_VAL);
    App_CanRx_INVL_InternalStates_HotSpotLimiting_Update(CANSIG_INVL_INTERNAL_STATES_HOT_SPOT_LIMITING_START_VAL);
    App_CanRx_INVL_InternalStates_LowSpeedLimiting_Update(CANSIG_INVL_INTERNAL_STATES_LOW_SPEED_LIMITING_START_VAL);
    App_CanRx_INVL_InternalStates_CoolantTemperatureLimiting_Update(CANSIG_INVL_INTERNAL_STATES_COOLANT_TEMPERATURE_LIMITING_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_MotorAngle_Update(CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Update(CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Update(CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_START_VAL);
    App_CanRx_INVR_InternalStates_VsmState_Update(CANSIG_INVR_INTERNAL_STATES_VSM_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_PwmFrequency_Update(CANSIG_INVR_INTERNAL_STATES_PWM_FREQUENCY_START_VAL);
    App_CanRx_INVR_InternalStates_InverterState_Update(CANSIG_INVR_INTERNAL_STATES_INVERTER_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_Relay1State_Update(CANSIG_INVR_INTERNAL_STATES_RELAY1_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_Relay2State_Update(CANSIG_INVR_INTERNAL_STATES_RELAY2_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_Relay3State_Update(CANSIG_INVR_INTERNAL_STATES_RELAY3_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_Relay4State_Update(CANSIG_INVR_INTERNAL_STATES_RELAY4_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_Relay5State_Update(CANSIG_INVR_INTERNAL_STATES_RELAY5_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_Relay6State_Update(CANSIG_INVR_INTERNAL_STATES_RELAY6_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_InverterRunMode_Update(CANSIG_INVR_INTERNAL_STATES_INVERTER_RUN_MODE_START_VAL);
    App_CanRx_INVR_InternalStates_InverterActiveDischargeState_Update(CANSIG_INVR_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_InverterCommandMode_Update(CANSIG_INVR_INTERNAL_STATES_INVERTER_COMMAND_MODE_START_VAL);
    App_CanRx_INVR_InternalStates_RollingCounter_Update(CANSIG_INVR_INTERNAL_STATES_ROLLING_COUNTER_START_VAL);
    App_CanRx_INVR_InternalStates_InverterEnableState_Update(CANSIG_INVR_INTERNAL_STATES_INVERTER_ENABLE_STATE_START_VAL);
    App_CanRx_INVR_InternalStates_StartModeActive_Update(CANSIG_INVR_INTERNAL_STATES_START_MODE_ACTIVE_START_VAL);
    App_CanRx_INVR_InternalStates_InverterLockoutEnabled_Update(CANSIG_INVR_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_START_VAL);
    App_CanRx_INVR_InternalStates_DirectionCommand_Update(CANSIG_INVR_INTERNAL_STATES_DIRECTION_COMMAND_START_VAL);
    App_CanRx_INVR_InternalStates_BmsActive_Update(CANSIG_INVR_INTERNAL_STATES_BMS_ACTIVE_START_VAL);
    App_CanRx_INVR_InternalStates_BmsTorqueLimiting_Update(CANSIG_INVR_INTERNAL_STATES_BMS_TORQUE_LIMITING_START_VAL);
    App_CanRx_INVR_InternalStates_MaxSpeedLimiting_Update(CANSIG_INVR_INTERNAL_STATES_MAX_SPEED_LIMITING_START_VAL);
    App_CanRx_INVR_InternalStates_HotSpotLimiting_Update(CANSIG_INVR_INTERNAL_STATES_HOT_SPOT_LIMITING_START_VAL);
    App_CanRx_INVR_InternalStates_LowSpeedLimiting_Update(CANSIG_INVR_INTERNAL_STATES_LOW_SPEED_LIMITING_START_VAL);
    App_CanRx_INVR_InternalStates_CoolantTemperatureLimiting_Update(CANSIG_INVR_INTERNAL_STATES_COOLANT_TEMPERATURE_LIMITING_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update(CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_START_VAL);
}

void App_CanRx_BMS_Vitals_Heartbeat_Update(bool value)
{
    rx_table.BMS_Vitals_signals.Heartbeat_value = value;
}

void App_CanRx_BMS_Vitals_CurrentState_Update(BmsState value)
{
    rx_table.BMS_Vitals_signals.CurrentState_value = (value > CANSIG_BMS_VITALS_CURRENT_STATE_MAX) ? CANSIG_BMS_VITALS_CURRENT_STATE_MAX : value;
}

void App_CanRx_BMS_Vitals_StateOfCharge_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_VITALS_STATE_OF_CHARGE_MIN ? CANSIG_BMS_VITALS_STATE_OF_CHARGE_MIN : value;
    rx_table.BMS_Vitals_signals.StateOfCharge_value = tmp > CANSIG_BMS_VITALS_STATE_OF_CHARGE_MAX ? CANSIG_BMS_VITALS_STATE_OF_CHARGE_MAX : tmp;
}

void App_CanRx_BMS_AvailablePower_AvailablePower_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_MIN ? CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_MIN : value;
    rx_table.BMS_AvailablePower_signals.AvailablePower_value = tmp > CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_MAX ? CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_MAX : tmp;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_MISSING_HEARTBEAT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_STATE_FAULT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERVOLTAGE_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERVOLTAGE_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_MODULE_COMM_ERROR_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERTEMP_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERTEMP_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_FAULT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_HAS_REACHED_MAX_V_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_TS_OVERCURRENT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_PRECHARGE_ERROR_value = value;
}

void App_CanRx_Debug_CanModes_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanModes_signals.EnableDebugMode_value = value;
}

void App_CanRx_DIM_Vitals_Heartbeat_Update(bool value)
{
    rx_table.DIM_Vitals_signals.Heartbeat_value = value;
}

void App_CanRx_DIM_Vitals_State_Update(DimState value)
{
    rx_table.DIM_Vitals_signals.State_value = (value > CANSIG_DIM_VITALS_STATE_MAX) ? CANSIG_DIM_VITALS_STATE_MAX : value;
}

void App_CanRx_DIM_Switches_StartSwitch_Update(SwitchState value)
{
    rx_table.DIM_Switches_signals.StartSwitch_value = (value > CANSIG_DIM_SWITCHES_START_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_START_SWITCH_MAX : value;
}

void App_CanRx_DIM_Switches_AuxSwitch_Update(SwitchState value)
{
    rx_table.DIM_Switches_signals.AuxSwitch_value = (value > CANSIG_DIM_SWITCHES_AUX_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_AUX_SWITCH_MAX : value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Update(bool value)
{
    rx_table.DIM_Faults_signals.DIM_FAULT_MISSING_HEARTBEAT_value = value;
}

void App_CanRx_FSM_Vitals_Heartbeat_Update(bool value)
{
    rx_table.FSM_Vitals_signals.Heartbeat_value = value;
}

void App_CanRx_FSM_Vitals_State_Update(State_Machine value)
{
    rx_table.FSM_Vitals_signals.State_value = (value > CANSIG_FSM_VITALS_STATE_MAX) ? CANSIG_FSM_VITALS_STATE_MAX : value;
}

void App_CanRx_FSM_Brake_FrontBrakePressure_Update(uint32_t value)
{
    rx_table.FSM_Brake_signals.FrontBrakePressure_value = (value > CANSIG_FSM_BRAKE_FRONT_BRAKE_PRESSURE_MAX) ? CANSIG_FSM_BRAKE_FRONT_BRAKE_PRESSURE_MAX : value;
}

void App_CanRx_FSM_Brake_RearBrakePressure_Update(uint32_t value)
{
    rx_table.FSM_Brake_signals.RearBrakePressure_value = (value > CANSIG_FSM_BRAKE_REAR_BRAKE_PRESSURE_MAX) ? CANSIG_FSM_BRAKE_REAR_BRAKE_PRESSURE_MAX : value;
}

void App_CanRx_FSM_Brake_BrakePedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Brake_signals.BrakePedalPercentage_value = tmp > CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_Brake_IsActuated_Update(bool value)
{
    rx_table.FSM_Brake_signals.IsActuated_value = value;
}

void App_CanRx_FSM_Brake_PressureSensorOpenShortCircuit_Update(bool value)
{
    rx_table.FSM_Brake_signals.PressureSensorOpenShortCircuit_value = value;
}

void App_CanRx_FSM_Brake_PedalOpenShortCircuit_Update(bool value)
{
    rx_table.FSM_Brake_signals.PedalOpenShortCircuit_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_MISSING_HEARTBEAT_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_STATE_FAULT_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_APPS_HAS_DISAGREEMENT_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value = value;
}

void App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.MotorAngle_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.MotorSpeed_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.ElectricalOutputFrequency_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.DeltaResolverFiltered_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

void App_CanRx_INVL_InternalStates_VsmState_Update(InverterVsmState value)
{
    rx_table.INVL_InternalStates_signals.VsmState_value = (value > CANSIG_INVL_INTERNAL_STATES_VSM_STATE_MAX) ? CANSIG_INVL_INTERNAL_STATES_VSM_STATE_MAX : value;
}

void App_CanRx_INVL_InternalStates_PwmFrequency_Update(uint32_t value)
{
    rx_table.INVL_InternalStates_signals.PwmFrequency_value = (value > CANSIG_INVL_INTERNAL_STATES_PWM_FREQUENCY_MAX) ? CANSIG_INVL_INTERNAL_STATES_PWM_FREQUENCY_MAX : value;
}

void App_CanRx_INVL_InternalStates_InverterState_Update(InverterState value)
{
    rx_table.INVL_InternalStates_signals.InverterState_value = (value > CANSIG_INVL_INTERNAL_STATES_INVERTER_STATE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVERTER_STATE_MAX : value;
}

void App_CanRx_INVL_InternalStates_Relay1State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.Relay1State_value = value;
}

void App_CanRx_INVL_InternalStates_Relay2State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.Relay2State_value = value;
}

void App_CanRx_INVL_InternalStates_Relay3State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.Relay3State_value = value;
}

void App_CanRx_INVL_InternalStates_Relay4State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.Relay4State_value = value;
}

void App_CanRx_INVL_InternalStates_Relay5State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.Relay5State_value = value;
}

void App_CanRx_INVL_InternalStates_Relay6State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.Relay6State_value = value;
}

void App_CanRx_INVL_InternalStates_InverterRunMode_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.InverterRunMode_value = value;
}

void App_CanRx_INVL_InternalStates_InverterActiveDischargeState_Update(InverterActiveDischargeState value)
{
    rx_table.INVL_InternalStates_signals.InverterActiveDischargeState_value = (value > CANSIG_INVL_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_MAX : value;
}

void App_CanRx_INVL_InternalStates_InverterCommandMode_Update(InverterCommandMode value)
{
    rx_table.INVL_InternalStates_signals.InverterCommandMode_value = (value > CANSIG_INVL_INTERNAL_STATES_INVERTER_COMMAND_MODE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVERTER_COMMAND_MODE_MAX : value;
}

void App_CanRx_INVL_InternalStates_RollingCounter_Update(uint32_t value)
{
    rx_table.INVL_InternalStates_signals.RollingCounter_value = (value > CANSIG_INVL_INTERNAL_STATES_ROLLING_COUNTER_MAX) ? CANSIG_INVL_INTERNAL_STATES_ROLLING_COUNTER_MAX : value;
}

void App_CanRx_INVL_InternalStates_InverterEnableState_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.InverterEnableState_value = value;
}

void App_CanRx_INVL_InternalStates_StartModeActive_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.StartModeActive_value = value;
}

void App_CanRx_INVL_InternalStates_InverterLockoutEnabled_Update(InverterLockout value)
{
    rx_table.INVL_InternalStates_signals.InverterLockoutEnabled_value = (value > CANSIG_INVL_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_MAX : value;
}

void App_CanRx_INVL_InternalStates_DirectionCommand_Update(InverterDirection value)
{
    rx_table.INVL_InternalStates_signals.DirectionCommand_value = (value > CANSIG_INVL_INTERNAL_STATES_DIRECTION_COMMAND_MAX) ? CANSIG_INVL_INTERNAL_STATES_DIRECTION_COMMAND_MAX : value;
}

void App_CanRx_INVL_InternalStates_BmsActive_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.BmsActive_value = value;
}

void App_CanRx_INVL_InternalStates_BmsTorqueLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.BmsTorqueLimiting_value = value;
}

void App_CanRx_INVL_InternalStates_MaxSpeedLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.MaxSpeedLimiting_value = value;
}

void App_CanRx_INVL_InternalStates_HotSpotLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.HotSpotLimiting_value = value;
}

void App_CanRx_INVL_InternalStates_LowSpeedLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.LowSpeedLimiting_value = value;
}

void App_CanRx_INVL_InternalStates_CoolantTemperatureLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.CoolantTemperatureLimiting_value = value;
}

void App_CanRx_INVR_MotorPositionInfo_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.MotorAngle_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.MotorSpeed_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.ElectricalOutputFrequency_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.DeltaResolverFiltered_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

void App_CanRx_INVR_InternalStates_VsmState_Update(InverterVsmState value)
{
    rx_table.INVR_InternalStates_signals.VsmState_value = (value > CANSIG_INVR_INTERNAL_STATES_VSM_STATE_MAX) ? CANSIG_INVR_INTERNAL_STATES_VSM_STATE_MAX : value;
}

void App_CanRx_INVR_InternalStates_PwmFrequency_Update(uint32_t value)
{
    rx_table.INVR_InternalStates_signals.PwmFrequency_value = (value > CANSIG_INVR_INTERNAL_STATES_PWM_FREQUENCY_MAX) ? CANSIG_INVR_INTERNAL_STATES_PWM_FREQUENCY_MAX : value;
}

void App_CanRx_INVR_InternalStates_InverterState_Update(InverterState value)
{
    rx_table.INVR_InternalStates_signals.InverterState_value = (value > CANSIG_INVR_INTERNAL_STATES_INVERTER_STATE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVERTER_STATE_MAX : value;
}

void App_CanRx_INVR_InternalStates_Relay1State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.Relay1State_value = value;
}

void App_CanRx_INVR_InternalStates_Relay2State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.Relay2State_value = value;
}

void App_CanRx_INVR_InternalStates_Relay3State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.Relay3State_value = value;
}

void App_CanRx_INVR_InternalStates_Relay4State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.Relay4State_value = value;
}

void App_CanRx_INVR_InternalStates_Relay5State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.Relay5State_value = value;
}

void App_CanRx_INVR_InternalStates_Relay6State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.Relay6State_value = value;
}

void App_CanRx_INVR_InternalStates_InverterRunMode_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.InverterRunMode_value = value;
}

void App_CanRx_INVR_InternalStates_InverterActiveDischargeState_Update(InverterActiveDischargeState value)
{
    rx_table.INVR_InternalStates_signals.InverterActiveDischargeState_value = (value > CANSIG_INVR_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_MAX : value;
}

void App_CanRx_INVR_InternalStates_InverterCommandMode_Update(InverterCommandMode value)
{
    rx_table.INVR_InternalStates_signals.InverterCommandMode_value = (value > CANSIG_INVR_INTERNAL_STATES_INVERTER_COMMAND_MODE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVERTER_COMMAND_MODE_MAX : value;
}

void App_CanRx_INVR_InternalStates_RollingCounter_Update(uint32_t value)
{
    rx_table.INVR_InternalStates_signals.RollingCounter_value = (value > CANSIG_INVR_INTERNAL_STATES_ROLLING_COUNTER_MAX) ? CANSIG_INVR_INTERNAL_STATES_ROLLING_COUNTER_MAX : value;
}

void App_CanRx_INVR_InternalStates_InverterEnableState_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.InverterEnableState_value = value;
}

void App_CanRx_INVR_InternalStates_StartModeActive_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.StartModeActive_value = value;
}

void App_CanRx_INVR_InternalStates_InverterLockoutEnabled_Update(InverterLockout value)
{
    rx_table.INVR_InternalStates_signals.InverterLockoutEnabled_value = (value > CANSIG_INVR_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_MAX : value;
}

void App_CanRx_INVR_InternalStates_DirectionCommand_Update(InverterDirection value)
{
    rx_table.INVR_InternalStates_signals.DirectionCommand_value = (value > CANSIG_INVR_INTERNAL_STATES_DIRECTION_COMMAND_MAX) ? CANSIG_INVR_INTERNAL_STATES_DIRECTION_COMMAND_MAX : value;
}

void App_CanRx_INVR_InternalStates_BmsActive_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.BmsActive_value = value;
}

void App_CanRx_INVR_InternalStates_BmsTorqueLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.BmsTorqueLimiting_value = value;
}

void App_CanRx_INVR_InternalStates_MaxSpeedLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.MaxSpeedLimiting_value = value;
}

void App_CanRx_INVR_InternalStates_HotSpotLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.HotSpotLimiting_value = value;
}

void App_CanRx_INVR_InternalStates_LowSpeedLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.LowSpeedLimiting_value = value;
}

void App_CanRx_INVR_InternalStates_CoolantTemperatureLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.CoolantTemperatureLimiting_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update(bool value)
{
    rx_table.PDM_Faults_signals.PDM_FAULT_DUMMY_value = value;
}

bool App_CanRx_BMS_Vitals_Heartbeat_Get()
{
    return rx_table.BMS_Vitals_signals.Heartbeat_value;
}

BmsState App_CanRx_BMS_Vitals_CurrentState_Get()
{
    return rx_table.BMS_Vitals_signals.CurrentState_value;
}

float App_CanRx_BMS_Vitals_StateOfCharge_Get()
{
    return rx_table.BMS_Vitals_signals.StateOfCharge_value;
}

float App_CanRx_BMS_AvailablePower_AvailablePower_Get()
{
    return rx_table.BMS_AvailablePower_signals.AvailablePower_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_MISSING_HEARTBEAT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_STATE_FAULT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERVOLTAGE_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERVOLTAGE_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_MODULE_COMM_ERROR_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERTEMP_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERTEMP_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_FAULT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_HAS_REACHED_MAX_V_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_TS_OVERCURRENT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_PRECHARGE_ERROR_value;
}

bool App_CanRx_Debug_CanModes_EnableDebugMode_Get()
{
    return rx_table.Debug_CanModes_signals.EnableDebugMode_value;
}

bool App_CanRx_DIM_Vitals_Heartbeat_Get()
{
    return rx_table.DIM_Vitals_signals.Heartbeat_value;
}

DimState App_CanRx_DIM_Vitals_State_Get()
{
    return rx_table.DIM_Vitals_signals.State_value;
}

SwitchState App_CanRx_DIM_Switches_StartSwitch_Get()
{
    return rx_table.DIM_Switches_signals.StartSwitch_value;
}

SwitchState App_CanRx_DIM_Switches_AuxSwitch_Get()
{
    return rx_table.DIM_Switches_signals.AuxSwitch_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Get()
{
    return rx_table.DIM_Faults_signals.DIM_FAULT_MISSING_HEARTBEAT_value;
}

bool App_CanRx_FSM_Vitals_Heartbeat_Get()
{
    return rx_table.FSM_Vitals_signals.Heartbeat_value;
}

State_Machine App_CanRx_FSM_Vitals_State_Get()
{
    return rx_table.FSM_Vitals_signals.State_value;
}

uint32_t App_CanRx_FSM_Brake_FrontBrakePressure_Get()
{
    return rx_table.FSM_Brake_signals.FrontBrakePressure_value;
}

uint32_t App_CanRx_FSM_Brake_RearBrakePressure_Get()
{
    return rx_table.FSM_Brake_signals.RearBrakePressure_value;
}

float App_CanRx_FSM_Brake_BrakePedalPercentage_Get()
{
    return rx_table.FSM_Brake_signals.BrakePedalPercentage_value;
}

bool App_CanRx_FSM_Brake_IsActuated_Get()
{
    return rx_table.FSM_Brake_signals.IsActuated_value;
}

bool App_CanRx_FSM_Brake_PressureSensorOpenShortCircuit_Get()
{
    return rx_table.FSM_Brake_signals.PressureSensorOpenShortCircuit_value;
}

bool App_CanRx_FSM_Brake_PedalOpenShortCircuit_Get()
{
    return rx_table.FSM_Brake_signals.PedalOpenShortCircuit_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_MISSING_HEARTBEAT_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_STATE_FAULT_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_APPS_HAS_DISAGREEMENT_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value;
}

float App_CanRx_INVL_MotorPositionInfo_MotorAngle_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.MotorAngle_value;
}

int App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.MotorSpeed_value;
}

float App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.ElectricalOutputFrequency_value;
}

float App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.DeltaResolverFiltered_value;
}

InverterVsmState App_CanRx_INVL_InternalStates_VsmState_Get()
{
    return rx_table.INVL_InternalStates_signals.VsmState_value;
}

uint32_t App_CanRx_INVL_InternalStates_PwmFrequency_Get()
{
    return rx_table.INVL_InternalStates_signals.PwmFrequency_value;
}

InverterState App_CanRx_INVL_InternalStates_InverterState_Get()
{
    return rx_table.INVL_InternalStates_signals.InverterState_value;
}

bool App_CanRx_INVL_InternalStates_Relay1State_Get()
{
    return rx_table.INVL_InternalStates_signals.Relay1State_value;
}

bool App_CanRx_INVL_InternalStates_Relay2State_Get()
{
    return rx_table.INVL_InternalStates_signals.Relay2State_value;
}

bool App_CanRx_INVL_InternalStates_Relay3State_Get()
{
    return rx_table.INVL_InternalStates_signals.Relay3State_value;
}

bool App_CanRx_INVL_InternalStates_Relay4State_Get()
{
    return rx_table.INVL_InternalStates_signals.Relay4State_value;
}

bool App_CanRx_INVL_InternalStates_Relay5State_Get()
{
    return rx_table.INVL_InternalStates_signals.Relay5State_value;
}

bool App_CanRx_INVL_InternalStates_Relay6State_Get()
{
    return rx_table.INVL_InternalStates_signals.Relay6State_value;
}

bool App_CanRx_INVL_InternalStates_InverterRunMode_Get()
{
    return rx_table.INVL_InternalStates_signals.InverterRunMode_value;
}

InverterActiveDischargeState App_CanRx_INVL_InternalStates_InverterActiveDischargeState_Get()
{
    return rx_table.INVL_InternalStates_signals.InverterActiveDischargeState_value;
}

InverterCommandMode App_CanRx_INVL_InternalStates_InverterCommandMode_Get()
{
    return rx_table.INVL_InternalStates_signals.InverterCommandMode_value;
}

uint32_t App_CanRx_INVL_InternalStates_RollingCounter_Get()
{
    return rx_table.INVL_InternalStates_signals.RollingCounter_value;
}

bool App_CanRx_INVL_InternalStates_InverterEnableState_Get()
{
    return rx_table.INVL_InternalStates_signals.InverterEnableState_value;
}

bool App_CanRx_INVL_InternalStates_StartModeActive_Get()
{
    return rx_table.INVL_InternalStates_signals.StartModeActive_value;
}

InverterLockout App_CanRx_INVL_InternalStates_InverterLockoutEnabled_Get()
{
    return rx_table.INVL_InternalStates_signals.InverterLockoutEnabled_value;
}

InverterDirection App_CanRx_INVL_InternalStates_DirectionCommand_Get()
{
    return rx_table.INVL_InternalStates_signals.DirectionCommand_value;
}

bool App_CanRx_INVL_InternalStates_BmsActive_Get()
{
    return rx_table.INVL_InternalStates_signals.BmsActive_value;
}

bool App_CanRx_INVL_InternalStates_BmsTorqueLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.BmsTorqueLimiting_value;
}

bool App_CanRx_INVL_InternalStates_MaxSpeedLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.MaxSpeedLimiting_value;
}

bool App_CanRx_INVL_InternalStates_HotSpotLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.HotSpotLimiting_value;
}

bool App_CanRx_INVL_InternalStates_LowSpeedLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.LowSpeedLimiting_value;
}

bool App_CanRx_INVL_InternalStates_CoolantTemperatureLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.CoolantTemperatureLimiting_value;
}

float App_CanRx_INVR_MotorPositionInfo_MotorAngle_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.MotorAngle_value;
}

int App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.MotorSpeed_value;
}

float App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.ElectricalOutputFrequency_value;
}

float App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.DeltaResolverFiltered_value;
}

InverterVsmState App_CanRx_INVR_InternalStates_VsmState_Get()
{
    return rx_table.INVR_InternalStates_signals.VsmState_value;
}

uint32_t App_CanRx_INVR_InternalStates_PwmFrequency_Get()
{
    return rx_table.INVR_InternalStates_signals.PwmFrequency_value;
}

InverterState App_CanRx_INVR_InternalStates_InverterState_Get()
{
    return rx_table.INVR_InternalStates_signals.InverterState_value;
}

bool App_CanRx_INVR_InternalStates_Relay1State_Get()
{
    return rx_table.INVR_InternalStates_signals.Relay1State_value;
}

bool App_CanRx_INVR_InternalStates_Relay2State_Get()
{
    return rx_table.INVR_InternalStates_signals.Relay2State_value;
}

bool App_CanRx_INVR_InternalStates_Relay3State_Get()
{
    return rx_table.INVR_InternalStates_signals.Relay3State_value;
}

bool App_CanRx_INVR_InternalStates_Relay4State_Get()
{
    return rx_table.INVR_InternalStates_signals.Relay4State_value;
}

bool App_CanRx_INVR_InternalStates_Relay5State_Get()
{
    return rx_table.INVR_InternalStates_signals.Relay5State_value;
}

bool App_CanRx_INVR_InternalStates_Relay6State_Get()
{
    return rx_table.INVR_InternalStates_signals.Relay6State_value;
}

bool App_CanRx_INVR_InternalStates_InverterRunMode_Get()
{
    return rx_table.INVR_InternalStates_signals.InverterRunMode_value;
}

InverterActiveDischargeState App_CanRx_INVR_InternalStates_InverterActiveDischargeState_Get()
{
    return rx_table.INVR_InternalStates_signals.InverterActiveDischargeState_value;
}

InverterCommandMode App_CanRx_INVR_InternalStates_InverterCommandMode_Get()
{
    return rx_table.INVR_InternalStates_signals.InverterCommandMode_value;
}

uint32_t App_CanRx_INVR_InternalStates_RollingCounter_Get()
{
    return rx_table.INVR_InternalStates_signals.RollingCounter_value;
}

bool App_CanRx_INVR_InternalStates_InverterEnableState_Get()
{
    return rx_table.INVR_InternalStates_signals.InverterEnableState_value;
}

bool App_CanRx_INVR_InternalStates_StartModeActive_Get()
{
    return rx_table.INVR_InternalStates_signals.StartModeActive_value;
}

InverterLockout App_CanRx_INVR_InternalStates_InverterLockoutEnabled_Get()
{
    return rx_table.INVR_InternalStates_signals.InverterLockoutEnabled_value;
}

InverterDirection App_CanRx_INVR_InternalStates_DirectionCommand_Get()
{
    return rx_table.INVR_InternalStates_signals.DirectionCommand_value;
}

bool App_CanRx_INVR_InternalStates_BmsActive_Get()
{
    return rx_table.INVR_InternalStates_signals.BmsActive_value;
}

bool App_CanRx_INVR_InternalStates_BmsTorqueLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.BmsTorqueLimiting_value;
}

bool App_CanRx_INVR_InternalStates_MaxSpeedLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.MaxSpeedLimiting_value;
}

bool App_CanRx_INVR_InternalStates_HotSpotLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.HotSpotLimiting_value;
}

bool App_CanRx_INVR_InternalStates_LowSpeedLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.LowSpeedLimiting_value;
}

bool App_CanRx_INVR_InternalStates_CoolantTemperatureLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.CoolantTemperatureLimiting_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Get()
{
    return rx_table.PDM_Faults_signals.PDM_FAULT_DUMMY_value;
}

