#include "app_shdnLast.h"
#include "app_canTx.h"
#include "app_canRx.h"

void app_shdnLast_broadcast(void)
{
    ShutdownNode shdn;
    if (!app_canRx_BMS_HVDShdnOKStatus_get())
        shdn = SHDN_HVD_ILCK;
    else if (!app_canTx_VC_SplitterBoxInterlockOKStatus_get())
        shdn = SHDN_SB_ILCK;
    else if (!app_canTx_VC_TSMSOKStatus_get())
        shdn = SHDN_TSMS;
    else if (!app_canTx_VC_REStopOKStatus_get())
        shdn = SHDN_R_EStop;
    else if (!app_canTx_VC_LEStopOKStatus_get())
        shdn = SHDN_L_EStop;
    else if (!app_canRx_FSM_BOTSOKStatus_get())
        shdn = SHDN_BOTS;
    else if (!app_canRx_CRIT_InertiaSenOKStatus_get())
        shdn = SHDN_Inertia;
    else if (!app_canRx_CRIT_CockpitEStopOKStatus_get())
        shdn = SHDN_Cockpit_EStop;
    else if (!app_canRx_BMS_TSIlckOKStatus_get())
        shdn = SHDN_TS_Ilck;
    else if (!app_canRx_BMS_ImdOk_get())
        shdn = SHDN_IMD_OK;
    else if (!app_canRx_BMS_BmsOk_get())
        shdn = SHDN_BMS_OK;
    else if (!app_canRx_BMS_BspdOk_get())
        shdn = SHDN_BSPD_OK;
    else
        shdn = SHDN_OK;
    app_canTx_VC_FirstFaultNode_set(shdn);
}
