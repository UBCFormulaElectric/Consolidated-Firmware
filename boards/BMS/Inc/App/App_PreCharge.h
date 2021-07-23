#pragma once

#include <stdbool.h>
#include <stdint.h>

struct PreCharge;

/**
 * Allocate and initialize a pre-charge sequence
 * @param enable A function that can be called to enable
 * the pre-charge sequence
 * @param disable A function that can be called to disable
 * the pre-charge sequence
 * @param get_ts_voltage A function that can be called to get the tractive
 * system voltage in V. Takes in the ADC voltage as an input parameter
 *      @see github.com/UBCFormulaElectric/Consolidated-Firmware/blob/master/boards/BMS/Src/Io/Io_VoltageSense.c#L4
 * @param min_duration_ms The time associated with the
 * inverter/charger capacitors being charged to 90% of the accumulator voltage
 * @param min_complete_duration_ms The minimum amount of time the precharge must take
 * @param max_complete_duration_ms The maximum amount of time the precharge can time. If precharge takes longer a
 * precharge fault error is broadcasted over CAN
 * @return The created pre-charge sequence, whose ownership is given to the
 * caller
 */
struct PreCharge *App_PreCharge_Create(
    void (*enable)(void),
    void (*disable)(void),
    float (*get_raw_ts_voltage)(void),
    float (*get_ts_voltage)(float),
    uint32_t min_duration_ms,
    uint32_t min_complete_duration_ms,
    uint32_t max_complete_duration_ms);

/**
 * Deallocate the memory used by the pre-charge sequence
 * @param pre_charge The pre-charge sequence to deallocate
 */
void App_PreCharge_Destroy(struct PreCharge *pre_charge);

/**
 * Enable the given pre-charge sequence
 * @param pre_charge The pre-charge sequence to enable
 */
void App_PreCharge_Enable(const struct PreCharge *const pre_charge);

/**
 * Disable the given pre-charge sequence
 * @param pre_charge The pre-charge sequence to disable
 */
void App_PreCharge_Disable(const struct PreCharge *const pre_charge);

/**
 * Get the tractive system voltage
 * @param pre_charge The pre-charge sequence to get the tractive system voltage
 * for
 * @return The tractive system voltage in V
 */
float App_PreCharge_GetTractiveSystemVoltage(
    const struct PreCharge *pre_charge);

/**
 * Get the minimum pre-charge duration
 * @param pre_charge The pre-charge sequence to get the minimum
 * pre-charge duration
 * @return The minimum pre-charge duration in ms
 */
uint32_t
    App_PreCharge_GetMinDurationMs(const struct PreCharge *const pre_charge);

/**
 * Get the minimum duration for a completed pre-charge
 * @param pre_charge The pre-charge sequence to get the minimum duration for a
 * completed pre-charge
 * @return The minimum duration for a completed pre-charge in ms
 */
uint32_t
    App_PreCharge_GetMinCompleteDurationMs(const struct PreCharge *pre_charge);

/**
 * Get the maximum duration for a completed pre-charge
 * @param pre_charge The pre-charge sequence to get the maximum duration for a
 * completed pre-charge
 * @return The maximum duration for a completed pre-charge in ms
 */
uint32_t
    App_PreCharge_GetMaxCompleteDurationMs(const struct PreCharge *pre_charge);
