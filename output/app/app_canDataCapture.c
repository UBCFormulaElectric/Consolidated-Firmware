/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "app_canDataCapture.h"
#include "app_canUtils.h"

/* -------------------------------- Macros -------------------------------- */
#define VC_Vitals_LOG_CYCLE_TIME (1000)
#define VC_Vitals_TELEM_CYCLE_TIME (1000)

/* -------------------------- Private Variables --------------------------- */
static uint32_t VC_Vitals_next_log_time_ms;
static uint32_t VC_Vitals_next_telem_time_ms;
/* ------------------------- Function Definitions ------------------------- */


void app_canDataCapture_init()
{
    // Reset next log capture times.
    VC_Vitals_next_log_time_ms = 0U;
    
    // Reset next telem capture times.
    VC_Vitals_next_telem_time_ms = 0U;
}

bool app_dataCapture_needsLog(uint16_t msg_id, uint32_t time_ms)
{
    switch (msg_id)
    {
        case CANMSG_BMS_VITALS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_ALERTS_CONTEXT_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_IMD_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_IMD_DATA_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CHARGER_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_BRUSA_CONTROLS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_OK_STATUSES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CONTACTORS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CELL_TEMPERATURES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CELL_STATS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_TRACTIVE_SYSTEM_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_COMMIT_INFO_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_BMS_SHDN_NODE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_WARNINGS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_FAULTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_WARNINGS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_FAULTS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VITALS_ID:
        {
            if (time_ms > VC_Vitals_next_log_time_ms)
            {
                // Capture msg and increment variable.
                VC_Vitals_next_log_time_ms = time_ms + VC_Vitals_LOG_CYCLE_TIME;
                return true;
            }
            else
            {
                // Not ready to capture yet.
                return false;
            }
            break;

        }
        case CANMSG_VC_LEFT_INVERTER_COMMAND_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_RIGHT_INVERTER_COMMAND_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ALERTS_CONTEXT_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_TIME_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_EULER_ANGLES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_COMMIT_INFO_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_POWER_ESTIMATE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_POWER_ESTIMATE_PID_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_PID_SLIP_RATIO_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_SLIP_RATIOS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ACTIVE_DIFF_OUTPUTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_POWER_LIMIT_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VEHICLE_DYNAMICS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_EKF_NAV_VELOCITY_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VOLTAGES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_CURRENT_SENSING_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_EFUSE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_IMU_LINEAR_ACCELERATION_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_COOLANT_PUMP_FLOW_RATE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VC_SHDN_NODE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_GLOBAL_SHDN_NODE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_PEDAL_PERCENTAGE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_WARNINGS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_FAULTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_WARNINGS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_FAULTS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }

        default:
        {
            return false;
            break;
        }
    }
}

bool app_dataCapture_needsTelem(uint16_t msg_id, uint32_t time_ms)
{
    switch (msg_id)
    {
        case CANMSG_BMS_VITALS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_ALERTS_CONTEXT_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_IMD_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_IMD_DATA_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CHARGER_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_BRUSA_CONTROLS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_OK_STATUSES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CONTACTORS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CELL_TEMPERATURES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_CELL_STATS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_TRACTIVE_SYSTEM_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_COMMIT_INFO_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_BMS_SHDN_NODE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_WARNINGS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_FAULTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_WARNINGS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_BMS_FAULTS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VITALS_ID:
        {
            if (time_ms > VC_Vitals_next_log_time_ms)
            {
                // Capture msg and increment variable.
                VC_Vitals_next_log_time_ms = time_ms + VC_Vitals_TELEM_CYCLE_TIME;
                return true;
            }
            else
            {
                // Not ready to capture yet.
                return false;
            }
            break;

        }
        case CANMSG_VC_LEFT_INVERTER_COMMAND_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_RIGHT_INVERTER_COMMAND_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ALERTS_CONTEXT_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_TIME_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_EULER_ANGLES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_COMMIT_INFO_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_POWER_ESTIMATE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_POWER_ESTIMATE_PID_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_PID_SLIP_RATIO_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_SLIP_RATIOS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ACTIVE_DIFF_OUTPUTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_POWER_LIMIT_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VEHICLE_DYNAMICS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_ELLIPSE_EKF_NAV_VELOCITY_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VOLTAGES_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_CURRENT_SENSING_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_EFUSE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_IMU_LINEAR_ACCELERATION_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_COOLANT_PUMP_FLOW_RATE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_VC_SHDN_NODE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_GLOBAL_SHDN_NODE_STATUS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_PEDAL_PERCENTAGE_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_WARNINGS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_FAULTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_WARNINGS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }
        case CANMSG_VC_FAULTS_COUNTS_ID:
        {
        // This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured.
            return true;
            break;

        }

        default:
        {
            return false;
            break;
        }
    }
}
