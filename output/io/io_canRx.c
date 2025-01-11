/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canRx.h"
#include "app_canRx.h"
#include "app_canUtils.h"

/* ------------------------- Function Definitions ------------------------- */

bool io_canRx_filterMessageId(uint32_t std_id)
{
    bool is_found = false;
    switch (std_id)
    {
        case CANMSG_BMS_TRACTIVE_SYSTEM_ID:
        case CANMSG_BMS_BMS_SHDN_NODE_STATUS_ID:
        case CANMSG_BMS_CELL_TEMPERATURES_ID:
        case CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_ID:
        case CANMSG_BMS_OK_STATUSES_ID:
        case CANMSG_BMS_VITALS_ID:
        {
            is_found = true;
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    
    return is_found;
}

void io_canRx_updateRxTableWithMessage(JsonCanMsg* msg)
{
    switch (msg->std_id)
    {
        case CANMSG_BMS_TRACTIVE_SYSTEM_ID:
        {
            BMS_TractiveSystem_Signals out_msg;
            app_canUtils_BMS_TractiveSystem_unpack(msg->data, &out_msg);
            app_canRx_BMS_TractiveSystemVoltage_update(out_msg.BMS_TractiveSystemVoltage_value);
            app_canRx_BMS_TractiveSystemCurrent_update(out_msg.BMS_TractiveSystemCurrent_value);
            app_canRx_BMS_TractiveSystemPower_update(out_msg.BMS_TractiveSystemPower_value);
            app_canRx_BMS_AvailablePower_update(out_msg.BMS_AvailablePower_value);
            app_canRx_BMS_BSPDCurrentThresholdExceeded_update(out_msg.BMS_BSPDCurrentThresholdExceeded_value);
            break;
        }
        case CANMSG_BMS_BMS_SHDN_NODE_STATUS_ID:
        {
            BMS_BMSShdnNodeStatus_Signals out_msg;
            app_canUtils_BMS_BMSShdnNodeStatus_unpack(msg->data, &out_msg);
            app_canRx_BMS_HVDShdnOKStatus_update(out_msg.BMS_HVDShdnOKStatus_value);
            app_canRx_BMS_TSIlckOKStatus_update(out_msg.BMS_TSIlckOKStatus_value);
            break;
        }
        case CANMSG_BMS_CELL_TEMPERATURES_ID:
        {
            BMS_CellTemperatures_Signals out_msg;
            app_canUtils_BMS_CellTemperatures_unpack(msg->data, &out_msg);
            app_canRx_BMS_MinCellTemperature_update(out_msg.BMS_MinCellTemperature_value);
            app_canRx_BMS_MaxCellTemperature_update(out_msg.BMS_MaxCellTemperature_value);
            break;
        }
        case CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_ID:
        {
            BMS_VoltageAndChargeStats_Signals out_msg;
            app_canUtils_BMS_VoltageAndChargeStats_unpack(msg->data, &out_msg);
            app_canRx_BMS_PackVoltage_update(out_msg.BMS_PackVoltage_value);
            app_canRx_BMS_Soc_update(out_msg.BMS_Soc_value);
            app_canRx_BMS_SocCorrupt_update(out_msg.BMS_SocCorrupt_value);
            app_canRx_BMS_MinCellVoltage_update(out_msg.BMS_MinCellVoltage_value);
            app_canRx_BMS_MaxCellVoltage_update(out_msg.BMS_MaxCellVoltage_value);
            break;
        }
        case CANMSG_BMS_OK_STATUSES_ID:
        {
            BMS_OkStatuses_Signals out_msg;
            app_canUtils_BMS_OkStatuses_unpack(msg->data, &out_msg);
            app_canRx_BMS_BmsOk_update(out_msg.BMS_BmsOk_value);
            app_canRx_BMS_ImdOk_update(out_msg.BMS_ImdOk_value);
            app_canRx_BMS_BspdOk_update(out_msg.BMS_BspdOk_value);
            app_canRx_BMS_BmsLatchedFault_update(out_msg.BMS_BmsLatchedFault_value);
            app_canRx_BMS_ImdLatchedFault_update(out_msg.BMS_ImdLatchedFault_value);
            app_canRx_BMS_BspdLatchedFault_update(out_msg.BMS_BspdLatchedFault_value);
            break;
        }
        case CANMSG_BMS_VITALS_ID:
        {
            BMS_Vitals_Signals out_msg;
            app_canUtils_BMS_Vitals_unpack(msg->data, &out_msg);
            app_canRx_BMS_Heartbeat_update(out_msg.BMS_Heartbeat_value);
            app_canRx_BMS_State_update(out_msg.BMS_State_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}