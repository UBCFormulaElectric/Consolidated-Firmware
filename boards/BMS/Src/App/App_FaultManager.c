#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_FaultManager.h"

#define _add_critical_fault(PREFIX, FAULT)       \
    if (PREFIX##_GetPeriodicSignal_##FAULT(can)) \
        return true;

#define add_critical_fault(PREFIX, FAULT) _add_critical_fault(PREFIX, FAULT)

static void *_fsm_can;
static void *_dcm_can;
static void *_pdm_can;
static void *_dim_can;
static void *_bms_can;

void App_FaultManager_Init(
    void *fsm_can,
    void *dcm_can,
    void *pdm_can,
    void *dim_can,
    void *bms_can)
{
    _fsm_can = fsm_can;
    _dcm_can = dcm_can;
    _pdm_can = pdm_can;
    _dim_can = dim_can;
    _bms_can = bms_can;
}

bool App_FaultManager_HasFsmCriticalFaultManager(void)
{
    void *can = _fsm_can;
    add_critical_fault(FSM_CAN, CRITICAL_FAULT);
    return false;
}

bool App_FaultManager_HasDcmCriticalFaultManager(void)
{
    void *can = _dcm_can;
    add_critical_fault(DCM_CAN, CRITICAL_FAULT);
    return false;
}

bool App_FaultManager_HasPdmCriticalFaultManager(void)
{
    void *can = _pdm_can;
    add_critical_fault(PDM_CAN, CRITICAL_FAULT);
    return false;
}

bool App_FaultManager_HasDimCriticalFaultManager(void)
{
    void *can = _dim_can;
    add_critical_fault(DIM_CAN, CRITICAL_FAULT);
    return false;
}

bool App_FaultManager_HasBmsCriticalFaultManager(void)
{
    void *can = _bms_can;
    add_critical_fault(BMS_CAN, CRITICAL_FAULT);
    return false;
}

bool App_FaultManager_HasCriticalFaultManager(void)
{
    if (App_FaultManager_HasFsmCriticalFaultManager() ||
        App_FaultManager_HasDcmCriticalFaultManager() ||
        App_FaultManager_HasPdmCriticalFaultManager() ||
        App_FaultManager_HasDimCriticalFaultManager() ||
        App_FaultManager_HasBmsCriticalFaultManager())
    {
        return true;
    }
    else
    {
        return false;
    }
}
