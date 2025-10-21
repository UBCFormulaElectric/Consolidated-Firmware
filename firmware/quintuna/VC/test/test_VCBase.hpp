#pragma once
#include "ecuTestBase.hpp"
#include "io/loadswitchFake.h"
#include <array>

extern "C"
{
#include "jobs.h"
#include "io/loadswitches.h"
#include "app_canRx.h"
#include "app/states/states.h"
}

class VCBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        suppress_heartbeat = false;
        // micro startup simulation
        for (const TI_LoadSwitch &tils : { rl_pump_loadswitch })
        {
            fake::io_loadswitch::reset_tiLoadswitch(const_cast<TI_LoadSwitch &>(tils));
        }
        for (const ST_LoadSwitch &stls :
             { inv_rsm_loadswitch, inv_bms_loadswitch, front_loadswitch, rad_fan_loadswitch })
        {
            fake::io_loadswitch::reset_stLoadswitch(const_cast<ST_LoadSwitch &>(stls));
        }

        register_task(
            [this]
            {
                if (suppress_heartbeat)
                {
                    app_canRx_BMS_Heartbeat_update(true);
                    app_canRx_CRIT_Heartbeat_update(true);
                    app_canRx_FSM_Heartbeat_update(true);
                    app_canRx_RSM_Heartbeat_update(true);
                }
                jobs_run100Hz_tick();
            },
            10);
        register_task(jobs_run1Hz_tick, 1000);

        rl_pump_loadswitch.pgood = false;
        auto *pumpEfuse          = const_cast<Efuse *>(efuse_channels[EFUSE_CHANNEL_RL_PUMP]);
        pumpEfuse->enabled       = true;

        jobs_init();
    }
    void board_teardown() override {}

  public:
    bool suppress_heartbeat = false;

    struct StateMetadata
    {
        const State *state;
        VCState      can_state;
        StateMetadata(const State *s, const VCState cs) : state(s), can_state(cs) {}
    };
    const std::array<StateMetadata, 7> state_metadatas = { {
        { &init_state, VC_INIT_STATE },
        { &inverterOn_state, VC_INVERTER_ON_STATE },
        { &bmsOn_state, VC_BMS_ON_STATE },
        { &pcmOn_state, VC_PCM_ON_STATE },
        { &hvInit_state, VC_HV_INIT_STATE },
        { &hv_state, VC_HV_ON_STATE },
        { &drive_state, VC_DRIVE_STATE },
    } };
};
