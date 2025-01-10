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
        case CANMSG_VC_VITALS_ID:
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
        case CANMSG_VC_VITALS_ID:
        {
            VC_Vitals_Signals out_msg;
            app_canUtils_VC_Vitals_unpack(msg->data, &out_msg);
            app_canRx_VC_Heartbeat_update(out_msg.VC_Heartbeat_value);
            app_canRx_VC_State_update(out_msg.VC_State_value);
            app_canRx_VC_BuzzerOn_update(out_msg.VC_BuzzerOn_value);
            app_canRx_VC_NumberOfCanDataLogs_update(out_msg.VC_NumberOfCanDataLogs_value);
            app_canRx_VC_CanLoggingRemainingErrors_update(out_msg.VC_CanLoggingRemainingErrors_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}