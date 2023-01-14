#include <math.h>
#include "App_AcceleratorPedalSignals.h"
#include "App_AcceleratorPedals.h"
#include "App_FsmWorld.h"
#include "Io_Adc.h"

bool App_AcceleratorPedalSignals_IsPappsAlarmActive(struct FsmWorld *world)
{
<<<<<<< HEAD
    const float raw_adc = Io_Adc_GetChannel1Voltage();
    return (raw_adc < 0.5f) || (raw_adc > 3.1f);
=======
    // BAD CODE!!!! DO NOT RECREATE!!!!! This entire file should not be a thing
    struct AcceleratorPedals *accelerator_pedals = App_FsmWorld_GetPappsAndSapps(world);
    return App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(accelerator_pedals);
>>>>>>> 9f4e826a1653caf1378f99c986767d1a000e3140
}

void App_AcceleratorPedalSignals_PappsAlarmCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(
        can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
}

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

bool App_AcceleratorPedalSignals_IsPappsAndSappsAlarmInactive(struct FsmWorld *world)
{
    struct AcceleratorPedals *papps_and_sapps = App_FsmWorld_GetPappsAndSapps(world);
<<<<<<< HEAD
    const float               raw_adc_voltage = Io_Adc_GetChannel1Voltage();
    return ((raw_adc_voltage > 1.00f) && (raw_adc_voltage < 2.9f)) &&
=======
    return !App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(papps_and_sapps) &&
>>>>>>> 9f4e826a1653caf1378f99c986767d1a000e3140
           !App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(papps_and_sapps);
}

bool App_AcceleratorPedalSignals_HasAppsDisagreement(struct FsmWorld *world)
{
    struct AcceleratorPedals *papps_and_sapps = App_FsmWorld_GetPappsAndSapps(world);
    return fabsf(
               App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps) -
               App_AcceleratorPedals_GetSecondaryPedalPercentage(papps_and_sapps)) > 10.0f;
}

bool App_AcceleratorPedalSignals_HasAppsAgreement(struct FsmWorld *world)
{
    return !App_AcceleratorPedalSignals_HasAppsDisagreement(world);
}

void App_AcceleratorPedalSignals_AppsDisagreementCallback(struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    App_CanTx_SetPeriodicSignal_APPS_HAS_DISAGREEMENT(
        can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE);
}

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
