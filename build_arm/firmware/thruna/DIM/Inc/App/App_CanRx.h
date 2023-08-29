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
 * Update value stored in RX table of signal EnableDebugMode in msg Debug_CanModes.
 */
void App_CanRx_Debug_CanModes_EnableDebugMode_Update(bool value);

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
 * Update value stored in RX table of signal BmsOk in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_OkStatuses_BmsOk_Update(bool value);

/**
 * Update value stored in RX table of signal ImdOk in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_OkStatuses_ImdOk_Update(bool value);

/**
 * Update value stored in RX table of signal BspdOk in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_OkStatuses_BspdOk_Update(bool value);

/**
 * Update value stored in RX table of signal ImdLatchedFaultStatus in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(bool value);

/**
 * Update value stored in RX table of signal BspdLatchedFaultStatus in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(bool value);

/**
 * Update value stored in RX table of signal AirPositive in msg BMS_Contactors.
 */
void App_CanRx_BMS_Contactors_AirPositive_Update(ContactorState value);

/**
 * Update value stored in RX table of signal AirNegative in msg BMS_Contactors.
 */
void App_CanRx_BMS_Contactors_AirNegative_Update(ContactorState value);

/**
 * Update value stored in RX table of signal PrechargeRelay in msg BMS_Contactors.
 */
void App_CanRx_BMS_Contactors_PrechargeRelay_Update(ContactorState value);

/**
 * Update value stored in RX table of signal MinCellVoltage in msg BMS_CellVoltages.
 */
void App_CanRx_BMS_CellVoltages_MinCellVoltage_Update(float value);

/**
 * Update value stored in RX table of signal MaxCellVoltage in msg BMS_CellVoltages.
 */
void App_CanRx_BMS_CellVoltages_MaxCellVoltage_Update(float value);

/**
 * Update value stored in RX table of signal TsVoltage in msg BMS_TractiveSystem.
 */
void App_CanRx_BMS_TractiveSystem_TsVoltage_Update(float value);

/**
 * Update value stored in RX table of signal TsCurrent in msg BMS_TractiveSystem.
 */
void App_CanRx_BMS_TractiveSystem_TsCurrent_Update(float value);

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
 * Update value stored in RX table of signal BMS_FAULT_LATCHED_BMS_FAULT in msg BMS_Faults.
 */
void App_CanRx_BMS_Faults_BMS_FAULT_LATCHED_BMS_FAULT_Update(bool value);

/**
 * Update value stored in RX table of signal Heartbeat in msg DCM_Vitals.
 */
void App_CanRx_DCM_Vitals_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal CurrentState in msg DCM_Vitals.
 */
void App_CanRx_DCM_Vitals_CurrentState_Update(DcmState value);

/**
 * Update value stored in RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_WATCHDOG_TIMEOUT in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_TX_OVERFLOW in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WARNING_RX_OVERFLOW in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_FAULT_STATE_FAULT in msg DCM_Faults.
 */
void App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_FAULT_MISSING_HEARTBEAT in msg DCM_Faults.
 */
void App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(bool value);

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
 * Update value stored in RX table of signal FSM_WARNING_BRAKE_ACC_DISAGREEMENT in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WARNING_APPS_HAS_DISAGREEMENT in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_WARNING_APPS_HAS_DISAGREEMENT_Update(bool value);

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
 * Update value stored in RX table of signal FSM_FAULT_FLOW_METER_HAS_UNDERFLOW in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_TORQUE_PLAUSABILITY_FAILED in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_Update(bool value);

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
 * Update value stored in RX table of signal ModuleATemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_Temperatures1_ModuleATemperature_Update(float value);

/**
 * Update value stored in RX table of signal ModuleBTemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_Temperatures1_ModuleBTemperature_Update(float value);

/**
 * Update value stored in RX table of signal ModuleCTemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_Temperatures1_ModuleCTemperature_Update(float value);

/**
 * Update value stored in RX table of signal GateDriverBoardTemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Update(float value);

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
 * Return value from RX table of signal EnableDebugMode in msg Debug_CanModes.
 */
bool App_CanRx_Debug_CanModes_EnableDebugMode_Get(void);

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
 * Return value from RX table of signal BmsOk in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_OkStatuses_BmsOk_Get(void);

/**
 * Return value from RX table of signal ImdOk in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_OkStatuses_ImdOk_Get(void);

/**
 * Return value from RX table of signal BspdOk in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_OkStatuses_BspdOk_Get(void);

/**
 * Return value from RX table of signal ImdLatchedFaultStatus in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Get(void);

/**
 * Return value from RX table of signal BspdLatchedFaultStatus in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Get(void);

/**
 * Return value from RX table of signal AirPositive in msg BMS_Contactors.
 */
ContactorState App_CanRx_BMS_Contactors_AirPositive_Get(void);

/**
 * Return value from RX table of signal AirNegative in msg BMS_Contactors.
 */
ContactorState App_CanRx_BMS_Contactors_AirNegative_Get(void);

/**
 * Return value from RX table of signal PrechargeRelay in msg BMS_Contactors.
 */
ContactorState App_CanRx_BMS_Contactors_PrechargeRelay_Get(void);

/**
 * Return value from RX table of signal MinCellVoltage in msg BMS_CellVoltages.
 */
float App_CanRx_BMS_CellVoltages_MinCellVoltage_Get(void);

/**
 * Return value from RX table of signal MaxCellVoltage in msg BMS_CellVoltages.
 */
float App_CanRx_BMS_CellVoltages_MaxCellVoltage_Get(void);

/**
 * Return value from RX table of signal TsVoltage in msg BMS_TractiveSystem.
 */
float App_CanRx_BMS_TractiveSystem_TsVoltage_Get(void);

/**
 * Return value from RX table of signal TsCurrent in msg BMS_TractiveSystem.
 */
float App_CanRx_BMS_TractiveSystem_TsCurrent_Get(void);

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
 * Return value from RX table of signal BMS_FAULT_LATCHED_BMS_FAULT in msg BMS_Faults.
 */
bool App_CanRx_BMS_Faults_BMS_FAULT_LATCHED_BMS_FAULT_Get(void);

/**
 * Return value from RX table of signal Heartbeat in msg DCM_Vitals.
 */
bool App_CanRx_DCM_Vitals_Heartbeat_Get(void);

/**
 * Return value from RX table of signal CurrentState in msg DCM_Vitals.
 */
DcmState App_CanRx_DCM_Vitals_CurrentState_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_WATCHDOG_TIMEOUT in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_TX_OVERFLOW in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal DCM_WARNING_RX_OVERFLOW in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from RX table of signal DCM_FAULT_STATE_FAULT in msg DCM_Faults.
 */
bool App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Get(void);

/**
 * Return value from RX table of signal DCM_FAULT_MISSING_HEARTBEAT in msg DCM_Faults.
 */
bool App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Get(void);

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
 * Return value from RX table of signal FSM_WARNING_BRAKE_ACC_DISAGREEMENT in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Get(void);

/**
 * Return value from RX table of signal FSM_WARNING_APPS_HAS_DISAGREEMENT in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_WARNING_APPS_HAS_DISAGREEMENT_Get(void);

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
 * Return value from RX table of signal FSM_FAULT_FLOW_METER_HAS_UNDERFLOW in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_TORQUE_PLAUSABILITY_FAILED in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_Get(void);

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

/**
 * Return value from RX table of signal ModuleATemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_Temperatures1_ModuleATemperature_Get(void);

/**
 * Return value from RX table of signal ModuleBTemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_Temperatures1_ModuleBTemperature_Get(void);

/**
 * Return value from RX table of signal ModuleCTemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_Temperatures1_ModuleCTemperature_Get(void);

/**
 * Return value from RX table of signal GateDriverBoardTemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Get(void);

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

