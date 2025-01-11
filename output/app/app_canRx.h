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
 * Update value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void app_canRx_BMS_Heartbeat_update(bool value);/**
 * Update value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
void app_canRx_BMS_State_update(BmsState value);


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
 * Get value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool app_canRx_BMS_Heartbeat_get(void);/**
 * Get value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState app_canRx_BMS_State_get(void);
