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
    INVL_MotorPositionInfo_Signals INVL_MotorPositionInfo_signals;
    INVL_InternalStates_Signals INVL_InternalStates_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_AvailablePower_Signals BMS_AvailablePower_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
    FSM_Vitals_Signals FSM_Vitals_signals;
    FSM_Apps_Signals FSM_Apps_signals;
    FSM_Brake_Signals FSM_Brake_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
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
    App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_DIM_WatchdogTimeoutWarning_Update(CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_DIM_TxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_RxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_MissingHeartbeatFault_Update(CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_START_VAL);
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
    App_CanRx_BMS_Heartbeat_Update(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_State_Update(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    App_CanRx_BMS_AvailablePower_Update(CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_BMS_WatchdogTimeoutWarning_Update(CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_BMS_TxOverflowWarning_Update(CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_BMS_RxOverflowWarning_Update(CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_BMS_MissingHeartbeatFault_Update(CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanRx_BMS_StateMachineFault_Update(CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_START_VAL);
    App_CanRx_BMS_CellUndervoltageFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_START_VAL);
    App_CanRx_BMS_CellOvervoltageFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_START_VAL);
    App_CanRx_BMS_ModuleCommunicationFault_Update(CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_START_VAL);
    App_CanRx_BMS_CellUndertempFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_START_VAL);
    App_CanRx_BMS_CellOvertempFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_START_VAL);
    App_CanRx_BMS_ChargerFault_Update(CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_START_VAL);
    App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Update(CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_START_VAL);
    App_CanRx_BMS_ChargerExternalShutdownFault_Update(CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_START_VAL);
    App_CanRx_BMS_TractiveSystemOvercurrentFault_Update(CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_START_VAL);
    App_CanRx_BMS_PrechargeFailureFault_Update(CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_PDM_WatchdogTimeoutWarning_Update(CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_PDM_TxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_RxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_DummyFault_Update(CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_START_VAL);
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
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
    App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_FSM_WatchdogTimeoutWarning_Update(CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_FSM_TxOverflowWarning_Update(CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_FSM_RxOverflowWarning_Update(CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_SteeringAngleOCSCWarning_Update(CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_START_VAL);
    App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_FlowRateOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_BrakeAppsDisagreementWarning_Update(CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_START_VAL);
    App_CanRx_FSM_AppsDisagreementWarning_Update(CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_START_VAL);
    App_CanRx_FSM_MissingHeartbeatFault_Update(CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanRx_FSM_StateMachineFault_Update(CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_START_VAL);
    App_CanRx_FSM_PappsOCSCFault_Update(CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_START_VAL);
    App_CanRx_FSM_SappsOCSCFault_Update(CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_START_VAL);
    App_CanRx_FSM_FlowMeterUnderflowFault_Update(CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_START_VAL);
    App_CanRx_FSM_TorquePlausabilityFailedFault_Update(CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_START_VAL);
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

void App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_DIM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_DIM_TxOverflowWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_TxOverflowWarning_value = value;
}

void App_CanRx_DIM_RxOverflowWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_RxOverflowWarning_value = value;
}

void App_CanRx_DIM_MissingHeartbeatFault_Update(bool value)
{
    rx_table.DIM_Faults_signals.DIM_MissingHeartbeatFault_value = value;
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

void App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_BMS_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_BMS_TxOverflowWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_TxOverflowWarning_value = value;
}

void App_CanRx_BMS_RxOverflowWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_RxOverflowWarning_value = value;
}

void App_CanRx_BMS_MissingHeartbeatFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_MissingHeartbeatFault_value = value;
}

void App_CanRx_BMS_StateMachineFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_StateMachineFault_value = value;
}

void App_CanRx_BMS_CellUndervoltageFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellUndervoltageFault_value = value;
}

void App_CanRx_BMS_CellOvervoltageFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellOvervoltageFault_value = value;
}

void App_CanRx_BMS_ModuleCommunicationFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ModuleCommunicationFault_value = value;
}

void App_CanRx_BMS_CellUndertempFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellUndertempFault_value = value;
}

void App_CanRx_BMS_CellOvertempFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellOvertempFault_value = value;
}

void App_CanRx_BMS_ChargerFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ChargerFault_value = value;
}

void App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ChargerDisconnectedDuringChargeFault_value = value;
}

void App_CanRx_BMS_ChargerExternalShutdownFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ChargerExternalShutdownFault_value = value;
}

void App_CanRx_BMS_TractiveSystemOvercurrentFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_TractiveSystemOvercurrentFault_value = value;
}

void App_CanRx_BMS_PrechargeFailureFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_PrechargeFailureFault_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_PDM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_PDM_TxOverflowWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_TxOverflowWarning_value = value;
}

void App_CanRx_PDM_RxOverflowWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_RxOverflowWarning_value = value;
}

void App_CanRx_PDM_DummyFault_Update(bool value)
{
    rx_table.PDM_Faults_signals.PDM_DummyFault_value = value;
}

void App_CanRx_Debug_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value = value;
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

void App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_FSM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_FSM_TxOverflowWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_TxOverflowWarning_value = value;
}

void App_CanRx_FSM_RxOverflowWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_RxOverflowWarning_value = value;
}

void App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_LeftWheelSpeedOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_RightWheelSpeedOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_SteeringAngleOCSCWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_SteeringAngleOCSCWarning_value = value;
}

void App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_SteeringAngleOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_FrontBrakePressureOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_RearBrakePressureOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_FlowRateOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_FlowRateOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_BrakeAppsDisagreementWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_BrakeAppsDisagreementWarning_value = value;
}

void App_CanRx_FSM_AppsDisagreementWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_AppsDisagreementWarning_value = value;
}

void App_CanRx_FSM_MissingHeartbeatFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_MissingHeartbeatFault_value = value;
}

void App_CanRx_FSM_StateMachineFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_StateMachineFault_value = value;
}

void App_CanRx_FSM_PappsOCSCFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_PappsOCSCFault_value = value;
}

void App_CanRx_FSM_SappsOCSCFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_SappsOCSCFault_value = value;
}

void App_CanRx_FSM_FlowMeterUnderflowFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FlowMeterUnderflowFault_value = value;
}

void App_CanRx_FSM_TorquePlausabilityFailedFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_TorquePlausabilityFailedFault_value = value;
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

bool App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_DIM_WatchdogTimeoutWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_DIM_TxOverflowWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_TxOverflowWarning_value;
}

bool App_CanRx_DIM_RxOverflowWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_RxOverflowWarning_value;
}

bool App_CanRx_DIM_MissingHeartbeatFault_Get()
{
    return rx_table.DIM_Faults_signals.DIM_MissingHeartbeatFault_value;
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

bool App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_BMS_WatchdogTimeoutWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WatchdogTimeoutWarning_value;
}

bool App_CanRx_BMS_TxOverflowWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_TxOverflowWarning_value;
}

bool App_CanRx_BMS_RxOverflowWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_RxOverflowWarning_value;
}

bool App_CanRx_BMS_MissingHeartbeatFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_MissingHeartbeatFault_value;
}

bool App_CanRx_BMS_StateMachineFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_StateMachineFault_value;
}

bool App_CanRx_BMS_CellUndervoltageFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellUndervoltageFault_value;
}

bool App_CanRx_BMS_CellOvervoltageFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellOvervoltageFault_value;
}

bool App_CanRx_BMS_ModuleCommunicationFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ModuleCommunicationFault_value;
}

bool App_CanRx_BMS_CellUndertempFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellUndertempFault_value;
}

bool App_CanRx_BMS_CellOvertempFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellOvertempFault_value;
}

bool App_CanRx_BMS_ChargerFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ChargerFault_value;
}

bool App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ChargerDisconnectedDuringChargeFault_value;
}

bool App_CanRx_BMS_ChargerExternalShutdownFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ChargerExternalShutdownFault_value;
}

bool App_CanRx_BMS_TractiveSystemOvercurrentFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_TractiveSystemOvercurrentFault_value;
}

bool App_CanRx_BMS_PrechargeFailureFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_PrechargeFailureFault_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_PDM_WatchdogTimeoutWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_PDM_TxOverflowWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_TxOverflowWarning_value;
}

bool App_CanRx_PDM_RxOverflowWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_RxOverflowWarning_value;
}

bool App_CanRx_PDM_DummyFault_Get()
{
    return rx_table.PDM_Faults_signals.PDM_DummyFault_value;
}

bool App_CanRx_Debug_EnableDebugMode_Get()
{
    return rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value;
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

bool App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_FSM_WatchdogTimeoutWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_FSM_TxOverflowWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_TxOverflowWarning_value;
}

bool App_CanRx_FSM_RxOverflowWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_RxOverflowWarning_value;
}

bool App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_LeftWheelSpeedOutOfRangeWarning_value;
}

bool App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_RightWheelSpeedOutOfRangeWarning_value;
}

bool App_CanRx_FSM_SteeringAngleOCSCWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_SteeringAngleOCSCWarning_value;
}

bool App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_SteeringAngleOutOfRangeWarning_value;
}

bool App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_FrontBrakePressureOutOfRangeWarning_value;
}

bool App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_RearBrakePressureOutOfRangeWarning_value;
}

bool App_CanRx_FSM_FlowRateOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_FlowRateOutOfRangeWarning_value;
}

bool App_CanRx_FSM_BrakeAppsDisagreementWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_BrakeAppsDisagreementWarning_value;
}

bool App_CanRx_FSM_AppsDisagreementWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_AppsDisagreementWarning_value;
}

bool App_CanRx_FSM_MissingHeartbeatFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_MissingHeartbeatFault_value;
}

bool App_CanRx_FSM_StateMachineFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_StateMachineFault_value;
}

bool App_CanRx_FSM_PappsOCSCFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_PappsOCSCFault_value;
}

bool App_CanRx_FSM_SappsOCSCFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_SappsOCSCFault_value;
}

bool App_CanRx_FSM_FlowMeterUnderflowFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FlowMeterUnderflowFault_value;
}

bool App_CanRx_FSM_TorquePlausabilityFailedFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_TorquePlausabilityFailedFault_value;
}

