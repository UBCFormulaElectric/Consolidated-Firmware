#include "app_states.h"
#include "app_canTx.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_canAlerts.h"
#include "io_pcm.h"

#define HV_READY_VOLTAGE (18.0f)
#define PCM_MAX_VOLTAGE (30.0f)
#define PCM_UNDER_VOLTAGE_TIMEOUT (100) //if voltage doesnt rise above 18V in this amout of time after entering pcmOnState then go into fault state
#define PCM_TOGGLE_TIMEOUT (10) // to retry PCM wait toggle PCM where it stays off for 10ms

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

static TimerChannel *pcm_under_voltage_timer;
static bool toggle_pcm;

static void pcmOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_PCM_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
    toggle_pcm = false; 
    io_pcm_set(true);
    app_timer_init(pcm_under_voltage_timer, PCM_UNDER_VOLTAGE_TIMEOUT);
    app_timer_restart(pcm_under_voltage_timer);

}
static void pcmOnStateRunOnTick1Hz(void) {};
static void pcmOnStateRunOnTick100Hz(void) 
{

    static float pcm_prev_voltage = 0.0f; 
    static float pcm_curr_voltage = 0.0f;

    switch (app_timer_updateAndGetState(pcm_under_voltage_timer))
    {

        case TIMER_STATE_RUNNING:
            pcm_curr_voltage = (float)app_canTx_VC_ChannelOneVoltage_get();
            break;
        
        case TIMER_STATE_EXPIRED:
            if(toggle_pcm)
            {
                // already retried, now go to fault state
                app_timer_stop(pcm_under_voltage_timer);
                app_canAlerts_VC_Fault_PcmUnderVoltageFault_set(true);
                app_stateMachine_setNextState(&fault_state);
            }
            else
            {
                toggle_pcm = true; 
                io_pcm_toggle();
            }
            break;
        
        case TIMER_STATE_IDLE:
            app_timer_restart(pcm_under_voltage_timer);
            break;
    }
    
    if (pcm_curr_voltage > PCM_MAX_VOLTAGE && pcm_prev_voltage)
    {
       
        app_canAlerts_VC_Fault_PcmOveVoltageFault_set(true);
        app_stateMachine_setNextState(&fault_state);
    }

    else if(HV_READY_VOLTAGE < pcm_curr_voltage &&  pcm_curr_voltage <= PCM_MAX_VOLTAGE)
    {
        app_stateMachine_setNextState(&hvInit_state);
    }

    pcm_prev_voltage = pcm_curr_voltage;
    

}
static void pcmOnStateRunOnExit(void) {}

State pcmOn_state = { .name              = "PCM ON",
                      .run_on_entry      = pcmOnStateRunOnEntry,
                      .run_on_tick_1Hz   = pcmOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz,
                      .run_on_exit       = pcmOnStateRunOnExit };