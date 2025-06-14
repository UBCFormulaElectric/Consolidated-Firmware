#include "app_pumpControl.h"
#include "io_rPump.h"
#include "io_time.h"
#include <app_canTx.h>

#define SLOPE (0.5f)

static bool     finished_ramp_up = false;
static uint16_t time             = 0;

#define CURRENT_THRESH 0.025f

static inline void pumpControl_rampUp(void)
{
    // if we are done ramping up note that we are not ramping up

    if (finished_ramp_up)
    {
        return;
    }
    // calculate percentage based on defined slope above
    uint8_t percentage = (uint8_t)(SLOPE * time);

    io_rPump_setPercentage(percentage);
    app_canTx_FSM_PumpRampUpSetPoint_set((uint32_t)percentage);

    if (percentage == 100)
    {
        time             = 0;
        finished_ramp_up = true;
    }
}

static inline void pumpControl_stopFlow(void)
{
    io_rPump_setPercentage(0);

    app_canTx_VC_PumpFailure_set(true);
    finished_ramp_up = false;
    time             = 0;
}

void app_pumpControl_MonitorPumps(void)
{
    time += 10;

    if (app_canRx_VC_RsmTurnOnPump())
        pumpControl_rampUp();
    else
        pumpControl_stopFlow();
}