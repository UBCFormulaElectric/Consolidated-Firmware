#include "App_Efuse.h"
#include <assert.h>
#include <malloc.h>

struct Efuse
{
    void (*enable_channel_0)(void);
    void (*disable_channel_0)(void);
    void (*enable_channel_1)(void);
    void (*disable_channel_1)(void);
    ExitCode (*get_status)(enum Efuse_Status *status);
    ExitCode (*get_channel_0_faults)(enum Efuse_Fault *fault);
    ExitCode (*get_channel_1_faults)(enum Efuse_Fault *fault);
    bool (*is_in_fault_mode)(void);
    bool (*is_in_failsafe_mode)(void);
    void (*delatch_faults)(void);
    float (*get_channel_0_current)(void);
    float (*get_channel_1_current)(void);
};

struct Efuse *App_EfuseCreate(
    void (*enable_channel_0)(void),
    void (*disable_channel_0)(void),
    void (*enable_channel_1)(void),
    void (*disable_channel_1)(void),
    //ExitCode (*get_status)(enum Efuse_Status *status),
    //ExitCode (*get_channel_0_faults)(enum Efuse_Fault *fault),
    //ExitCode (*get_channel_1_faults)(enum Efuse_Fault *fault),
    bool (*is_in_fault_mode)(void),
    bool (*is_in_failsafe_mode)(void),
    void (*delatch_faults)(void),
    float (*get_channel_0_current)(void),
    float (*get_channel_1_current)(void))
{
    struct Efuse *efuse = malloc(sizeof(struct Efuse));
    assert(efuse != NULL);

    efuse->enable_channel_0 = enable_channel_0;
    efuse->disable_channel_0 = disable_channel_0;
    efuse->enable_channel_1 = enable_channel_1;
    efuse->disable_channel_1 = disable_channel_1;

    efuse->is_in_fault_mode = is_in_fault_mode;
    efuse->is_in_failsafe_mode = is_in_failsafe_mode;
    efuse->get_channel_0_current = get_channel_0_current;
    efuse->get_channel_1_current = get_channel_1_current;

}

void App_EfuseDestroy(struct Efuse *efuse)
{
    free(efuse);
}

void App_EfuseEnableChannel0(struct Efuse *efuse)
{
    efuse->enable_channel_0();
}

void App_EfuseDisableChannel0(struct Efuse *efuse)
{
    efuse->disable_channel_0();
}

void App_Efuse_EnableChannel1(struct Efuse *efuse)
{
    efuse->enable_channel_1();
}

void App_EfuseDisableChannel1(struct Efuse *efuse)
{
    efuse->disable_channel_1();
}

bool App_EfuseInFaultMode(struct Efuse *efuse)
{
    return efuse->is_in_fault_mode();
}

bool App_EfuseInFailSafeMode(struct Efuse *efuse)
{
    return efuse->is_in_failsafe_mode();
}

float App_EfuseGetChannel0Current(struct Efuse *efuse)
{
    return efuse->get_channel_0_current();
}

float App_EfuseGetChannel2Current(struct Efuse *efuse)
{
    return efuse->get_channel_1_current();
}