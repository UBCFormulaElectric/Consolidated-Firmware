/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "app_canUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize TX signals to their starting values.
 */
void app_canTx_init(void);



/**
 * Update value stored in TX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void app_canTx_BMS_Heartbeat_set(bool value);
/**
 * Update value stored in TX table of signal BMS_State in msg BMS_Vitals.
 */
void app_canTx_BMS_State_set(BmsState value);
/**
 * Update value stored in TX table of signal BMS_TxOverflowCount in msg BMS_AlertsContext.
 */
void app_canTx_BMS_TxOverflowCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_RxOverflowCount in msg BMS_AlertsContext.
 */
void app_canTx_BMS_RxOverflowCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_WatchdogTimeoutTaskName in msg BMS_AlertsContext.
 */
void app_canTx_BMS_WatchdogTimeoutTaskName_set(RtosTaskName value);
/**
 * Update value stored in TX table of signal BMS_ModuleCommunication_NumCommTries in msg BMS_AlertsContext.
 */
void app_canTx_BMS_ModuleCommunication_NumCommTries_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_ModuleCommunication_MonitorState in msg BMS_AlertsContext.
 */
void app_canTx_BMS_ModuleCommunication_MonitorState_set(CAN_AccumulatorMonitorState value);
/**
 * Update value stored in TX table of signal BMS_ImdOkHs in msg BMS_ImdStatus.
 */
void app_canTx_BMS_ImdOkHs_set(bool value);
/**
 * Update value stored in TX table of signal BMS_ImdTimeSincePowerOn in msg BMS_ImdStatus.
 */
void app_canTx_BMS_ImdTimeSincePowerOn_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_ImdValidDutyCycle in msg BMS_ImdStatus.
 */
void app_canTx_BMS_ImdValidDutyCycle_set(bool value);
/**
 * Update value stored in TX table of signal BMS_ImdCondition in msg BMS_ImdStatus.
 */
void app_canTx_BMS_ImdCondition_set(ImdConditionName value);
/**
 * Update value stored in TX table of signal BMS_ImdDutyCycle in msg BMS_ImdStatus.
 */
void app_canTx_BMS_ImdDutyCycle_set(float value);
/**
 * Update value stored in TX table of signal BMS_ImdFrequency in msg BMS_ImdStatus.
 */
void app_canTx_BMS_ImdFrequency_set(float value);
/**
 * Update value stored in TX table of signal BMS_ImdActiveFrequency in msg BMS_ImdData.
 */
void app_canTx_BMS_ImdActiveFrequency_set(ImdActiveFrequency value);
/**
 * Update value stored in TX table of signal BMS_ImdInsulationMeasurementDcp10Hz in msg BMS_ImdData.
 */
void app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_set(float value);
/**
 * Update value stored in TX table of signal BMS_ImdInsulationMeasurementDcp20Hz in msg BMS_ImdData.
 */
void app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_set(float value);
/**
 * Update value stored in TX table of signal BMS_ImdSpeedStartStatus30Hz in msg BMS_ImdData.
 */
void app_canTx_BMS_ImdSpeedStartStatus30Hz_set(float value);
/**
 * Update value stored in TX table of signal BMS_ChargerConnected in msg BMS_Charger.
 */
void app_canTx_BMS_ChargerConnected_set(bool value);
/**
 * Update value stored in TX table of signal BMS_ChargerEnable in msg BMS_BrusaControls.
 */
void app_canTx_BMS_ChargerEnable_set(bool value);
/**
 * Update value stored in TX table of signal BMS_ClearLatch in msg BMS_BrusaControls.
 */
void app_canTx_BMS_ClearLatch_set(bool value);
/**
 * Update value stored in TX table of signal BMS_MaxMainsCurrent in msg BMS_BrusaControls.
 */
void app_canTx_BMS_MaxMainsCurrent_set(float value);
/**
 * Update value stored in TX table of signal BMS_ChargingVoltage in msg BMS_BrusaControls.
 */
void app_canTx_BMS_ChargingVoltage_set(float value);
/**
 * Update value stored in TX table of signal BMS_ChargingCurrent in msg BMS_BrusaControls.
 */
void app_canTx_BMS_ChargingCurrent_set(float value);
/**
 * Update value stored in TX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
void app_canTx_BMS_BmsOk_set(bool value);
/**
 * Update value stored in TX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
void app_canTx_BMS_ImdOk_set(bool value);
/**
 * Update value stored in TX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
void app_canTx_BMS_BspdOk_set(bool value);
/**
 * Update value stored in TX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
void app_canTx_BMS_BmsLatchedFault_set(bool value);
/**
 * Update value stored in TX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
void app_canTx_BMS_ImdLatchedFault_set(bool value);
/**
 * Update value stored in TX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
void app_canTx_BMS_BspdLatchedFault_set(bool value);
/**
 * Update value stored in TX table of signal BMS_AirPositive in msg BMS_Contactors.
 */
void app_canTx_BMS_AirPositive_set(ContactorState value);
/**
 * Update value stored in TX table of signal BMS_AirNegative in msg BMS_Contactors.
 */
void app_canTx_BMS_AirNegative_set(ContactorState value);
/**
 * Update value stored in TX table of signal BMS_PrechargeRelay in msg BMS_Contactors.
 */
void app_canTx_BMS_PrechargeRelay_set(ContactorState value);
/**
 * Update value stored in TX table of signal BMS_MinCellTemperature in msg BMS_CellTemperatures.
 */
void app_canTx_BMS_MinCellTemperature_set(float value);
/**
 * Update value stored in TX table of signal BMS_MaxCellTemperature in msg BMS_CellTemperatures.
 */
void app_canTx_BMS_MaxCellTemperature_set(float value);
/**
 * Update value stored in TX table of signal BMS_PackVoltage in msg BMS_VoltageAndChargeStats.
 */
void app_canTx_BMS_PackVoltage_set(float value);
/**
 * Update value stored in TX table of signal BMS_Soc in msg BMS_VoltageAndChargeStats.
 */
void app_canTx_BMS_Soc_set(float value);
/**
 * Update value stored in TX table of signal BMS_SocCorrupt in msg BMS_VoltageAndChargeStats.
 */
void app_canTx_BMS_SocCorrupt_set(bool value);
/**
 * Update value stored in TX table of signal BMS_MinCellVoltage in msg BMS_VoltageAndChargeStats.
 */
void app_canTx_BMS_MinCellVoltage_set(float value);
/**
 * Update value stored in TX table of signal BMS_MaxCellVoltage in msg BMS_VoltageAndChargeStats.
 */
void app_canTx_BMS_MaxCellVoltage_set(float value);
/**
 * Update value stored in TX table of signal BMS_MinCellVoltageSegment in msg BMS_CellStats.
 */
void app_canTx_BMS_MinCellVoltageSegment_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MinCellVoltageIdx in msg BMS_CellStats.
 */
void app_canTx_BMS_MinCellVoltageIdx_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MaxCellVoltageSegment in msg BMS_CellStats.
 */
void app_canTx_BMS_MaxCellVoltageSegment_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MaxCellVoltageIdx in msg BMS_CellStats.
 */
void app_canTx_BMS_MaxCellVoltageIdx_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MinTempSegment in msg BMS_CellStats.
 */
void app_canTx_BMS_MinTempSegment_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MinTempIdx in msg BMS_CellStats.
 */
void app_canTx_BMS_MinTempIdx_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MaxTempSegment in msg BMS_CellStats.
 */
void app_canTx_BMS_MaxTempSegment_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_MaxTempIdx in msg BMS_CellStats.
 */
void app_canTx_BMS_MaxTempIdx_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
void app_canTx_BMS_TractiveSystemVoltage_set(float value);
/**
 * Update value stored in TX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
void app_canTx_BMS_TractiveSystemCurrent_set(float value);
/**
 * Update value stored in TX table of signal BMS_TractiveSystemPower in msg BMS_TractiveSystem.
 */
void app_canTx_BMS_TractiveSystemPower_set(float value);
/**
 * Update value stored in TX table of signal BMS_AvailablePower in msg BMS_TractiveSystem.
 */
void app_canTx_BMS_AvailablePower_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_BSPDCurrentThresholdExceeded in msg BMS_TractiveSystem.
 */
void app_canTx_BMS_BSPDCurrentThresholdExceeded_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Hash in msg BMS_CommitInfo.
 */
void app_canTx_BMS_Hash_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Clean in msg BMS_CommitInfo.
 */
void app_canTx_BMS_Clean_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Segment0_OWC_Cells_Status in msg BMS_OWC_Segment0to2_Status.
 */
void app_canTx_BMS_Segment0_OWC_Cells_Status_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Segment1_OWC_Cells_Status in msg BMS_OWC_Segment0to2_Status.
 */
void app_canTx_BMS_Segment1_OWC_Cells_Status_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Segment2_OWC_Cells_Status in msg BMS_OWC_Segment0to2_Status.
 */
void app_canTx_BMS_Segment2_OWC_Cells_Status_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Segment3_OWC_Cells_Status in msg BMS_OWC_Segment3to4_Status.
 */
void app_canTx_BMS_Segment3_OWC_Cells_Status_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Segment4_OWC_Cells_Status in msg BMS_OWC_Segment3to4_Status.
 */
void app_canTx_BMS_Segment4_OWC_Cells_Status_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_HVDShdnOKStatus in msg BMS_BMSShdnNodeStatus.
 */
void app_canTx_BMS_HVDShdnOKStatus_set(bool value);
/**
 * Update value stored in TX table of signal BMS_TSIlckOKStatus in msg BMS_BMSShdnNodeStatus.
 */
void app_canTx_BMS_TSIlckOKStatus_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_WatchdogTimeout_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_TxOverflow_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_RxOverflow_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheckFault in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_OpenWireCheckFault_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment0_GND in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment1_GND in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment2_GND in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment3_GND in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment4_GND in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_MissingVCHeartbeat in msg BMS_Warnings.
 */
void app_canTx_BMS_Warning_MissingVCHeartbeat_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_StateMachine_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_CellUndervoltage_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_CellOvervoltage_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_ModuleCommunicationError_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_CellUndertemp_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_CellOvertemp_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_ChargerReportedError in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_ChargerReportedError_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_ChargerShutdownLoopOpen in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_ChargerShutdownLoopOpen_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_TractiveSystemOvercurrent_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
void app_canTx_BMS_Fault_PrechargeFailure_set(bool value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1HzCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTask1HzCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask100HzCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTask100HzCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1kHzCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRxCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTxCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_WatchdogTimeoutCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_WatchdogTimeoutCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_TxOverflowCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_TxOverflowCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_RxOverflowCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_RxOverflowCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheckFaultCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_OpenWireCheckFaultCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment0_GNDCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment1_GNDCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment2_GNDCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment3_GNDCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment4_GNDCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Warning_MissingVCHeartbeatCount in msg BMS_WarningsCounts.
 */
void app_canTx_BMS_Warning_MissingVCHeartbeatCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_StateMachineCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_StateMachineCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellUndervoltageCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_CellUndervoltageCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellOvervoltageCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_CellOvervoltageCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_ModuleCommunicationErrorCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_ModuleCommunicationErrorCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellUndertempCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_CellUndertempCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_CellOvertempCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_CellOvertempCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_ChargerReportedErrorCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_ChargerReportedErrorCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_ChargerDisconnectedDuringChargeCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_ChargerShutdownLoopOpenCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_ChargerShutdownLoopOpenCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_TractiveSystemOvercurrentCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_TractiveSystemOvercurrentCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal BMS_Fault_PrechargeFailureCount in msg BMS_FaultsCounts.
 */
void app_canTx_BMS_Fault_PrechargeFailureCount_set(uint32_t value);


/**
 * Get value stored in TX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool app_canTx_BMS_Vitals_BMS_Heartbeat_get(void);
/**
 * Get value stored in TX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState app_canTx_BMS_Vitals_BMS_State_get(void);
/**
 * Get value stored in TX table of signal BMS_TxOverflowCount in msg BMS_AlertsContext.
 */
uint32_t app_canTx_BMS_AlertsContext_BMS_TxOverflowCount_get(void);
/**
 * Get value stored in TX table of signal BMS_RxOverflowCount in msg BMS_AlertsContext.
 */
uint32_t app_canTx_BMS_AlertsContext_BMS_RxOverflowCount_get(void);
/**
 * Get value stored in TX table of signal BMS_WatchdogTimeoutTaskName in msg BMS_AlertsContext.
 */
RtosTaskName app_canTx_BMS_AlertsContext_BMS_WatchdogTimeoutTaskName_get(void);
/**
 * Get value stored in TX table of signal BMS_ModuleCommunication_NumCommTries in msg BMS_AlertsContext.
 */
uint32_t app_canTx_BMS_AlertsContext_BMS_ModuleCommunication_NumCommTries_get(void);
/**
 * Get value stored in TX table of signal BMS_ModuleCommunication_MonitorState in msg BMS_AlertsContext.
 */
CAN_AccumulatorMonitorState app_canTx_BMS_AlertsContext_BMS_ModuleCommunication_MonitorState_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdOkHs in msg BMS_ImdStatus.
 */
bool app_canTx_BMS_ImdStatus_BMS_ImdOkHs_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdTimeSincePowerOn in msg BMS_ImdStatus.
 */
uint32_t app_canTx_BMS_ImdStatus_BMS_ImdTimeSincePowerOn_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdValidDutyCycle in msg BMS_ImdStatus.
 */
bool app_canTx_BMS_ImdStatus_BMS_ImdValidDutyCycle_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdCondition in msg BMS_ImdStatus.
 */
ImdConditionName app_canTx_BMS_ImdStatus_BMS_ImdCondition_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdDutyCycle in msg BMS_ImdStatus.
 */
float app_canTx_BMS_ImdStatus_BMS_ImdDutyCycle_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdFrequency in msg BMS_ImdStatus.
 */
float app_canTx_BMS_ImdStatus_BMS_ImdFrequency_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdActiveFrequency in msg BMS_ImdData.
 */
ImdActiveFrequency app_canTx_BMS_ImdData_BMS_ImdActiveFrequency_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdInsulationMeasurementDcp10Hz in msg BMS_ImdData.
 */
float app_canTx_BMS_ImdData_BMS_ImdInsulationMeasurementDcp10Hz_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdInsulationMeasurementDcp20Hz in msg BMS_ImdData.
 */
float app_canTx_BMS_ImdData_BMS_ImdInsulationMeasurementDcp20Hz_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdSpeedStartStatus30Hz in msg BMS_ImdData.
 */
float app_canTx_BMS_ImdData_BMS_ImdSpeedStartStatus30Hz_get(void);
/**
 * Get value stored in TX table of signal BMS_ChargerConnected in msg BMS_Charger.
 */
bool app_canTx_BMS_Charger_BMS_ChargerConnected_get(void);
/**
 * Get value stored in TX table of signal BMS_ChargerEnable in msg BMS_BrusaControls.
 */
bool app_canTx_BMS_BrusaControls_BMS_ChargerEnable_get(void);
/**
 * Get value stored in TX table of signal BMS_ClearLatch in msg BMS_BrusaControls.
 */
bool app_canTx_BMS_BrusaControls_BMS_ClearLatch_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxMainsCurrent in msg BMS_BrusaControls.
 */
float app_canTx_BMS_BrusaControls_BMS_MaxMainsCurrent_get(void);
/**
 * Get value stored in TX table of signal BMS_ChargingVoltage in msg BMS_BrusaControls.
 */
float app_canTx_BMS_BrusaControls_BMS_ChargingVoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_ChargingCurrent in msg BMS_BrusaControls.
 */
float app_canTx_BMS_BrusaControls_BMS_ChargingCurrent_get(void);
/**
 * Get value stored in TX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
bool app_canTx_BMS_OkStatuses_BMS_BmsOk_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
bool app_canTx_BMS_OkStatuses_BMS_ImdOk_get(void);
/**
 * Get value stored in TX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
bool app_canTx_BMS_OkStatuses_BMS_BspdOk_get(void);
/**
 * Get value stored in TX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
bool app_canTx_BMS_OkStatuses_BMS_BmsLatchedFault_get(void);
/**
 * Get value stored in TX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
bool app_canTx_BMS_OkStatuses_BMS_ImdLatchedFault_get(void);
/**
 * Get value stored in TX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
bool app_canTx_BMS_OkStatuses_BMS_BspdLatchedFault_get(void);
/**
 * Get value stored in TX table of signal BMS_AirPositive in msg BMS_Contactors.
 */
ContactorState app_canTx_BMS_Contactors_BMS_AirPositive_get(void);
/**
 * Get value stored in TX table of signal BMS_AirNegative in msg BMS_Contactors.
 */
ContactorState app_canTx_BMS_Contactors_BMS_AirNegative_get(void);
/**
 * Get value stored in TX table of signal BMS_PrechargeRelay in msg BMS_Contactors.
 */
ContactorState app_canTx_BMS_Contactors_BMS_PrechargeRelay_get(void);
/**
 * Get value stored in TX table of signal BMS_MinCellTemperature in msg BMS_CellTemperatures.
 */
float app_canTx_BMS_CellTemperatures_BMS_MinCellTemperature_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxCellTemperature in msg BMS_CellTemperatures.
 */
float app_canTx_BMS_CellTemperatures_BMS_MaxCellTemperature_get(void);
/**
 * Get value stored in TX table of signal BMS_PackVoltage in msg BMS_VoltageAndChargeStats.
 */
float app_canTx_BMS_VoltageAndChargeStats_BMS_PackVoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_Soc in msg BMS_VoltageAndChargeStats.
 */
float app_canTx_BMS_VoltageAndChargeStats_BMS_Soc_get(void);
/**
 * Get value stored in TX table of signal BMS_SocCorrupt in msg BMS_VoltageAndChargeStats.
 */
bool app_canTx_BMS_VoltageAndChargeStats_BMS_SocCorrupt_get(void);
/**
 * Get value stored in TX table of signal BMS_MinCellVoltage in msg BMS_VoltageAndChargeStats.
 */
float app_canTx_BMS_VoltageAndChargeStats_BMS_MinCellVoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxCellVoltage in msg BMS_VoltageAndChargeStats.
 */
float app_canTx_BMS_VoltageAndChargeStats_BMS_MaxCellVoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_MinCellVoltageSegment in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MinCellVoltageSegment_get(void);
/**
 * Get value stored in TX table of signal BMS_MinCellVoltageIdx in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MinCellVoltageIdx_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxCellVoltageSegment in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MaxCellVoltageSegment_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxCellVoltageIdx in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MaxCellVoltageIdx_get(void);
/**
 * Get value stored in TX table of signal BMS_MinTempSegment in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MinTempSegment_get(void);
/**
 * Get value stored in TX table of signal BMS_MinTempIdx in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MinTempIdx_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxTempSegment in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MaxTempSegment_get(void);
/**
 * Get value stored in TX table of signal BMS_MaxTempIdx in msg BMS_CellStats.
 */
uint32_t app_canTx_BMS_CellStats_BMS_MaxTempIdx_get(void);
/**
 * Get value stored in TX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
float app_canTx_BMS_TractiveSystem_BMS_TractiveSystemVoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
float app_canTx_BMS_TractiveSystem_BMS_TractiveSystemCurrent_get(void);
/**
 * Get value stored in TX table of signal BMS_TractiveSystemPower in msg BMS_TractiveSystem.
 */
float app_canTx_BMS_TractiveSystem_BMS_TractiveSystemPower_get(void);
/**
 * Get value stored in TX table of signal BMS_AvailablePower in msg BMS_TractiveSystem.
 */
uint32_t app_canTx_BMS_TractiveSystem_BMS_AvailablePower_get(void);
/**
 * Get value stored in TX table of signal BMS_BSPDCurrentThresholdExceeded in msg BMS_TractiveSystem.
 */
bool app_canTx_BMS_TractiveSystem_BMS_BSPDCurrentThresholdExceeded_get(void);
/**
 * Get value stored in TX table of signal BMS_Hash in msg BMS_CommitInfo.
 */
uint32_t app_canTx_BMS_CommitInfo_BMS_Hash_get(void);
/**
 * Get value stored in TX table of signal BMS_Clean in msg BMS_CommitInfo.
 */
bool app_canTx_BMS_CommitInfo_BMS_Clean_get(void);
/**
 * Get value stored in TX table of signal BMS_Segment0_OWC_Cells_Status in msg BMS_OWC_Segment0to2_Status.
 */
uint32_t app_canTx_BMS_OWC_Segment0to2_Status_BMS_Segment0_OWC_Cells_Status_get(void);
/**
 * Get value stored in TX table of signal BMS_Segment1_OWC_Cells_Status in msg BMS_OWC_Segment0to2_Status.
 */
uint32_t app_canTx_BMS_OWC_Segment0to2_Status_BMS_Segment1_OWC_Cells_Status_get(void);
/**
 * Get value stored in TX table of signal BMS_Segment2_OWC_Cells_Status in msg BMS_OWC_Segment0to2_Status.
 */
uint32_t app_canTx_BMS_OWC_Segment0to2_Status_BMS_Segment2_OWC_Cells_Status_get(void);
/**
 * Get value stored in TX table of signal BMS_Segment3_OWC_Cells_Status in msg BMS_OWC_Segment3to4_Status.
 */
uint32_t app_canTx_BMS_OWC_Segment3to4_Status_BMS_Segment3_OWC_Cells_Status_get(void);
/**
 * Get value stored in TX table of signal BMS_Segment4_OWC_Cells_Status in msg BMS_OWC_Segment3to4_Status.
 */
uint32_t app_canTx_BMS_OWC_Segment3to4_Status_BMS_Segment4_OWC_Cells_Status_get(void);
/**
 * Get value stored in TX table of signal BMS_HVDShdnOKStatus in msg BMS_BMSShdnNodeStatus.
 */
bool app_canTx_BMS_BMSShdnNodeStatus_BMS_HVDShdnOKStatus_get(void);
/**
 * Get value stored in TX table of signal BMS_TSIlckOKStatus in msg BMS_BMSShdnNodeStatus.
 */
bool app_canTx_BMS_BMSShdnNodeStatus_BMS_TSIlckOKStatus_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_StackWaterMarkHighTask1Hz_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_StackWaterMarkHighTask100Hz_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_StackWaterMarkHighTask1kHz_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_StackWaterMarkHighTaskCanRx_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_StackWaterMarkHighTaskCanTx_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_WatchdogTimeout_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_TxOverflow_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_RxOverflow_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheckFault in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_OpenWireCheckFault_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment0_GND in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_OpenWireCheck_Segment0_GND_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment1_GND in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_OpenWireCheck_Segment1_GND_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment2_GND in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_OpenWireCheck_Segment2_GND_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment3_GND in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_OpenWireCheck_Segment3_GND_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment4_GND in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_OpenWireCheck_Segment4_GND_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_MissingVCHeartbeat in msg BMS_Warnings.
 */
bool app_canTx_BMS_Warnings_BMS_Warning_MissingVCHeartbeat_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_StateMachine_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_CellUndervoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_CellOvervoltage_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_ModuleCommunicationError_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_CellUndertemp_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_CellOvertemp_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ChargerReportedError in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_ChargerReportedError_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_ChargerDisconnectedDuringCharge_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ChargerShutdownLoopOpen in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_ChargerShutdownLoopOpen_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_TractiveSystemOvercurrent_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
bool app_canTx_BMS_Faults_BMS_Fault_PrechargeFailure_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1HzCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_StackWaterMarkHighTask1HzCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask100HzCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_StackWaterMarkHighTask100HzCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1kHzCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_StackWaterMarkHighTask1kHzCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRxCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_StackWaterMarkHighTaskCanRxCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTxCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_StackWaterMarkHighTaskCanTxCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_WatchdogTimeoutCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_WatchdogTimeoutCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_TxOverflowCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_TxOverflowCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_RxOverflowCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_RxOverflowCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheckFaultCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_OpenWireCheckFaultCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment0_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_OpenWireCheck_Segment0_GNDCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment1_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_OpenWireCheck_Segment1_GNDCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment2_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_OpenWireCheck_Segment2_GNDCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment3_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_OpenWireCheck_Segment3_GNDCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_OpenWireCheck_Segment4_GNDCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_OpenWireCheck_Segment4_GNDCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Warning_MissingVCHeartbeatCount in msg BMS_WarningsCounts.
 */
uint32_t app_canTx_BMS_WarningsCounts_BMS_Warning_MissingVCHeartbeatCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_StateMachineCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_StateMachineCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellUndervoltageCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_CellUndervoltageCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellOvervoltageCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_CellOvervoltageCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ModuleCommunicationErrorCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_ModuleCommunicationErrorCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellUndertempCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_CellUndertempCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_CellOvertempCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_CellOvertempCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ChargerReportedErrorCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_ChargerReportedErrorCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ChargerDisconnectedDuringChargeCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_ChargerDisconnectedDuringChargeCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_ChargerShutdownLoopOpenCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_ChargerShutdownLoopOpenCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_TractiveSystemOvercurrentCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_TractiveSystemOvercurrentCount_get(void);
/**
 * Get value stored in TX table of signal BMS_Fault_PrechargeFailureCount in msg BMS_FaultsCounts.
 */
uint32_t app_canTx_BMS_FaultsCounts_BMS_Fault_PrechargeFailureCount_get(void);


/**
 * Get pointer to value stored in TX table of msg BMS_Vitals.
 */
BMS_Vitals_Signals* app_canTx_BMS_Vitals_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_AlertsContext.
 */
BMS_AlertsContext_Signals* app_canTx_BMS_AlertsContext_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_ImdStatus.
 */
BMS_ImdStatus_Signals* app_canTx_BMS_ImdStatus_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_ImdData.
 */
BMS_ImdData_Signals* app_canTx_BMS_ImdData_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_Charger.
 */
BMS_Charger_Signals* app_canTx_BMS_Charger_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_BrusaControls.
 */
BMS_BrusaControls_Signals* app_canTx_BMS_BrusaControls_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_OkStatuses.
 */
BMS_OkStatuses_Signals* app_canTx_BMS_OkStatuses_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_Contactors.
 */
BMS_Contactors_Signals* app_canTx_BMS_Contactors_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_CellTemperatures.
 */
BMS_CellTemperatures_Signals* app_canTx_BMS_CellTemperatures_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_VoltageAndChargeStats.
 */
BMS_VoltageAndChargeStats_Signals* app_canTx_BMS_VoltageAndChargeStats_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_CellStats.
 */
BMS_CellStats_Signals* app_canTx_BMS_CellStats_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_TractiveSystem.
 */
BMS_TractiveSystem_Signals* app_canTx_BMS_TractiveSystem_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_CommitInfo.
 */
BMS_CommitInfo_Signals* app_canTx_BMS_CommitInfo_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_OWC_Segment0to2_Status.
 */
BMS_OWC_Segment0to2_Status_Signals* app_canTx_BMS_OWC_Segment0to2_Status_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_OWC_Segment3to4_Status.
 */
BMS_OWC_Segment3to4_Status_Signals* app_canTx_BMS_OWC_Segment3to4_Status_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_BMSShdnNodeStatus.
 */
BMS_BMSShdnNodeStatus_Signals* app_canTx_BMS_BMSShdnNodeStatus_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_Warnings.
 */
BMS_Warnings_Signals* app_canTx_BMS_Warnings_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_Faults.
 */
BMS_Faults_Signals* app_canTx_BMS_Faults_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_WarningsCounts.
 */
BMS_WarningsCounts_Signals* app_canTx_BMS_WarningsCounts_getData();
/**
 * Get pointer to value stored in TX table of msg BMS_FaultsCounts.
 */
BMS_FaultsCounts_Signals* app_canTx_BMS_FaultsCounts_getData();



