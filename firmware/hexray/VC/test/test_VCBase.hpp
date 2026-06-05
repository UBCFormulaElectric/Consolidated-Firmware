#pragma once

#include "ecuTestBase.hpp"
#include "jobs.hpp"
#include <array>
#include "jobs.hpp"
#include "app_canRx.hpp"
#include "states/app_states.hpp"

class VCBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        suppress_heartbeat = false;

        register_task(jobs_run1Hz_tick, 1000);
        register_task(
            [this]
            {
                if (suppress_heartbeat)
                {
                    app::can_rx::RSM_Heartbeat_update(true);
                    app::can_rx::BMS_Heartbeat_update(true);
                    app::can_rx::DAM_Heartbeat_update(true);
                    app::can_rx::FSM_Heartbeat_update(true);
                    app::can_rx::CRIT_Heartbeat_update(true);
                }
                jobs_run100Hz_tick();
            },
            10);

        register_task(jobs_run1kHz_tick, 1);

        jobs_init();
    }
    void board_teardown() override {}

  public:
    bool suppress_heartbeat = false;

    struct StateMetadata
    {
        const app::State       *state;
        app::can_utils::VCState can_state;
        StateMetadata(const app::State *s, const app::can_utils::VCState cs) : state(s), can_state(cs) {}
    };

    const std::array<StateMetadata, 7> state_metadatas = { {
        { &app::states::init_state, app::can_utils::VCState::VC_INIT_STATE },
        { &app::states::inverterOn_state, app::can_utils::VCState::VC_INVERTER_ON_STATE },
        { &app::states::bmsOn_state, app::can_utils::VCState::VC_BMS_ON_STATE },
        { &app::states::pcmOn_state, app::can_utils::VCState::VC_PCM_ON_STATE },
        { &app::states::hvInit_state, app::can_utils::VCState::VC_HV_INIT_STATE },
        { &app::states::hv_state, app::can_utils::VCState::VC_HV_ON_STATE },
        { &app::states::drive_state, app::can_utils::VCState::VC_DRIVE_STATE },
    } };
};
