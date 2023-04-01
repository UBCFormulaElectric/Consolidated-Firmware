#include "App_Efuse.h"
#include "App_Timer.h"

struct Efuse
{
    EfuseChannel io_efuse_channel0;
    EfuseChannel io_efuse_channel1;
    void        (*set_channel)(EfuseChannel, bool);
    bool        (*is_channel_enabled)(EfuseChannel);
    float       (*get_channel_current)(EfuseChannel);
    void        (*standby_reset)(EfuseChannel);
    float        channel_0_min_current;
    float        channel_0_max_current;
    float        channel_1_min_current;
    float        channel_1_max_current;
    int          max_reset_attempts;
    TimerChannel efuse_timer_channel_0;
    TimerChannel efuse_timer_channel_1;
    int          num_attempts_channel_0;
    int          num_attempts_channel_1;
    bool         has_timer_started_channel_0;
    bool         has_timer_started_channel_1;
};

struct Efuse *App_Efuse_Create(
    EfuseChannel io_efuse_channel0,
    EfuseChannel io_efuse_channel1,
    void        (*set_channel)(EfuseChannel, bool),
    bool        (*is_channel_enabled)(EfuseChannel),
    float       (*get_channel_current)(EfuseChannel),
    void        (*standby_reset)(EfuseChannel),
    float        channel_0_min_current,
    float        channel_0_max_current,
    float        channel_1_min_current,
    float        channel_1_max_current,
    int          max_reset_attempts)
{
    struct Efuse *efuse = malloc(sizeof(struct Efuse));
    assert(efuse != NULL);

    efuse->io_efuse_channel0     = io_efuse_channel0;
    efuse->io_efuse_channel1     = io_efuse_channel1;
    efuse->set_channel           = set_channel;
    efuse->is_channel_enabled    = is_channel_enabled;
    efuse->get_channel_current   = get_channel_current;
    efuse->standby_reset         = standby_reset;
    efuse->channel_0_min_current = channel_0_min_current;
    efuse->channel_0_max_current = channel_0_max_current;
    efuse->channel_1_min_current = channel_1_min_current;
    efuse->channel_1_max_current = channel_1_max_current;

    efuse->max_reset_attempts    = max_reset_attempts;
    efuse->num_attempts_channel_0      = 0;
    efuse->num_attempts_channel_1      = 0;
    efuse->has_timer_started_channel_0 = false;
    efuse->has_timer_started_channel_1 = false;

    App_Timer_InitTimer(&efuse->efuse_timer_channel_0, 1000);
    App_Timer_InitTimer(&efuse->efuse_timer_channel_1, 1000);

    return efuse;
}

void App_Efuse_Destroy(struct Efuse *efuse)
{
    free(efuse);
}

void App_Efuse_EnableChannel0(struct Efuse *efuse, bool status)
{
    efuse->set_channel(efuse->io_efuse_channel0, status);
}

void App_Efuse_EnableChannel1(struct Efuse *efuse, bool status)
{
    efuse->set_channel(efuse->io_efuse_channel1, status);
}

bool App_Efuse_IsChannel0Enabled(struct Efuse *efuse)
{
    return efuse->is_channel_enabled(efuse->io_efuse_channel0);
}

bool App_Efuse_IsChannel1Enabled(struct Efuse *efuse)
{
    return efuse->is_channel_enabled(efuse->io_efuse_channel1);
}

void App_Efuse_StandbyReset(struct Efuse *efuse)
{
    efuse->standby_reset(efuse->io_efuse_channel0);
}

float App_Efuse_GetChannel0Current(struct Efuse *efuse)
{
    return efuse->get_channel_current(efuse->io_efuse_channel0);
}

float App_Efuse_GetChannel1Current(struct Efuse *efuse)
{
    return efuse->get_channel_current(efuse->io_efuse_channel1);
}

bool App_Efuse_Channel0_CurrentLowCheck(struct Efuse *efuse)
{
    return App_Efuse_GetChannel0Current(efuse) < efuse->channel_0_min_current;
}

bool App_Efuse_Channel0_CurrentHighCheck(struct Efuse *efuse)
{
    return App_Efuse_GetChannel0Current(efuse) > efuse->channel_0_max_current;
}

bool App_Efuse_Channel1_CurrentLowCheck(struct Efuse *efuse)
{
    return App_Efuse_GetChannel1Current(efuse) < efuse->channel_1_min_current;
}

bool App_Efuse_Channel1_CurrentHighCheck(struct Efuse *efuse)
{
    return App_Efuse_GetChannel1Current(efuse) > efuse->channel_1_max_current;
}

bool App_Efuse_FaultCheckChannel0(struct Efuse *efuse)
{
    if ((App_Efuse_Channel0_CurrentLowCheck(efuse) && App_Efuse_IsChannel0Enabled(efuse)) ||
        App_Efuse_Channel0_CurrentHighCheck(efuse))
    {
        if (!efuse->has_timer_started_channel_0)
        {
            App_Timer_Restart(&efuse->efuse_timer_channel_0);
            efuse->has_timer_started_channel_0 = true;
        }
    }
    else
    {
        App_Timer_Stop(&efuse->efuse_timer_channel_0);
        efuse->has_timer_started_channel_0 = false;
        efuse->num_attempts_channel_0      = 0;
    }

    if (efuse->has_timer_started_channel_0 &&
        App_Timer_UpdateAndGetState(&efuse->efuse_timer_channel_0) == TIMER_STATE_EXPIRED)
    {
        App_Efuse_StandbyReset(efuse);
        efuse->num_attempts_channel_0++;
    }
    if (efuse->num_attempts_channel_0 >= efuse->max_reset_attempts)
    {
        App_Timer_Stop(&efuse->efuse_timer_channel_0);
        efuse->has_timer_started_channel_0 = false;
        return true; // channel failed
    }

    return false; // channel fine
}

bool App_Efuse_FaultCheckChannel1(struct Efuse *efuse)
{
    if ((App_Efuse_Channel1_CurrentLowCheck(efuse) && App_Efuse_IsChannel1Enabled(efuse)) ||
        App_Efuse_Channel1_CurrentHighCheck(efuse))
    {
        if (!efuse->has_timer_started_channel_1)
        {
            App_Timer_Restart(&efuse->efuse_timer_channel_1);
            efuse->has_timer_started_channel_1 = true;
        }
    }
    else
    {
        App_Timer_Stop(&efuse->efuse_timer_channel_1);
        efuse->has_timer_started_channel_1 = false;
        efuse->num_attempts_channel_1      = 0;
    }

    if (efuse->has_timer_started_channel_1 &&
        App_Timer_UpdateAndGetState(&efuse->efuse_timer_channel_1) == TIMER_STATE_EXPIRED)
    {
        App_Efuse_StandbyReset(efuse);
        efuse->num_attempts_channel_1++;
    }
    if (efuse->num_attempts_channel_1 >= efuse->max_reset_attempts)
    {
        App_Timer_Stop(&efuse->efuse_timer_channel_1);
        efuse->has_timer_started_channel_1 = false;
        return true; // channel failed
    }

    return false; // fine
}
