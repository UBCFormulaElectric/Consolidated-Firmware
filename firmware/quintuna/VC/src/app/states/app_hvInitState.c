#include "app_stateMachine.h"
#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <stdint.h>

static TimerChannel *const load_timer;
static const uint32_t load_delay;
typedef enum{
    RL_PUMP,
    RR_PUMP,
    F_PUMP,
    L_RAD,
    R_RAD,
    ALL_LOADS_ON
} LoadOrder;

static LoadOrder turn_on_load = RL_PUMP;

static const PowerState power_manager_hvInit = {
    .efuses = { [EFUSE_CHANNEL_F_INV]   = true,
                [EFUSE_CHANNEL_RSM]     = true,
                [EFUSE_CHANNEL_BMS]     = true,
                [EFUSE_CHANNEL_R_INV]   = true,
                [EFUSE_CHANNEL_DAM]     = true,
                [EFUSE_CHANNEL_FRONT]   = true,
                [EFUSE_CHANNEL_RL_PUMP] = true, //turn on the first load on entry and then start sequencing after in the 100Hz 
                [EFUSE_CHANNEL_RR_PUMP] = false,
                [EFUSE_CHANNEL_F_PUMP]  = false,
                [EFUSE_CHANNEL_L_RAD]   = false,
                [EFUSE_CHANNEL_R_RAD]   = false },
};

static bool power_sequencing_done = false;
static bool ready_for_drive = true;

static void power_sequencing(){
    app_timer_updateAndGetState(load_timer);

    if (turn_on_load == RL_PUMP){
        app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_RL_PUMP], true);
        app_timer_restart(load_timer);
        turn_on_load = RR_PUMP;
    }

    else if (turn_on_load == RR_PUMP && load_timer->state == TIMER_STATE_EXPIRED) {
        app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_RR_PUMP], true);
        app_timer_restart(load_timer);
        turn_on_load = F_PUMP;
    }
    else if (turn_on_load == F_PUMP && load_timer->state == TIMER_STATE_EXPIRED) {
        app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_F_PUMP], true);
        app_timer_restart(load_timer);
        turn_on_load = L_RAD;
    }
    else if (turn_on_load == L_RAD && load_timer->state == TIMER_STATE_EXPIRED) {
        app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_F_PUMP], true);
        app_timer_restart(load_timer);
        turn_on_load = R_RAD;
    }
    else if (turn_on_load == R_RAD && load_timer->state == TIMER_STATE_EXPIRED) {
        app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_F_PUMP], true);
        app_timer_restart(load_timer);
        turn_on_load = ALL_LOADS_ON;
    }
    else if (turn_on_load == ALL_LOADS_ON) {
        power_sequencing_done = true;
    }
}

static void hvInitStateRunOnEntry(void) {
    app_powerManager_updateConfig(power_manager_hvInit);
    app_canTx_VC_State_set(VC_HV_INIT_STATE);
    app_timer_init(load_timer, load_delay);
}
static void hvInitStateRunOnTick1Hz(void) {}
static void hvInitStateRunOnTick100Hz(void) {
    //Turning on the first 

    //Get the current state of the timer
    power_sequencing();

    if (power_sequencing_done){
        for (uint8_t efuse = 0; efuse<NUM_EFUSE_CHANNELS; efuse++){
            if(!power_manager_hvInit.efuses[efuse]){
                ready_for_drive = false;
            }
        }

        if (ready_for_drive) {
            app_stateMachine_setNextState(&hv_state);
        }
    }



}
static void hvInitStateRunOnExit(void) {}

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitStateRunOnEntry,
                       .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
                       .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                       .run_on_exit       = hvInitStateRunOnExit };
