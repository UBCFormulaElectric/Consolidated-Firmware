#include "app_shdnLast.hpp"
#include "io_shdnLoopNode.hpp"
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include <app_canUtils.h>
}

namespace vc::app::shdnLast
{
static ShutdownNode get_first_shutdown()
{
    if (!app_canRx_BMS_BmsLatchOk_get())
        return SHDN_BMS_OK;
    if (!app_canRx_BMS_ImdLatchOk_get())
        return SHDN_IMD_OK;
    if (!app_canRx_BMS_BspdLatchOk_get())
        return SHDN_BSPD_OK;
    if (!app_canRx_BMS_HVPShdnOKStatus_get())
        return SHDN_HV_P_ILCK;
    if (!app_canRx_BMS_HVNShdnOKStatus_get())
        return SHDN_HV_N_ILCK;
    if (!app_canTx_VC_MSDOrEMeterOKStatus_get())
        return SHDN_MSD_EMETER_ILCK;
    if (!app_canRx_RSM_RearLeftMotorInterlock_get())
        return SHDN_RL_ILCK;
    if (!app_canTx_VC_RearRightMotorInterlock_get())
        return SHDN_RR_ILCK;
    if (!app_canRx_DAM_REStopOKStatus_get())
        return SHDN_R_EStop;
    if (!app_canRx_DAM_LEStopOKStatus_get())
        return SHDN_L_EStop;
    if (!app_canRx_FSM_COCKPITOKStatus_get())
        return SHDN_Cockpit_EStop;
    if (!app_canRx_FSM_FrontLeftILCKInertiaOKStatus_get())
        return SHDN_FL_INERTIA_ILCK;
    if (!app_canRx_FSM_BOTSOKStatus_get())
        return SHDN_BOTS;
    if (!app_canRx_FSM_FrontRightILCKOKStatus_get())
        return SHDN_FR_ILCK;
    if (!app_canTx_VC_TSMSOKStatus_get())
        return SHDN_TSMS;
    return SHDN_OK;
}

void app_shdnLast_broadcast(void)
{
    app_canTx_VC_FirstFaultNode_set(get_first_shutdown());
}

} //  namespace vc::app::shdnLast
