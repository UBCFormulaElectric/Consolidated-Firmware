#include "app_allStates.h"
#include "app_bmsOnState.h"
#include "app_canUtils.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_stateMachine.h"
#include "app_pcmOnState.h"

static void bmsOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_BMS_ON_STATE);

    // Notify BMS to go into precharge
    app_canTx_VC_EnablePrecharge_set(true);
}

static void bmsOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void bmsOnStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();

    bool bms_drive_state = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    if (bms_drive_state)
    {
        app_stateMachine_setNextState(&pcmOn_state);
    }
}

static void bmsOnStateRunOnExit(void) {}

State bmsOn_state = { .name              = "BMS ON",
                      .run_on_entry      = bmsOnStateRunOnEntry,
                      .run_on_tick_1Hz   = bmsOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = bmsOnStateRunOnTick100Hz,
                      .run_on_exit       = bmsOnStateRunOnExit };
