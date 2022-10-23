#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"
#include "App_Brake.h"
#include "App_Timer.h"

#define PAPPS_ENCODER_FULLY_PRESSED_VALUE (937U)
#define PAPPS_ENCODER_UNPRESSED_VALUE (1353U)
#define SAPPS_ENCODER_FULLY_PRESSED_VALUE (310U)
#define SAPPS_ENCODER_UNPRESSED_VALUE (0U)
#define SAPPS_ENCODER_RESET_VALUE (2400U)

#define MAX_ACCELERATOR_PEDAL_PRESS (100.0f)
#define MIN_ACCELERATOR_PEDAL_PRESS (0.0f)
#define PERCENT_DEFLECTION (0.03f)

struct AcceleratorPedals;
struct Brake;

/**
 * Allocate and initialize a pair of accelerator pedals
 * @param is_primary_encoder_alarm_active A function that can be called to check whether the primary encoder alarm is
 * active (Modeled after MAX3097E)
 * @param is_secondary_encoder_alarm_active A function that can be called to check whether the secondary encoder alarm
 * is active (Modeled after MAX3097E)
 * @param get_primary_encoder_counter_value A function that can be called to get the counter value of the primary
 * encoder
 * @param get_secondary_encoder_counter_value A function that can be called to get the counter value of the secondary
 * encoder
 * @param reset_primary_encoder_counter A function that can be called to reset the counter value of the primary encoder
 * to 0
 * @param reset_secondary_encoder_counter A function that can be called to reset the counter value of the secondary
 * encoder to 0
 * @param primary_encoder_fully_pressed_value The value of the primary encoder counter calibrated to the pedal box
 * corresponding to when the pedal is fully pressed
 * @param secondary_encoder_fully_pressed_value The value of the secondary
 * encoder counter calibrated to the pedal box corresponding to when the pedal is fully pressed
 * @return The created pair of accelerator pedals, whose ownership is given to the caller
 */
struct AcceleratorPedals *App_AcceleratorPedals_Create(
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    float (*get_primary_pedal_percent)(void),
    float (*get_secondary_pedal_percent)(void));

/**
 * Deallocate the memory used by the given pair of accelerator pedals
 * @param accelerator_pedals The pair of accelerator pedals to deallocate
 */
void App_AcceleratorPedals_Destroy(struct AcceleratorPedals *accelerator_pedals);

/**
 * Check if the encoder alarm for the primary accelerator pedal is active
 * @param accelerator_pedals The pair of accelerator pedals containing the
 * primary accelerator pedal to check
 * @return true if the encoder alarm for the given accelerator pedal is active, else false
 */
bool App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Check if the encoder alarm for the secondary accelerator pedal is active
 * @param accelerator_pedals The pair of accelerator pedals containing the
 * secondary accelerator pedal to check
 * @return true if the encoder alarm for the given accelerator pedal is active,
 *         else false
 */
bool App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Get the pedal percentage of the primary accelerator pedal, a value in [0,100]
 * @param accelerator_pedals The pair of accelerator pedals to get the primary pedal percentage from
 * @return The pedal percentage of the primary accelerator pedal
 */
float App_AcceleratorPedals_GetPrimaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Get the pedal percentage of the secondary accelerator pedal, a value in [0,100]
 * @param accelerator_pedals The pair of accelerator pedals to get the secondary pedal percentage from
 * @return The pedal percentage of the secondary accelerator pedal
 */
float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Get the entry timer for app agreement
 * @param accelerator_pedals The pair of accelerator pedals to get the entry timer for app agreement from
 * @return The entry timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_GetAppAgreementEntryTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the exit timer for app agreement
 * @param accelerator_pedals The pair of accelerator pedals to get the exit timer for app agreement from
 * @return The entry timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_GetAppAgreementExitTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the entry timer for APP and Brake Plausability
 * @param accelerator_pedals The pair of accelerator pedals to get the entry timer for APP and Brake Plausability
 * @return The entry timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_GetAppBreakPlausabilityEntryTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the entry timer for APP and Brake Plausability
 * @param accelerator_pedals The pair of accelerator pedals to get the exit timer for APP and Brake Plausability from
 * @return The exit timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_GetAppBreakPlausabilityExitTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the entry timer for Papp Alarm
 * @param accelerator_pedals The pair of accelerator pedals to get the entry timer for Papp Alarm from
 * @return The entry timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_PappAlarmEntryTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the entry timer for Papp Alarm
 * @param accelerator_pedals The pair of accelerator pedals to get the entry timer for Papp Alarm from
 * @return The entry timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_PappAlarmExitTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the entry timer for Sapp Alarm
 * @param accelerator_pedals The pair of accelerator pedals to get the exit timer for Sapp Alarm from
 * @return The exit timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_SappAlarmEntryTimer(const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the entry timer for Sapp Alarm
 * @param accelerator_pedals The pair of accelerator pedals to get the exit timer forSapp Alarm from
 * @return The exit timer of the accelerator pedal
 */
TimerChannel * App_AcceleratorPedals_SappAlarmExitTimer(const struct AcceleratorPedals *accelerator_pedals);


/**
 * Primary Broadcast Function. Very important.
 * @param can_tx The Can Bus it is broadcasting to
 * @param accelerator_pedals Accelerator System from which to pull data
 * @param brake Brakes System Object from which to pull data
 */
void App_AcceleratorPedals_Broadcast(
    struct FsmCanTxInterface *can_tx,
    struct AcceleratorPedals *accelerator_pedals,
    struct Brake *            brake);
