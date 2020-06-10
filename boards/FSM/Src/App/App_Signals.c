#include "App_AcceleratorPedal.h"
#include "App_Signals.h"

struct AcceleratorPedal *_papps;
struct AcceleratorPedal *_sapps;

static bool IsPappsAlarmActive(void)
{
    return App_AcceleratorPedal_IsEncoderAlarmActive(_papps);
}

static void PappsAlarmCallback(void *context)
{
    struct FsmWorld *         world  = (struct FsmWorld *)context;
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_PAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}

static bool IsSappsAlarmActive(void)
{
    return App_AcceleratorPedal_IsEncoderAlarmActive(_papps);
}

static void SappsAlarmCallback(void *context)
{
    struct FsmWorld *         world  = (struct FsmWorld *)context;
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}

void App_Signals_Init(struct FsmWorld *world)
{
    _papps = App_FsmWorld_GetPapps(world);
    _sapps = App_FsmWorld_GetSapps(world);

    struct SignalCallback callback;
    struct Signal *       signal;

    callback.function         = PappsAlarmCallback;
    callback.high_duration_ms = 10;
    callback.context          = world;
    signal = App_SharedSignal_Create(0, IsPappsAlarmActive, callback);
    App_FsmWorld_RegisterSignal(world, signal);

    callback.function         = SappsAlarmCallback;
    callback.high_duration_ms = 10;
    callback.context          = world;
    signal = App_SharedSignal_Create(0, IsSappsAlarmActive, callback);
    App_FsmWorld_RegisterSignal(world, signal);
}
