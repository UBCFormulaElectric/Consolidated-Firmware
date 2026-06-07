#include "app_powerManager.hpp"
#include "io_efuse_TI_TPS28.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "app_canTx.hpp"

namespace app::powerManager
{
namespace
{
    PowerManagerConfig                                          state_{};
    Timer                                                       sequencing_timer_{ 0 };
    constexpr std::array<const io::Efuse *, NUM_EFUSE_CHANNELS> efuses = { {
        &rr_pump_efuse,
        &rl_pump_efuse,
        &r_rad_fan_efuse,
        &l_rad_fan_efuse,
        &f_inv_efuse,
        &r_inv_efuse,
        &rsm_efuse,
        &bms_efuse,
        &dam_efuse,
        &front_efuse,
    } };
    std::array<uint8_t, NUM_EFUSE_CHANNELS>                     retries_{};

    void sequenceWhileIdle()
    {
        for (size_t ch = 0; ch < NUM_EFUSE_CHANNELS; ch++)
        {
            if (sequencing_timer_.updateAndGetState() != Timer::TimerState::IDLE)
                break;
            const io::Efuse *efuse = efuses[ch];
            assert(efuse != nullptr);

            const bool desired = state_.efuse_configs[ch].efuse_enable;

            if (!desired) // turning off
            {
                efuse->setChannel(false);
                continue;
            }
            assert(desired == true);
            if (efuse->isChannelEnabled()) // already on bruh
            {
                if (not efuse->ok()) // pbad
                {
                    // retry time
                    if (retries_[ch] > state_.efuse_configs[ch].max_retry)
                    {
                        // LOG_WARN("max retry exceedded");
                        continue;
                    }
                    ++retries_[ch];

                    // TODO think about a retry timeout
                    if (const uint32_t timeout = state_.efuse_configs[ch].timeout; timeout != 0)
                    {
                        sequencing_timer_.update_duration(timeout);
                        sequencing_timer_.restart();
                    }
                    efuse->setChannel(false);
                }
            }
            else
            {
                // enable it lmaomao
                if (const uint32_t timeout = state_.efuse_configs[ch].timeout; timeout != 0)
                {
                    sequencing_timer_.update_duration(timeout);
                    sequencing_timer_.restart();
                }
                efuse->setChannel(true);
            }
        }
    }
} // namespace

void updateConfig(const PowerManagerConfig &new_cfg)
{
    state_   = new_cfg;
    retries_ = {};
    sequencing_timer_.stop();

    const bool pcm_volt_ok = not can_tx::VC_CHANNEL4_PCM_UV_get();
    state_.efuse_configs[0].efuse_enable &= pcm_volt_ok;
    state_.efuse_configs[1].efuse_enable &= pcm_volt_ok;
    state_.efuse_configs[2].efuse_enable &= pcm_volt_ok;
    state_.efuse_configs[3].efuse_enable &= pcm_volt_ok;
}
void efuseProtocolTick_100Hz()
{
    switch (sequencing_timer_.updateAndGetState())
    {
        case Timer::TimerState::RUNNING:
        default:
            return;

        case Timer::TimerState::EXPIRED:
            sequencing_timer_.stop();
            // fallthrough so we can start the next sequence right after
            [[fallthrough]];
        case Timer::TimerState::IDLE:
            sequenceWhileIdle();
            return;
    }
}
#ifdef TARGET_TEST
PowerManagerConfig getConfig()
{
    return state_;
}
bool getEfuse(int channel)
{
    return state_.efuse_configs[channel].efuse_enable;
}

#endif
} // namespace app::powerManager