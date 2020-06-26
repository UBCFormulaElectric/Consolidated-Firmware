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
    App_CanTx_SetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
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
    App_CanTx_SetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
}

bool App_AcceleratorPedalSignals_HasAppsDisagreement(struct FsmWorld *world)
{
    struct AcceleratorPedal *papps = App_FsmWorld_GetPapps(world);
    struct AcceleratorPedal *sapps = App_FsmWorld_GetSapps(world);

    return fabsf(
               App_AcceleratorPedal_GetPedalPercentage(papps) -
               App_AcceleratorPedal_GetPedalPercentage(sapps)) > 10.0f;
}

void App_AcceleratorPedalSignals_AppsDisagreementCallback(
    struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_APPS_HAS_DISAGREEMENT(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE);
}

bool App_AcceleratorPedalSignals_HasAppsAndBrakePlausibilityFailure(
    struct FsmWorld *world)
{
    struct AcceleratorPedal *papps = App_FsmWorld_GetPapps(world);
    struct Brake *           brake = App_FsmWorld_GetBrake(world);

    return App_Brake_IsBrakeActuated(brake) &&
           App_AcceleratorPedal_GetPedalPercentage(papps) > 25.0f;
}

void App_AcceleratorPedalSignals_AppsAndBrakePlausibilityFailureCallback(
    struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_PEDAL_IS_PRESSED_AND_BRAKE_IS_ACTUATED(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PEDAL_IS_PRESSED_AND_BRAKE_IS_ACTUATED_TRUE_CHOICE);
}
