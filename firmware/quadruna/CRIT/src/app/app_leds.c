#include "app_leds.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_leds.h"

BoardLEDStatus board_worst_status(CanAlertBoard b)
{
    if (app_canAlerts_BoardHasFault(b))
        return FAULT;
    if (app_canAlerts_BoardHasWarning(b))
        return WARNING;
    return OK;
}

void app_leds_update(void)
{
    const bool imd_fault_latched = app_canRx_BMS_ImdLatchedFault_get();
    io_led_imd_set(imd_fault_latched);
    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    io_led_bspd_set(bspd_fault_latched);
    const bool ams_fault_latched = app_canRx_BMS_BmsLatchedFault_get();
    io_led_ams_set(ams_fault_latched);
    const bool shutdown_sensor = false; // TODO implement when SigmaWrath's changes are in
    io_led_shutdown_set(shutdown_sensor);
    const bool start_led_on = app_canRx_VC_State_get() == VC_DRIVE_STATE;
    io_led_start_set(start_led_on);
    const bool regen_light_on = app_canRx_VC_RegenEnabled_get();
    io_led_regen_set(regen_light_on);
    const bool torquevec_light_on = app_canRx_VC_TorqueVectoringEnabled_get();
    io_led_torquevec_set(torquevec_light_on);
    
    BoardLEDStatus bms_status = board_worst_status(BMS_ALERT_BOARD);
    io_led_bms_status_set(bms_status);

    BoardLEDStatus fsm_status = board_worst_status(FSM_ALERT_BOARD);
    io_led_fsm_status_set(fsm_status);

    BoardLEDStatus vc_status = board_worst_status(VC_ALERT_BOARD);
    io_led_vc_status_set(vc_status);

    BoardLEDStatus crit_status = board_worst_status(CRIT_ALERT_BOARD);
    io_led_crit_status_set(crit_status);

    BoardLEDStatus rsm_status = board_worst_status(RSM_ALERT_BOARD);
    io_led_rsm_status_set(rsm_status);

    // TODO AUX status
}
