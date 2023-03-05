#include <assert.h>
#include <stdlib.h>

#include "App_AcceleratorPedals.h"
#include "App_SharedSignal.h"
#include "configs/App_AcceleratorSignalTimings.h"

struct AcceleratorPedals
{
    float (*get_primary_pedal_percent)(void);
    float (*get_secondary_pedal_percent)(void);

    bool (*primary_pedal_OCSC)(void);
    bool (*secondary_pedal_OCSC)(void);

    struct Signal *app_agreement_signal;
    struct Signal *papp_alarm_signal;
    struct Signal *sapp_alarm_signal;
    struct Signal *app_brake_signal;
};

/**
 * Get the pedal percentage, a value in [0, 100], for an encoder configured to
 * count up
 * @param encoder_fully_pressed_value The value of the encoder counter when the
 * given accelerator pedal is fully pressed
 * @param encoder_counter_value The current counter value of the encoder for the
 * given accelerator pedal
 * @param set_encoder_counter A function that can be called to set the
 * counter value of the encoder for the given accelerator pedal
 * @return The pedal percentage of the given accelerator pedal
 */

/*
static float App_GetPedalPercentage_CountUp(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_underflow_threshold,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t))
{
    float low_count_deadzone_threshold =
        (float)(encoder_fully_pressed_value - encoder_unpressed_value) * PERCENT_DEFLECTION +
        (float)encoder_unpressed_value;

    float high_count_deadzone_threshold =
        (float)(encoder_fully_pressed_value - encoder_unpressed_value) * (1.0f - PERCENT_DEFLECTION) +
        (float)encoder_unpressed_value;

    if (encoder_counter_value > encoder_underflow_threshold)
    {
        // If the accelerator pedal underflows (if counter exceeds a threshold
        // only possible to be reached via wrapping on underflow), reset the
        // corresponding encoder's counter register and set the pedal percentage
        // to
        set_encoder_counter(encoder_unpressed_value);
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value <= low_count_deadzone_threshold)
    {
        // Set the pedal percentage to 0.0% when the pedal is within the
        // low end deadzone
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value >= high_count_deadzone_threshold)
    {
        // Set the mapped pedal percentage to 100% when the pedal is within the
        // high end deadzone
        return MAX_ACCELERATOR_PEDAL_PRESS;
    }

    // Pedal % = 100% * (encoder value - low end deadzone threshold) /
    // (high end deadzone threshold - low end deadzone threshold)
    return MAX_ACCELERATOR_PEDAL_PRESS * ((float)encoder_counter_value - low_count_deadzone_threshold) /
           (high_count_deadzone_threshold - low_count_deadzone_threshold);
}
 */

/**
 * Get the pedal percentage, a value in [0, 100], for an encoder configured to
 * count down
 * @param encoder_fully_pressed_value The value of the encoder counter when the
 * given accelerator pedal is fully pressed
 * @param encoder_counter_value The current counter value of the encoder for the
 * given accelerator pedal
 * @param set_encoder_counter A function that can be called to set the
 * counter value of the encoder for the given accelerator pedal
 * @return The pedal percentage of the given accelerator pedal
 */
/*
static float App_GetPedalPercentage_CountDown(
   uint32_t encoder_fully_pressed_value,
   uint32_t encoder_unpressed_value,
   uint32_t encoder_counter_value,
   void (*set_encoder_counter)(uint32_t))
{
   // float low_count_deadzone_threshold =
   //    (float)(encoder_unpressed_value - encoder_fully_pressed_value) * PERCENT_DEFLECTION +
   //    (float)encoder_fully_pressed_value;

   // float high_count_deadzone_threshold =
   //    (float)(encoder_unpressed_value - encoder_fully_pressed_value) * (1.0f - PERCENT_DEFLECTION) +
   //    (float)encoder_fully_pressed_value;

   // if (encoder_counter_value > encoder_unpressed_value)
   //{
   //    // For down counting, when the encoder unpressed value is exceeded,
   //    // reset the encoder to the unpressed value and set 0% pedal press
   //    set_encoder_counter(encoder_unpressed_value);
   //    return MIN_ACCELERATOR_PEDAL_PRESS;
   //}
   // else if (encoder_counter_value >= high_count_deadzone_threshold)
   //{
   //    // For down count, set mapped pedal percentage to 0% when within
   //    // high end deadzone
   //    return MIN_ACCELERATOR_PEDAL_PRESS;
   //}
   // else if (encoder_counter_value <= low_count_deadzone_threshold)
   //{
   //    // For down count, set mapped pedal percentage to 100% when within the
   //    // low end deadzone
   //    return MAX_ACCELERATOR_PEDAL_PRESS;
   //}

   //// Pedal % = 100% * (high end deadzone threshold - encoder value) /
   //// (high end deadzone threshold - low end deadzone threshold)
   // return MAX_ACCELERATOR_PEDAL_PRESS * (high_count_deadzone_threshold - (float)encoder_counter_value) /
   //       (high_count_deadzone_threshold - low_count_deadzone_threshold);
   return 0.0f;
}
*/

struct AcceleratorPedals *App_AcceleratorPedals_Create(
    float (*get_primary_pedal_percent)(void),
    bool (*primary_pedal_OCSC)(void),
    float (*get_secondary_pedal_percent)(void),
    bool (*secondary_pedal_OCSC)(void))
{
    struct AcceleratorPedals *accelerator_pedals = malloc(sizeof(struct AcceleratorPedals));
    assert(accelerator_pedals != NULL);

    accelerator_pedals->get_primary_pedal_percent   = get_primary_pedal_percent;
    accelerator_pedals->get_secondary_pedal_percent = get_secondary_pedal_percent;
    accelerator_pedals->primary_pedal_OCSC          = primary_pedal_OCSC;
    accelerator_pedals->secondary_pedal_OCSC        = secondary_pedal_OCSC;

    accelerator_pedals->app_agreement_signal =
        App_SharedSignal_Create(AGREEMENT_TIME_TO_FAULT, AGREEMENT_TIME_TO_CLEAR);
    accelerator_pedals->papp_alarm_signal = App_SharedSignal_Create(PAPPS_OCSC_TIME_TO_FAULT, PAPPS_OCSC_TIME_TO_CLEAR);
    accelerator_pedals->sapp_alarm_signal = App_SharedSignal_Create(SAPPS_OCSC_TIME_TO_FAULT, SAPPS_OCSC_TIME_TO_CLEAR);
    accelerator_pedals->app_brake_signal  = App_SharedSignal_Create(APP_BRAKE_TIME_TO_FAULT, APP_BRAKE_TIME_TO_CLEAR);

    return accelerator_pedals;
}
void App_AcceleratorPedals_Destroy(struct AcceleratorPedals *accelerator_pedals)
{
    App_SharedSignal_Destroy(accelerator_pedals->app_agreement_signal);
    App_SharedSignal_Destroy(accelerator_pedals->papp_alarm_signal);
    App_SharedSignal_Destroy(accelerator_pedals->sapp_alarm_signal);
    App_SharedSignal_Destroy(accelerator_pedals->app_brake_signal);
    free(accelerator_pedals);
}

float App_AcceleratorPedals_GetPrimaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals)
{
    return accelerator_pedals->get_primary_pedal_percent();
}
float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals)
{
    return accelerator_pedals->get_secondary_pedal_percent();
}

void App_AcceleratorPedals_Broadcast(const struct FsmWorld *world)
{
    struct AcceleratorPedals *accelerator_pedals = App_FsmWorld_GetPappsAndSapps(world);

    float papps_pedal_percentage = accelerator_pedals->get_primary_pedal_percent();
    float sapps_pedal_percentage = accelerator_pedals->get_secondary_pedal_percent();
    App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Set(papps_pedal_percentage);
    App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Set(sapps_pedal_percentage);

    // Open Short Circuit Tests (non-understandable data test)
    const bool  primary_pedal_ocsc = accelerator_pedals->primary_pedal_OCSC();
    SignalState papp_signal_state =
        App_SharedSignal_Update(accelerator_pedals->papp_alarm_signal, primary_pedal_ocsc, !primary_pedal_ocsc);
    App_CanTx_FSM_Warning_PappsOCSCIsActive_Set(papp_signal_state == SIGNAL_STATE_ACTIVE);

    const bool  secondary_pedal_ocsc = accelerator_pedals->secondary_pedal_OCSC();
    SignalState sapp_signal_state =
        App_SharedSignal_Update(accelerator_pedals->sapp_alarm_signal, secondary_pedal_ocsc, !secondary_pedal_ocsc);
    App_CanTx_FSM_Warning_SappsOCSCIsActive_Set(sapp_signal_state == SIGNAL_STATE_ACTIVE);
    // torque 0
    if (papp_signal_state == SIGNAL_STATE_ACTIVE || sapp_signal_state == SIGNAL_STATE_ACTIVE)
    {
        App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Set(0.0f);
        App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Set(0.0f);
    }

    // Primary Secondary Accelerator Agreement (Inaccurate data)
    const float papp_sapp_diff =
        accelerator_pedals->get_primary_pedal_percent() - accelerator_pedals->get_secondary_pedal_percent();
    SignalState app_agreement_signal_state = App_SharedSignal_Update(
        accelerator_pedals->app_agreement_signal, (papp_sapp_diff) > 10.f, (papp_sapp_diff) <= 10.f);
    App_CanTx_FSM_Warning_AppsDisagreementIsActive_Set(app_agreement_signal_state == SIGNAL_STATE_ACTIVE);
    if (app_agreement_signal_state == SIGNAL_STATE_ACTIVE)
    {
        App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Set(0.0f);
        App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Set(0.0f);
    }

    // Accelerator Brake Plausibility (bad user input safety issues)
    struct Brake *brake                  = App_FsmWorld_GetBrake(world);
    SignalState   app_brake_disagreement = App_SharedSignal_Update(
        accelerator_pedals->app_brake_signal,
        App_Brake_IsBrakeActuated(brake) && accelerator_pedals->get_primary_pedal_percent() > 25,
        accelerator_pedals->get_primary_pedal_percent() < 5);

    App_CanTx_FSM_Warning_BrakeAccDisagreement_Set(app_brake_disagreement == SIGNAL_STATE_ACTIVE);
    if (app_brake_disagreement == SIGNAL_STATE_ACTIVE)
    {
        App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Set(0.0f);
    }
}
