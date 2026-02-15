#include "app_shdnLast.hpp"
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include <app_canUtils.h>
}

namespace app::shdnLast
{
static ShutdownNode get_first_shutdown()
{
    // The shutdowns in the accumulator  
    if (!app::can::rx::BMS_BmsLatchOk_get())
        return SHDN_BMS_OK;

    // In the Splitter idk which is for emeter
    if (!app::can::rx::BMS_ImdLatchOk_get())
        return SHDN_IMD_OK;
    if (!app::can::rx::BMS_BspdLatchOk_get())
        return SHDN_BSPD_OK;
        // IDK WHERE THESE GUYS GO!! currently not on the diagram
    // if (!app::can::rx::BMS_HVPShdnOKStatus_get())
    //     return SHDN_HV_P_ILCK;
    // if (!app::can::rx::BMS_HVNShdnOKStatus_get())
    //     return SHDN_HV_N_ILCK;

    if (!app:can::tx::VC_MSDOrEMeterOKStatus_get())
        return SHDN_MSD_EMETER_ILCK;

    // RSMs Rl and RR interlocks 
    if (!app::can::rx::RSM_RearLeftMotorInterlock_get())
        return SHDN_RL_ILCK;
    // RSM or VC?
    if (!app:can::tx::VC_RearRightMotorInterlock_get())
        return SHDN_RR_ILCK;

    // right and left estops
    if (!app::can::rx::DAM_REStopOKStatus_get())
        return SHDN_R_EStop;
    if (!app::can::rx::DAM_LEStopOKStatus_get())
        return SHDN_L_EStop;

    // cockpit for fsm
    if (!app::can::rx::FSM_COCKPITOKStatus_get())
        return SHDN_Cockpit_EStop;

    if (!app::can::rx::FSM_BOTSOKStatus_get())
        return SHDN_BOTS;

    if (!app::can::rx::FSM_FrontLeftILCKInertiaOKStatus_get())
        return SHDN_FL_INERTIA_ILCK;
    if (!app::can::rx::FSM_FrontRightILCKOKStatus_get())
        return SHDN_FR_ILCK;

    // idk if this is right signal cuz its FL interial? I think I just need inertia 
    if (!app::can::tx::VC_TSMSOKStatus_get())
        return SHDN_TSMS;
    return SHDN_OK;
}

void broadcast()
{
    // REFACTOR!
    app::can::tx::VC_FirstFaultNode_set(get_first_shutdown());
}

} //  namespace app::shdnLast
