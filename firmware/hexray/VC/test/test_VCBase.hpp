#pragma once

#include "ecuTestBase.hpp"
#include "jobs.hpp"
#include <array>
#include "jobs.hpp"
#include "app_canRx.hpp"
#include "states/app_states.hpp"

using namespace app::states;
using namespace app::can_utils;
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
            const app::State *state;
            VCState can_state;
            StateMetadata(const app::State *s, const VCState cs) : state(s), can_state(cs) {}
        };
        
        const std::array<StateMetadata, 7> state_metadatas = { {
            { &init_state, VCState::VC_INIT_STATE },
            { &inverterOn_state, VCState::VC_INVERTER_ON_STATE },
            { &bmsOn_state, VCState::VC_BMS_ON_STATE },
            { &pcmOn_state, VCState::VC_PCM_ON_STATE },
            { &hvInit_state, VCState::VC_HV_INIT_STATE },
            { &hv_state, VCState::VC_HV_ON_STATE },
            { &drive_state, VCState::VC_DRIVE_STATE },
        } };
};