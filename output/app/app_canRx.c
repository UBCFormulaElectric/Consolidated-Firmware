/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by  (i.e. the RX table).
 */
typedef struct
{
    BMS_TractiveSystem_Signals BMS_TractiveSystem_signals;
    BMS_BMSShdnNodeStatus_Signals BMS_BMSShdnNodeStatus_signals;
    BMS_CellTemperatures_Signals BMS_CellTemperatures_signals;
    BMS_VoltageAndChargeStats_Signals BMS_VoltageAndChargeStats_signals;
    BMS_OkStatuses_Signals BMS_OkStatuses_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    // Initialize all signals to Start-up value
    app_canRx_BMS_TractiveSystemVoltage_update(CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    app_canRx_BMS_TractiveSystemCurrent_update(CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
    app_canRx_BMS_TractiveSystemPower_update(CANSIG_BMS_TRACTIVE_SYSTEM_POWER_START_VAL);
    app_canRx_BMS_AvailablePower_update(CANSIG_BMS_AVAILABLE_POWER_START_VAL);
    app_canRx_BMS_BSPDCurrentThresholdExceeded_update(CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_START_VAL);
    app_canRx_BMS_HVDShdnOKStatus_update(CANSIG_BMS_HVD_SHDN_OK_STATUS_START_VAL);
    app_canRx_BMS_TSIlckOKStatus_update(CANSIG_BMS_TS_ILCK_OK_STATUS_START_VAL);
    app_canRx_BMS_MinCellTemperature_update(CANSIG_BMS_MIN_CELL_TEMPERATURE_START_VAL);
    app_canRx_BMS_MaxCellTemperature_update(CANSIG_BMS_MAX_CELL_TEMPERATURE_START_VAL);
    app_canRx_BMS_PackVoltage_update(CANSIG_BMS_PACK_VOLTAGE_START_VAL);
    app_canRx_BMS_Soc_update(CANSIG_BMS_SOC_START_VAL);
    app_canRx_BMS_SocCorrupt_update(CANSIG_BMS_SOC_CORRUPT_START_VAL);
    app_canRx_BMS_MinCellVoltage_update(CANSIG_BMS_MIN_CELL_VOLTAGE_START_VAL);
    app_canRx_BMS_MaxCellVoltage_update(CANSIG_BMS_MAX_CELL_VOLTAGE_START_VAL);
    app_canRx_BMS_BmsOk_update(CANSIG_BMS_BMS_OK_START_VAL);
    app_canRx_BMS_ImdOk_update(CANSIG_BMS_IMD_OK_START_VAL);
    app_canRx_BMS_BspdOk_update(CANSIG_BMS_BSPD_OK_START_VAL);
    app_canRx_BMS_BmsLatchedFault_update(CANSIG_BMS_BMS_LATCHED_FAULT_START_VAL);
    app_canRx_BMS_ImdLatchedFault_update(CANSIG_BMS_IMD_LATCHED_FAULT_START_VAL);
    app_canRx_BMS_BspdLatchedFault_update(CANSIG_BMS_BSPD_LATCHED_FAULT_START_VAL);
    app_canRx_BMS_Heartbeat_update(CANSIG_BMS_HEARTBEAT_START_VAL);
    app_canRx_BMS_State_update(CANSIG_BMS_STATE_START_VAL);
    
}

void app_canRx_BRUSA_IsConnected_update(bool value)
{
    rx_table.BRUSA_Vitals_signals.BRUSA_IsConnected_value = value;
}

void app_canRx_BMS_TractiveSystemVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_TractiveSystemCurrent_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL;
    }
}
void app_canRx_BMS_TractiveSystemPower_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL;
    }
}
void app_canRx_BMS_AvailablePower_update(uint32_t value)
{
    tx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_AVAILABLE_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_AVAILABLE_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MIN_VAL;
    }
}
void app_canRx_BMS_BSPDCurrentThresholdExceeded_update(bool value)
{
    tx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL;
    }
}

void app_canRx_BMS_HVDShdnOKStatus_update(bool value)
{
    tx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL;
    }
}
void app_canRx_BMS_TSIlckOKStatus_update(bool value)
{
    tx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL;
    }
}

void app_canRx_BMS_MinCellTemperature_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL;
    }
}
void app_canRx_BMS_MaxCellTemperature_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL;
    }
}

void app_canRx_BMS_PackVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_PACK_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_PACK_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_PACK_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_PACK_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_Soc_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SOC_MAX_VAL)
    {
        value = CANSIG_BMS_SOC_MAX_VAL;
    }
    if (value < CANSIG_BMS_SOC_MIN_VAL)
    {
        value = CANSIG_BMS_SOC_MIN_VAL;
    }
}
void app_canRx_BMS_SocCorrupt_update(bool value)
{
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SOC_CORRUPT_MAX_VAL)
    {
        value = CANSIG_BMS_SOC_CORRUPT_MAX_VAL;
    }
    if (value < CANSIG_BMS_SOC_CORRUPT_MIN_VAL)
    {
        value = CANSIG_BMS_SOC_CORRUPT_MIN_VAL;
    }
}
void app_canRx_BMS_MinCellVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_MaxCellVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL;
    }
}

void app_canRx_BMS_BmsOk_update(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BMS_OK_MAX_VAL)
    {
        value = CANSIG_BMS_BMS_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_BMS_OK_MIN_VAL)
    {
        value = CANSIG_BMS_BMS_OK_MIN_VAL;
    }
}
void app_canRx_BMS_ImdOk_update(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_OK_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_OK_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_OK_MIN_VAL;
    }
}
void app_canRx_BMS_BspdOk_update(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_OK_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_OK_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_OK_MIN_VAL;
    }
}
void app_canRx_BMS_BmsLatchedFault_update(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canRx_BMS_ImdLatchedFault_update(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canRx_BMS_BspdLatchedFault_update(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL;
    }
}

void app_canRx_BMS_Heartbeat_update(bool value)
{
    tx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_BMS_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_BMS_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_BMS_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_BMS_State_update(BmsState value)
{
    tx_table.BMS_Vitals_signals.BMS_State_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_STATE_MAX_VAL)
    {
        value = CANSIG_BMS_STATE_MAX_VAL;
    }
    if (value < CANSIG_BMS_STATE_MIN_VAL)
    {
        value = CANSIG_BMS_STATE_MIN_VAL;
    }
}




float app_canRx_BMS_TractiveSystemVoltage_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}
float app_canRx_BMS_TractiveSystemCurrent_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
}
float app_canRx_BMS_TractiveSystemPower_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value;
}
uint32_t app_canRx_BMS_AvailablePower_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value;
}
bool app_canRx_BMS_BSPDCurrentThresholdExceeded_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value;
}

bool app_canRx_BMS_HVDShdnOKStatus_get()
{
    return rx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value;
}
bool app_canRx_BMS_TSIlckOKStatus_get()
{
    return rx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value;
}

float app_canRx_BMS_MinCellTemperature_get()
{
    return rx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value;
}
float app_canRx_BMS_MaxCellTemperature_get()
{
    return rx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value;
}

float app_canRx_BMS_PackVoltage_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value;
}
float app_canRx_BMS_Soc_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value;
}
bool app_canRx_BMS_SocCorrupt_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value;
}
float app_canRx_BMS_MinCellVoltage_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value;
}
float app_canRx_BMS_MaxCellVoltage_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value;
}

bool app_canRx_BMS_BmsOk_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BmsOk_value;
}
bool app_canRx_BMS_ImdOk_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_ImdOk_value;
}
bool app_canRx_BMS_BspdOk_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BspdOk_value;
}
bool app_canRx_BMS_BmsLatchedFault_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value;
}
bool app_canRx_BMS_ImdLatchedFault_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value;
}
bool app_canRx_BMS_BspdLatchedFault_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value;
}

bool app_canRx_BMS_Heartbeat_get()
{
    return rx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}
BmsState app_canRx_BMS_State_get()
{
    return rx_table.BMS_Vitals_signals.BMS_State_value;
}



