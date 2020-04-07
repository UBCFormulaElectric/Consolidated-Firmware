#pragma once

#include "auto_generated/App_CanTx.h"

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
    const char *name,
    float (*get_voltage)(void),
    float (*get_min_voltage)(void),
    float (*get_max_voltage)(void),
    void (*error_callback)(struct VoltageMonitor *));

/**
 * Destroy the given voltage monitor
 * @param voltage_monitor The voltage monitor to destroy
 */
void App_VoltageMonitor_Destroy(struct VoltageMonitor *voltage_check);

/**
 * Update the status for the given voltage monitor
 * @param voltage_monitor The voltage monitor to update for
 */
void App_VoltageMonitor_Tick(struct VoltageMonitor *voltage_monitor);

/**
 * Get the status for the given voltage monitor
 * @param voltage_monitor The voltage monitor to get status for
 * @return The status for the given voltage monitor
 */
enum VoltageMonitor_Status
    App_VoltageMonitor_GetStatus(const struct VoltageMonitor *voltage_monitor);

/**
 * Get the name for the given voltage monitor
 * @param voltage_monitor The voltage monitor to get name for
 * @return The name for the given voltage monitor
 */
const char *
    App_VoltageMonitor_GetName(const struct VoltageMonitor *voltage_monitor);
