#include "app_shdnLast.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"

namespace app::shdnLast
{
using namespace app::can_utils;
static ShutdownNode get_first_shutdown()
{
    // The shutdowns in the accumulator
    if (!app::can_rx::BMS_BmsLatchOk_get())
        return ShutdownNode::BMS_OK;
    if (!app::can_rx::BMS_ImdLatchOk_get())
        return ShutdownNode::IMD_OK;
    if (!app::can_rx::BMS_BspdLatchOk_get())
        return ShutdownNode::BSPD_OK;
    if (!app::can_rx::BMS_HVPShdnOKStatus_get())
        return ShutdownNode::HV_P_ILCK;
    if (!app::can_rx::BMS_HVNShdnOKStatus_get())
        return ShutdownNode::HV_N_ILCK;
    if (!app::can_tx::VC_MSDOrEMeterOKStatus_get())
        return ShutdownNode::MSD_EMETER_ILCK;
    if (!app::can_rx::RSM_RearLeftMotorInterlock_get())
        return ShutdownNode::RL_ILCK;
    if (!app::can_tx::VC_RearRightMotorInterlock_get())
        return ShutdownNode::RR_ILCK;
    if (!app::can_rx::DAM_REStopOKStatus_get())
        return ShutdownNode::R_EStop;
    if (!app::can_rx::DAM_LEStopOKStatus_get())
        return ShutdownNode::L_EStop;
    if (!app::can_rx::FSM_COCKPITOKStatus_get())
        return ShutdownNode::Cockpit_EStop;
    if (!app::can_rx::FSM_BOTSOKStatus_get())
        return ShutdownNode::BOTS;
    if (!app::can_rx::FSM_FrontLeftILCKInertiaOKStatus_get())
        return ShutdownNode::FL_INERTIA_ILCK;
    if (!app::can_rx::FSM_FrontRightILCKOKStatus_get())
        return ShutdownNode::FR_ILCK;
    if (!app::can_tx::VC_TSMSOKStatus_get())
        return ShutdownNode::TSMS;
    return ShutdownNode::OK;
}

static bool consistent(const ShutdownNode first)
{
    switch (first)
    {
        case ShutdownNode::OK:
            if (not app::can_tx::VC_TSMSOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::TSMS:
            if (not app::can_rx::FSM_FrontRightILCKOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::FR_ILCK:
            if (not app::can_rx::FSM_FrontLeftILCKInertiaOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::FL_INERTIA_ILCK:
            if (not app::can_rx::FSM_BOTSOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::BOTS:
            if (not app::can_rx::FSM_COCKPITOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::Cockpit_EStop:
            if (not app::can_rx::DAM_LEStopOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::L_EStop:
            if (not app::can_rx::DAM_REStopOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::R_EStop:
            if (not app::can_tx::VC_RearRightMotorInterlock_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::RR_ILCK:
            if (not app::can_rx::RSM_RearLeftMotorInterlock_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::RL_ILCK:
            if (not app::can_tx::VC_MSDOrEMeterOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::MSD_EMETER_ILCK:
            if (not app::can_rx::BMS_HVNShdnOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::HV_N_ILCK:
            if (not app::can_rx::BMS_HVPShdnOKStatus_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::HV_P_ILCK:
            if (not app::can_rx::BMS_BspdLatchOk_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::BSPD_OK:
            if (not app::can_rx::BMS_ImdLatchOk_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::IMD_OK:
            if (not app::can_rx::BMS_BmsLatchOk_get())
                return false;
            __attribute__((fallthrough));
        case ShutdownNode::BMS_OK:
            return true;
        default:
            return false;
    }
}

void broadcast()
{
    const auto first_shdn_node = get_first_shutdown();
    app::can_tx::VC_FirstFaultNode_set(first_shdn_node);
    app::can_tx::VC_ShdnLoopConsistent_set(consistent(first_shdn_node));
}

} //  namespace app::shdnLast
