#pragma once
#include "io_ltc6813.h"

extern ExitCode cell_voltage_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];

// =========== CONFIGS ===========

/**
 * Sets the balancing configuration of all segments
 * @param balance_config the balancing configuration in question
 */
void app_segments_setBalanceConfig(const bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT]);

/**
 * Writes the default config to the internal config
 */
void app_segments_writeDefaultConfig(void);

/**
 * Syncs configs on LTC with the internal configs
 * @note you can use this to wake up the chip
 * @note This will block until it succeeds, during this time it will broadcast module comm error
 */
ExitCode app_segments_configSync(void);

// =========== CONVERSION AND TEST RUNNERS ===========

/**
 * Perform IO operations to convert cell voltages.
 */
ExitCode app_segments_runVoltageConversion(void);

/**
 * Perform IO operations to convert AUX voltages.
 */
ExitCode app_segments_runAuxConversion(void);

/**
 * Perform IO operations to convert status registers.
 */
ExitCode app_segments_runStatusConversion(void);

/**
 * This performs two tests:
 * First, it checks that the reference voltage is correct.
 * Then, it measures cells 7/13 (index 6/12) on ADC1/2 and ADC2/3 respectively.
 * If all tests pass, by transitivity, all ADCs must be working correctly.
 */
ExitCode app_segments_runAdcAccuracyTest(void);

/**
 * Performs a voltage self test: it sends a command, which forces a generator to put specific values into the registers
 * This gives us confidence that there are no internal digital ADC problems in the ltc
 */
ExitCode app_segments_runVoltageSelfTest(void);

/**
 * Performs a aux self test: it sends a command, which forces a generator to put specific values into the registers
 * This gives us confidence that there are no internal digital ADC problems in the ltc
 */
ExitCode app_segments_runAuxSelfTest(void);

/**
 * Performs a status self test: it sends a command, which forces a generator to put specific values into the registers
 * This gives us confidence that there are no internal digital ADC problems in the ltc
 */
ExitCode app_segments_runStatusSelfTest(void);

/**
 * Checks for open wires
 * It does this by pulling up then pulling down the GPIOs
 * @note That this function cleans up after itself, it will pull it back down once it is done
 * @note This takes about 1 second to run. Make sure to not do this unless you absolutely need to.
 */
ExitCode app_segments_runOpenWireCheck(void);

// =========== BROADCAST MESSAGES ===========

/**
 * Unpack and broadcast values previously read from cell voltage registers.
 * Performs no IO operations.
 */
void app_segments_broadcastCellVoltages(void);

/**
 * Unpack and broadcast values previously read from aux registers (containing temp and Vref).
 * Performs no IO operations.
 */
void app_segments_broadcastTempsVRef(void);

/**
 * Unpack and broadcast values previously read from status registers.
 * Performs no IO operations.
 */
void app_segments_broadcastStatus(void);

/**
 * Unpack and broadcast values previously read for an ADC accuracy test.
 * Performs no IO operations.
 */
void app_segments_broadcastAdcAccuracyTest(void);

/**
 * Unpack and broadcast values previously read for a voltage self test.
 * Performs no IO operations.
 */
void app_segments_broadcastVoltageSelfTest(void);

/**
 * Unpack and broadcast values previously read for an aux self test.
 * Performs no IO operations.
 */
void app_segments_broadcastAuxSelfTest(void);

/**
 * Unpack and broadcast values previously read for a status self test.
 * Performs no IO operations.
 */
void app_segments_broadcastStatusSelfTest(void);

/**
 * Unpack and broadcast values previously read for an open wire check.
 * Performs no IO operations.
 */
void app_segments_broadcastOpenWireCheck(void);
