#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_powerManager.hpp"
#include "io_log.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace bmsOnStates
{

    static void runOnEntry(void)
    {
                LOG_INFO("entering bms on state!");

        static const app::powerManager::PowerManagerConfig power_manager_state = { .efuse_configs = { {
                                                                                       { false, 0, 5 }, // rr_pump
                                                                                       { false, 0, 5 }, // rl_pump
                                                                                       { false, 0, 5 }, // r_rad_fan
                                                                                       { false, 0, 5 }, // l_rad_fan
                                                                                       { true, 0, 5 },  // f_inv
                                                                                       { true, 0, 5 },  // r_inv
                                                                                       { true, 0, 5 },  // rsm
                                                                                       { true, 0, 5 },  // bms
                                                                                       { true, 0, 5 },  // dam
                                                                                       { true, 0, 5 },  // front
                                                                                   } } };
        app::powerManager::updateConfig(power_manager_state);

        app::can_tx::VC_State_set(VCState::VC_BMS_ON_STATE);
    }

    static void runOnTick100Hz(void)
    {
        if (app::can_alerts::BoardHasFault(can_utils::CanNode::BMS_NODE))
        {
            app::StateMachine::set_next_state(&fault_state);
        }
        // Once we have succesfully transitioned here, the BMS will read the state of the VC based on the associated CAN
        // message and then transition to the appropriate stage Note that if the BMS transitons to drive state we
        // transition to PCM_ON state
        if (app::can_rx::BMS_State_get() == BmsState::BMS_DRIVE_STATE)
        {
            app::StateMachine::set_next_state(&pcmOn_state);
        }
    }

    static void runOnExit(void) {
                LOG_INFO("exiting bms on state!");

    }
} // namespace bmsOnStates

State bmsOn_state = { .name              = "BMS ON",
                      .run_on_entry      = bmsOnStates::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = bmsOnStates::runOnTick100Hz,
                      .run_on_exit       = bmsOnStates::runOnExit };
} // namespace app::states
