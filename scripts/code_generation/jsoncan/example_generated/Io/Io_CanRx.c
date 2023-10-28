/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "Io_CanRx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"

/* ------------------------- Function Definitions ------------------------- */

bool Io_CanRx_FilterMessageId(uint32_t std_id)
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

void Io_CanRx_UpdateRxTableWithMessage(struct CanMsg* msg)
{
    switch (msg->std_id)
    {
        case CAN_MSG_FSM_APPS_ID:
        {
            FSM_Apps_Signals out_msg;
            App_CanUtils_FSM_Apps_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_FSM_PappsMappedPedalPercentage_Update(out_msg.FSM_PappsMappedPedalPercentage_value);
            App_CanRx_FSM_FSM_SappsMappedPedalPercentage_Update(out_msg.FSM_SappsMappedPedalPercentage_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FSM_Warnings_Signals out_msg;
            App_CanUtils_FSM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_TEST1_Update(out_msg.FSM_TEST1_value);
            App_CanRx_FSM_TEST2_Update(out_msg.FSM_TEST2_value);
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            App_CanUtils_FSM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_FAULT_TEST3_Update(out_msg.FSM_FAULT_TEST3_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

