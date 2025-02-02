/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "app_canDataCapture.h"
#include "app_canUtils.h"

/* -------------------------------- Macros -------------------------------- */


/* -------------------------- Private Variables --------------------------- */



/* ------------------------- Function Definitions ------------------------- */


void app_canDataCapture_init()
{
    // Reset next log capture times.
    
    // Reset next telem capture times.
}

bool app_dataCapture_needsLog(uint16_t msg_id, uint32_t time_ms)
{
    switch (msg_id)
    {
        case CAN_MSG_BOOT_BMS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_FSM_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_VC_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_RSM_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_CRIT_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_H7_DEV_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_LATCHED_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_SWITCHES_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_DRIVE_MODE_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_ALERTS_CONTEXT_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_COMMIT_INFO_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_CRIT_SHDN_NODE_STATUS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_WATERMARK_LEVEL_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_VEHICLE_DYNAMICS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_GLOBAL_SHDN_NODE_STATUS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_CHARGING_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_CELL_BALANCING_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_RESET_SOC_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_SET_COOLANT_PUMP_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_SET_BSPD_TEST_CURRENT_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
    }
}

bool app_dataCapture_needsTelem(uint16_t msg_id, uint32_t time_ms)
{
    switch (msg_id)
    {
        case CAN_MSG_BOOT_BMS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_FSM_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_VC_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_RSM_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_CRIT_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BOOT_H7_DEV_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_LATCHED_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_BMS_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_FSM_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_SWITCHES_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_DRIVE_MODE_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_ALERTS_CONTEXT_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_COMMIT_INFO_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_CRIT_SHDN_NODE_STATUS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_WATERMARK_LEVEL_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_CRIT_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_VEHICLE_DYNAMICS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_GLOBAL_SHDN_NODE_STATUS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_VC_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_VITALS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_WARNINGS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_FAULTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_WARNINGS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_RSM_FAULTS_COUNTS_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_CHARGING_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_CELL_BALANCING_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_RESET_SOC_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_SET_COOLANT_PUMP_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
        case CAN_MSG_DEBUG_SET_BSPD_TEST_CURRENT_ID:
        {
            // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;
        }
    }
}

