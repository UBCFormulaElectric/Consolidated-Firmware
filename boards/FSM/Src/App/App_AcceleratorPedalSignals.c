#include <math.h>
#include "App_AcceleratorPedalSignals.h"
#include "App_FsmWorld.h"

// Activity Checks
//1. check if the alarms are active
//2. check if primary and secondary are within 10% of each other
//3. Brake + Accelerator Pedal Double Pushing

//activate entry for enough time = error
//corrected = exit for enough time.


// papp entry/callback
bool App_AcceleratorPedalSignals_IsPappsAlarmActive(struct FsmWorld *world)
{
    // BAD CODE!!!! DO NOT RECREATE!!!!! This entire file should not be a thing
    struct AcceleratorPedals *accelerator_pedals = App_FsmWorld_GetPappsAndSapps(world);
    return App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(accelerator_pedals);
}
void App_AcceleratorPedalSignals_PappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(
        can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
}
// sapp entry/callback
bool App_AcceleratorPedalSignals_IsSappsAlarmActive(struct FsmWorld *world)
{
    struct AcceleratorPedals *papps_and_sapps = App_FsmWorld_GetPappsAndSapps(world);

    return App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(papps_and_sapps);
}
void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(
        can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
}
// combination exit
bool App_AcceleratorPedalSignals_IsPappsAndSappsAlarmInactive(struct FsmWorld *world)
{
    struct AcceleratorPedals *papps_and_sapps = App_FsmWorld_GetPappsAndSapps(world);
    return !App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(papps_and_sapps) &&
           !App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(papps_and_sapps);
}

// floored accelerator and brake activated (25%+ with break=high, <5% =low)
//  entry/exit/callback
bool App_AcceleratorPedalSignals_HasAppsAndBrakePlausibilityFailure(struct FsmWorld *world)
{
    struct AcceleratorPedals *papps_and_sapps = App_FsmWorld_GetPappsAndSapps(world);
    struct Brake *            brake           = App_FsmWorld_GetBrake(world);

    return App_Brake_IsBrakeActuated(brake) && App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps) > 25.0f;
}
bool App_AcceleratorPedalSignals_IsAppsAndBrakePlausibilityOk(struct FsmWorld *world)
{
    struct AcceleratorPedals *papps_and_sapps = App_FsmWorld_GetPappsAndSapps(world);

    return App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps) < 5.0f;
}
void App_AcceleratorPedalSignals_AppsAndBrakePlausibilityFailureCallback(struct FsmWorld *world)
{
    // struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    // App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    // App_CanTx_SetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(
    //    can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE);
}

// disagreement between primary and secondary sensors
// Diff > 10% = High, <= 10 = Low
//entry/exit/callback
bool App_AcceleratorPedalSignals_HasAppsDisagreement(const struct AcceleratorPedals *papps_and_sapps){
    return fabsf(
            App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps) -
            App_AcceleratorPedals_GetSecondaryPedalPercentage(papps_and_sapps)) > 10.0f;
}
