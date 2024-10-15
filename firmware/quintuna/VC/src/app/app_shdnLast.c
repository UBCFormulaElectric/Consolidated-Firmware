#include "app_shdnLast.h"
#include "app_canTx.h"
#include "app_canRx.h"

void app_shdnLast_broadcast(void)
{
    ShutdownNode node;

    if (!app_canRx_BMS_HVDShdnOKStatus_get())
    {
        node = SHDN_HVD_ILCK;
    }
    else if (!app_canTx_VC_SplitterBoxInterlockOKStatus_get())
    {
        node = SHDN_SB_ILCK;
    }
    else if (!app_canTx_VC_TSMSOKStatus_get())
    {
        node = SHDN_TSMS;
    }
    else if (!app_canTx_VC_REStopOKStatus_get())
    {
        node = SHDN_R_EStop;
    }
    else if (!app_canTx_VC_LEStopOKStatus_get())
    {
        node = SHDN_L_EStop;
    }
    else if (!app_canRx_FSM_BOTSOKStatus_get())
    {
        node = SHDN_BOTS;
    }
    else if (!app_canRx_CRIT_InertiaSenOKStatus_get())
    {
        node = SHDN_Inertia;
    }
    else if (!app_canRx_CRIT_CockpitEStopOKStatus_get())
    {
        node = SHDN_Cockpit_EStop;
    }
    else if (!app_canRx_BMS_TSIlckOKStatus_get())
    {
        node = SHDN_TS_Ilck;
    }
    else if (!app_canRx_BMS_BmsOk_get())
    {
        node = SHDN_BMS_OK;
    }
    else if (!app_canRx_BMS_BspdOk_get())
    {
        node = SHDN_BSPD_OK;
    }
    else if (!app_canRx_BMS_ImdOk_get())
    {
        node = SHDN_IMD_OK;
    }
    else
    {
        node = SHDN_OK;
    }

    app_canTx_VC_FirstFaultNode_set(node);
}
