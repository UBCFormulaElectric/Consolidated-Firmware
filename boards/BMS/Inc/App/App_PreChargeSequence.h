#pragma once

#include <stdbool.h>

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
 * @param pre_charge_sequence
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
