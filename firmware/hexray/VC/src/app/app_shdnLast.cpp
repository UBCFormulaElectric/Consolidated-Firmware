#include "app_shdnLast.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "io_shdnLoopNode.hpp"
#include "app_canUtils.hpp"

namespace app::shdnLast
{
static app::can_utils::ShutdownNode get_first_shutdown()
{
    // The shutdowns in the accumulator
    if (!app::can_rx::BMS_BmsLatchOk_get())
        return SHDN_BMS_OK;
    if (!app::can_rx::BMS_ImdLatchOk_get())
        return SHDN_IMD_OK;
    if (!app::can_rx::BMS_BspdLatchOk_get())
        return SHDN_BSPD_OK;
    if (!app::can_rx::BMS_HVPShdnOKStatus_get())
        return SHDN_HV_P_ILCK;
    if (!app::can_rx::BMS_HVNShdnOKStatus_get())
        return SHDN_HV_N_ILCK;
    if (!app::can_tx::VC_MSDOrEMeterOKStatus_get())
        return SHDN_MSD_EMETER_ILCK;

    // RSMs Rl and RR interlocks
    if (!app::can_rx::RSM_RearLeftMotorInterlock_get())
        return SHDN_RL_ILCK;
    // RSM or VC?
    if (!app::can_tx::VC_RearRightMotorInterlock_get())
        return SHDN_RR_ILCK;

    // right and left estops
    if (!app::can_rx::DAM_REStopOKStatus_get())
        return SHDN_R_EStop;
    if (!app::can_rx::DAM_LEStopOKStatus_get())
        return SHDN_L_EStop;

    // cockpit for fsm
    if (!app::can_rx::FSM_COCKPITOKStatus_get())
        return SHDN_Cockpit_EStop;

    if (!app::can_rx::FSM_BOTSOKStatus_get())
        return SHDN_BOTS;

    if (!app::can_rx::FSM_FrontLeftILCKInertiaOKStatus_get())
        return SHDN_FL_INERTIA_ILCK;
    if (!app::can_rx::FSM_FrontRightILCKOKStatus_get())
        return SHDN_FR_ILCK;

    if (!app::can_tx::VC_TSMSOKStatus_get())
        return SHDN_TSMS;
    return SHDN_OK;
}

void broadcast()
{
    app::can_tx::VC_FirstFaultNode_set(get_first_shutdown());
}

} //  namespace app::shdnLast
