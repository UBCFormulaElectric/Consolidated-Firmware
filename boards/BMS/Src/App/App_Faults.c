#include "App_CanRx.h"
#include "App_Faults.h"

bool has_fsm_critical_fault(void* can_rx)
{
    if (App_CanRx_FSM_ERRORS_GetSignal_CRITICAL_FAULT(can_rx))
        return true;
    return false;
}

bool has_dcm_critical_fault(void* can_rx)
{
    if (App_CanRx_DCM_ERRORS_GetSignal_CRITICAL_FAULT(can_rx))
        return true;
    return false;
}


bool has_pdm_critical_fault(void* can_rx)
{
    if (App_CanRx_PDM_ERRORS_GetSignal_CRITICAL_FAULT(can_rx))
        return true;
    return false;
}

bool has_bms_critical_fault(void* can_rx)
{
    // Not sure what to do here because BMS ERRORS are in CAN TX
    // from the perspective of BMS
    (void)can_rx;
    return false;
}

bool has_critical_fault(void *can_rx)
{
    if (has_fsm_critical_fault(can_rx)||
        has_dcm_critical_fault(can_rx) ||
        has_pdm_critical_fault(can_rx))
    {
        return true;
    }
    return false;
}
