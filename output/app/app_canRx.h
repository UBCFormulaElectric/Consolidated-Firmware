/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "app_canUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void app_canRx_init(void);

/**
 * Update value stored in RX table of signal BRUSA_IsConnected in msg BRUSA_Vitals.
 */
void app_canRx_BRUSA_IsConnected_update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTask100Hz_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTask1kHz_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRx_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTx_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_WatchdogTimeout_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_TxOverflow_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_RxOverflow_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheckFault in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_OpenWireCheckFault_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment0_GND in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment0_GND_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment1_GND in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment1_GND_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment2_GND in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment2_GND_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment3_GND in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment3_GND_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment4_GND in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment4_GND_update(bool value);/**
 * Update value stored in RX table of signal BMS_Warning_MissingVCHeartbeat in msg BMS_Warnings.
 */
void app_canRx_BMS_Warning_MissingVCHeartbeat_update(bool value);
/**
 * Update value stored in RX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_StateMachine_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_CellUndervoltage_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_CellOvervoltage_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_ModuleCommunicationError_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_CellUndertemp_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_CellOvertemp_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_ChargerReportedError in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_ChargerReportedError_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_ChargerDisconnectedDuringCharge_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_ChargerShutdownLoopOpen in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_ChargerShutdownLoopOpen_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_TractiveSystemOvercurrent_update(bool value);/**
 * Update value stored in RX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_PrechargeFailure_update(bool value);
/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1HzCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTask1HzCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask100HzCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTask100HzCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1kHzCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTask1kHzCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRxCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTxCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_WatchdogTimeoutCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_WatchdogTimeoutCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_TxOverflowCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_TxOverflowCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_RxOverflowCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_RxOverflowCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheckFaultCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_OpenWireCheckFaultCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment0_GNDCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment1_GNDCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment2_GNDCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment3_GNDCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment4_GNDCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Warning_MissingVCHeartbeatCount in msg BMS_WarningsCounts.
 */
void app_canRx_BMS_Warning_MissingVCHeartbeatCount_update(uint32_t value);
/**
 * Update value stored in RX table of signal BMS_Fault_StateMachineCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_StateMachineCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_CellUndervoltageCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_CellUndervoltageCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_CellOvervoltageCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_CellOvervoltageCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_ModuleCommunicationErrorCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_ModuleCommunicationErrorCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_CellUndertempCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_CellUndertempCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_CellOvertempCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_CellOvertempCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_ChargerReportedErrorCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_ChargerReportedErrorCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_ChargerDisconnectedDuringChargeCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_ChargerDisconnectedDuringChargeCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_ChargerShutdownLoopOpenCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_ChargerShutdownLoopOpenCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_TractiveSystemOvercurrentCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_TractiveSystemOvercurrentCount_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_Fault_PrechargeFailureCount in msg BMS_FaultsCounts.
 */
void app_canRx_BMS_Fault_PrechargeFailureCount_update(uint32_t value);
/**
 * Update value stored in RX table of signal FSM_Warning_WatchdogTimeout in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_WatchdogTimeout_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_TxOverflow in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_TxOverflow_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_RxOverflow in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_RxOverflow_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1Hz in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask100Hz in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTask100Hz_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1kHz in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTask1kHz_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanRx in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRx_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanTx in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTx_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_SteeringAngleOCSC in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_SteeringAngleOCSC_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_SteeringAngleOutOfRange in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_SteeringAngleOutOfRange_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_LeftWheelSpeedOutOfRange in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_LeftWheelSpeedOutOfRange_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_RightWheelSpeedOutOfRange in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_RightWheelSpeedOutOfRange_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_AppsDisagreement in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_AppsDisagreement_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_FrontBrakePressureOutOfRange in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_FrontBrakePressureOutOfRange_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_RearBrakePressureOutOfRange in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_RearBrakePressureOutOfRange_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_FrontBrakePressureOcSc in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_FrontBrakePressureOcSc_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_RearBrakePressureOcSc in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_RearBrakePressureOcSc_update(bool value);/**
 * Update value stored in RX table of signal FSM_Warning_BrakeOcScNotOk in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_BrakeOcScNotOk_update(bool value);
/**
 * Update value stored in RX table of signal FSM_Fault_PappsOCSC in msg FSM_Faults.
 */
void app_canRx_FSM_Fault_PappsOCSC_update(bool value);/**
 * Update value stored in RX table of signal FSM_Fault_SappsOCSC in msg FSM_Faults.
 */
void app_canRx_FSM_Fault_SappsOCSC_update(bool value);
/**
 * Update value stored in RX table of signal FSM_Warning_WatchdogTimeoutCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_WatchdogTimeoutCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_TxOverflowCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_TxOverflowCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_RxOverflowCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_RxOverflowCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1HzCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTask1HzCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask100HzCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTask100HzCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1kHzCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTask1kHzCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanRxCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanTxCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_SteeringAngleOCSCCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_SteeringAngleOCSCCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_SteeringAngleOutOfRangeCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_SteeringAngleOutOfRangeCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_LeftWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_RightWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_RightWheelSpeedOutOfRangeCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_AppsDisagreementCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_AppsDisagreementCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_FrontBrakePressureOutOfRangeCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_FrontBrakePressureOutOfRangeCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_RearBrakePressureOutOfRangeCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_RearBrakePressureOutOfRangeCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_FrontBrakePressureOcScCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_FrontBrakePressureOcScCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_RearBrakePressureOcScCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_RearBrakePressureOcScCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Warning_BrakeOcScNotOkCount in msg FSM_WarningsCounts.
 */
void app_canRx_FSM_Warning_BrakeOcScNotOkCount_update(uint32_t value);
/**
 * Update value stored in RX table of signal FSM_Fault_PappsOCSCCount in msg FSM_FaultsCounts.
 */
void app_canRx_FSM_Fault_PappsOCSCCount_update(uint32_t value);/**
 * Update value stored in RX table of signal FSM_Fault_SappsOCSCCount in msg FSM_FaultsCounts.
 */
void app_canRx_FSM_Fault_SappsOCSCCount_update(uint32_t value);
/**
 * Update value stored in RX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_TractiveSystemVoltage_update(float value);/**
 * Update value stored in RX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_TractiveSystemCurrent_update(float value);/**
 * Update value stored in RX table of signal BMS_TractiveSystemPower in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_TractiveSystemPower_update(float value);/**
 * Update value stored in RX table of signal BMS_AvailablePower in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_AvailablePower_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_BSPDCurrentThresholdExceeded in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_BSPDCurrentThresholdExceeded_update(bool value);
/**
 * Update value stored in RX table of signal BMS_HVDShdnOKStatus in msg BMS_BMSShdnNodeStatus.
 */
void app_canRx_BMS_HVDShdnOKStatus_update(bool value);/**
 * Update value stored in RX table of signal BMS_TSIlckOKStatus in msg BMS_BMSShdnNodeStatus.
 */
void app_canRx_BMS_TSIlckOKStatus_update(bool value);
/**
 * Update value stored in RX table of signal BMS_MinCellTemperature in msg BMS_CellTemperatures.
 */
void app_canRx_BMS_MinCellTemperature_update(float value);/**
 * Update value stored in RX table of signal BMS_MaxCellTemperature in msg BMS_CellTemperatures.
 */
void app_canRx_BMS_MaxCellTemperature_update(float value);
/**
 * Update value stored in RX table of signal BMS_PackVoltage in msg BMS_VoltageAndChargeStats.
 */
void app_canRx_BMS_PackVoltage_update(float value);/**
 * Update value stored in RX table of signal BMS_Soc in msg BMS_VoltageAndChargeStats.
 */
void app_canRx_BMS_Soc_update(float value);/**
 * Update value stored in RX table of signal BMS_SocCorrupt in msg BMS_VoltageAndChargeStats.
 */
void app_canRx_BMS_SocCorrupt_update(bool value);/**
 * Update value stored in RX table of signal BMS_MinCellVoltage in msg BMS_VoltageAndChargeStats.
 */
void app_canRx_BMS_MinCellVoltage_update(float value);/**
 * Update value stored in RX table of signal BMS_MaxCellVoltage in msg BMS_VoltageAndChargeStats.
 */
void app_canRx_BMS_MaxCellVoltage_update(float value);
/**
 * Update value stored in RX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
void app_canRx_BMS_BmsOk_update(bool value);/**
 * Update value stored in RX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
void app_canRx_BMS_ImdOk_update(bool value);/**
 * Update value stored in RX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
void app_canRx_BMS_BspdOk_update(bool value);/**
 * Update value stored in RX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
void app_canRx_BMS_BmsLatchedFault_update(bool value);/**
 * Update value stored in RX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
void app_canRx_BMS_ImdLatchedFault_update(bool value);/**
 * Update value stored in RX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
void app_canRx_BMS_BspdLatchedFault_update(bool value);
/**
 * Update value stored in RX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_TractiveSystemVoltage_update(float value);/**
 * Update value stored in RX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_TractiveSystemCurrent_update(float value);/**
 * Update value stored in RX table of signal BMS_TractiveSystemPower in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_TractiveSystemPower_update(float value);/**
 * Update value stored in RX table of signal BMS_AvailablePower in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_AvailablePower_update(uint32_t value);/**
 * Update value stored in RX table of signal BMS_BSPDCurrentThresholdExceeded in msg BMS_TractiveSystem.
 */
void app_canRx_BMS_BSPDCurrentThresholdExceeded_update(bool value);
/**
 * Update value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void app_canRx_BMS_Heartbeat_update(bool value);/**
 * Update value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
void app_canRx_BMS_State_update(BmsState value);


/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_StackWaterMarkHighTask100Hz_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_StackWaterMarkHighTask1kHz_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRx_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTx_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_WatchdogTimeout_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_TxOverflow_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_RxOverflow_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheckFault in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_OpenWireCheckFault_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment0_GND in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_OpenWireCheck_Segment0_GND_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment1_GND in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_OpenWireCheck_Segment1_GND_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment2_GND in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_OpenWireCheck_Segment2_GND_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment3_GND in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_OpenWireCheck_Segment3_GND_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment4_GND in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_OpenWireCheck_Segment4_GND_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_MissingVCHeartbeat in msg BMS_Warnings.
 */
bool app_canRx_BMS_Warning_MissingVCHeartbeat_get(void);
/**
 * Get value stored in RX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_StateMachine_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_CellUndervoltage_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_CellOvervoltage_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_ModuleCommunicationError_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_CellUndertemp_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_CellOvertemp_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ChargerReportedError in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_ChargerReportedError_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_ChargerDisconnectedDuringCharge_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ChargerShutdownLoopOpen in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_ChargerShutdownLoopOpen_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_TractiveSystemOvercurrent_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_PrechargeFailure_get(void);
/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1HzCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTask1HzCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask100HzCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTask100HzCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1kHzCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTask1kHzCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRxCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTxCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_WatchdogTimeoutCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_WatchdogTimeoutCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_TxOverflowCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_TxOverflowCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_RxOverflowCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_RxOverflowCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheckFaultCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_OpenWireCheckFaultCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment0_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment1_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment2_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment3_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_OpenWireCheck_Segment4_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_get(void);/**
 * Get value stored in RX table of signal BMS_Warning_MissingVCHeartbeatCount in msg BMS_WarningsCounts.
 */
uint32_t app_canRx_BMS_Warning_MissingVCHeartbeatCount_get(void);
/**
 * Get value stored in RX table of signal BMS_Fault_StateMachineCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_StateMachineCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellUndervoltageCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_CellUndervoltageCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellOvervoltageCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_CellOvervoltageCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ModuleCommunicationErrorCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_ModuleCommunicationErrorCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellUndertempCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_CellUndertempCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_CellOvertempCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_CellOvertempCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ChargerReportedErrorCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_ChargerReportedErrorCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ChargerDisconnectedDuringChargeCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_ChargerDisconnectedDuringChargeCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_ChargerShutdownLoopOpenCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_ChargerShutdownLoopOpenCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_TractiveSystemOvercurrentCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_TractiveSystemOvercurrentCount_get(void);/**
 * Get value stored in RX table of signal BMS_Fault_PrechargeFailureCount in msg BMS_FaultsCounts.
 */
uint32_t app_canRx_BMS_Fault_PrechargeFailureCount_get(void);
/**
 * Get value stored in RX table of signal FSM_Warning_WatchdogTimeout in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_WatchdogTimeout_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_TxOverflow in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_TxOverflow_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RxOverflow in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_RxOverflow_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1Hz in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask100Hz in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_StackWaterMarkHighTask100Hz_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1kHz in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_StackWaterMarkHighTask1kHz_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanRx in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRx_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanTx in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTx_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_SteeringAngleOCSC in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_SteeringAngleOCSC_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_SteeringAngleOutOfRange in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_SteeringAngleOutOfRange_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_LeftWheelSpeedOutOfRange in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_LeftWheelSpeedOutOfRange_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RightWheelSpeedOutOfRange in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_RightWheelSpeedOutOfRange_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_AppsDisagreement in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_AppsDisagreement_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_FrontBrakePressureOutOfRange in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_FrontBrakePressureOutOfRange_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RearBrakePressureOutOfRange in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_RearBrakePressureOutOfRange_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_FrontBrakePressureOcSc in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_FrontBrakePressureOcSc_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RearBrakePressureOcSc in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_RearBrakePressureOcSc_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_BrakeOcScNotOk in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_BrakeOcScNotOk_get(void);
/**
 * Get value stored in RX table of signal FSM_Fault_PappsOCSC in msg FSM_Faults.
 */
bool app_canRx_FSM_Fault_PappsOCSC_get(void);/**
 * Get value stored in RX table of signal FSM_Fault_SappsOCSC in msg FSM_Faults.
 */
bool app_canRx_FSM_Fault_SappsOCSC_get(void);
/**
 * Get value stored in RX table of signal FSM_Warning_WatchdogTimeoutCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_WatchdogTimeoutCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_TxOverflowCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_TxOverflowCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RxOverflowCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_RxOverflowCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1HzCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTask1HzCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask100HzCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTask100HzCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1kHzCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTask1kHzCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanRxCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanTxCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_SteeringAngleOCSCCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_SteeringAngleOCSCCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_SteeringAngleOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_SteeringAngleOutOfRangeCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_LeftWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RightWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_RightWheelSpeedOutOfRangeCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_AppsDisagreementCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_AppsDisagreementCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_FrontBrakePressureOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_FrontBrakePressureOutOfRangeCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RearBrakePressureOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_RearBrakePressureOutOfRangeCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_FrontBrakePressureOcScCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_FrontBrakePressureOcScCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_RearBrakePressureOcScCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_RearBrakePressureOcScCount_get(void);/**
 * Get value stored in RX table of signal FSM_Warning_BrakeOcScNotOkCount in msg FSM_WarningsCounts.
 */
uint32_t app_canRx_FSM_Warning_BrakeOcScNotOkCount_get(void);
/**
 * Get value stored in RX table of signal FSM_Fault_PappsOCSCCount in msg FSM_FaultsCounts.
 */
uint32_t app_canRx_FSM_Fault_PappsOCSCCount_get(void);/**
 * Get value stored in RX table of signal FSM_Fault_SappsOCSCCount in msg FSM_FaultsCounts.
 */
uint32_t app_canRx_FSM_Fault_SappsOCSCCount_get(void);
/**
 * Get value stored in RX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
float app_canRx_BMS_TractiveSystemVoltage_get(void);/**
 * Get value stored in RX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
float app_canRx_BMS_TractiveSystemCurrent_get(void);/**
 * Get value stored in RX table of signal BMS_TractiveSystemPower in msg BMS_TractiveSystem.
 */
float app_canRx_BMS_TractiveSystemPower_get(void);/**
 * Get value stored in RX table of signal BMS_AvailablePower in msg BMS_TractiveSystem.
 */
uint32_t app_canRx_BMS_AvailablePower_get(void);/**
 * Get value stored in RX table of signal BMS_BSPDCurrentThresholdExceeded in msg BMS_TractiveSystem.
 */
bool app_canRx_BMS_BSPDCurrentThresholdExceeded_get(void);
/**
 * Get value stored in RX table of signal BMS_HVDShdnOKStatus in msg BMS_BMSShdnNodeStatus.
 */
bool app_canRx_BMS_HVDShdnOKStatus_get(void);/**
 * Get value stored in RX table of signal BMS_TSIlckOKStatus in msg BMS_BMSShdnNodeStatus.
 */
bool app_canRx_BMS_TSIlckOKStatus_get(void);
/**
 * Get value stored in RX table of signal BMS_MinCellTemperature in msg BMS_CellTemperatures.
 */
float app_canRx_BMS_MinCellTemperature_get(void);/**
 * Get value stored in RX table of signal BMS_MaxCellTemperature in msg BMS_CellTemperatures.
 */
float app_canRx_BMS_MaxCellTemperature_get(void);
/**
 * Get value stored in RX table of signal BMS_PackVoltage in msg BMS_VoltageAndChargeStats.
 */
float app_canRx_BMS_PackVoltage_get(void);/**
 * Get value stored in RX table of signal BMS_Soc in msg BMS_VoltageAndChargeStats.
 */
float app_canRx_BMS_Soc_get(void);/**
 * Get value stored in RX table of signal BMS_SocCorrupt in msg BMS_VoltageAndChargeStats.
 */
bool app_canRx_BMS_SocCorrupt_get(void);/**
 * Get value stored in RX table of signal BMS_MinCellVoltage in msg BMS_VoltageAndChargeStats.
 */
float app_canRx_BMS_MinCellVoltage_get(void);/**
 * Get value stored in RX table of signal BMS_MaxCellVoltage in msg BMS_VoltageAndChargeStats.
 */
float app_canRx_BMS_MaxCellVoltage_get(void);
/**
 * Get value stored in RX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
bool app_canRx_BMS_BmsOk_get(void);/**
 * Get value stored in RX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
bool app_canRx_BMS_ImdOk_get(void);/**
 * Get value stored in RX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
bool app_canRx_BMS_BspdOk_get(void);/**
 * Get value stored in RX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
bool app_canRx_BMS_BmsLatchedFault_get(void);/**
 * Get value stored in RX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
bool app_canRx_BMS_ImdLatchedFault_get(void);/**
 * Get value stored in RX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
bool app_canRx_BMS_BspdLatchedFault_get(void);
/**
 * Get value stored in RX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
float app_canRx_BMS_TractiveSystemVoltage_get(void);/**
 * Get value stored in RX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
float app_canRx_BMS_TractiveSystemCurrent_get(void);/**
 * Get value stored in RX table of signal BMS_TractiveSystemPower in msg BMS_TractiveSystem.
 */
float app_canRx_BMS_TractiveSystemPower_get(void);/**
 * Get value stored in RX table of signal BMS_AvailablePower in msg BMS_TractiveSystem.
 */
uint32_t app_canRx_BMS_AvailablePower_get(void);/**
 * Get value stored in RX table of signal BMS_BSPDCurrentThresholdExceeded in msg BMS_TractiveSystem.
 */
bool app_canRx_BMS_BSPDCurrentThresholdExceeded_get(void);
/**
 * Get value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool app_canRx_BMS_Heartbeat_get(void);/**
 * Get value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState app_canRx_BMS_State_get(void);
