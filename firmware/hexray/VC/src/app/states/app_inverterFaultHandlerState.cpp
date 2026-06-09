#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "io_log.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace inverterFaultHandler
{

    static void runOnEntry()
    {
        LOG_INFO("entering inverter fault handler state!");

        can_tx::VC_State_set(VCState::VC_INVERTER_FAULT_HANDELER);
        can_alerts::infos::InverterRetry_set(true);
    }

    static void runOnTick100Hz()
    {
        switch (inverter::FaultHandler())
        {
            case inverter::FaultHandlerState::INV_FAULT_RECOVERED:
            {
                StateMachine::set_next_state(inverter::recovery_state());
                break;
            }
            case inverter::FaultHandlerState::INV_FAULT_LOCKOUT:
            {
                can_tx::VC_Fault_InvLockoutFault_set(true);
                StateMachine::set_next_state(&fault_state);
                break;
            }
            case inverter::FaultHandlerState::INV_FAULT_RETRY:
            default:
                break;
        }
    }

    static void runOnExit()
    {
        LOG_INFO("exiting inverter fault handler state!");
        can_alerts::infos::InverterRetry_set(false);
    }

} // namespace inverterFaultHandler

State inverter_fault_handling_state = { .name              = "INVERTER FAULT HANDLER STATE",
                                        .run_on_entry      = inverterFaultHandler::runOnEntry,
                                        .run_on_tick_1Hz   = nullptr,
                                        .run_on_tick_100Hz = inverterFaultHandler::runOnTick100Hz,
                                        .run_on_exit       = inverterFaultHandler::runOnExit };

} // namespace app::states
