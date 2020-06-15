#include <math.h>
#include "App_AcceleratorPedalSignals.h"
#include "App_FsmWorld.h"

bool App_AcceleratorPedalSignals_IsPappsAlarmActive(struct FsmWorld *world)
{
    struct AcceleratorPedal *papps = App_FsmWorld_GetPapps(world);

    return App_AcceleratorPedal_IsEncoderAlarmActive(papps);
}

void App_AcceleratorPedalSignals_PappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    // TODO: Throw a motor shutdown
}

bool App_AcceleratorPedalSignals_IsSappsAlarmActive(struct FsmWorld *world)
{
    struct AcceleratorPedal *sapps = App_FsmWorld_GetSapps(world);

    return App_AcceleratorPedal_IsEncoderAlarmActive(sapps);
}

void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    // TODO: Throw a motor shutdown
}

bool App_AcceleratorPedalSignals_HasDisagreement(struct FsmWorld *world)
{
    struct AcceleratorPedal *papps = App_FsmWorld_GetPapps(world);
    struct AcceleratorPedal *sapps = App_FsmWorld_GetSapps(world);

    return fabsf(
               App_AcceleratorPedal_GetPedalPercentage(papps) -
               App_AcceleratorPedal_GetPedalPercentage(sapps)) >= 10.0f;
}

void App_AcceleratorPedalSignals_HasDisagreementCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    // TODO: Set motor shutdown fault
    // App_CanTx_SetPeriodicSignal_DUMMY_MOTOR_SHUTDOWN(can_tx, true);
}

bool App_AcceleratorPedalSignals_BrakeIsActuatedAndPedalIsPressed(
    struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    return App_CanTx_GetPeriodicSignal_BRAKE_IS_ACTUATED(can_tx) &&
           (App_CanTx_GetPeriodicSignal_PAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx) >=
            25.0f);
}

void App_AcceleratorPedalSignals_BrakeIsActuatedAndPedalIsPressedCallback(
    struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    // TODO: Throw a motor shutdown
    // App_CanTx_SetPeriodicSignal_DUMMY_MOTOR_SHUTDOWN(can_tx, true);
}
