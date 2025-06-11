#include "app_shdnLast.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include <app_canUtils.h>

void app_shdnLast_broadcast(void)
{
    ShutdownNode node;

    if (!app_canTx_VC_TSMSOKStatus_get())
    {
        node = SHDN_TSMS;
    }
    else if (!app_canRx_FSM_FrontRightILCKOKStatus_get())
    {
        node = SHDN_FR_ILCK;
    }
    else if (!app_canRx_FSM_FrontLeftILCKOKStatus_get())
    {
        node = SHDN_FL_INERTIA_ILCK;
    }
    else if (!app_canRx_FSM_BOTSOKStatus_get())
    {
        node = SHDN_BOTS;
    }
    else if (!app_canRx_FSM_COCKPITOKStatus_get())
    {
        node = SHDN_Cockpit_EStop;
    }
    else if (!app_canRx_DAM_REStopOKStatus_get())
    {
        node = SHDN_R_EStop;
    }
    else if (!app_canRx_DAM_LEStopOKStatus_get())
    {
        node = SHDN_L_EStop;
    }
    else if (!app_canTx_VC_SplitterBoxInterlockOKStatus_get())
    {
        node = SHDN_SB_ILCK;
    }
    else if (!app_canTx_VC_RearRightMotorInterlock_get())
    {
        node = SHDN_RL_ILCK;
    }
    else if (!app_canRx_RSM_RearLeftMotorInterlock_get())
    {
        node = SHDN_RR_ILCK;
    }
    else if (!app_canRx_BMS_HVPShdnOKStatus_get())
    {
        node = SHDN_HV_P_Ilck;
    }
    else if (!app_canRx_BMS_HVNShdnOKStatus_get())
    {
        node = SHDN_HV_N_Ilck;
    }
    else if (!app_canRx_BMS_BmsLatchOk_get())
    {
        node = SHDN_BMS_OK;
    }
    else if (!app_canRx_BMS_BspdLatchOk_get())
    {
        node = SHDN_BSPD_OK;
    }
    else if (!app_canRx_BMS_ImdLatchOk_get())
    {
        node = SHDN_IMD_OK;
    }
    else
    {
        node = SHDN_OK;
    }

    app_canTx_VC_FirstFaultNode_set(node);
}
