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
    DIM_Vitals_Signals DIM_Vitals_signals;
    DIM_Switches_Signals DIM_Switches_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    INVR_MotorPositionInfo_Signals INVR_MotorPositionInfo_signals;
    INVR_InternalStates_Signals INVR_InternalStates_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_AvailablePower_Signals BMS_AvailablePower_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    FSM_Vitals_Signals FSM_Vitals_signals;
    FSM_Apps_Signals FSM_Apps_signals;
    FSM_Brake_Signals FSM_Brake_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    INVL_MotorPositionInfo_Signals INVL_MotorPositionInfo_signals;
    INVL_InternalStates_Signals INVL_InternalStates_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
} DCM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static DCM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(DCM_RxMsgs));
    App_CanRx_DIM_Heartbeat_Update(CANSIG_DIM_VITALS_DIM_HEARTBEAT_START_VAL);
    App_CanRx_DIM_State_Update(CANSIG_DIM_VITALS_DIM_STATE_START_VAL);
    App_CanRx_DIM_StartSwitch_Update(CANSIG_DIM_SWITCHES_DIM_START_SWITCH_START_VAL);
    App_CanRx_DIM_AuxSwitch_Update(CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_DIM_Warning_WatchdogTimeout_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_DIM_Warning_TxOverflow_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Warning_RxOverflow_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Fault_MissingHeartbeat_Update(CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_INVR_MotorAngle_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVR_MotorSpeed_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_START_VAL);
    App_CanRx_INVR_ElectricalOutputFrequency_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVR_DeltaResolverFiltered_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_START_VAL);
    App_CanRx_INVR_VsmState_Update(CANSIG_INVR_INTERNAL_STATES_INVR_VSM_STATE_START_VAL);
    App_CanRx_INVR_PwmFrequency_Update(CANSIG_INVR_INTERNAL_STATES_INVR_PWM_FREQUENCY_START_VAL);
    App_CanRx_INVR_InverterState_Update(CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_STATE_START_VAL);
    App_CanRx_INVR_Relay1State_Update(CANSIG_INVR_INTERNAL_STATES_INVR_RELAY1_STATE_START_VAL);
    App_CanRx_INVR_Relay2State_Update(CANSIG_INVR_INTERNAL_STATES_INVR_RELAY2_STATE_START_VAL);
    App_CanRx_INVR_Relay3State_Update(CANSIG_INVR_INTERNAL_STATES_INVR_RELAY3_STATE_START_VAL);
    App_CanRx_INVR_Relay4State_Update(CANSIG_INVR_INTERNAL_STATES_INVR_RELAY4_STATE_START_VAL);
    App_CanRx_INVR_Relay5State_Update(CANSIG_INVR_INTERNAL_STATES_INVR_RELAY5_STATE_START_VAL);
    App_CanRx_INVR_Relay6State_Update(CANSIG_INVR_INTERNAL_STATES_INVR_RELAY6_STATE_START_VAL);
    App_CanRx_INVR_InverterRunMode_Update(CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_RUN_MODE_START_VAL);
    App_CanRx_INVR_InverterActiveDischargeState_Update(CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ACTIVE_DISCHARGE_STATE_START_VAL);
    App_CanRx_INVR_InverterCommandMode_Update(CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_COMMAND_MODE_START_VAL);
    App_CanRx_INVR_RollingCounter_Update(CANSIG_INVR_INTERNAL_STATES_INVR_ROLLING_COUNTER_START_VAL);
    App_CanRx_INVR_InverterEnableState_Update(CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ENABLE_STATE_START_VAL);
    App_CanRx_INVR_StartModeActive_Update(CANSIG_INVR_INTERNAL_STATES_INVR_START_MODE_ACTIVE_START_VAL);
    App_CanRx_INVR_InverterLockoutEnabled_Update(CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_LOCKOUT_ENABLED_START_VAL);
    App_CanRx_INVR_DirectionCommand_Update(CANSIG_INVR_INTERNAL_STATES_INVR_DIRECTION_COMMAND_START_VAL);
    App_CanRx_INVR_BmsActive_Update(CANSIG_INVR_INTERNAL_STATES_INVR_BMS_ACTIVE_START_VAL);
    App_CanRx_INVR_BmsTorqueLimiting_Update(CANSIG_INVR_INTERNAL_STATES_INVR_BMS_TORQUE_LIMITING_START_VAL);
    App_CanRx_INVR_MaxSpeedLimiting_Update(CANSIG_INVR_INTERNAL_STATES_INVR_MAX_SPEED_LIMITING_START_VAL);
    App_CanRx_INVR_HotSpotLimiting_Update(CANSIG_INVR_INTERNAL_STATES_INVR_HOT_SPOT_LIMITING_START_VAL);
    App_CanRx_INVR_LowSpeedLimiting_Update(CANSIG_INVR_INTERNAL_STATES_INVR_LOW_SPEED_LIMITING_START_VAL);
    App_CanRx_INVR_CoolantTemperatureLimiting_Update(CANSIG_INVR_INTERNAL_STATES_INVR_COOLANT_TEMPERATURE_LIMITING_START_VAL);
    App_CanRx_BMS_Heartbeat_Update(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_State_Update(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    App_CanRx_BMS_AvailablePower_Update(CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_BMS_Warning_WatchdogTimeout_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_BMS_Warning_TxOverflow_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Warning_RxOverflow_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Fault_MissingHeartbeat_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Fault_StateMachine_Update(CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_START_VAL);
    App_CanRx_BMS_Fault_CellUndervoltage_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    App_CanRx_BMS_Fault_CellOvervoltage_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    App_CanRx_BMS_Fault_ModuleCommunicationError_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_START_VAL);
    App_CanRx_BMS_Fault_CellUndertemp_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    App_CanRx_BMS_Fault_CellOvertemp_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    App_CanRx_BMS_Fault_Charger_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_START_VAL);
    App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_START_VAL);
    App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_START_VAL);
    App_CanRx_BMS_Fault_PrechargeFailure_Update(CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_START_VAL);
    App_CanRx_FSM_Heartbeat_Update(CANSIG_FSM_VITALS_FSM_HEARTBEAT_START_VAL);
    App_CanRx_FSM_State_Update(CANSIG_FSM_VITALS_FSM_STATE_START_VAL);
    App_CanRx_FSM_PappsMappedPedalPercentage_Update(CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_SappsMappedPedalPercentage_Update(CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_PappsRawPedalPercentage_Update(CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_SappsRawPedalPercentage_Update(CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_TorqueLimit_Update(CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_START_VAL);
    App_CanRx_FSM_TorquePlausabilityFailed_Update(CANSIG_FSM_APPS_FSM_TORQUE_PLAUSABILITY_FAILED_START_VAL);
    App_CanRx_FSM_FrontBrakePressure_Update(CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_START_VAL);
    App_CanRx_FSM_RearBrakePressure_Update(CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_START_VAL);
    App_CanRx_FSM_BrakePedalPercentage_Update(CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_BrakeActuated_Update(CANSIG_FSM_BRAKE_FSM_BRAKE_ACTUATED_START_VAL);
    App_CanRx_FSM_BrakePressureSensorOCSC_Update(CANSIG_FSM_BRAKE_FSM_BRAKE_PRESSURE_SENSOR_OCSC_START_VAL);
    App_CanRx_FSM_BrakePedalSensorOCSC_Update(CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_SENSOR_OCSC_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_FSM_Warning_WatchdogTimeout_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_FSM_Warning_TxOverflow_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warning_RxOverflow_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_START_VAL);
    App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_Warning_FlowRateOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_BrakeAppsDisagreement_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Warning_AppsDisagreement_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Fault_MissingHeartbeat_Update(CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Fault_StateMachine_Update(CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_START_VAL);
    App_CanRx_FSM_Fault_PappsOCSC_Update(CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_START_VAL);
    App_CanRx_FSM_Fault_SappsOCSCFault_Update(CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_START_VAL);
    App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_START_VAL);
    App_CanRx_FSM_Fault_TorquePlausabilityFailed_Update(CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_START_VAL);
    App_CanRx_INVL_MotorAngle_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVL_MotorSpeed_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_START_VAL);
    App_CanRx_INVL_ElectricalOutputFrequency_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVL_DeltaResolverFiltered_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_START_VAL);
    App_CanRx_INVL_VsmState_Update(CANSIG_INVL_INTERNAL_STATES_INVL_VSM_STATE_START_VAL);
    App_CanRx_INVL_PwmFrequency_Update(CANSIG_INVL_INTERNAL_STATES_INVL_PWM_FREQUENCY_START_VAL);
    App_CanRx_INVL_InverterState_Update(CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_STATE_START_VAL);
    App_CanRx_INVL_Relay1State_Update(CANSIG_INVL_INTERNAL_STATES_INVL_RELAY1_STATE_START_VAL);
    App_CanRx_INVL_Relay2State_Update(CANSIG_INVL_INTERNAL_STATES_INVL_RELAY2_STATE_START_VAL);
    App_CanRx_INVL_Relay3State_Update(CANSIG_INVL_INTERNAL_STATES_INVL_RELAY3_STATE_START_VAL);
    App_CanRx_INVL_Relay4State_Update(CANSIG_INVL_INTERNAL_STATES_INVL_RELAY4_STATE_START_VAL);
    App_CanRx_INVL_Relay5State_Update(CANSIG_INVL_INTERNAL_STATES_INVL_RELAY5_STATE_START_VAL);
    App_CanRx_INVL_Relay6State_Update(CANSIG_INVL_INTERNAL_STATES_INVL_RELAY6_STATE_START_VAL);
    App_CanRx_INVL_InverterRunMode_Update(CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_RUN_MODE_START_VAL);
    App_CanRx_INVL_InverterActiveDischargeState_Update(CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ACTIVE_DISCHARGE_STATE_START_VAL);
    App_CanRx_INVL_InverterCommandMode_Update(CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_COMMAND_MODE_START_VAL);
    App_CanRx_INVL_RollingCounter_Update(CANSIG_INVL_INTERNAL_STATES_INVL_ROLLING_COUNTER_START_VAL);
    App_CanRx_INVL_InverterEnableState_Update(CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ENABLE_STATE_START_VAL);
    App_CanRx_INVL_StartModeActive_Update(CANSIG_INVL_INTERNAL_STATES_INVL_START_MODE_ACTIVE_START_VAL);
    App_CanRx_INVL_InverterLockoutEnabled_Update(CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_LOCKOUT_ENABLED_START_VAL);
    App_CanRx_INVL_DirectionCommand_Update(CANSIG_INVL_INTERNAL_STATES_INVL_DIRECTION_COMMAND_START_VAL);
    App_CanRx_INVL_BmsActive_Update(CANSIG_INVL_INTERNAL_STATES_INVL_BMS_ACTIVE_START_VAL);
    App_CanRx_INVL_BmsTorqueLimiting_Update(CANSIG_INVL_INTERNAL_STATES_INVL_BMS_TORQUE_LIMITING_START_VAL);
    App_CanRx_INVL_MaxSpeedLimiting_Update(CANSIG_INVL_INTERNAL_STATES_INVL_MAX_SPEED_LIMITING_START_VAL);
    App_CanRx_INVL_HotSpotLimiting_Update(CANSIG_INVL_INTERNAL_STATES_INVL_HOT_SPOT_LIMITING_START_VAL);
    App_CanRx_INVL_LowSpeedLimiting_Update(CANSIG_INVL_INTERNAL_STATES_INVL_LOW_SPEED_LIMITING_START_VAL);
    App_CanRx_INVL_CoolantTemperatureLimiting_Update(CANSIG_INVL_INTERNAL_STATES_INVL_COOLANT_TEMPERATURE_LIMITING_START_VAL);
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_PDM_Warning_WatchdogTimeout_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_PDM_Warning_TxOverflow_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Warning_RxOverflow_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Fault_DummyFault_Update(CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_FAULT_START_VAL);
}

void App_CanRx_DIM_Heartbeat_Update(bool value)
{
    rx_table.DIM_Vitals_signals.DIM_Heartbeat_value = value;
}

void App_CanRx_DIM_State_Update(DimState value)
{
    rx_table.DIM_Vitals_signals.DIM_State_value = (value > CANSIG_DIM_VITALS_DIM_STATE_MAX) ? CANSIG_DIM_VITALS_DIM_STATE_MAX : value;
}

void App_CanRx_DIM_StartSwitch_Update(SwitchState value)
{
    rx_table.DIM_Switches_signals.DIM_StartSwitch_value = (value > CANSIG_DIM_SWITCHES_DIM_START_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_DIM_START_SWITCH_MAX : value;
}

void App_CanRx_DIM_AuxSwitch_Update(SwitchState value)
{
    rx_table.DIM_Switches_signals.DIM_AuxSwitch_value = (value > CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_MAX : value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_DIM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_DIM_Warning_TxOverflow_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_TxOverflow_value = value;
}

void App_CanRx_DIM_Warning_RxOverflow_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_RxOverflow_value = value;
}

void App_CanRx_DIM_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.DIM_Faults_signals.DIM_Fault_MissingHeartbeat_value = value;
}

void App_CanRx_INVR_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_MotorAngle_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVR_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_MotorSpeed_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVR_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_ElectricalOutputFrequency_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVR_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_DeltaResolverFiltered_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

void App_CanRx_INVR_VsmState_Update(InverterVsmState value)
{
    rx_table.INVR_InternalStates_signals.INVR_VsmState_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_VSM_STATE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_VSM_STATE_MAX : value;
}

void App_CanRx_INVR_PwmFrequency_Update(uint32_t value)
{
    rx_table.INVR_InternalStates_signals.INVR_PwmFrequency_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_PWM_FREQUENCY_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_PWM_FREQUENCY_MAX : value;
}

void App_CanRx_INVR_InverterState_Update(InverterState value)
{
    rx_table.INVR_InternalStates_signals.INVR_InverterState_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_STATE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_STATE_MAX : value;
}

void App_CanRx_INVR_Relay1State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_Relay1State_value = value;
}

void App_CanRx_INVR_Relay2State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_Relay2State_value = value;
}

void App_CanRx_INVR_Relay3State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_Relay3State_value = value;
}

void App_CanRx_INVR_Relay4State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_Relay4State_value = value;
}

void App_CanRx_INVR_Relay5State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_Relay5State_value = value;
}

void App_CanRx_INVR_Relay6State_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_Relay6State_value = value;
}

void App_CanRx_INVR_InverterRunMode_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_InverterRunMode_value = value;
}

void App_CanRx_INVR_InverterActiveDischargeState_Update(InverterActiveDischargeState value)
{
    rx_table.INVR_InternalStates_signals.INVR_InverterActiveDischargeState_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ACTIVE_DISCHARGE_STATE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ACTIVE_DISCHARGE_STATE_MAX : value;
}

void App_CanRx_INVR_InverterCommandMode_Update(InverterCommandMode value)
{
    rx_table.INVR_InternalStates_signals.INVR_InverterCommandMode_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_COMMAND_MODE_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_COMMAND_MODE_MAX : value;
}

void App_CanRx_INVR_RollingCounter_Update(uint32_t value)
{
    rx_table.INVR_InternalStates_signals.INVR_RollingCounter_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_ROLLING_COUNTER_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_ROLLING_COUNTER_MAX : value;
}

void App_CanRx_INVR_InverterEnableState_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_InverterEnableState_value = value;
}

void App_CanRx_INVR_StartModeActive_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_StartModeActive_value = value;
}

void App_CanRx_INVR_InverterLockoutEnabled_Update(InverterLockout value)
{
    rx_table.INVR_InternalStates_signals.INVR_InverterLockoutEnabled_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_LOCKOUT_ENABLED_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_LOCKOUT_ENABLED_MAX : value;
}

void App_CanRx_INVR_DirectionCommand_Update(InverterDirection value)
{
    rx_table.INVR_InternalStates_signals.INVR_DirectionCommand_value = (value > CANSIG_INVR_INTERNAL_STATES_INVR_DIRECTION_COMMAND_MAX) ? CANSIG_INVR_INTERNAL_STATES_INVR_DIRECTION_COMMAND_MAX : value;
}

void App_CanRx_INVR_BmsActive_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_BmsActive_value = value;
}

void App_CanRx_INVR_BmsTorqueLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_BmsTorqueLimiting_value = value;
}

void App_CanRx_INVR_MaxSpeedLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_MaxSpeedLimiting_value = value;
}

void App_CanRx_INVR_HotSpotLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_HotSpotLimiting_value = value;
}

void App_CanRx_INVR_LowSpeedLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_LowSpeedLimiting_value = value;
}

void App_CanRx_INVR_CoolantTemperatureLimiting_Update(bool value)
{
    rx_table.INVR_InternalStates_signals.INVR_CoolantTemperatureLimiting_value = value;
}

void App_CanRx_BMS_Heartbeat_Update(bool value)
{
    rx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value;
}

void App_CanRx_BMS_State_Update(BmsState value)
{
    rx_table.BMS_Vitals_signals.BMS_State_value = (value > CANSIG_BMS_VITALS_BMS_STATE_MAX) ? CANSIG_BMS_VITALS_BMS_STATE_MAX : value;
}

void App_CanRx_BMS_AvailablePower_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MIN ? CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MIN : value;
    rx_table.BMS_AvailablePower_signals.BMS_AvailablePower_value = tmp > CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MAX ? CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MAX : tmp;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_BMS_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_BMS_Warning_TxOverflow_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value = value;
}

void App_CanRx_BMS_Warning_RxOverflow_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value = value;
}

void App_CanRx_BMS_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_MissingHeartbeat_value = value;
}

void App_CanRx_BMS_Fault_StateMachine_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value = value;
}

void App_CanRx_BMS_Fault_CellUndervoltage_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value = value;
}

void App_CanRx_BMS_Fault_CellOvervoltage_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value = value;
}

void App_CanRx_BMS_Fault_ModuleCommunicationError_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value = value;
}

void App_CanRx_BMS_Fault_CellUndertemp_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value = value;
}

void App_CanRx_BMS_Fault_CellOvertemp_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value = value;
}

void App_CanRx_BMS_Fault_Charger_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_Charger_value = value;
}

void App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value = value;
}

void App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerExternalShutdown_value = value;
}

void App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value = value;
}

void App_CanRx_BMS_Fault_PrechargeFailure_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value = value;
}

void App_CanRx_FSM_Heartbeat_Update(bool value)
{
    rx_table.FSM_Vitals_signals.FSM_Heartbeat_value = value;
}

void App_CanRx_FSM_State_Update(State_Machine value)
{
    rx_table.FSM_Vitals_signals.FSM_State_value = (value > CANSIG_FSM_VITALS_FSM_STATE_MAX) ? CANSIG_FSM_VITALS_FSM_STATE_MAX : value;
}

void App_CanRx_FSM_PappsMappedPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_SappsMappedPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_PappsRawPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_PappsRawPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_SappsRawPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_SappsRawPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_TorqueLimit_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MIN ? CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MIN : value;
    rx_table.FSM_Apps_signals.FSM_TorqueLimit_value = tmp > CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MAX ? CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MAX : tmp;
}

void App_CanRx_FSM_TorquePlausabilityFailed_Update(bool value)
{
    rx_table.FSM_Apps_signals.FSM_TorquePlausabilityFailed_value = value;
}

void App_CanRx_FSM_FrontBrakePressure_Update(uint32_t value)
{
    rx_table.FSM_Brake_signals.FSM_FrontBrakePressure_value = (value > CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_MAX) ? CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_MAX : value;
}

void App_CanRx_FSM_RearBrakePressure_Update(uint32_t value)
{
    rx_table.FSM_Brake_signals.FSM_RearBrakePressure_value = (value > CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_MAX) ? CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_MAX : value;
}

void App_CanRx_FSM_BrakePedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Brake_signals.FSM_BrakePedalPercentage_value = tmp > CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_BrakeActuated_Update(bool value)
{
    rx_table.FSM_Brake_signals.FSM_BrakeActuated_value = value;
}

void App_CanRx_FSM_BrakePressureSensorOCSC_Update(bool value)
{
    rx_table.FSM_Brake_signals.FSM_BrakePressureSensorOCSC_value = value;
}

void App_CanRx_FSM_BrakePedalSensorOCSC_Update(bool value)
{
    rx_table.FSM_Brake_signals.FSM_BrakePedalSensorOCSC_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_FSM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_FSM_Warning_TxOverflow_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value = value;
}

void App_CanRx_FSM_Warning_RxOverflow_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value = value;
}

void App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value = value;
}

void App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_Warning_FlowRateOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_FlowRateOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_BrakeAppsDisagreement_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_BrakeAppsDisagreement_value = value;
}

void App_CanRx_FSM_Warning_AppsDisagreement_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value = value;
}

void App_CanRx_FSM_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_MissingHeartbeat_value = value;
}

void App_CanRx_FSM_Fault_StateMachine_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_StateMachine_value = value;
}

void App_CanRx_FSM_Fault_PappsOCSC_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value = value;
}

void App_CanRx_FSM_Fault_SappsOCSCFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_SappsOCSCFault_value = value;
}

void App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_FlowMeterUnderflow_value = value;
}

void App_CanRx_FSM_Fault_TorquePlausabilityFailed_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_TorquePlausabilityFailed_value = value;
}

void App_CanRx_INVL_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_MotorAngle_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVL_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_MotorSpeed_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVL_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_ElectricalOutputFrequency_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVL_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_DeltaResolverFiltered_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

void App_CanRx_INVL_VsmState_Update(InverterVsmState value)
{
    rx_table.INVL_InternalStates_signals.INVL_VsmState_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_VSM_STATE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_VSM_STATE_MAX : value;
}

void App_CanRx_INVL_PwmFrequency_Update(uint32_t value)
{
    rx_table.INVL_InternalStates_signals.INVL_PwmFrequency_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_PWM_FREQUENCY_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_PWM_FREQUENCY_MAX : value;
}

void App_CanRx_INVL_InverterState_Update(InverterState value)
{
    rx_table.INVL_InternalStates_signals.INVL_InverterState_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_STATE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_STATE_MAX : value;
}

void App_CanRx_INVL_Relay1State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_Relay1State_value = value;
}

void App_CanRx_INVL_Relay2State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_Relay2State_value = value;
}

void App_CanRx_INVL_Relay3State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_Relay3State_value = value;
}

void App_CanRx_INVL_Relay4State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_Relay4State_value = value;
}

void App_CanRx_INVL_Relay5State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_Relay5State_value = value;
}

void App_CanRx_INVL_Relay6State_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_Relay6State_value = value;
}

void App_CanRx_INVL_InverterRunMode_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_InverterRunMode_value = value;
}

void App_CanRx_INVL_InverterActiveDischargeState_Update(InverterActiveDischargeState value)
{
    rx_table.INVL_InternalStates_signals.INVL_InverterActiveDischargeState_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ACTIVE_DISCHARGE_STATE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ACTIVE_DISCHARGE_STATE_MAX : value;
}

void App_CanRx_INVL_InverterCommandMode_Update(InverterCommandMode value)
{
    rx_table.INVL_InternalStates_signals.INVL_InverterCommandMode_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_COMMAND_MODE_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_COMMAND_MODE_MAX : value;
}

void App_CanRx_INVL_RollingCounter_Update(uint32_t value)
{
    rx_table.INVL_InternalStates_signals.INVL_RollingCounter_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_ROLLING_COUNTER_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_ROLLING_COUNTER_MAX : value;
}

void App_CanRx_INVL_InverterEnableState_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_InverterEnableState_value = value;
}

void App_CanRx_INVL_StartModeActive_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_StartModeActive_value = value;
}

void App_CanRx_INVL_InverterLockoutEnabled_Update(InverterLockout value)
{
    rx_table.INVL_InternalStates_signals.INVL_InverterLockoutEnabled_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_LOCKOUT_ENABLED_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_LOCKOUT_ENABLED_MAX : value;
}

void App_CanRx_INVL_DirectionCommand_Update(InverterDirection value)
{
    rx_table.INVL_InternalStates_signals.INVL_DirectionCommand_value = (value > CANSIG_INVL_INTERNAL_STATES_INVL_DIRECTION_COMMAND_MAX) ? CANSIG_INVL_INTERNAL_STATES_INVL_DIRECTION_COMMAND_MAX : value;
}

void App_CanRx_INVL_BmsActive_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_BmsActive_value = value;
}

void App_CanRx_INVL_BmsTorqueLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_BmsTorqueLimiting_value = value;
}

void App_CanRx_INVL_MaxSpeedLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_MaxSpeedLimiting_value = value;
}

void App_CanRx_INVL_HotSpotLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_HotSpotLimiting_value = value;
}

void App_CanRx_INVL_LowSpeedLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_LowSpeedLimiting_value = value;
}

void App_CanRx_INVL_CoolantTemperatureLimiting_Update(bool value)
{
    rx_table.INVL_InternalStates_signals.INVL_CoolantTemperatureLimiting_value = value;
}

void App_CanRx_Debug_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_PDM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_PDM_Warning_TxOverflow_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_TxOverflow_value = value;
}

void App_CanRx_PDM_Warning_RxOverflow_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_RxOverflow_value = value;
}

void App_CanRx_PDM_Fault_DummyFault_Update(bool value)
{
    rx_table.PDM_Faults_signals.PDM_Fault_DummyFault_value = value;
}

bool App_CanRx_DIM_Heartbeat_Get()
{
    return rx_table.DIM_Vitals_signals.DIM_Heartbeat_value;
}

DimState App_CanRx_DIM_State_Get()
{
    return rx_table.DIM_Vitals_signals.DIM_State_value;
}

SwitchState App_CanRx_DIM_StartSwitch_Get()
{
    return rx_table.DIM_Switches_signals.DIM_StartSwitch_value;
}

SwitchState App_CanRx_DIM_AuxSwitch_Get()
{
    return rx_table.DIM_Switches_signals.DIM_AuxSwitch_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_DIM_Warning_WatchdogTimeout_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_DIM_Warning_TxOverflow_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_TxOverflow_value;
}

bool App_CanRx_DIM_Warning_RxOverflow_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_RxOverflow_value;
}

bool App_CanRx_DIM_Fault_MissingHeartbeat_Get()
{
    return rx_table.DIM_Faults_signals.DIM_Fault_MissingHeartbeat_value;
}

float App_CanRx_INVR_MotorAngle_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_MotorAngle_value;
}

int App_CanRx_INVR_MotorSpeed_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_MotorSpeed_value;
}

float App_CanRx_INVR_ElectricalOutputFrequency_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_ElectricalOutputFrequency_value;
}

float App_CanRx_INVR_DeltaResolverFiltered_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_DeltaResolverFiltered_value;
}

InverterVsmState App_CanRx_INVR_VsmState_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_VsmState_value;
}

uint32_t App_CanRx_INVR_PwmFrequency_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_PwmFrequency_value;
}

InverterState App_CanRx_INVR_InverterState_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_InverterState_value;
}

bool App_CanRx_INVR_Relay1State_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_Relay1State_value;
}

bool App_CanRx_INVR_Relay2State_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_Relay2State_value;
}

bool App_CanRx_INVR_Relay3State_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_Relay3State_value;
}

bool App_CanRx_INVR_Relay4State_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_Relay4State_value;
}

bool App_CanRx_INVR_Relay5State_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_Relay5State_value;
}

bool App_CanRx_INVR_Relay6State_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_Relay6State_value;
}

bool App_CanRx_INVR_InverterRunMode_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_InverterRunMode_value;
}

InverterActiveDischargeState App_CanRx_INVR_InverterActiveDischargeState_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_InverterActiveDischargeState_value;
}

InverterCommandMode App_CanRx_INVR_InverterCommandMode_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_InverterCommandMode_value;
}

uint32_t App_CanRx_INVR_RollingCounter_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_RollingCounter_value;
}

bool App_CanRx_INVR_InverterEnableState_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_InverterEnableState_value;
}

bool App_CanRx_INVR_StartModeActive_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_StartModeActive_value;
}

InverterLockout App_CanRx_INVR_InverterLockoutEnabled_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_InverterLockoutEnabled_value;
}

InverterDirection App_CanRx_INVR_DirectionCommand_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_DirectionCommand_value;
}

bool App_CanRx_INVR_BmsActive_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_BmsActive_value;
}

bool App_CanRx_INVR_BmsTorqueLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_BmsTorqueLimiting_value;
}

bool App_CanRx_INVR_MaxSpeedLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_MaxSpeedLimiting_value;
}

bool App_CanRx_INVR_HotSpotLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_HotSpotLimiting_value;
}

bool App_CanRx_INVR_LowSpeedLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_LowSpeedLimiting_value;
}

bool App_CanRx_INVR_CoolantTemperatureLimiting_Get()
{
    return rx_table.INVR_InternalStates_signals.INVR_CoolantTemperatureLimiting_value;
}

bool App_CanRx_BMS_Heartbeat_Get()
{
    return rx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}

BmsState App_CanRx_BMS_State_Get()
{
    return rx_table.BMS_Vitals_signals.BMS_State_value;
}

float App_CanRx_BMS_AvailablePower_Get()
{
    return rx_table.BMS_AvailablePower_signals.BMS_AvailablePower_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_BMS_Warning_WatchdogTimeout_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value;
}

bool App_CanRx_BMS_Warning_TxOverflow_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value;
}

bool App_CanRx_BMS_Warning_RxOverflow_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value;
}

bool App_CanRx_BMS_Fault_MissingHeartbeat_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_MissingHeartbeat_value;
}

bool App_CanRx_BMS_Fault_StateMachine_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value;
}

bool App_CanRx_BMS_Fault_CellUndervoltage_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value;
}

bool App_CanRx_BMS_Fault_CellOvervoltage_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value;
}

bool App_CanRx_BMS_Fault_ModuleCommunicationError_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value;
}

bool App_CanRx_BMS_Fault_CellUndertemp_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value;
}

bool App_CanRx_BMS_Fault_CellOvertemp_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value;
}

bool App_CanRx_BMS_Fault_Charger_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_Charger_value;
}

bool App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value;
}

bool App_CanRx_BMS_Fault_ChargerExternalShutdown_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerExternalShutdown_value;
}

bool App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value;
}

bool App_CanRx_BMS_Fault_PrechargeFailure_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value;
}

bool App_CanRx_FSM_Heartbeat_Get()
{
    return rx_table.FSM_Vitals_signals.FSM_Heartbeat_value;
}

State_Machine App_CanRx_FSM_State_Get()
{
    return rx_table.FSM_Vitals_signals.FSM_State_value;
}

float App_CanRx_FSM_PappsMappedPedalPercentage_Get()
{
    return rx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value;
}

float App_CanRx_FSM_SappsMappedPedalPercentage_Get()
{
    return rx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value;
}

float App_CanRx_FSM_PappsRawPedalPercentage_Get()
{
    return rx_table.FSM_Apps_signals.FSM_PappsRawPedalPercentage_value;
}

float App_CanRx_FSM_SappsRawPedalPercentage_Get()
{
    return rx_table.FSM_Apps_signals.FSM_SappsRawPedalPercentage_value;
}

float App_CanRx_FSM_TorqueLimit_Get()
{
    return rx_table.FSM_Apps_signals.FSM_TorqueLimit_value;
}

bool App_CanRx_FSM_TorquePlausabilityFailed_Get()
{
    return rx_table.FSM_Apps_signals.FSM_TorquePlausabilityFailed_value;
}

uint32_t App_CanRx_FSM_FrontBrakePressure_Get()
{
    return rx_table.FSM_Brake_signals.FSM_FrontBrakePressure_value;
}

uint32_t App_CanRx_FSM_RearBrakePressure_Get()
{
    return rx_table.FSM_Brake_signals.FSM_RearBrakePressure_value;
}

float App_CanRx_FSM_BrakePedalPercentage_Get()
{
    return rx_table.FSM_Brake_signals.FSM_BrakePedalPercentage_value;
}

bool App_CanRx_FSM_BrakeActuated_Get()
{
    return rx_table.FSM_Brake_signals.FSM_BrakeActuated_value;
}

bool App_CanRx_FSM_BrakePressureSensorOCSC_Get()
{
    return rx_table.FSM_Brake_signals.FSM_BrakePressureSensorOCSC_value;
}

bool App_CanRx_FSM_BrakePedalSensorOCSC_Get()
{
    return rx_table.FSM_Brake_signals.FSM_BrakePedalSensorOCSC_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_FSM_Warning_WatchdogTimeout_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_FSM_Warning_TxOverflow_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value;
}

bool App_CanRx_FSM_Warning_RxOverflow_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value;
}

bool App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value;
}

bool App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value;
}

bool App_CanRx_FSM_Warning_SteeringAngleOCSC_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value;
}

bool App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value;
}

bool App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value;
}

bool App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRangeWarning_value;
}

bool App_CanRx_FSM_Warning_FlowRateOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_FlowRateOutOfRange_value;
}

bool App_CanRx_FSM_Warning_BrakeAppsDisagreement_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_BrakeAppsDisagreement_value;
}

bool App_CanRx_FSM_Warning_AppsDisagreement_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value;
}

bool App_CanRx_FSM_Fault_MissingHeartbeat_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_MissingHeartbeat_value;
}

bool App_CanRx_FSM_Fault_StateMachine_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_StateMachine_value;
}

bool App_CanRx_FSM_Fault_PappsOCSC_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value;
}

bool App_CanRx_FSM_Fault_SappsOCSCFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_SappsOCSCFault_value;
}

bool App_CanRx_FSM_Fault_FlowMeterUnderflow_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_FlowMeterUnderflow_value;
}

bool App_CanRx_FSM_Fault_TorquePlausabilityFailed_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_TorquePlausabilityFailed_value;
}

float App_CanRx_INVL_MotorAngle_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_MotorAngle_value;
}

int App_CanRx_INVL_MotorSpeed_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_MotorSpeed_value;
}

float App_CanRx_INVL_ElectricalOutputFrequency_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_ElectricalOutputFrequency_value;
}

float App_CanRx_INVL_DeltaResolverFiltered_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_DeltaResolverFiltered_value;
}

InverterVsmState App_CanRx_INVL_VsmState_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_VsmState_value;
}

uint32_t App_CanRx_INVL_PwmFrequency_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_PwmFrequency_value;
}

InverterState App_CanRx_INVL_InverterState_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_InverterState_value;
}

bool App_CanRx_INVL_Relay1State_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_Relay1State_value;
}

bool App_CanRx_INVL_Relay2State_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_Relay2State_value;
}

bool App_CanRx_INVL_Relay3State_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_Relay3State_value;
}

bool App_CanRx_INVL_Relay4State_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_Relay4State_value;
}

bool App_CanRx_INVL_Relay5State_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_Relay5State_value;
}

bool App_CanRx_INVL_Relay6State_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_Relay6State_value;
}

bool App_CanRx_INVL_InverterRunMode_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_InverterRunMode_value;
}

InverterActiveDischargeState App_CanRx_INVL_InverterActiveDischargeState_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_InverterActiveDischargeState_value;
}

InverterCommandMode App_CanRx_INVL_InverterCommandMode_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_InverterCommandMode_value;
}

uint32_t App_CanRx_INVL_RollingCounter_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_RollingCounter_value;
}

bool App_CanRx_INVL_InverterEnableState_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_InverterEnableState_value;
}

bool App_CanRx_INVL_StartModeActive_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_StartModeActive_value;
}

InverterLockout App_CanRx_INVL_InverterLockoutEnabled_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_InverterLockoutEnabled_value;
}

InverterDirection App_CanRx_INVL_DirectionCommand_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_DirectionCommand_value;
}

bool App_CanRx_INVL_BmsActive_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_BmsActive_value;
}

bool App_CanRx_INVL_BmsTorqueLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_BmsTorqueLimiting_value;
}

bool App_CanRx_INVL_MaxSpeedLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_MaxSpeedLimiting_value;
}

bool App_CanRx_INVL_HotSpotLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_HotSpotLimiting_value;
}

bool App_CanRx_INVL_LowSpeedLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_LowSpeedLimiting_value;
}

bool App_CanRx_INVL_CoolantTemperatureLimiting_Get()
{
    return rx_table.INVL_InternalStates_signals.INVL_CoolantTemperatureLimiting_value;
}

bool App_CanRx_Debug_EnableDebugMode_Get()
{
    return rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_PDM_Warning_WatchdogTimeout_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_PDM_Warning_TxOverflow_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_TxOverflow_value;
}

bool App_CanRx_PDM_Warning_RxOverflow_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_RxOverflow_value;
}

bool App_CanRx_PDM_Fault_DummyFault_Get()
{
    return rx_table.PDM_Faults_signals.PDM_Fault_DummyFault_value;
}

