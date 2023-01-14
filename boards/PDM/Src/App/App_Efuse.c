#include "App_Efuse.h"

struct Efuse
{
    struct Efuse_Context(*io_efuse);
    void (*enable_channel_0)(const struct Efuse_Context *const);
    void (*disable_channel_0)(const struct Efuse_Context *const);
    void (*enable_channel_1)(const struct Efuse_Context *const);
    void (*disable_channel_1)(const struct Efuse_Context *const);
    /* All functions related to Latch off and standby:
     * Maybe if current is out of range, it would call an  io_function to delatch,
     * and then once the error is cleared, we would have an io_function to reset
     * the stdby pins?
     *
     * If you want a different condition for the delatch we will have to determine that
     */

    // IDK if we are going to use these (depends on what we are going to do above):
    // bool (*in_fault_mode)(const struct Efuse_Context *const);
    // bool (*in_failsafe_mode)(const struct Efuse_Context *const);
    void (*efuse_delatch)(struct Efuse_Context *const);
    void (*stdby_reset)(struct Efuse_Context *const);
    float (*get_channel_0_current)(struct Efuse_Context *const);
    float (*get_channel_1_current)(struct Efuse_Context *const);
    float (channel_0_min_current);
    float (channel_0_max_current);
    float (channel_1_min_current);
    float (channel_1_max_current);
};

struct Efuse *App_Efuse_Create(
    struct Efuse_Context (*io_efuse),
    void (*enable_channel_0)(const struct Efuse_Context *const),
    void (*disable_channel_0)(const struct Efuse_Context *const),
    void (*enable_channel_1)(const struct Efuse_Context *const),
    void (*disable_channel_1)(const struct Efuse_Context *const),
    // bool (*in_fault_mode)(const struct Efuse_Context *const),
    // bool (*in_failsafe_mode)(const struct Efuse_Context *const),
    void (*efuse_delatch)(struct Efuse_Context *const),
    void (*stdby_reset)(struct Efuse_Context *const),
    float (*get_channel_0_current)(struct Efuse_Context *const),
    float (*get_channel_1_current)(struct Efuse_Context *const),
    float(channel_0_min_current),
    float(channel_0_max_current),
    float(channel_1_min_current),
    float(channel_1_max_current))
{
    struct Efuse *efuse = malloc(sizeof(struct Efuse));
    assert(efuse != NULL);

    efuse->io_efuse          = io_efuse;
    efuse->enable_channel_0  = enable_channel_0;
    efuse->disable_channel_0 = disable_channel_0;
    efuse->enable_channel_1  = enable_channel_1;
    efuse->disable_channel_1 = disable_channel_1;
    // efuse->in_fault_mode         = in_fault_mode;
    // efuse->in_failsafe_mode      = in_failsafe_mode;
    // efuse->delatch_fault         = delatch_fault;
    efuse->stdby_reset           = stdby_reset;
    efuse->get_channel_0_current = get_channel_0_current;
    efuse->get_channel_1_current = get_channel_1_current;
    efuse->channel_0_min_current = channel_0_min_current;
    efuse->channel_0_max_current = channel_0_max_current;
    efuse->channel_1_min_current = channel_1_min_current;
    efuse->channel_1_max_current = channel_1_max_current;

    return efuse;
}

void App_Efuse_Destroy(struct Efuse *efuse)
{
    free(efuse);
}

void App_Efuse_EnableChannel0(struct Efuse *efuse)
{
    efuse->enable_channel_0(efuse->io_efuse);
}

void App_Efuse_DisableChannel0(struct Efuse *efuse)
{
    efuse->disable_channel_0(efuse->io_efuse);
}

void App_Efuse_EnableChannel1(struct Efuse *efuse)
{
    efuse->enable_channel_1(efuse->io_efuse);
}

void App_Efuse_DisableChannel1(struct Efuse *efuse)
{
    efuse->disable_channel_1(efuse->io_efuse);
}

bool App_Efuse_IsEfuseInFaultMode(struct Efuse *efuse)
{
    // return efuse->in_fault_mode(efuse->io_efuse);
}

bool App_Efuse_IsEfuseInFailSafeMode(struct Efuse *efuse)
{
    // return efuse->in_failsafe_mode(efuse->io_efuse);
}

void App_Efuse_Delatch(struct Efuse *efuse)
{
    efuse->efuse_delatch(efuse->io_efuse);
}

void App_Efuse_StandbyReset(struct Efuse *efuse)
{
    efuse->stdby_reset(efuse->io_efuse);
}

float App_Efuse_GetChannel0Current(struct Efuse *efuse)
{
    return efuse->get_channel_0_current(efuse->io_efuse);
}

float App_Efuse_GetChannel1Current(struct Efuse *efuse)
{
    return efuse->get_channel_1_current(efuse->io_efuse);
}

bool App_Efuse_CurrentTooLow(float value, float min_value)
{
    if (value >= min_value)
        return false;
    return true;
}

bool App_Efuse_CurrentTooHigh(float value, float max_value)
{
    if (value <= max_value)
        return false;
    return true;
}

bool App_Efuse_Channel0_CurrentLowCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentTooLow(efuse->get_channel_0_current(efuse->io_efuse), efuse->channel_0_min_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel0_CurrentHighCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentTooHigh(efuse->get_channel_0_current(efuse->io_efuse), efuse->channel_0_max_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel1_CurrentLowCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentTooLow(efuse->get_channel_1_current(efuse->io_efuse), efuse->channel_1_min_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel1_CurrentHighCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentTooHigh(efuse->get_channel_1_current(efuse->io_efuse), efuse->channel_1_max_current) == true)
        return true;
    return false;
}
