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
        case CAN_MSG_FSM_APPS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
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
        case CAN_MSG_FSM_APPS_ID:
        {
            FSM_Apps_Signals out_msg;
            app_canUtils_FSM_Apps_unpack(msg->data, &out_msg);
            
            app_canRx_FSM_PappsMappedPedalPercentage_update(out_msg.FSM_PappsMappedPedalPercentage_value);
            app_canRx_FSM_SappsMappedPedalPercentage_update(out_msg.FSM_SappsMappedPedalPercentage_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FSM_Warnings_Signals out_msg;
            app_canUtils_FSM_Warnings_unpack(msg->data, &out_msg);
            
            app_canRx_FSM_Warning_Warning_Test1_update(out_msg.FSM_Warning_Warning_Test1_value);
            app_canRx_FSM_Warning_Warning_Test2_update(out_msg.FSM_Warning_Warning_Test2_value);
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            app_canUtils_FSM_Faults_unpack(msg->data, &out_msg);
            
            app_canRx_FSM_Fault_Fault_Test3_update(out_msg.FSM_Fault_Fault_Test3_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

