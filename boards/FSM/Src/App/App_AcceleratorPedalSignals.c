#include <math.h>
#include "App_AcceleratorPedalSignals.h"
#include "App_FsmWorld.h"

// floored accelerator and brake activated (25%+ with break=high, <5% =low)
//  entry/exit/callback
bool App_AcceleratorPedalSignals_HasAppsAndBrakePlausibilityFailure(struct AcceleratorPedals *papps_and_sapps, struct Brake * brake){
    return App_Brake_IsBrakeActuated(brake) && App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps) > 25.0f;
}
bool App_AcceleratorPedalSignals_IsAppsAndBrakePlausibilityOk(struct AcceleratorPedals *papps_and_sapps){
    return App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps) < 5.0f;
}
void App_AcceleratorPedalSignals_AppsAndBrakePlausibilityFailureCallback(struct FsmWorld *world)
{
    // struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    // App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    // App_CanTx_SetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(
    //    can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE);
}
