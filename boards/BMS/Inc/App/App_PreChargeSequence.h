#pragma once

#include <stdbool.h>
#include <stdint.h>

struct PreChargeSequence;

/**
 * Allocate and initialize a pre-charge sequence
 * @param enable_pre_charge_sequence A function that can be called to enable
 * the pre-charge sequence
 * @param disable_pre_charge_sequence A function that can be called to disable
 * the pre-charge sequence
 * @return The created pre-charge sequence, whose ownership is given to the
 * caller
 */
struct PreChargeSequence *App_PreChargeSequence_Create(
    void (*enable_pre_charge_sequence)(void),
    void (*disable_pre_charge_sequence)(void));

/**
 * Deallocate the memory used by the pre-charge sequence
 * @param pre_charge_sequence The pre-charge sequence to deallocate
 */
void App_PreChargeSequence_Destroy(
    struct PreChargeSequence *pre_charge_sequence);

/**
 * Enable the given pre-charge sequence
 * @param pre_charge_sequence The pre-charge sequence to enable
 */
void App_PreChargeSequence_Enable(
    const struct PreChargeSequence *pre_charge_sequence);

/**
 * Disable the given pre-charge sequence
 * @param pre_charge_sequence The pre-charge sequence to disable
 */
void App_PreChargeSequence_Disable(
    const struct PreChargeSequence *pre_charge_sequence);

/**
 * Gets the precharge threshold as a percent decimal value
 * @param pre_charge_sequence The pre-charge sequence to get the threshold from
 * @return pre_charge_threshold_percent
 */
float App_PreChargeSequence_GetThreshold_Percent(
    const struct PreChargeSequence *pre_charge_sequence);

/**
 * Estimates an upper time bound for the pre-charge sequence.
 * The pre-charge sequence should not take longer than this time.
 * @param pre_charge_sequence The pre-charge sequence to estimate for
 * @param tractive_system_capacitance The capacitance of the connected tractive
 * system
 * @return pre_charge_upper_time_bound The upper time bound for the pre-charge
 * sequence
 */
uint32_t App_PreChargeSequence_GetUpperTimeBound_MS(
    const struct PreChargeSequence *const pre_charge_sequence,
    float                           tractive_system_capacitance);

/**
 * Estimates a lower time bound for the pre-charge sequence.
 * The pre-charge sequence should be quicker than this time.
 * @param pre_charge_sequence The pre-charge sequence to estimate for
 * @param tractive_system_capacitance The capacitance of the connected tractive
 * system
 * @return precharge_upper_time_bound The upper time bound for the pre-charge
 * sequence
 */
uint32_t App_PreChargeSequence_GetLowerTimeBound_MS(
    const struct PreChargeSequence *const pre_charge_sequence,
    float                           tractive_system_capacitance);
