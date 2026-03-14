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
        for (auto *e : efuses)
        {
            assert(e != nullptr);
        }
    }

    void updateConfig(const PowerManagerConfig &new_cfg)
    {
        state_ = new_cfg;

        const bool under_v = app::can_alerts::VC_Info_PcmUnderVoltage_get();
        state_.efuse_configs[io::loadswitches::RL_PUMP_Efuse].efuse_enable = !under_v;
        state_.efuse_configs[io::loadswitches::R_RAD_Efuse].efuse_enable   = !under_v;
    }

    // 100 Hz tasdk
    void efuseProtocolTick_100Hz()
    {
        const auto time_state = sequencing_timer_.updateAndGetState();

        switch (time_state)
        {
            case app::Timer::TimerState::RUNNING:
            default:
                return;

            case app::Timer::TimerState::EXPIRED:
                // timeout expired or no timeout
                sequencing_timer_.stop();
                [[fallthrough]];

            case app::Timer::TimerState::IDLE:
                sequenceWhileIdle();
                return;
        }
    }

#ifdef TARGET_TEST
    PowerManagerConfig getConfig() const { return state_; }
    bool getEfuse(const LoadswitchChannel channel) const { return state_.efuse_configs[channel].efuse_enable; }
#endif

  private:
    PowerManagerConfig state_{};
    app::Timer         sequencing_timer_{ 0 };

    std::array<io::Efuse *, NUM_EFUSE_CHANNELS> efuses_{};
    std::array<uint8_t, NUM_EFUSE_CHANNELS>     retries_{};

    void sequenceWhileIdle()
    {
        for (int ch = 0; ch < NUM_EFUSE_CHANNELS; ++ch)
        {
            if (sequencing_timer_.updateAndGetState() != app::Timer::TimerState::IDLE)
            {
                break;
            }

            io::Efuse *efuse = efuses_[ch];
            assert(efuse != nullptr);

            const bool desired = state_.efuse_configs[ch].efuse_enable;
            const bool actual  = efuse->isChannelEnabled();

            if (actual == desired)
            {
                continue; // channel already in desired state
            }

            // retry limit logic (same as C)
            if (retries_[ch] > state_.efuse_configs[ch].max_retry)
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
                ++retries_[ch];
            }

            const uint8_t timeout = state_.efuse_configs[ch].timeout;
            if (timeout != 0)
            {
                sequencing_timer_ = app::Timer(timeout);
            }

            efuse->setChannel(true);
        }
    }
};

} // namespace app