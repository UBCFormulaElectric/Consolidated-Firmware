#include <assert.h>
#include <stdlib.h>
#include "App_PreChargeSequence.h"

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
