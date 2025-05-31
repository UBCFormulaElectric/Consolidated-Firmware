#include "app_pcmOnState.h"
#include "app_canUtils.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_stateMachine.h"
#include "app_allStates.h"
#include "app_hvInitState.h"
#include "hw_gpio.h"
#include "hw_gpios.h"

#define TRANSITION_VOLTAGE 18.0f

static void pcmOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_PCM_ON_STATE);

    // Enable PCM
    hw_gpio_writePin(&pcm_en, true);
}

static void pcmOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void pcmOnStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();

    float_t pcm_voltage = app_canTx_VC_ChannelTwoVoltage_get();

    // check PCM voltage (how???)
}

State pcmOn_state = { .name              = "PCM ON",
                      .run_on_entry      = pcmOnStateRunOnEntry,
                      .run_on_tick_1Hz   = pcmOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz };