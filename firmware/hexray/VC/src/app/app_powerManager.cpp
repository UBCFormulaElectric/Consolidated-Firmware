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

            if (const bool actual = efuse->isChannelEnabled(); actual == desired)
                continue;

            if (retries_[ch] > state_.efuse_configs[ch].max_retry)
                break;

            if (!desired)
            {
                efuse->setChannel(false);
                continue;
            }

            if (!efuse->ok())
                ++retries_[ch];

            if (const uint8_t timeout = state_.efuse_configs[ch].timeout; timeout != 0)
                sequencing_timer_ = Timer(timeout);

            efuse->setChannel(true);
        }
    }
} // namespace

void updateConfig(const PowerManagerConfig &new_cfg)
{
    state_                               = new_cfg;
    const bool under_v                   = can_tx::VC_Info_PcmUnderVoltage_get();
    state_.efuse_configs[0].efuse_enable = !under_v;
    state_.efuse_configs[2].efuse_enable = !under_v;
    state_.efuse_configs[1].efuse_enable = !under_v;
    state_.efuse_configs[3].efuse_enable = !under_v;
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
