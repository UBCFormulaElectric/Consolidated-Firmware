#pragma once

struct VoltageMonitor;

enum VoltageMonitor_Status
{
    VOLTAGEMONITOR_IN_RANGE,
    VOLTAGEMONITOR_UNDERVOLTAGE,
    VOLTAGEMONITOR_OVERVOLTAGE
};

/**
 * Allocate and initialize a voltage monitor
 * @get_voltage A function that can be called to get the voltage
 * @get_min_voltage A function that can be called to get the minimum voltage
 * @get_max_voltage A function that can be called to get the maximum voltage
 * @return A pointer to the created voltage monitor, whose ownership is given to
 * the caller
 */
struct VoltageMonitor *App_VoltageMonitor_Create(
    float (*const get_voltage)(void),
    float (*const get_min_voltage)(void),
    float (*const get_max_voltage)(void));

/**
 * Update the status for the given voltage monitor
 * @param voltage_monitor The voltage monitor to update for
 */
void App_VoltageMonitor_Tick(struct VoltageMonitor *const voltage_monitor);

/**
 * Get the status for the given voltage monitor
 * @param voltage_monitor The voltage monitor to get status for
 * @return The status for the given voltage monitor
 */
enum VoltageMonitor_Status App_VoltageMonitor_GetStatus(
    const struct VoltageMonitor *const voltage_monitor);

/**
 * Destroy the given voltage monitor
 * @param voltage_monitor The voltage monitor to destroy
 */
void App_VoltageMonitor_Destroy(struct VoltageMonitor *const voltage_check);
