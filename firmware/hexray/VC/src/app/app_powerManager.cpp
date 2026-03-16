#include "app_powerManager.hpp"
#include "io_tps28_efuse.hpp"
#include "io_tps25_efuse.hpp"
#include "io_efuse.hpp"

app::powerManager
{
    namespace
    {
        PowerManagerConfig                          state_{};
        app::Timer                                  sequencing_timer_{ 0 };
        std::array<io::Efuse *, NUM_EFUSE_CHANNELS> efuses_{};
        std::array<uint8_t, NUM_EFUSE_CHANNELS>     retries_{};

        void sequenceWhileIdle()
        {
            for (int ch = 0; ch < NUM_EFUSE_CHANNELS; ch++)
            {
                if (sequencing_timer_.updateAndGetState() != app::Timer::TimerState::IDLE)
                    break;
                io::Efuse *efuse = efuses_[ch];
                assert(efuse != nullptr);

                const bool desired = state_.efuse_configs[ch].efuse_enable;
                const bool actual  = efuse->isChannelEnabled();

                if (actual == desired)
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

                const uint8_t timeout = state_.efuse_configs[ch].timeout;
                if (timeout != 0)
                    sequencing_timer_ = app::Timer(timeout);

                efuse->setChannel(true);
            }
        }
    } // namespace

    void init()
    {
        static const std::array<io::Efuse *, NUM_EFUSE_CHANNELS> &efuses = {
            &io::loadswitches::RR_PUMP_Efuse, &io::loadswitches::RL_PUMP_Efuse, &io::loadswitches::R_RAD_Efuse,
            &io::loadswitches::L_RAD_Efuse,   &io::loadswitches::F_INV_Efuse,   &io::loadswitches::R_INV_Efuse,
            &io::loadswitches::RSM_Efuse,     &io::loadswitches::BMS_Efuse,     &io::loadswitches::DAM_Efuse,
            &io::loadswitches::FRONT_Efuse,
        };

        for (auto *e : efuses)
        {
            assert(e != nullptr);
            efuses_ = efuses;
        }
    }

    void updateConfig(const PowerManagerConfig &new_cfg)
    {
        state_             = new_cfg;
        const bool under_v = app::can_alerts::VC_Info_PcmUnderVoltage_get();
        state_.efuse_configs[io::loadswitches::RR_PUMP_Efuse].efuse_enable = !under_v;
        state_.efuse_configs[io::loadswitches::R_RAD_Efuse].efuse_enable   = !under_v;
        state_.efuse_configs[io::loadswitches::RL_PUMP_Efuse].efuse_enable = !under_v;
        state_.efuse_configs[io::loadswitches::L_RAD_Efuse].efuse_enable   = !under_v;
    }
    void efuseProtocolTick_100Hz()
    {
        switch (sequencing_timer_.updateAndGetState())
        {
            case app::Timer::TimerState::RUNNING:
            default:
                return;

            case app::Timer::TimerState::EXPIRED:
                sequencing_timer_.stop();
                // fallthrough so we can start the next sequence right after
                [[fallthrough]];
            case app::Timer::TimerState::IDLE:
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
