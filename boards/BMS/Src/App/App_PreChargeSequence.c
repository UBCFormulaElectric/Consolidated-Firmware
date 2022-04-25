#include <assert.h>
#include <stdlib.h>
#include "App_PreChargeSequence.h"

#define PRECHARGE_THRESHOLD 0.90f // EV.6.6.1: at least 90%
#define PRECHARGE_TIME_LOWER_BOUND_PERCENTAGE 0.10f
#define PRECHARGE_TIME_UPPER_BOUND_PERCENTAGE 1.50f
#define PRECHARGE_RESISTANCE_OHMS (1000U)
// COEFFICIENT_TO_THRESHOLD is the result of solving the exponential (for time
// to threshold) divided by RC (i.e. 4 would correspond to 98%)
#define COEFFICIENT_TO_THRESHOLD 2.30f

struct PreChargeSequence
{
    void (*enable_pre_charge_sequence)(void);
    void (*disable_pre_charge_sequence)(void);
};

struct PreChargeSequence *App_PreChargeSequence_Create(
    void (*enable_pre_charge_sequence)(void),
    void (*disable_pre_charge_sequence)(void))
{
    struct PreChargeSequence *pre_charge_sequence =
        malloc(sizeof(struct PreChargeSequence));
    assert(pre_charge_sequence != NULL);

    pre_charge_sequence->enable_pre_charge_sequence =
        enable_pre_charge_sequence;
    pre_charge_sequence->disable_pre_charge_sequence =
        disable_pre_charge_sequence;

    return pre_charge_sequence;
}

void App_PreChargeSequence_Destroy(
    struct PreChargeSequence *pre_charge_sequence)
{
    free(pre_charge_sequence);
}

void App_PreChargeSequence_Enable(
    const struct PreChargeSequence *const pre_charge_sequence)
{
    pre_charge_sequence->enable_pre_charge_sequence();
}

void App_PreChargeSequence_Disable(
    const struct PreChargeSequence *const pre_charge_sequence)
{
    pre_charge_sequence->disable_pre_charge_sequence();
}

float App_PreChargeSequence_GetThreshold_Percent(
    const struct PreChargeSequence *const pre_charge_sequence)
{
    return PRECHARGE_THRESHOLD;
}

uint32_t App_PreChargeSequence_GetUpperTimeBound_MS(
    const struct PreChargeSequence *const pre_charge_sequence,
    float                                 tractive_system_capacitance)
{
    return (uint32_t)(COEFFICIENT_TO_THRESHOLD * PRECHARGE_RESISTANCE_OHMS *
           tractive_system_capacitance * (float)PRECHARGE_TIME_UPPER_BOUND_PERCENTAGE);
}

uint32_t App_PreChargeSequence_GetLowerTimeBound_MS(
    const struct PreChargeSequence *const pre_charge_sequence,
    float                                 tractive_system_capacitance)
{
    return (uint32_t)(COEFFICIENT_TO_THRESHOLD * PRECHARGE_RESISTANCE_OHMS *
           tractive_system_capacitance * (float)PRECHARGE_TIME_UPPER_BOUND_PERCENTAGE);
}
