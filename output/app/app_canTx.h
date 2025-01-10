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




