#include "App_AcceleratorPedal.h"
#include "App_Signals.h"

struct AcceleratorPedal *_papps;
struct AcceleratorPedal *_sapps;

static bool IsPappsAlarmActive(void)
{
    return App_AcceleratorPedal_IsEncoderAlarmActive(_papps);
}

static void PappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_PAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}

static bool IsSappsAlarmActive(void)
{
    return App_AcceleratorPedal_IsEncoderAlarmActive(_sapps);
}

static void SappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}

void App_Signals_Init(struct FsmWorld *world)
{
    _papps = App_FsmWorld_GetPapps(world);
    _sapps = App_FsmWorld_GetSapps(world);

    struct SignalCallback papps_callback = {
        .function         = PappsAlarmCallback,
        .high_duration_ms = 10,
        .world            = world,
    };
    struct Signal *papps_signal =
        App_SharedSignal_Create(0, IsPappsAlarmActive, papps_callback);
    App_FsmWorld_RegisterSignal(world, papps_signal);

    struct SignalCallback sapps_callback = {
        .function         = SappsAlarmCallback,
        .high_duration_ms = 10,
        .world            = world,
    };
    struct Signal *sapps_signal =
        App_SharedSignal_Create(0, IsSappsAlarmActive, sapps_callback);
    App_FsmWorld_RegisterSignal(world, sapps_signal);
}
