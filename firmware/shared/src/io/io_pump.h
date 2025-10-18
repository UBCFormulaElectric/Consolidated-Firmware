#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "app_utils.h"
#include "io_efuse/io_efuse.h"
#include "io_efuse/io_efuse_datatypes.h"
#include "io_potentiometer.h"

typedef enum
{
    RR_PUMP = 0,
    RL_PUMP,
    F_PUMP,
    PUMP_COUNT 
} PumpID;

typedef struct
{
    const Potentiometer *pot;
    POTENTIOMETER_WIPER  wiper;
    const Efuse         *efuse;
    bool                 invert;
} PumpConfig;

/**
 * Initialize the table of pumps and ensure they have been created 
 * @param table table of pumps @param count the amount of pumps 
 */
void io_pumps_init(const PumpConfig *table, size_t count);

/**
 * Register a table of pump configuration pointers.
 * @param table Pointer array indexed by PumpID
 * @param count Number of valid entries in table
 */
void io_pumps_registerConfig(PumpConfig *const table[], size_t count);

/**
 * Takes the requested percentage for the wiper and converts it into a hw_percentage and writes to the pot
 * @param id id of pump based on enum @param id user requested percentage 
 * @return pot being fed the percentage
 */
ExitCode io_pump_setPercent(PumpID id, uint8_t percent);

/**
 * Reads the resistance from the pot and displays as a logical_percent
 * @param id id of pump based on enum @param id 
 * @return logical_percent
 */
ExitCode io_pump_getPercent(PumpID id, uint8_t *percent_out);

/**
 * Enables or disables the pump's eFuse load switch
 * @param id Pump to control
 * @param enable true to turn on, false to turn off
 * @return EXIT_CODE_OK on success, EXIT_CODE_UNIMPLEMENTED if the pump lacks a load switch
 */
ExitCode io_pump_enable(PumpID id, bool enable);

/**
 * Returns whether the pump's load switch is currently enabled
 * @param id Pump to query
 * @param enabled_out Destination for the enabled state
 * @return EXIT_CODE_OK on success, EXIT_CODE_INVALID_ARGS/EXIT_CODE_UNIMPLEMENTED on error
 */
ExitCode io_pump_isEnabled(PumpID id, bool *enabled_out);

/**
 * Reads the signal for the pump
 * @param id Pump to query
 * @param pgood_out Destination for the power-good status
 * @return EXIT_CODE_OK on success, EXIT_CODE_INVALID_ARGS/EXIT_CODE_UNIMPLEMENTED on error
 */
ExitCode io_pump_getHealth(PumpID id, bool *pgood_out);
