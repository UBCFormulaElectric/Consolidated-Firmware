#include "app_faultCheck.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"

bool app_boardFaultCheck(){
    const bool bms_fault  = app_canAlerts_BoardHasFault(BMS_ALERT_BOARD);
    const bool vc_fault   = app_canAlerts_BoardHasFault(VC_ALERT_BOARD);
    const bool fsm_fault  = app_canAlerts_BoardHasFault(FSM_ALERT_BOARD);
    const bool crit_fault = app_canAlerts_BoardHasFault(CRIT_ALERT_BOARD);

    return (bms_fault || vc_fault || fsm_fault || crit_fault);
}


bool app_inverterFaultCheck(){

    const bool left_inverter_fault  = app_canRx_INVL_VsmState_get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    const bool right_inverter_fault = app_canRx_INVR_VsmState_get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    app_canAlerts_VC_Fault_LeftInverterFault_set(left_inverter_fault);
    app_canAlerts_VC_Fault_RightInverterFault_set(right_inverter_fault);

    return (left_inverter_fault || right_inverter_fault);
}



