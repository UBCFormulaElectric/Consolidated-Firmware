#include "app_states.h"
#include "app_canTx.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_canAlerts.h"
#include "io_pcm.h"

#define HV_READY_VOLTAGE (18.0f)
#define PCM_MAX_VOLTAGE (30.0f)
#define PCM_MAX_CURRENT (40.0f)
#define PCM_TIMEOUT \
    (100) // if voltage doesnt rise above 18V in this amout of time after entering pcmOnState then go into fault state

typedef enum
{
    NO_RETRY,
    RETRY_TRIGGERED,
    RETRY_DONE
} pcmRetryStates;

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static TimerChannel   pcm_voltage_in_range_timer;
static TimerChannel   pcm_toggle_timer;
static pcmRetryStates pcm_retry_state;
static float          pcm_curr_voltage;
static float          pcm_prev_voltage;

static bool pcmUnderVoltage(void);
static bool toggleTimer(void);

static void pcmOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_PCM_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
    pcm_retry_state  = NO_RETRY;
    pcm_curr_voltage = 0.0f;
    pcm_prev_voltage = 0.0f;

    io_pcm_set(true);
    app_timer_init(&pcm_voltage_in_range_timer, PCM_TIMEOUT);
    app_timer_init(&pcm_toggle_timer, PCM_TIMEOUT);
    app_timer_restart(&pcm_voltage_in_range_timer);
}
static void pcmOnStateRunOnTick100Hz(void)
{
    if (RETRY_TRIGGERED == pcm_retry_state)
    {
        if (toggleTimer())
        {
            app_timer_restart(&pcm_voltage_in_range_timer);
        }
        else
        {
            return;
        }
    }

    switch (app_timer_updateAndGetState(&pcm_voltage_in_range_timer))
    {
        case TIMER_STATE_RUNNING:
            pcm_curr_voltage = (float)app_canTx_VC_ChannelOneVoltage_get();
            break;

        case TIMER_STATE_EXPIRED:
            if (NO_RETRY == pcm_retry_state)
            {
                pcm_retry_state = RETRY_TRIGGERED;
                io_pcm_set(false); // for retry we turn the pcm off and then turn it on, on the next tick
            }
            else if (RETRY_TRIGGERED == pcm_retry_state)
            {
                // already retried, now go to fault state
                app_timer_stop(&pcm_voltage_in_range_timer);
                pcm_retry_state = RETRY_DONE;
            }
            break;

        case TIMER_STATE_IDLE:
            app_timer_restart(&pcm_voltage_in_range_timer);
            break;
    }

    if (!pcmUnderVoltage())
    {
        app_stateMachine_setNextState(&hvInit_state);
    }
    else if (RETRY_DONE == pcm_retry_state)
    {
        app_canAlerts_VC_Info_PcmUnderVoltage_set(true);
        app_stateMachine_setNextState(&hvInit_state);
    }

    pcm_prev_voltage = pcm_curr_voltage;
}
static void pcmOnStateRunOnExit(void)
{
    app_timer_stop(&pcm_voltage_in_range_timer);
    app_timer_stop(&pcm_toggle_timer);
}

static bool toggleTimer(void)
{
    bool timer_done = false;
    switch (app_timer_updateAndGetState(&pcm_toggle_timer))
    {
        case TIMER_STATE_RUNNING:
            // do nothing
            break;

        case TIMER_STATE_EXPIRED:
            io_pcm_set(true);
            timer_done = true;
            break;

        case TIMER_STATE_IDLE:
            app_timer_restart(&pcm_toggle_timer);
            break;
    }
    return timer_done;
}

static bool pcmUnderVoltage(void)
{
    // debouncing
    if (HV_READY_VOLTAGE <= pcm_curr_voltage && pcm_curr_voltage <= PCM_MAX_VOLTAGE)
    {
        if (HV_READY_VOLTAGE <= pcm_prev_voltage && pcm_prev_voltage <= PCM_MAX_VOLTAGE)
        {
            return false;
        }
    }
    return true;
}

State pcmOn_state = { .name              = "PCM ON",
                      .run_on_entry      = pcmOnStateRunOnEntry,
                      .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz,
                      .run_on_exit       = pcmOnStateRunOnExit };