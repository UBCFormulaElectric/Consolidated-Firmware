#include "app_states.h"

#include "app_canTx.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_canUtils.h"

#include "io_pcm.h"

#include <app_canAlerts.h>
#include <sys/types.h>

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

#define PCM_NOMINAL_VOLTAGE (18.0f)
#define PCM_MAX_VOLTAGE (30.0f)
#define PCM_MAX_CURRENT (40.0f)
// if voltage doesnt rise above 18V in this amout of time after entering pcmOnState then go into fault state
#define PCM_TIMEOUT (100)
static TimerChannel pcm_on_timer;
static TimerChannel pcm_cooldown_timer;
static float        pcm_prev_voltage;

typedef enum
{
    PCM_ON_STATE,
    PCM_COOLDOWN_STATE
} PCM_INTERNAL;
static PCM_INTERNAL state;
#define PCM_MAX_RETRIES (5)
static uint32_t pcm_retries;

static bool pcmOnDone(const float pcm_curr_voltage)
{
    return (
        PCM_NOMINAL_VOLTAGE <= pcm_curr_voltage && pcm_curr_voltage <= PCM_MAX_VOLTAGE &&
        PCM_NOMINAL_VOLTAGE <= pcm_prev_voltage && pcm_prev_voltage <= PCM_MAX_VOLTAGE);
}

static void pcmOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_PCM_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
    pcm_prev_voltage = 0.0f;
    pcm_retries      = 0;
    state            = PCM_ON_STATE;
    app_timer_init(&pcm_on_timer, PCM_TIMEOUT);
    app_timer_init(&pcm_cooldown_timer, PCM_TIMEOUT);
    app_canAlerts_VC_Info_PcmUnderVoltage_set(false);
}

static void pcmOnStateRunOnTick100Hz(void)
{

    const float pcm_curr_voltage = app_canTx_VC_ChannelOneVoltage_get();

    switch (state)
    {
        case PCM_ON_STATE:
            switch (app_timer_updateAndGetState(&pcm_on_timer))
            {
                case TIMER_STATE_IDLE:
                    app_timer_restart(&pcm_on_timer);
                    break;
                case TIMER_STATE_RUNNING:
                    if (pcmOnDone(pcm_curr_voltage))
                    {
                        app_stateMachine_setNextState(&hvInit_state);
                        return;
                    }
                    break;
                case TIMER_STATE_EXPIRED:
                    state = PCM_COOLDOWN_STATE;
                    pcm_retries++;
                    app_canTx_VC_PcmRetryCount_set(pcm_retries);
                    app_timer_stop(&pcm_on_timer);
                    break;
            }
            break;
        case PCM_COOLDOWN_STATE:
            switch (app_timer_updateAndGetState(&pcm_cooldown_timer))
            {
                case TIMER_STATE_IDLE:
                    app_timer_restart(&pcm_cooldown_timer);
                    break;
                case TIMER_STATE_RUNNING:
                    break;
                case TIMER_STATE_EXPIRED:
                    state = PCM_ON_STATE;
                    app_timer_stop(&pcm_cooldown_timer);
                    break;
            }
            break;
    }
    io_pcm_set(state == PCM_ON_STATE);

    pcm_prev_voltage = pcm_curr_voltage;
    if (pcm_retries >= PCM_MAX_RETRIES)
    {
        app_canAlerts_VC_Info_PcmUnderVoltage_set(true);
        app_stateMachine_setNextState(&hvInit_state);
    }
}
static void pcmOnStateRunOnExit(void)
{
    app_timer_stop(&pcm_on_timer);
    app_timer_stop(&pcm_cooldown_timer);
}

State pcmOn_state = { .name              = "PCM ON",
                      .run_on_entry      = pcmOnStateRunOnEntry,
                      .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz,
                      .run_on_exit       = pcmOnStateRunOnExit };