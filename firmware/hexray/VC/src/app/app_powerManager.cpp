#include "app_powerManager.hpp"
#include "app_canTx.hpp"
#include "app_timer.hpp"
#include "io_efuses.hpp"

#include <cassert>

namespace app::powerManager
{
namespace
{
    constexpr size_t    NUM_EFUSE_CHANNELS = 10;
    Efuses<EfuseConfig> state_{};
    Timer               sequencing_timer_{ 0 };

    constexpr Efuses<const io::Efuse *> efuses = {
        &rr_pump_efuse, &rl_pump_efuse, &r_rad_fan_efuse, &l_rad_fan_efuse, &f_inv_efuse,
        &r_inv_efuse,   &rsm_efuse,     &bms_efuse,       &dam_efuse,       &front_efuse,
    };

    constexpr Efuses efuse_status_setters = {
        app::can_tx::VC_RearRightPumpStatus_set,
        app::can_tx::VC_RearLeftPumpStatus_set,
        app::can_tx::VC_RightRadiatorFanStatus_set,
        app::can_tx::VC_LeftRadiatorFanStatus_set,
        app::can_tx::VC_FrontInvertersStatus_set,
        app::can_tx::VC_RearInvertersStatus_set,
        app::can_tx::VC_RSMStatus_set,
        app::can_tx::VC_BMSStatus_set,
        app::can_tx::VC_DAMStatus_set,
        app::can_tx::VC_FrontStatus_set,
    };

    constexpr Efuses efuse_current_setters = {
        app::can_tx::VC_RearRightPumpCurrent_set,
        app::can_tx::VC_RearLeftPumpCurrent_set,
        app::can_tx::VC_RightRadiatorFanCurrent_set,
        app::can_tx::VC_LeftRadiatorFanCurrent_set,
        app::can_tx::VC_FrontInvertersCurrent_set,
        app::can_tx::VC_RearInvertersCurrent_set,
        app::can_tx::VC_RSMCurrent_set,
        app::can_tx::VC_BMSCurrent_set,
        app::can_tx::VC_DAMCurrent_set,
        app::can_tx::VC_FrontCurrent_set,
    };

    std::array<uint8_t, NUM_EFUSE_CHANNELS> retries_{};

    void sequenceWhileIdle()
    {
        for (size_t ch = 0; ch < NUM_EFUSE_CHANNELS; ch++)
        {
            if (sequencing_timer_.updateAndGetState() != Timer::TimerState::IDLE)
                break;
            const io::Efuse *efuse = efuses[ch];
            assert(efuse != nullptr);

            const bool desired = state_[ch].efuse_enable;

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
                    if (retries_[ch] > state_[ch].max_retry)
                    {
                        continue;
                    }
                    ++retries_[ch];

                    // TODO think about a retry timeout
                    if (const uint32_t timeout = state_[ch].timeout; timeout != 0)
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
                if (const uint32_t timeout = state_[ch].timeout; timeout != 0)
                {
                    sequencing_timer_.update_duration(timeout);
                    sequencing_timer_.restart();
                }
                efuse->setChannel(true);
            }
        }
    }
} // namespace

void updateConfig(const Efuses<EfuseConfig> &new_cfg)
{
    state_   = new_cfg;
    retries_ = {};
    sequencing_timer_.stop();

    // TODO these are completely unnecessary checks which should
    // const bool pcm_volt_ok = not can_tx::VC_CHANNEL4_PCM_UV_get();
    // state_.l_rad_fan_efuse.efuse_enable &= pcm_volt_ok;
    // state_.r_rad_fan_efuse.efuse_enable &= pcm_volt_ok;
    // state_.rl_pump_efuse.efuse_enable &= pcm_volt_ok;
    // state_.rr_pump_efuse.efuse_enable &= pcm_volt_ok;
}

void broadcastRetryCounts()
{
    app::can_tx::VC_RearRightPumpRetry_set(retries_[0]);
    app::can_tx::VC_RearLeftPumpRetry_set(retries_[1]);
    app::can_tx::VC_RightRadiatorRetry_set(retries_[2]);
    app::can_tx::VC_LeftRadiatorRetry_set(retries_[3]);
    app::can_tx::VC_LInvRetry_set(retries_[4]);
    app::can_tx::VC_RInvRetry_set(retries_[5]);
    app::can_tx::VC_RSMRetry_set(retries_[6]);
    app::can_tx::VC_BMSRetry_set(retries_[7]);
    app::can_tx::VC_DAMRetry_set(retries_[8]);
    app::can_tx::VC_FrontRetry_set(retries_[9]);
}

void efuseProtocolTick_100Hz()
{
    switch (sequencing_timer_.updateAndGetState())
    {
        case Timer::TimerState::RUNNING:
            break;
        default:
            assert(false);
            break;
        case Timer::TimerState::EXPIRED:
            sequencing_timer_.stop();
            // fallthrough so we can start the next sequence right after
            [[fallthrough]];
        case Timer::TimerState::IDLE:
            sequenceWhileIdle();
            break;
    }

    // broadcast new state
    for (size_t ch = 0; ch < NUM_EFUSE_CHANNELS; ch++)
    {
        const io::Efuse *efuse = efuses[ch];
        assert(efuse != nullptr);
        efuse_status_setters[ch](efuse->isChannelEnabled());
        efuse_current_setters[ch](efuse->getChannelCurrent());
    }
}
#ifdef TARGET_TEST
Efuses<EfuseConfig> getConfig()
{
    return state_;
}
bool getEfuse(int channel)
{
    return state_.efuse_configs[channel].efuse_enable;
}

#endif
} // namespace app::powerManager