#include "app_stateMachine.hpp"
#include "app_canTx.hpp"
#include "app_states.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"
#include "app_timer.hpp"
#include "app_powerManager.hpp"
#include "io_log.hpp"
#include "io_pcm.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace pcmOnState
{
    constexpr float   PCM_NOMINAL_VOLTAGE = 20.0f;
    constexpr float   PCM_MAX_VOLTAGE     = 28.0f;
    constexpr float   PCM_MAX_CURRENT     = 40.0f;
    constexpr uint8_t PCM_MAX_RETRIES     = 5;

    constexpr uint32_t PCM_TIMOUT = 1000;

    static Timer   pcm_timer{ PCM_TIMOUT };
    static Timer   pcm_cooldown_timer{ PCM_TIMOUT };
    static uint8_t pcm_retries;
    static float   pcm_prev_voltage;

    enum : uint8_t
    {
        PCM_ON_STATE,
        PCM_COOLDOWN_STATE,
    } pcm_retry_states;

    static bool pcmOnDone(const float pcm_voltage)
    {
        return (
            PCM_NOMINAL_VOLTAGE <= pcm_voltage && pcm_voltage <= PCM_MAX_VOLTAGE &&
            PCM_NOMINAL_VOLTAGE <= pcm_prev_voltage && pcm_prev_voltage <= PCM_MAX_VOLTAGE);
    }

    static void runOnEntry()
    {
        LOG_INFO("entering pcm on state!");
        static constexpr powerManager::Efuses<powerManager::EfuseConfig> power_manager_state = {
            .front_efuse     = { true, 0, 5 },    // front
            .rsm_efuse       = { true, 0, 5 },    // rsm
            .bms_efuse       = { true, 0, 5 },    // bms
            .dam_efuse       = { true, 0, 5 },    // dam
            .f_inv_efuse     = { true, 0, 5 },    // f_inv
            .r_inv_efuse     = { true, 0, 5 },    // r_inv
            .r_rad_fan_efuse = { false, 200, 5 }, // r_rad_fan
            .l_rad_fan_efuse = { false, 200, 5 }, // l_rad_fan
            .rr_pump_efuse   = { false, 200, 5 }, // rr_pump
            .rl_pump_efuse   = { false, 200, 5 }, // rl_pump
        };
        app::powerManager::updateConfig(power_manager_state);

        can_tx::VC_State_set(VCState::VC_PCM_ON_STATE);
        pcm_prev_voltage = 0.0f;
        pcm_retries      = 0;
        pcm_retry_states = PCM_ON_STATE;
        can_alerts::infos::PcmUnderVoltage_set(false);
    }

    static void runOnTick100Hz()
    {
        if (can_rx::BMS_State_get() == BmsState::BMS_INIT_STATE)
        {
            StateMachine::set_next_state(&init_state);
        }
        else
        {
            const float pcm_curr_voltage = can_tx::VC_PcmChannelVoltage_get();
            switch (pcm_retry_states)
            {
                case PCM_ON_STATE:
                {
                    switch (pcm_timer.updateAndGetState())
                    {
                        case Timer::TimerState::IDLE:
                            pcm_timer.restart();
                            break;
                        case Timer::TimerState::RUNNING:
                            if (pcmOnDone(pcm_curr_voltage))
                            {
                                StateMachine::set_next_state(&hvInit_state);
                                return;
                            }
                            break;
                        case Timer::TimerState::EXPIRED:
                            pcm_retry_states = PCM_COOLDOWN_STATE;
                            pcm_retries++;
                            can_tx::VC_PcmRetryCount_set(pcm_retries);
                            pcm_timer.stop();
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case PCM_COOLDOWN_STATE:
                    switch (pcm_cooldown_timer.updateAndGetState())
                    {
                        case Timer::TimerState::IDLE:
                            pcm_cooldown_timer.restart();
                            break;
                        case Timer::TimerState::RUNNING:
                            break;
                        case Timer::TimerState::EXPIRED:
                            pcm_retry_states = PCM_ON_STATE;
                            pcm_cooldown_timer.stop();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            io::pcm::set(pcm_retry_states == PCM_ON_STATE);

            pcm_prev_voltage = pcm_curr_voltage;
            if (pcm_retries >= PCM_MAX_RETRIES)
            {
                can_alerts::infos::PcmUnderVoltage_set(true);
                StateMachine::set_next_state(&fault_state);
            }
        }
    }

    static void runOnExit()
    {
        LOG_INFO("exiting pcm on state!");
        pcm_timer.stop();
        pcm_cooldown_timer.stop();
    }

} // namespace pcmOnState

State pcmOn_state = { .name              = "PCM_ON",
                      .run_on_entry      = pcmOnState::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = pcmOnState::runOnTick100Hz,
                      .run_on_exit       = pcmOnState::runOnExit };
} // namespace app::states
