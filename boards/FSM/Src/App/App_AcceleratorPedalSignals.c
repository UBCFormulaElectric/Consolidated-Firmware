#include "App_AcceleratorPedalSignals.h"
#include "App_AcceleratorPedal.h"

bool App_AcceleratorPedalSignals_IsPappsAlarmActive(struct FsmWorld *world)
{
    struct AcceleratorPedal *papps = App_FsmWorld_GetPapps(world);
    return App_AcceleratorPedal_IsEncoderAlarmActive(papps);
}

void App_AcceleratorPedalSignals_PappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_PAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}

bool App_AcceleratorPedalSignals_IsSappsAlarmActive(struct FsmWorld *world)
{
    struct AcceleratorPedal *sapps = App_FsmWorld_GetSapps(world);
    return App_AcceleratorPedal_IsEncoderAlarmActive(sapps);
}

void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}
