#include "app_shdnLast.h"
#include "app_canTx.h"
#include "app_canRx.h"

void app_shdnLast_broadcast(void)
{
    ShutdownNode shdn;
    if (!app_canRx_BMS_HVDShdnOKStatus_get())
        shdn = SHDN_BMS_HVD_ILCK;
    else if (!app_canTx_VC_SplitterBoxInterlockOKStatus_get())
        shdn = SHDN_VC_SB_ILCK;
    else if (!app_canTx_VC_TSMSOKStatus_get())
        shdn = SHDN_VC_TSMS;
    else if (!app_canTx_VC_REStopOKStatus_get())
        shdn = SHDN_VC_REstop;
    else if (!app_canTx_VC_LEStopOKStatus_get())
        shdn = SHDN_VC_LEstop;
    else if (!app_canRx_FSM_BOTSOKStatus_get())
        shdn = SHDN_FSM_BOTS;
    else if (!app_canRx_CRIT_InertiaSenOKStatus_get())
        shdn = SHDN_CRIT_Inertia;
    else if (!app_canRx_CRIT_CockpitEStopOKStatus_get())
        shdn = SHDN_CRIT_Cockpit_EStop;
    else if (!app_canRx_BMS_TSIlckOKStatus_get())
        shdn = SHDN_BMS_TS_Ilck;
    else if (!app_canRx_BMS_ImdOk_get())
        shdn = SHDN_BMS_IMD;
    else if (!app_canRx_BMS_BmsOk_get())
        shdn = SHDN_BMS_OK;
    else if (!app_canRx_BMS_BspdOk_get())
        shdn = SHDN_BMS_BSPD;
    else
        shdn = SHDN_OK;
    app_canTx_VC_FirstFaultNode_set(shdn);
}
