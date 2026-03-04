#pragma once
#include <array>
#include <cstdint>
#include <cassert>
#include "app_timer.hpp"
#include "app_canAlerts.hpp"
#include "io_efuse.hpp"
#include "io_efuses.hpp"

namespace app
{

struct EfuseConfig
{
    bool    efuse_enable{ false };
    uint8_t timeout{ 0 };
    uint8_t max_retry{ 0 };
};

struct PowerManagerConfig
{
    std::array<EfuseConfig, NUM_EFUSE_CHANNELS> efuse_configs{};
};

class PowerManager
{
  public:
    explicit PowerManager(const std::array<io::Efuse *, NUM_EFUSE_CHANNELS> &efuses) : efuses_(efuses)
    {
        for (auto *e : efuses_)
        {
            assert(e != nullptr);
        }
    }

    void init() { app::Timer sequencing_timer = {}; }

    void updateConfig(const PowerManagerConfig &new_cfg)
    {
        state = new_cfg;

        const bool under_v                                      = app::can_alerts::VC_Info_PcmUnderVoltage_get();
        state.efuse_configs[EFUSE_CHANNEL_RL_PUMP].efuse_enable = !under_v;
        state.efuse_configs[EFUSE_CHANNEL_R_RAD].efuse_enable   = !under_v;
    }

    // 100 Hz tasdk
    void efuseProtocolTick_100Hz()
    {
        const auto time_state = sequencing_timer.updateAndGetState();

        switch (time_state)
        {
            case TIMER_STATE_RUNNING:
            default:
                return;

            case TIMER_STATE_EXPIRED:
                // timeout expired or no timeout
                sequencing_timer.stop();
                [[fallthrough]];

            case TIMER_STATE_IDLE:
                sequenceWhileIdle();
                return;
        }
    }

#ifdef TARGET_TEST
    PowerManagerConfig getConfig() const { return state; }
    bool getEfuse(const LoadswitchChannel channel) const { return state_.efuse_configs[channel].efuse_enable; }
#endif

  private:
    void sequenceWhileIdle()
    {
        for (int ch = 0; ch < NUM_EFUSE_CHANNELS; ++ch)
        {
            if (sequencing_timer.updateAndGetState() != TIMER_STATE_IDLE)
            {
                break;
            }

            io::Efuse *efuse = efuses_[ch];
            assert(efuse != nullptr);

            const bool desired = state.efuse_configs[ch].efuse_enable;
            const bool actual  = efuse->isChannelEnabled();

            if (actual == desired)
            {
                continue; // channel already in desired state
            }

            // retry limit logic (same as C)
            if (retries[ch] > state_.efuse_configs[ch].max_retry)
            {
                break;
            }

            if (!desired)
            {
                // case 1: on and trying to turn off
                efuse->setChannel(false);
                continue;
            }

            // case 2: off and trying to turn on
            if (!efuse->ok())
            {
                ++retries[ch];
            }

            const uint8_t timeout = state.efuse_configs[ch].timeout;
            if (timeout != 0)
            {
                app_timer_init(&sequencing_timer, timeout);
                app_timer_restart(&sequencing_timer);
                // After starting a timeout, we stop sequencing more channels this tick
            }

            efuse->setChannel(true);
        }
    }

  private:
    PowerManagerConfig state{};
    TimerChannel       sequencing_timer{};

    std::array<io::Efuse *, NUM_EFUSE_CHANNELS> efuses{};
    std::array<uint8_t, NUM_EFUSE_CHANNELS>     retries{};
};

} // namespace app