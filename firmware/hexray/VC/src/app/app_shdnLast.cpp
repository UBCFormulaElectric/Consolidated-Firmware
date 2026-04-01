#include "app_shdnLast.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "io_shdnLoopNode.hpp"
#include "app_canUtils.hpp"

namespace app::shdnLast
{
using namespace app::can_utils;
static ShutdownNode get_first_shutdown()
{
    // The shutdowns in the accumulator
    if (!app::can_rx::BMS_BmsLatchOk_get())
        return ShutdownNode::SHDN_BMS_OK;
    if (!app::can_rx::BMS_ImdLatchOk_get())
        return ShutdownNode::SHDN_IMD_OK;
    if (!app::can_rx::BMS_BspdLatchOk_get())
        return ShutdownNode::SHDN_BSPD_OK;
    if (!app::can_rx::BMS_HVPShdnOKStatus_get())
        return ShutdownNode::SHDN_HV_P_ILCK;
    if (!app::can_rx::BMS_HVNShdnOKStatus_get())
        return ShutdownNode::SHDN_HV_N_ILCK;
    if (!app::can_tx::VC_MSDOrEMeterOKStatus_get())
        return ShutdownNode::SHDN_MSD_EMETER_ILCK;
    if (!app::can_rx::RSM_RearLeftMotorInterlock_get())
        return ShutdownNode::SHDN_RL_ILCK;
    if (!app::can_tx::VC_RearRightMotorInterlock_get())
        return ShutdownNode::SHDN_RR_ILCK;
    if (!app::can_rx::DAM_REStopOKStatus_get())
        return ShutdownNode::SHDN_R_EStop;
    if (!app::can_rx::DAM_LEStopOKStatus_get())
        return ShutdownNode::SHDN_L_EStop;
    if (!app::can_rx::FSM_COCKPITOKStatus_get())
        return ShutdownNode::SHDN_Cockpit_EStop;
    if (!app::can_rx::FSM_BOTSOKStatus_get())
        return ShutdownNode::SHDN_BOTS;
    if (!app::can_rx::FSM_FrontLeftILCKInertiaOKStatus_get())
        return ShutdownNode::SHDN_FL_INERTIA_ILCK;
    if (!app::can_rx::FSM_FrontRightILCKOKStatus_get())
        return ShutdownNode::SHDN_FR_ILCK;
    if (!app::can_tx::VC_TSMSOKStatus_get())
        return ShutdownNode::SHDN_TSMS;
    return ShutdownNode::SHDN_OK;
}

void broadcast()
{
    app::can_tx::VC_FirstFaultNode_set(get_first_shutdown());
}

} //  namespace app::shdnLast
