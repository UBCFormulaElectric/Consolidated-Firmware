#include <assert.h>
#include <stdlib.h>
#include "App_AcceleratorPedals.h"
#include "App_SharedSignal.h"


// config
#define APPS_TIME_TO_FAULT (100U)
#define APPS_TIME_TO_CLEAR (1000U)
#define PAPPS_TIME_TO_FAULT (10U)
#define PAPPS_TIME_TO_CLEAR (10U)
#define SAPPS_TIME_TO_FAULT (10U)
#define SAPPS_TIME_TO_CLEAR (10U)

struct AcceleratorPedals
{
    bool (*is_primary_encoder_alarm_active)(void);
    bool (*is_secondary_encoder_alarm_active)(void);
    float (*get_primary_pedal_percent)(void);
    float (*get_secondary_pedal_percent)(void);

    struct Signal *app_agreement_signal;
    struct Signal *papp_alarm_signal;
    struct Signal *sapp_alarm_signal;

    bool AppBreakInplausable;
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
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    float (*get_primary_pedal_percent)(void),
    float (*get_secondary_pedal_percent)(void))
{
    struct AcceleratorPedals *accelerator_pedals = malloc(sizeof(struct AcceleratorPedals));
    assert(accelerator_pedals != NULL);

    accelerator_pedals->is_primary_encoder_alarm_active   = is_primary_encoder_alarm_active;
    accelerator_pedals->is_secondary_encoder_alarm_active = is_secondary_encoder_alarm_active;
    accelerator_pedals->get_primary_pedal_percent         = get_primary_pedal_percent;
    accelerator_pedals->get_secondary_pedal_percent       = get_secondary_pedal_percent;

    accelerator_pedals->app_agreement_signal = App_SharedSignal_Create(APPS_TIME_TO_FAULT, APPS_TIME_TO_CLEAR);
    accelerator_pedals->papp_alarm_signal    = App_SharedSignal_Create(PAPPS_TIME_TO_FAULT, PAPPS_TIME_TO_CLEAR);
    accelerator_pedals->sapp_alarm_signal    = App_SharedSignal_Create(SAPPS_TIME_TO_FAULT, SAPPS_TIME_TO_CLEAR);

    accelerator_pedals->AppBreakInplausable = false;

    return accelerator_pedals;
}
void App_AcceleratorPedals_Destroy(struct AcceleratorPedals *accelerator_pedals)
{
    free(accelerator_pedals);
}

bool App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(const struct AcceleratorPedals *const accelerator_pedals)
{
    return accelerator_pedals->is_primary_encoder_alarm_active();
}
bool App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(const struct AcceleratorPedals *const accelerator_pedals)
{
    return accelerator_pedals->is_secondary_encoder_alarm_active();
}
float App_AcceleratorPedals_GetPrimaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals)
{
    return accelerator_pedals->get_primary_pedal_percent();
    // return App_GetPedalPercentage_CountDown(
    //    PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
    //    accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
}
float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals)
{
    return accelerator_pedals->get_primary_pedal_percent();
    /*
    return App_GetPedalPercentage_CountDown(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
        */
}

void App_AcceleratorPedals_Broadcast(struct FsmWorld* world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);
    struct AcceleratorPedals *accelerator_pedals = App_FsmWorld_GetPappsAndSapps(world);
    struct Brake *            brake = App_FsmWorld_GetBrake(world);

    const float papps_pedal_percentage = App_AcceleratorPedals_GetPrimaryPedalPercentage(accelerator_pedals);
    const float sapps_pedal_percentage = App_AcceleratorPedals_GetSecondaryPedalPercentage(accelerator_pedals);

    // update signals
    SignalState app_agreement_signal_state = App_SharedSignal_Update(
        accelerator_pedals->app_agreement_signal,
        (accelerator_pedals->get_primary_pedal_percent() - accelerator_pedals->get_secondary_pedal_percent()) > 10.f,
        (accelerator_pedals->get_primary_pedal_percent() - accelerator_pedals->get_secondary_pedal_percent()) <= 10.f);
    SignalState papp_signal_state = App_SharedSignal_Update(
        accelerator_pedals->papp_alarm_signal, accelerator_pedals->is_primary_encoder_alarm_active(),
        !(accelerator_pedals->is_primary_encoder_alarm_active() ||
          accelerator_pedals->is_secondary_encoder_alarm_active));
    SignalState sapp_signal_state = App_SharedSignal_Update(
        accelerator_pedals->sapp_alarm_signal, accelerator_pedals->is_secondary_encoder_alarm_active(),
        !(accelerator_pedals->is_primary_encoder_alarm_active() ||
          accelerator_pedals->is_secondary_encoder_alarm_active));

    if (accelerator_pedals->AppBreakInplausable)
    {
        if (accelerator_pedals->get_primary_pedal_percent() < 0.05f)
        {
            accelerator_pedals->AppBreakInplausable = false;
            // TODO signal cancel for failure
        }
        else
        {
            App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
        }
    }
    else if (App_Brake_IsBrakeActuated(brake) && accelerator_pedals->get_primary_pedal_percent() > 0.25f)
    {
        accelerator_pedals->AppBreakInplausable = true;
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
        //TODO SIGNAL FOR THIS FAILURE
    }
    else if (app_agreement_signal_state == SIGNAL_STATE_CLEAR)
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
        App_CanTx_SetPeriodicSignal_APPS_HAS_DISAGREEMENT(
            can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE);
    }
    else if (app_agreement_signal_state == SIGNAL_STATE_ACTIVE)
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, papps_pedal_percentage);
        App_CanTx_SetPeriodicSignal_APPS_HAS_DISAGREEMENT(
            can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);
    }
    else
    {
        // SIGNAL FAILURE??
    }

    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, sapps_pedal_percentage);

    if (papp_signal_state == SIGNAL_STATE_ACTIVE)
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
        App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
        App_CanTx_SetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(
            can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
    }
    else if (papp_signal_state == SIGNAL_STATE_CLEAR)
    {
        App_CanTx_SetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(
            can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE);
    }
    else
    {
        // signal failure?
    }
    if (sapp_signal_state == SIGNAL_STATE_ACTIVE)
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
        App_CanTx_SetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(
            can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE);
    }
    else if (sapp_signal_state == SIGNAL_STATE_CLEAR)
    {
        App_CanTx_SetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(
            can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE);
    }
    else
    {
        // signal failure?
    }
}
