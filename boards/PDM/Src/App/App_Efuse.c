#include "App_Efuse.h"

struct Io_Efuse;
struct Efuse
{
    struct Io_Efuse (*io_efuse);
    void (*enable_channel_0)(struct Io_Efuse);
    void (*disable_channel_0)(struct Io_Efuse);
    void (*enable_channel_1)(struct Io_Efuse);
    void (*disable_channel_1)(struct Io_Efuse);
    bool (*is_channel_0_enabled)(struct Io_Efuse);
    bool (*is_channel_1_enabled)(struct Io_Efuse);
    void (*stdby_reset)(struct Io_Efuse);
    float (*get_channel_0_current)(struct Io_Efuse);
    float (*get_channel_1_current)(struct Io_Efuse);
    float (*channel_0_min_current);
    float (*channel_0_max_current);
    float (*channel_1_min_current);
    float (*channel_1_max_current);
};

struct Efuse *App_Efuse_Create(
        struct Io_Efuse (*io_efuse),
        void (*enable_channel_0)(struct Io_Efuse),
        void (*disable_channel_0)(struct Io_Efuse),
        void (*enable_channel_1)(struct Io_Efuse),
        void (*disable_channel_1)(struct Io_Efuse),
        bool (*is_channel_0_enabled)(struct Io_Efuse),
        bool (*is_channel_1_enabled)(struct Io_Efuse),
        void (*stdby_reset)(struct Io_Efuse),
        float (*get_channel_0_current)(struct Io_Efuse),
        float (*get_channel_1_current)(struct Io_Efuse),
        float (*channel_0_min_current),
        float (*channel_0_max_current),
        float (*channel_1_min_current),
        float (*channel_1_max_current))
{
    struct Efuse *efuse = malloc(sizeof(struct Efuse));
    assert(efuse != NULL);

    efuse->io_efuse              = io_efuse;
    efuse->enable_channel_0      = enable_channel_0;
    efuse->disable_channel_0     = disable_channel_0;
    efuse->enable_channel_1      = enable_channel_1;
    efuse->disable_channel_1     = disable_channel_1;
    efuse->is_channel_0_enabled  = is_channel_0_enabled;
    efuse->is_channel_1_enabled  = is_channel_1_enabled;
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
    efuse->enable_channel_0(*efuse->io_efuse);
}

void App_Efuse_DisableChannel0(struct Efuse *efuse)
{
    efuse->disable_channel_0(*efuse->io_efuse);
}

void App_Efuse_EnableChannel1(struct Efuse *efuse)
{
    efuse->enable_channel_1(*efuse->io_efuse);
}

void App_Efuse_DisableChannel1(struct Efuse *efuse)
{
    efuse->disable_channel_1(*efuse->io_efuse);
}

bool App_Efuse_IsChannel0Enabled(struct Efuse *efuse)
{
    return efuse->is_channel_0_enabled(*efuse->io_efuse);
}

bool App_Efuse_IsChannel1Enabled(struct Efuse *efuse)
{
    return efuse->is_channel_1_enabled(*efuse->io_efuse);
}

void App_Efuse_StandbyReset(struct Efuse *efuse)
{
    efuse->stdby_reset(*efuse->io_efuse);
}

float App_Efuse_GetChannel0Current(struct Efuse *efuse)
{
    return efuse->get_channel_0_current(*efuse->io_efuse);
}

float App_Efuse_GetChannel1Current(struct Efuse *efuse)
{
    return efuse->get_channel_1_current(*efuse->io_efuse);
}

bool App_Efuse_CurrentLowCheck(float value, float min_value)
{
    if (value >= min_value)
        return false;
    return true;
}

bool App_Efuse_CurrentHighCheck(float value, float max_value)
{
    if (value <= max_value)
        return false;
    return true;
}

bool App_Efuse_Channel0_CurrentLowCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentLowCheck(efuse->get_channel_0_current(*efuse->io_efuse), efuse->channel_0_min_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel0_CurrentHighCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentHighCheck(efuse->get_channel_0_current(*efuse->io_efuse), efuse->channel_0_max_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel1_CurrentLowCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentLowCheck(efuse->get_channel_1_current(*efuse->io_efuse), efuse->channel_1_min_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel1_CurrentHighCheck(struct Efuse *efuse)
{
    if (App_Efuse_CurrentHighCheck(efuse->get_channel_1_current(*efuse->io_efuse), efuse->channel_1_max_current) == true)
        return true;
    return false;
}
