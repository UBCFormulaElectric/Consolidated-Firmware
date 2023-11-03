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
 * Update value stored in RX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
void App_CanRx_DCM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_State in msg DCM_Vitals.
 */
void App_CanRx_DCM_State_Update(DcmState value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterTorqueCommand in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterTorqueCommand_Update(float value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterSpeedCommand in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterSpeedCommand_Update(int value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterDirectionCommand in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterDirectionCommand_Update(InverterDirection value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterEnable in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterEnable_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterEnableDischarge in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterEnableDischarge_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterEnableSpeedMode in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterEnableSpeedMode_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterTorqueLimit in msg DCM_LeftInverterCommand.
 */
void App_CanRx_DCM_LeftInverterTorqueLimit_Update(float value);

/**
 * Update value stored in RX table of signal DCM_RightInverterTorqueCommand in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterTorqueCommand_Update(float value);

/**
 * Update value stored in RX table of signal DCM_RightInverterSpeedCommand in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterSpeedCommand_Update(int value);

/**
 * Update value stored in RX table of signal DCM_RightInverterDirectionCommand in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterDirectionCommand_Update(InverterDirection value);

/**
 * Update value stored in RX table of signal DCM_RightInverterEnable in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterEnable_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_RightInverterEnableDischarge in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterEnableDischarge_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_RightInverterEnableSpeedMode in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterEnableSpeedMode_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_RightInverterTorqueLimit in msg DCM_RightInverterCommand.
 */
void App_CanRx_DCM_RightInverterTorqueLimit_Update(float value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTask1Hz in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTask100Hz in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTask1kHz in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTaskCanRx in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTaskCanTx in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_WatchdogTimeout in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_TxOverflow in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_RxOverflow in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Fault_MissingHeartbeat in msg DCM_Faults.
 */
void App_CanRx_DCM_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Fault_LeftInverterFault in msg DCM_Faults.
 */
void App_CanRx_DCM_Fault_LeftInverterFault_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Fault_RightInverterFault in msg DCM_Faults.
 */
void App_CanRx_DCM_Fault_RightInverterFault_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTask1Hz in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTask100Hz in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTask1kHz in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTaskCanRx in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTaskCanTx in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_WatchdogTimeout in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_TxOverflow in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_RxOverflow in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Fault_MissingHeartbeat in msg DIM_Faults.
 */
void App_CanRx_DIM_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void App_CanRx_BMS_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
void App_CanRx_BMS_State_Update(BmsState value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_MissingHeartbeat in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_StateMachine_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellUndervoltage_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellOvervoltage_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_ModuleCommunicationError_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellUndertemp_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellOvertemp_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_Charger in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_Charger_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_ChargerExternalShutdown in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_PrechargeFailure_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
void App_CanRx_Debug_EnableDebugMode_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTask1Hz in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTask100Hz in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTask1kHz in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTaskCanRx in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTaskCanTx in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_WatchdogTimeout in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_TxOverflow in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_RxOverflow in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Fault_DummyFault in msg PDM_Faults.
 */
void App_CanRx_PDM_Fault_DummyFault_Update(bool value);

/**
 * Return value from RX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
bool App_CanRx_DCM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal DCM_State in msg DCM_Vitals.
 */
DcmState App_CanRx_DCM_State_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterTorqueCommand in msg DCM_LeftInverterCommand.
 */
float App_CanRx_DCM_LeftInverterTorqueCommand_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterSpeedCommand in msg DCM_LeftInverterCommand.
 */
int App_CanRx_DCM_LeftInverterSpeedCommand_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterDirectionCommand in msg DCM_LeftInverterCommand.
 */
InverterDirection App_CanRx_DCM_LeftInverterDirectionCommand_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterEnable in msg DCM_LeftInverterCommand.
 */
bool App_CanRx_DCM_LeftInverterEnable_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterEnableDischarge in msg DCM_LeftInverterCommand.
 */
bool App_CanRx_DCM_LeftInverterEnableDischarge_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterEnableSpeedMode in msg DCM_LeftInverterCommand.
 */
bool App_CanRx_DCM_LeftInverterEnableSpeedMode_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterTorqueLimit in msg DCM_LeftInverterCommand.
 */
float App_CanRx_DCM_LeftInverterTorqueLimit_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterTorqueCommand in msg DCM_RightInverterCommand.
 */
float App_CanRx_DCM_RightInverterTorqueCommand_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterSpeedCommand in msg DCM_RightInverterCommand.
 */
int App_CanRx_DCM_RightInverterSpeedCommand_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterDirectionCommand in msg DCM_RightInverterCommand.
 */
InverterDirection App_CanRx_DCM_RightInverterDirectionCommand_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterEnable in msg DCM_RightInverterCommand.
 */
bool App_CanRx_DCM_RightInverterEnable_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterEnableDischarge in msg DCM_RightInverterCommand.
 */
bool App_CanRx_DCM_RightInverterEnableDischarge_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterEnableSpeedMode in msg DCM_RightInverterCommand.
 */
bool App_CanRx_DCM_RightInverterEnableSpeedMode_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterTorqueLimit in msg DCM_RightInverterCommand.
 */
float App_CanRx_DCM_RightInverterTorqueLimit_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTask1Hz in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTask100Hz in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTask1kHz in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTaskCanRx in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTaskCanTx in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_WatchdogTimeout in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_TxOverflow in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_RxOverflow in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal DCM_Fault_MissingHeartbeat in msg DCM_Faults.
 */
bool App_CanRx_DCM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal DCM_Fault_LeftInverterFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_Fault_LeftInverterFault_Get(void);

/**
 * Return value from RX table of signal DCM_Fault_RightInverterFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_Fault_RightInverterFault_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTask1Hz in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTask100Hz in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTask1kHz in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTaskCanRx in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTaskCanTx in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_WatchdogTimeout in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_TxOverflow in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_RxOverflow in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal DIM_Fault_MissingHeartbeat in msg DIM_Faults.
 */
bool App_CanRx_DIM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool App_CanRx_BMS_Heartbeat_Get(void);

/**
 * Return value from RX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState App_CanRx_BMS_State_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_MissingHeartbeat in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_StateMachine_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellUndervoltage_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellOvervoltage_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_ModuleCommunicationError_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellUndertemp_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellOvertemp_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_Charger in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_Charger_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_ChargerExternalShutdown in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_ChargerExternalShutdown_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_PrechargeFailure_Get(void);

/**
 * Return value from RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
bool App_CanRx_Debug_EnableDebugMode_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTask1Hz in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTask100Hz in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTask1kHz in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTaskCanRx in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTaskCanTx in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_WatchdogTimeout in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_TxOverflow in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_RxOverflow in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal PDM_Fault_DummyFault in msg PDM_Faults.
 */
bool App_CanRx_PDM_Fault_DummyFault_Get(void);

