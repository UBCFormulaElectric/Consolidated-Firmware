#pragma once
#include "App_SharedExitCode.h"

struct CellMonitor;

/**
 * Allocate and initialize a cell monitor.
 * @param configure_daisy_chain A function that can be called to configure the
 * daisy chain of cell monitoring chips.
 * @param read_cell_voltages A function that can be called to read cell voltages
 * from the daisy chain of cell monitoring chips.
 * @param get_cell_voltages A function that can be called to get the pointer to
 * a 2D array containing the cell voltages read from the daisy chain of cell
 * monitoring chips.
 * @param min_cell_voltage The minimum allowed cell voltage, exclusive.
 * @param max_cell_voltage The maximum allowed cell voltage, exclusive.
 * @param num_of_cells_per_segment The number of cell voltages measured per
 * accumulator segment.
 * @return A pointer to the created cell monitor, whose ownership is given to
 * the caller.
 */
struct CellMonitor *App_CellMonitor_Create(
    void (*configure_daisy_chain)(void),
    ExitCode (*read_cell_voltages)(void),
    uint16_t *(*get_cell_voltages)(void),
    float    min_cell_voltage,
    float    max_cell_voltage,
    uint32_t num_of_cells_per_segment);

/**
 * Deallocate the memory used by the given cell monitor.
 * @param cell_monitor The cell monitor to deallocate.
 */
void App_CellMonitor_Destroy(struct CellMonitor *cell_monitor);

/**
 * Configure the daisy chain of cell monitoring chips.
 * @param cell_monitor The given cell monitor.
 */
void App_CellMonitor_Configure(const struct CellMonitor *cell_monitor);

/**
 * Read cell voltages from the daisy chain of cell monitoring chips.
 * @param cell_monitor The given cell monitor.
 * @return EXIT_CODE_OK if cell voltages were read successfully from the daisy
 * chain of cell monitoring chips. Else, return EXIT_CODE_ERROR.
 */
ExitCode App_CellMonitor_ReadCellVoltages(
    const struct CellMonitor *const cell_monitor);

/**
 * Get the the average voltage in V for the 0th segment.
 * @param cell_monitor The given cell monitor.
 * @return The average voltage in V for the 0th segment.
 */
struct InRangeCheck *
    App_CellMonitor_GetAverageSegment0Voltage(struct CellMonitor *cell_monitor);

/**
 * Get the the average voltage in V for the 1st segment.
 * @param cell_monitor The given cell monitor.
 * @return The average voltage in V for the 1st segment.
 */
struct InRangeCheck *
    App_CellMonitor_GetAverageSegment1Voltage(struct CellMonitor *cell_monitor);

/**
 * Get the the average voltage in V for the 2nd segment.
 * @param cell_monitor The given cell monitor.
 * @return The average voltage in V for the 2nd segment.
 */
struct InRangeCheck *App_CellMonitoring_GetAverageSegment2Voltage(
    struct CellMonitor *cell_monitor);

/**
 * Get the the average voltage in V for the 3rd segment.
 * @param cell_monitor The given cell monitor.
 * @return The average voltage in V for the 3rd segment.
 */
struct InRangeCheck *
    App_CellMonitor_GetAverageSegment3Voltage(struct CellMonitor *cell_monitor);

/**
 * Get the the average voltage in V for the 4th segment.
 * @param cell_monitor The given cell monitor.
 * @return The average voltage in V for the 4th segment.
 */
struct InRangeCheck *
    App_CellMonitor_GetAverageSegment4Voltage(struct CellMonitor *cell_monitor);

/**
 * Get the the average voltage in V for the 5th segment.
 * @param cell_monitor The given cell monitor.
 * @return The average voltage in V for the 5th segment.
 */
struct InRangeCheck *
    App_CellMonitor_GetAverageSegment5Voltage(struct CellMonitor *cell_monitor);

/**
 * Get the the pack voltage in V.
 * @param cell_monitor The given cell monitor.
 * @return The pack voltage in V.
 */
struct InRangeCheck *
    App_CellMonitor_GetPackVoltage(struct CellMonitor *cell_monitor);

/**
 * Get the absolute minimum voltage in V.
 * @param cell_monitor The given cell monitor.
 * @return The minimum cell voltage in V.
 */
struct InRangeCheck *
    App_CellMonitor_GetMinVoltage(struct CellMonitor *cell_monitor);

/**
 * Get the absolute maximum cell voltage in V.
 * @param cell_monitor
 * @return The maximum cell voltage in V.
 */
struct InRangeCheck *
    App_CellMonitor_GetMaxVoltage(struct CellMonitor *cell_monitor);

/**
 * Get the average cell voltage in V.
 * @param cell_monitor The given cell monitor.
 * @return The average cell voltage in V.
 */
struct InRangeCheck *
    App_CellMonitor_GetAverageCellVoltage(struct CellMonitor *cell_monitor);
