#include "app_leds.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_leds.h"

static BoardLEDStatus worstBoardStatus(CanAlertBoard board)
{
    bool is_missing_heartbeat;
    switch (board)
    {
        case BMS_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get();
            break;
        }
        case CRIT_ALERT_BOARD:
        {
            is_missing_heartbeat = false; // lmao like
            break;
        }
        case FSM_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get();
            break;
        }
        case RSM_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get();
            break;
        }
        case VC_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get();
            break;
        }
        default:
        {
            is_missing_heartbeat = false;
            break;
        }
    }

    if (is_missing_heartbeat)
    {
        return BOARD_LED_STATUS_MISSING_HEARTBEAT;
    }
    else if (app_canAlerts_BoardHasFault(board))
    {
        return BOARD_LED_STATUS_FAULT;
    }
    else if (app_canAlerts_BoardHasWarning(board))
    {
        return BOARD_LED_STATUS_WARNING;
    }

    return BOARD_LED_STATUS_OK;
}

void app_leds_update(void)
{
    const bool imd_fault_latched = app_canRx_BMS_ImdLatchedFault_get();
    io_led_imd_set(imd_fault_latched);
    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    io_led_bspd_set(bspd_fault_latched);
    const bool ams_fault_latched = app_canRx_BMS_BmsLatchedFault_get();
    io_led_ams_set(ams_fault_latched);
    const bool start_led_on = app_canRx_VC_State_get() == VC_DRIVE_STATE;
    io_led_start_set(start_led_on);
    const bool regen_light_on = app_canRx_VC_RegenEnabled_get();
    io_led_regen_set(regen_light_on);
    const bool torquevec_light_on = app_canRx_VC_TorqueVectoringEnabled_get();
    io_led_torquevec_set(torquevec_light_on);

    // or driven by BMS_drive_state???
    const BoardLEDStatus shutdown_sensor_ok =
        app_canRx_VC_FirstFaultNode_get() == SHDN_OK ? BOARD_LED_STATUS_OK : BOARD_LED_STATUS_FAULT;
    io_led_shutdown_set(shutdown_sensor_ok);

    const BoardLEDStatus bms_status = worstBoardStatus(BMS_ALERT_BOARD);
    io_led_bms_status_set(bms_status);

    const BoardLEDStatus fsm_status = worstBoardStatus(FSM_ALERT_BOARD);
    io_led_fsm_status_set(fsm_status);

    const BoardLEDStatus vc_status = worstBoardStatus(VC_ALERT_BOARD);
    io_led_vc_status_set(vc_status);

    const BoardLEDStatus crit_status = worstBoardStatus(CRIT_ALERT_BOARD);
    io_led_crit_status_set(crit_status);

    const BoardLEDStatus rsm_status = worstBoardStatus(RSM_ALERT_BOARD);
    io_led_rsm_status_set(rsm_status);

    // TODO AUX status
    io_led_aux_status_set(BOARD_LED_STATUS_NOT_IMPLEMENTED);
}
