#pragma once

#include <stdbool.h>
#include <stdint.h>

struct PreChargeSequence;

enum PreChargingStatus
{
    PRE_CHARGING_VOLTAGE_IN_RANGE,
    PRE_CHARGING_OVERVOLTAGE_ERROR,
    PRE_CHARGING_UNDERVOLTAGE_ERROR,
    PRE_CHARGING_SUCCESS
};

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
    void (*disable_pre_charge_sequence)(void),
    float (*get_ts_adc_voltage)(void),
    float (*get_ts_voltage)(float),
    float    accumulator_voltage,
    uint32_t rc_time_constant);

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
 *
 * @param pre_charge_sequence
 * @param current_time_ms
 */
void App_PreChargeSequence_SetPrevTimeMs(
    struct PreChargeSequence *const pre_charge_sequence,
    uint32_t                        current_time_ms);

/**
 *
 * @param pre_charge_sequence
 * @return
 */
struct PreChargeStateMachine *App_PreChargeSequence_GetStateMachine(
    const struct PreChargeSequence *pre_charge_sequence);

/**
 *
 * @param pre_charge_sequence
 * @return
 */
enum PreChargingStatus App_PreChargeSequence_GetPreChargingStatus(
    const struct PreChargeSequence *pre_charge_sequence);

/**
 *
 * @param pre_charge_sequence
 * @param current_ms
 */
enum PreChargingStatus App_PreChargeSequence_CheckPreChargingBusVoltage(
    struct PreChargeSequence *pre_charge_sequence,
    uint32_t                  current_ms);
