/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "App_CanUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void App_CanRx_Init(void);

/**
 * Update value stored in RX table of signal Heartbeat in msg BMS_Vitals.
 */
void App_CanRx_BMS_Vitals_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal CurrentState in msg BMS_Vitals.
 */
void App_CanRx_BMS_Vitals_CurrentState_Update(BmsState value);

/**
 * Update value stored in RX table of signal StateOfCharge in msg BMS_Vitals.
 */
void App_CanRx_BMS_Vitals_StateOfCharge_Update(float value);

/**
 * Update value stored in RX table of signal AvailablePower in msg BMS_AvailablePower.
 */
void App_CanRx_BMS_AvailablePower_AvailablePower_Update(float value);

/**
 * Update value stored in RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_WATCHDOG_TIMEOUT in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_TX_OVERFLOW in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WARNING_RX_OVERFLOW in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_MISSING_HEARTBEAT in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_STATE_FAULT in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CELL_UNDERVOLTAGE in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CELL_OVERVOLTAGE in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_MODULE_COMM_ERROR in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CELL_UNDERTEMP in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CELL_OVERTEMP in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CHARGER_FAULT in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_HAS_REACHED_MAX_V in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_TS_OVERCURRENT in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_FAULT_PRECHARGE_ERROR in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Update(bool value);

/**
 * Update value stored in RX table of signal EnableDebugMode in msg Debug_CanModes.
 */
void App_CanRx_Debug_CanModes_EnableDebugMode_Update(bool value);

/**
 * Update value stored in RX table of signal Heartbeat in msg DIM_Vitals.
 */
void App_CanRx_DIM_Vitals_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal State in msg DIM_Vitals.
 */
void App_CanRx_DIM_Vitals_State_Update(DimState value);

/**
 * Update value stored in RX table of signal StartSwitch in msg DIM_Switches.
 */
void App_CanRx_DIM_Switches_StartSwitch_Update(SwitchState value);

/**
 * Update value stored in RX table of signal AuxSwitch in msg DIM_Switches.
 */
void App_CanRx_DIM_Switches_AuxSwitch_Update(SwitchState value);

/**
 * Update value stored in RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_WATCHDOG_TIMEOUT in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_TX_OVERFLOW in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WARNING_RX_OVERFLOW in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_FAULT_MISSING_HEARTBEAT in msg DIM_Faults.
 */
void App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Update(bool value);

/**
 * Update value stored in RX table of signal Heartbeat in msg FSM_Vitals.
 */
void App_CanRx_FSM_Vitals_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal State in msg FSM_Vitals.
 */
void App_CanRx_FSM_Vitals_State_Update(State_Machine value);

/**
 * Update value stored in RX table of signal FrontBrakePressure in msg FSM_Brake.
 */
void App_CanRx_FSM_Brake_FrontBrakePressure_Update(uint32_t value);

/**
 * Update value stored in RX table of signal RearBrakePressure in msg FSM_Brake.
 */
void App_CanRx_FSM_Brake_RearBrakePressure_Update(uint32_t value);

/**
 * Update value stored in RX table of signal BrakePedalPercentage in msg FSM_Brake.
 */
void App_CanRx_FSM_Brake_BrakePedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal IsActuated in msg FSM_Brake.
 */
void App_CanRx_FSM_Brake_IsActuated_Update(bool value);

/**
 * Update value stored in RX table of signal PressureSensorOpenShortCircuit in msg FSM_Brake.
 */
void App_CanRx_FSM_Brake_PressureSensorOpenShortCircuit_Update(bool value);

/**
 * Update value stored in RX table of signal PedalOpenShortCircuit in msg FSM_Brake.
 */
void App_CanRx_FSM_Brake_PedalOpenShortCircuit_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_WATCHDOG_TIMEOUT in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_TX_OVERFLOW in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_RX_OVERFLOW in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_BRAKE_ACC_DISAGREEMENT in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_FLOW_RATE_OUT_OF_RANGE in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_MISSING_HEARTBEAT in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_STATE_FAULT in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_APPS_HAS_DISAGREEMENT in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_FLOW_METER_HAS_UNDERFLOW in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal MotorAngle in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(float value);

/**
 * Update value stored in RX table of signal MotorSpeed in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(int value);

/**
 * Update value stored in RX table of signal ElectricalOutputFrequency in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Update(float value);

/**
 * Update value stored in RX table of signal DeltaResolverFiltered in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Update(float value);

/**
 * Update value stored in RX table of signal VsmState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_VsmState_Update(InverterVsmState value);

/**
 * Update value stored in RX table of signal PwmFrequency in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_PwmFrequency_Update(uint32_t value);

/**
 * Update value stored in RX table of signal InverterState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_InverterState_Update(InverterState value);

/**
 * Update value stored in RX table of signal Relay1State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_Relay1State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay2State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_Relay2State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay3State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_Relay3State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay4State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_Relay4State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay5State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_Relay5State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay6State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_Relay6State_Update(bool value);

/**
 * Update value stored in RX table of signal InverterRunMode in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_InverterRunMode_Update(bool value);

/**
 * Update value stored in RX table of signal InverterActiveDischargeState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_InverterActiveDischargeState_Update(InverterActiveDischargeState value);

/**
 * Update value stored in RX table of signal InverterCommandMode in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_InverterCommandMode_Update(InverterCommandMode value);

/**
 * Update value stored in RX table of signal RollingCounter in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_RollingCounter_Update(uint32_t value);

/**
 * Update value stored in RX table of signal InverterEnableState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_InverterEnableState_Update(bool value);

/**
 * Update value stored in RX table of signal StartModeActive in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_StartModeActive_Update(bool value);

/**
 * Update value stored in RX table of signal InverterLockoutEnabled in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_InverterLockoutEnabled_Update(InverterLockout value);

/**
 * Update value stored in RX table of signal DirectionCommand in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_DirectionCommand_Update(InverterDirection value);

/**
 * Update value stored in RX table of signal BmsActive in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_BmsActive_Update(bool value);

/**
 * Update value stored in RX table of signal BmsTorqueLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_BmsTorqueLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal MaxSpeedLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_MaxSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal HotSpotLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_HotSpotLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal LowSpeedLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_LowSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal CoolantTemperatureLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InternalStates_CoolantTemperatureLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal MotorAngle in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_MotorPositionInfo_MotorAngle_Update(float value);

/**
 * Update value stored in RX table of signal MotorSpeed in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(int value);

/**
 * Update value stored in RX table of signal ElectricalOutputFrequency in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Update(float value);

/**
 * Update value stored in RX table of signal DeltaResolverFiltered in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Update(float value);

/**
 * Update value stored in RX table of signal VsmState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_VsmState_Update(InverterVsmState value);

/**
 * Update value stored in RX table of signal PwmFrequency in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_PwmFrequency_Update(uint32_t value);

/**
 * Update value stored in RX table of signal InverterState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_InverterState_Update(InverterState value);

/**
 * Update value stored in RX table of signal Relay1State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_Relay1State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay2State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_Relay2State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay3State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_Relay3State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay4State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_Relay4State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay5State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_Relay5State_Update(bool value);

/**
 * Update value stored in RX table of signal Relay6State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_Relay6State_Update(bool value);

/**
 * Update value stored in RX table of signal InverterRunMode in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_InverterRunMode_Update(bool value);

/**
 * Update value stored in RX table of signal InverterActiveDischargeState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_InverterActiveDischargeState_Update(InverterActiveDischargeState value);

/**
 * Update value stored in RX table of signal InverterCommandMode in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_InverterCommandMode_Update(InverterCommandMode value);

/**
 * Update value stored in RX table of signal RollingCounter in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_RollingCounter_Update(uint32_t value);

/**
 * Update value stored in RX table of signal InverterEnableState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_InverterEnableState_Update(bool value);

/**
 * Update value stored in RX table of signal StartModeActive in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_StartModeActive_Update(bool value);

/**
 * Update value stored in RX table of signal InverterLockoutEnabled in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_InverterLockoutEnabled_Update(InverterLockout value);

/**
 * Update value stored in RX table of signal DirectionCommand in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_DirectionCommand_Update(InverterDirection value);

/**
 * Update value stored in RX table of signal BmsActive in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_BmsActive_Update(bool value);

/**
 * Update value stored in RX table of signal BmsTorqueLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_BmsTorqueLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal MaxSpeedLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_MaxSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal HotSpotLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_HotSpotLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal LowSpeedLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_LowSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal CoolantTemperatureLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InternalStates_CoolantTemperatureLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_WATCHDOG_TIMEOUT in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_TX_OVERFLOW in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WARNING_RX_OVERFLOW in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_FAULT_DUMMY in msg PDM_Faults.
 */
void App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update(bool value);

/**
 * Return value from RX table of signal Heartbeat in msg BMS_Vitals.
 */
bool App_CanRx_BMS_Vitals_Heartbeat_Get(void);

/**
 * Return value from RX table of signal CurrentState in msg BMS_Vitals.
 */
BmsState App_CanRx_BMS_Vitals_CurrentState_Get(void);

/**
 * Return value from RX table of signal StateOfCharge in msg BMS_Vitals.
 */
float App_CanRx_BMS_Vitals_StateOfCharge_Get(void);

/**
 * Return value from RX table of signal AvailablePower in msg BMS_AvailablePower.
 */
float App_CanRx_BMS_AvailablePower_AvailablePower_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_WATCHDOG_TIMEOUT in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_TX_OVERFLOW in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal BMS_WARNING_RX_OVERFLOW in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_MISSING_HEARTBEAT in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_STATE_FAULT in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CELL_UNDERVOLTAGE in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CELL_OVERVOLTAGE in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_MODULE_COMM_ERROR in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CELL_UNDERTEMP in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CELL_OVERTEMP in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CHARGER_FAULT in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_HAS_REACHED_MAX_V in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_TS_OVERCURRENT in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Get(void);

/**
 * Return value from RX table of signal BMS_FAULT_PRECHARGE_ERROR in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Get(void);

/**
 * Return value from RX table of signal EnableDebugMode in msg Debug_CanModes.
 */
bool App_CanRx_Debug_CanModes_EnableDebugMode_Get(void);

/**
 * Return value from RX table of signal Heartbeat in msg DIM_Vitals.
 */
bool App_CanRx_DIM_Vitals_Heartbeat_Get(void);

/**
 * Return value from RX table of signal State in msg DIM_Vitals.
 */
DimState App_CanRx_DIM_Vitals_State_Get(void);

/**
 * Return value from RX table of signal StartSwitch in msg DIM_Switches.
 */
SwitchState App_CanRx_DIM_Switches_StartSwitch_Get(void);

/**
 * Return value from RX table of signal AuxSwitch in msg DIM_Switches.
 */
SwitchState App_CanRx_DIM_Switches_AuxSwitch_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_WATCHDOG_TIMEOUT in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_TX_OVERFLOW in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal DIM_WARNING_RX_OVERFLOW in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal DIM_FAULT_MISSING_HEARTBEAT in msg DIM_Faults.
 */
bool App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Get(void);

/**
 * Return value from RX table of signal Heartbeat in msg FSM_Vitals.
 */
bool App_CanRx_FSM_Vitals_Heartbeat_Get(void);

/**
 * Return value from RX table of signal State in msg FSM_Vitals.
 */
State_Machine App_CanRx_FSM_Vitals_State_Get(void);

/**
 * Return value from RX table of signal FrontBrakePressure in msg FSM_Brake.
 */
uint32_t App_CanRx_FSM_Brake_FrontBrakePressure_Get(void);

/**
 * Return value from RX table of signal RearBrakePressure in msg FSM_Brake.
 */
uint32_t App_CanRx_FSM_Brake_RearBrakePressure_Get(void);

/**
 * Return value from RX table of signal BrakePedalPercentage in msg FSM_Brake.
 */
float App_CanRx_FSM_Brake_BrakePedalPercentage_Get(void);

/**
 * Return value from RX table of signal IsActuated in msg FSM_Brake.
 */
bool App_CanRx_FSM_Brake_IsActuated_Get(void);

/**
 * Return value from RX table of signal PressureSensorOpenShortCircuit in msg FSM_Brake.
 */
bool App_CanRx_FSM_Brake_PressureSensorOpenShortCircuit_Get(void);

/**
 * Return value from RX table of signal PedalOpenShortCircuit in msg FSM_Brake.
 */
bool App_CanRx_FSM_Brake_PedalOpenShortCircuit_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_WATCHDOG_TIMEOUT in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_TX_OVERFLOW in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_RX_OVERFLOW in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_BRAKE_ACC_DISAGREEMENT in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_FLOW_RATE_OUT_OF_RANGE in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_MISSING_HEARTBEAT in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_STATE_FAULT in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_APPS_HAS_DISAGREEMENT in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_FLOW_METER_HAS_UNDERFLOW in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Get(void);

/**
 * Return value from RX table of signal MotorAngle in msg INVL_MotorPositionInfo.
 */
float App_CanRx_INVL_MotorPositionInfo_MotorAngle_Get(void);

/**
 * Return value from RX table of signal MotorSpeed in msg INVL_MotorPositionInfo.
 */
int App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Get(void);

/**
 * Return value from RX table of signal ElectricalOutputFrequency in msg INVL_MotorPositionInfo.
 */
float App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Get(void);

/**
 * Return value from RX table of signal DeltaResolverFiltered in msg INVL_MotorPositionInfo.
 */
float App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Get(void);

/**
 * Return value from RX table of signal VsmState in msg INVL_InternalStates.
 */
InverterVsmState App_CanRx_INVL_InternalStates_VsmState_Get(void);

/**
 * Return value from RX table of signal PwmFrequency in msg INVL_InternalStates.
 */
uint32_t App_CanRx_INVL_InternalStates_PwmFrequency_Get(void);

/**
 * Return value from RX table of signal InverterState in msg INVL_InternalStates.
 */
InverterState App_CanRx_INVL_InternalStates_InverterState_Get(void);

/**
 * Return value from RX table of signal Relay1State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_Relay1State_Get(void);

/**
 * Return value from RX table of signal Relay2State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_Relay2State_Get(void);

/**
 * Return value from RX table of signal Relay3State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_Relay3State_Get(void);

/**
 * Return value from RX table of signal Relay4State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_Relay4State_Get(void);

/**
 * Return value from RX table of signal Relay5State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_Relay5State_Get(void);

/**
 * Return value from RX table of signal Relay6State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_Relay6State_Get(void);

/**
 * Return value from RX table of signal InverterRunMode in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_InverterRunMode_Get(void);

/**
 * Return value from RX table of signal InverterActiveDischargeState in msg INVL_InternalStates.
 */
InverterActiveDischargeState App_CanRx_INVL_InternalStates_InverterActiveDischargeState_Get(void);

/**
 * Return value from RX table of signal InverterCommandMode in msg INVL_InternalStates.
 */
InverterCommandMode App_CanRx_INVL_InternalStates_InverterCommandMode_Get(void);

/**
 * Return value from RX table of signal RollingCounter in msg INVL_InternalStates.
 */
uint32_t App_CanRx_INVL_InternalStates_RollingCounter_Get(void);

/**
 * Return value from RX table of signal InverterEnableState in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_InverterEnableState_Get(void);

/**
 * Return value from RX table of signal StartModeActive in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_StartModeActive_Get(void);

/**
 * Return value from RX table of signal InverterLockoutEnabled in msg INVL_InternalStates.
 */
InverterLockout App_CanRx_INVL_InternalStates_InverterLockoutEnabled_Get(void);

/**
 * Return value from RX table of signal DirectionCommand in msg INVL_InternalStates.
 */
InverterDirection App_CanRx_INVL_InternalStates_DirectionCommand_Get(void);

/**
 * Return value from RX table of signal BmsActive in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_BmsActive_Get(void);

/**
 * Return value from RX table of signal BmsTorqueLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_BmsTorqueLimiting_Get(void);

/**
 * Return value from RX table of signal MaxSpeedLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_MaxSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal HotSpotLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_HotSpotLimiting_Get(void);

/**
 * Return value from RX table of signal LowSpeedLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_LowSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal CoolantTemperatureLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InternalStates_CoolantTemperatureLimiting_Get(void);

/**
 * Return value from RX table of signal MotorAngle in msg INVR_MotorPositionInfo.
 */
float App_CanRx_INVR_MotorPositionInfo_MotorAngle_Get(void);

/**
 * Return value from RX table of signal MotorSpeed in msg INVR_MotorPositionInfo.
 */
int App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get(void);

/**
 * Return value from RX table of signal ElectricalOutputFrequency in msg INVR_MotorPositionInfo.
 */
float App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Get(void);

/**
 * Return value from RX table of signal DeltaResolverFiltered in msg INVR_MotorPositionInfo.
 */
float App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Get(void);

/**
 * Return value from RX table of signal VsmState in msg INVR_InternalStates.
 */
InverterVsmState App_CanRx_INVR_InternalStates_VsmState_Get(void);

/**
 * Return value from RX table of signal PwmFrequency in msg INVR_InternalStates.
 */
uint32_t App_CanRx_INVR_InternalStates_PwmFrequency_Get(void);

/**
 * Return value from RX table of signal InverterState in msg INVR_InternalStates.
 */
InverterState App_CanRx_INVR_InternalStates_InverterState_Get(void);

/**
 * Return value from RX table of signal Relay1State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_Relay1State_Get(void);

/**
 * Return value from RX table of signal Relay2State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_Relay2State_Get(void);

/**
 * Return value from RX table of signal Relay3State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_Relay3State_Get(void);

/**
 * Return value from RX table of signal Relay4State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_Relay4State_Get(void);

/**
 * Return value from RX table of signal Relay5State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_Relay5State_Get(void);

/**
 * Return value from RX table of signal Relay6State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_Relay6State_Get(void);

/**
 * Return value from RX table of signal InverterRunMode in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_InverterRunMode_Get(void);

/**
 * Return value from RX table of signal InverterActiveDischargeState in msg INVR_InternalStates.
 */
InverterActiveDischargeState App_CanRx_INVR_InternalStates_InverterActiveDischargeState_Get(void);

/**
 * Return value from RX table of signal InverterCommandMode in msg INVR_InternalStates.
 */
InverterCommandMode App_CanRx_INVR_InternalStates_InverterCommandMode_Get(void);

/**
 * Return value from RX table of signal RollingCounter in msg INVR_InternalStates.
 */
uint32_t App_CanRx_INVR_InternalStates_RollingCounter_Get(void);

/**
 * Return value from RX table of signal InverterEnableState in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_InverterEnableState_Get(void);

/**
 * Return value from RX table of signal StartModeActive in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_StartModeActive_Get(void);

/**
 * Return value from RX table of signal InverterLockoutEnabled in msg INVR_InternalStates.
 */
InverterLockout App_CanRx_INVR_InternalStates_InverterLockoutEnabled_Get(void);

/**
 * Return value from RX table of signal DirectionCommand in msg INVR_InternalStates.
 */
InverterDirection App_CanRx_INVR_InternalStates_DirectionCommand_Get(void);

/**
 * Return value from RX table of signal BmsActive in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_BmsActive_Get(void);

/**
 * Return value from RX table of signal BmsTorqueLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_BmsTorqueLimiting_Get(void);

/**
 * Return value from RX table of signal MaxSpeedLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_MaxSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal HotSpotLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_HotSpotLimiting_Get(void);

/**
 * Return value from RX table of signal LowSpeedLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_LowSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal CoolantTemperatureLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InternalStates_CoolantTemperatureLimiting_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_WATCHDOG_TIMEOUT in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_TX_OVERFLOW in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal PDM_WARNING_RX_OVERFLOW in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal PDM_FAULT_DUMMY in msg PDM_Faults.
 */
bool App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Get(void);

