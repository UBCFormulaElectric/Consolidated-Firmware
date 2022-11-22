#include "App_Efuse.h"

struct Efuse
{
    // void (*enable_channel_0)(void);
    // void (*disable_channel_0)(void);
    // void (*enable_channel_1)(void);
    // void (*disable_channel_1)(void);
    float (*get_channel_0_current)(void);
    float (*get_channel_1_current)(void);
    bool (*is_pin_0_on)(void);
    bool (*is_pin_1_on)(void);
    bool (*is_pin_stby_on)(void);
};

struct Efuse *App_Efuse_Create(
    float (*get_channel_0_current)(void),
    float (*get_channel_1_current)(void),
    bool (*is_pin_0_on)(void),
    bool (*is_pin_1_on)(void),
    bool (*is_pin_stby_on)(void))
{
    struct Efuse *efuse = malloc(sizeof(struct Efuse));
    assert(efuse != NULL);

    efuse->get_channel_0_current = get_channel_0_current;
    efuse->get_channel_1_current = get_channel_1_current;
    efuse->is_pin_0_on           = is_pin_0_on;
    efuse->is_pin_1_on           = is_pin_1_on;
    efuse->is_pin_stby_on        = is_pin_stby_on;

    return efuse;
}

void App_EfuseDestroy(struct Efuse *efuse)
{
    free(efuse);
}

bool App_EfuseEnableorDisableChannel0(struct Efuse *efuse, bool (*function)(bool))
{
    return function(efuse->is_pin_0_on());
}

bool App_EfuseEnableorDisableChannel1(struct Efuse *efuse, bool (*function)(bool))
{
    return function(efuse->is_pin_1_on());
}

bool App_EfuseEnableorDisableSTBYChannel(struct Efuse *efuse, bool (*function)(bool))
{
    return function(efuse->is_pin_stby_on());
}

float App_Efuse_GetChannel0Current(struct Efuse *efuse)
{
    return efuse->get_channel_0_current();
}

float App_Efuse_GetChannel1Current(struct Efuse *efuse)
{
    return efuse->get_channel_1_current();
}

int App_Efuse_InRangeCheck(float value, float min_value, float max_value)
{
    int status;
    if (value < min_value)
        status = 1;

    else if (value > max_value)
        status = 2;

    else
        status = 0;

    return status;
}

bool *App_Efuse_Channel0_CurrentCheck(struct Efuse *efuse, float min_value, float max_value)
{
    if (App_Efuse_InRangeCheck(efuse->get_channel_0_current(), min_value, max_value) == 0)
        return (bool*) true;
    return (bool*) false;
}

bool *App_Efuse_Channel1_CurrentCheck(struct Efuse *efuse, float min_value, float max_value)
{
    if (App_Efuse_InRangeCheck(efuse->get_channel_1_current(), min_value, max_value) == 0)
        return (bool*) true;
    return (bool*) false;
}
