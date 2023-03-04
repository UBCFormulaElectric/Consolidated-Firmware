#include "App_Efuse.h"
#include "App_Timer.h"

struct Efuse
{
    struct Io_Efuse *io_efuse;
    void (*enable_channel_0)(struct Io_Efuse *);
    void (*disable_channel_0)(struct Io_Efuse *);
    void (*enable_channel_1)(struct Io_Efuse *);
    void (*disable_channel_1)(struct Io_Efuse *);
    bool (*is_channel_0_enabled)(struct Io_Efuse *);
    bool (*is_channel_1_enabled)(struct Io_Efuse *);
    void (*stdby_reset)(struct Io_Efuse *);
    float (*get_channel_0_current)(struct Io_Efuse *);
    float (*get_channel_1_current)(struct Io_Efuse *);
    float channel_0_min_current;
    float channel_0_max_current;
    float channel_1_min_current;
    float channel_1_max_current;
    // Next 6 values are for Efuse Fault Procedure Checks
    TimerChannel efuse_timer_channel_0;
    TimerChannel efuse_timer_channel_1;
    int num_attempts_channel_0;
    int num_attempts_channel_1;
    bool has_timer_started_channel_0;
    bool has_timer_started_channel_1;

};

struct Efuse *App_Efuse_Create(
    struct Io_Efuse(*io_efuse),
    void (*enable_channel_0)(struct Io_Efuse *),
    void (*disable_channel_0)(struct Io_Efuse *),
    void (*enable_channel_1)(struct Io_Efuse *),
    void (*disable_channel_1)(struct Io_Efuse *),
    bool (*is_channel_0_enabled)(struct Io_Efuse *),
    bool (*is_channel_1_enabled)(struct Io_Efuse *),
    void (*stdby_reset)(struct Io_Efuse *),
    float (*get_channel_0_current)(struct Io_Efuse *),
    float (*get_channel_1_current)(struct Io_Efuse *),
    float channel_0_min_current,
    float channel_0_max_current,
    float channel_1_min_current,
    float channel_1_max_current)
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
    efuse->num_attempts_channel_0 = 0;
    efuse->num_attempts_channel_1 = 0;
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

bool App_Efuse_IsChannel0Enabled(struct Efuse *efuse)
{
    return efuse->is_channel_0_enabled(efuse->io_efuse);
}

bool App_Efuse_IsChannel1Enabled(struct Efuse *efuse)
{
    return efuse->is_channel_1_enabled(efuse->io_efuse);
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

bool App_Efuse_Channel0_CurrentLowCheck(struct Efuse *efuse)
{
    return efuse->get_channel_0_current(efuse->io_efuse) < efuse->channel_0_min_current;
}

bool App_Efuse_Channel0_CurrentHighCheck(struct Efuse *efuse)
{
    return efuse->get_channel_0_current(efuse->io_efuse) > efuse->channel_0_max_current;
}

bool App_Efuse_Channel1_CurrentLowCheck(struct Efuse *efuse)
{
    return efuse->get_channel_1_current(efuse->io_efuse) < efuse->channel_1_min_current;
}

bool App_Efuse_Channel1_CurrentHighCheck(struct Efuse *efuse)
{
    return efuse->get_channel_1_current(efuse->io_efuse) > efuse->channel_1_max_current;
}

bool App_Efuse_FaultProcedureChannel0(struct Efuse *efuse, int max_attempts)
{
    // When efuse-> has_timer_started_channel_0 = true, timer is running

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
        efuse->has_timer_started_channel_0 = 0;
    }

    if (efuse->has_timer_started_channel_0 && App_Timer_UpdateAndGetState((&efuse->efuse_timer_channel_0)) == TIMER_STATE_EXPIRED)
    {
        App_Efuse_StandbyReset(efuse);
        efuse->num_attempts_channel_0++;
    }
    if (efuse->num_attempts_channel_0 >= max_attempts)
    {
        App_Timer_Stop(&efuse->efuse_timer_channel_0);
        efuse->has_timer_started_channel_0 = false;
        return 0; // Channel Failed
    }

    return 1; // No Fault
}

bool App_Efuse_FaultProcedureChannel1(struct Efuse *efuse, int max_attempts)
{
    // When efuse-> has_timer_started_channel_1 = true, timer is running

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
        efuse->has_timer_started_channel_1      = 0;
    }

    if (efuse->has_timer_started_channel_1 && App_Timer_UpdateAndGetState((&efuse->efuse_timer_channel_1)) == TIMER_STATE_EXPIRED)
    {
        App_Efuse_StandbyReset(efuse);
        efuse->num_attempts_channel_1++;
    }
    if (efuse->num_attempts_channel_1 >= max_attempts)
    {
        App_Timer_Stop(&efuse->efuse_timer_channel_1);
        efuse->has_timer_started_channel_1 = false;
        return 0; // Channel Failed
    }

    return 1; // No Fault
}
