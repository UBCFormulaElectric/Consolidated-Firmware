#include "app_states.hpp"
#include "app_timer.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "io_log.hpp"
#include "app_canUtils.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace hvInitState
{
    static VCInverterState current_inverter_state;
    constexpr uint32_t     INV_QUIT_TIMEOUT_MS = 10 * 1000;

    static Timer start_up_timer{ INV_QUIT_TIMEOUT_MS };

    static void reset_inverter_power_requests(void)
    {
        // FL FR RL RR
        inverter_on_toggle(false, false, false, false);
        inverter_enable_toggle(false, false, false, false);
        inverter_dc_toggle(false, false, false, false);
    }

    static void runOnEntry(void)
    {
        app::can_tx::VC_State_set(VCState::VC_HV_INIT_STATE);

        reset_inverter_power_requests();

        // FL FR RL RR
        send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        set_torque_limit_negative(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        set_torque_limit_positive(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
    }

    static void runOnTick100Hz(void)
    {
        const bool inv_bsystemReady = app::can_rx::INVFL_bSystemReady_get() && app::can_rx::INVFR_bSystemReady_get() &&
                                      app::can_rx::INVRL_bSystemReady_get() && app::can_rx::INVRR_bSystemReady_get();

        const bool inverter_dc_quit = app::can_rx::INVFL_bQuitDcOn_get() && app::can_rx::INVFR_bQuitDcOn_get() &&
                                      app::can_rx::INVRL_bQuitDcOn_get() && app::can_rx::INVRR_bQuitDcOn_get();

        const bool inverter_invOn_quit =
            app::can_rx::INVFL_bQuitInverterOn_get() && app::can_rx::INVFR_bQuitInverterOn_get() &&
            app::can_rx::INVRL_bQuitInverterOn_get() && app::can_rx::INVRR_bQuitInverterOn_get();

        for (bool retry_transition = true; retry_transition;)
        {
            retry_transition = false;

            switch (current_inverter_state)
            {
                case VCInverterState::INV_SYSTEM_READY:
                    if (inv_bsystemReady)
                    {
                        LOG_INFO("inv_system_ready -> inv_dc_on");
                        current_inverter_state = VCInverterState::INV_DC_ON;
                        retry_transition       = true;
                        start_up_timer.stop();

                        // Error reset should be set to false cause we were successful
                        inverter_bError_reset_set(false, false, false, false);
                    }
                    // else if (app_canAlerts_VC_Info_InverterRetry_get())
                    // {
                    //     LOG_INFO("inv_system_ready -> inv_error_retry");
                    //     current_inverter_state = INV_ERROR_RETRY;
                    // }
                    break;
                case VCInverterState::INV_DC_ON:
                    if (!inv_bsystemReady)
                    {
                        current_inverter_state = VCInverterState::INV_SYSTEM_READY;
                        retry_transition       = true;
                        break;
                    }

                    if (inverter_dc_quit)
                    {
                        LOG_INFO("inv_dc_on -> inv_enable");
                        current_inverter_state = VCInverterState::INV_ENABLE;
                        // retry_transition       = true; // TODO we need a tick in INV_ENABLE?
                        start_up_timer.stop();
                    }
                    else if (start_up_timer.runIfCondition(!inverter_dc_quit) == app::Timer::TimerState::EXPIRED)
                    {
                        LOG_INFO("dc quit timeout");
                        current_inverter_state = VCInverterState::INV_SYSTEM_READY;
                        retry_transition       = true;
                    }

                    break;

                case VCInverterState::INV_ENABLE:
                {
                    if (!(inv_bsystemReady && inverter_dc_quit))
                    {
                        current_inverter_state = VCInverterState::INV_DC_ON;
                        retry_transition       = true;
                        break;
                    }

                    current_inverter_state = VCInverterState::INV_INVERTER_ON;
                    // retry_transition       = true; // TODO if we did this might as well consolidate the states
                    break;
                }

                case VCInverterState::INV_INVERTER_ON:
                    if (!(inv_bsystemReady && inverter_dc_quit))
                    {
                        current_inverter_state = VCInverterState::INV_ENABLE;
                        retry_transition       = true;
                        break;
                    }

                    if (inverter_invOn_quit)
                    {
                        LOG_INFO("inv_on -> inv_ready_for_drive");
                        current_inverter_state = VCInverterState::INV_READY_FOR_DRIVE;
                        retry_transition       = true;
                        start_up_timer.stop();
                    }
                    else if (start_up_timer.runIfCondition(!inverter_invOn_quit) == app::Timer::TimerState::EXPIRED)
                    {
                        LOG_INFO("inv on quit timeout");
                        current_inverter_state = VCInverterState::INV_SYSTEM_READY;
                        retry_transition       = true;
                    }

                    break;
                case VCInverterState::INV_READY_FOR_DRIVE: // locked in to drive
                    if (!(inv_bsystemReady && inverter_dc_quit && inverter_invOn_quit))
                    {
                        current_inverter_state = VCInverterState::INV_INVERTER_ON;
                        retry_transition       = true;
                    }
                    break;
                case VCInverterState::INV_ERROR_RETRY:
                default:
                    break;
            }

            app::can_tx::VC_InverterState_set(current_inverter_state);
        }

        reset_inverter_power_requests();
        switch (current_inverter_state)
        {
            case VCInverterState::INV_INVERTER_ON:
                inverter_on_toggle(true, true, true, true);
                __attribute__((fallthrough));
            case VCInverterState::INV_ENABLE:
                inverter_enable_toggle(true, true, true, true);
                __attribute__((fallthrough));
            case VCInverterState::INV_DC_ON:
                inverter_dc_toggle(true, true, true, true);
                break;
            case VCInverterState::INV_READY_FOR_DRIVE:
                app::StateMachine::set_next_state(&hv_state);
                break;
            case VCInverterState::INV_ERROR_RETRY:
                start_up_timer.stop();
            default:
            case VCInverterState::INV_SYSTEM_READY:
                break;
        }
    }

    static void runOnExit(void)
    {
        current_inverter_state = VCInverterState::INV_SYSTEM_READY;
        start_up_timer.stop();
        // app_canAlerts_VC_Info_InverterRetry_set(false);
    }

} // namespace hvInitState

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitState::runOnEntry,
                       .run_on_tick_1Hz   = nullptr,
                       .run_on_tick_100Hz = hvInitState::runOnTick100Hz,
                       .run_on_exit       = hvInitState::runOnExit };
} // namespace app::states
