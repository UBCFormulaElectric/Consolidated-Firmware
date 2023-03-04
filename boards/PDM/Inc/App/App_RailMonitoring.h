#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "configs/App_VoltageLimits.h"

struct RailMonitoring;

struct RailMonitoring *App_RailMonitoring_Create(
        float (*get_vbat_voltage)(void),
        float (*get_24v_acc_voltage)(void),
        float (*get_22v_aux_voltage)(void));
/**
 * Destroys RailMonitoring structure from memory
 * @param rail_monitor
 */
void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor);

/**
 * Gets VBAT Voltage Value from PDM LV Battery
 * @param rail_monitor
 * @return VBAT Voltage in Volts
 */
float App_RailMonitoring_GetVbatVoltage(struct RailMonitoring *rail_monitor);

/**
 * Gets 24V Accumulator Voltage Value from PDM LV Battery
 * @param rail_monitor
 * @return 24V Accumulator Voltage in Volts
 */
float App_RailMonitoring_Get_24VAccumulatorVoltage(struct RailMonitoring *rail_monitor);

/**
 * Gets 22V Auxiliary Voltage Value from PDM LV Battery
 * @param rail_monitor
 * @return 22V Auxiliary Voltage in Volts
 */
float App_RailMonitoring_Get_22VAuxiliaryVoltage(struct RailMonitoring *rail_monitor);

/**
 * Checks if Vbat Voltage from PDM is too low
 * @param rail_monitor
 * @return True if VBAT Voltage too low, false otherwise
 */
bool App_RailMonitoring_VbatVoltageLowCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if Vbat Voltage from PDM is too high
 * @param rail_monitor
 * @return True if VBAT Voltage too high, false otherwise
 */
bool App_RailMonitoring_VbatVoltageHighCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if 24V Accumulator Voltage from PDM is too low
 * @param rail_monitor
 * @return True if 24V Accumulator Voltage too low, false otherwise
 */
bool App_RailMonitoring_24VAccumulatorVoltageLowCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if 24V Accumulator Voltage from PDM is too high
 * @param rail_monitor
 * @return True if 24V Accumulator Voltage too high, false otherwise
 */
bool App_RailMonitoring_24VAccumulatorVoltageHighCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if 22V Auxiliary Voltage from PDM is too low
 * @param rail_monitor
 * @return True if 24V Auxiliary Voltage too low, false otherwise
 */
bool App_RailMonitoring_22VAuxiliaryVoltageLowCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if 22V Auxiliary Voltage from PDM is too high
 * @param rail_monitor
 * @return True if 22V Auxiliary Voltage too high, false otherwise
 */
bool App_RailMonitoring_22VAuxiliaryVoltageHighCheck(struct RailMonitoring *rail_monitor);
