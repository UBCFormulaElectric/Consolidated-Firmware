
#include "app_powerSequencing.h"
#include "app_timer.h"
#include "io_loadswitches.h"
#include <stdbool.h>

static TimerChannel *const load_timer;
static const uint32_t      load_delay = 100U;

typedef enum
{
    RL_PUMP,
    RR_PUMP,
    F_PUMP,
    L_RAD,
    R_RAD,
    ALL_LOADS_ON
} LoadOrder;

static LoadOrder turn_on_load = RL_PUMP;

void app_powerSequencing_init(void)
{
    app_timer_init(load_timer, load_delay);
    turn_on_load = RL_PUMP;
}

bool app_powerSequencing_run(void)
{
    app_timer_updateAndGetState(load_timer);

    if (load_timer->state == TIMER_STATE_EXPIRED || load_timer->state == TIMER_STATE_IDLE)
    {
        switch (turn_on_load)
        {
            case RL_PUMP:
                app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_RL_PUMP], true);
                turn_on_load = RR_PUMP;
                break;
            case RR_PUMP:
                app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_RR_PUMP], true);
                turn_on_load = F_PUMP;
                break;
            case F_PUMP:
                app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_F_PUMP], true);
                turn_on_load = L_RAD;
                break;
            case L_RAD:
                app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_L_RAD], true);
                turn_on_load = R_RAD;
                break;
            case R_RAD:
                app_powerManager_setEfuse(efuse_channels[EFUSE_CHANNEL_R_RAD], true);
                turn_on_load = R_RAD;
                break;
            case ALL_LOADS_ON:
                return true;
        }
        app_timer_restart(load_timer);
    }
    return false;
}

bool app_powerSequencing_checkAllLoads(PowerState power_state)
{
    for (uint8_t efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        if (!power_state.efuses[efuse])
        {
            return false;
        }
    }
    return true;
}