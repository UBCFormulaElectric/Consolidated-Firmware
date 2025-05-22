#pragma once
#include "io_ltc6813.h"

// =========== CONFIGS ===========
/**
 * Sets the ADC
 * @param speed desired speed to set it to
 */
void app_segments_adcSpeed(ADCSpeed speed);

/**
 * Sets the balancing configuration of all segments
 * @param balance_config the balancing configuration in question
 */
void app_segments_setBalanceConfig(const bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT]);

/**
 * Writes the default config to the internal config
 */
void app_segments_writeDefaultConfig();

/**
 * Syncs configs on LTC with the internal configs
 * @note you can use this to wake up the chip
 * @note This will block until it succeeds, during this time it will broadcast module comm error
 */
ExitCode app_segments_configSync();

// =========== BROADCAST MESSAGES ===========
/**
 * Reads the voltages, broadcasts them appropriately
 */
ExitCode app_segments_broadcastCellVoltages();

/**
 * Reads the temperatures, broadcasts them appropriately
 */
ExitCode app_segments_broadcastTempsVRef();

/**
 * Reads the status registers, broadcasts them appropriately
 */
ExitCode app_segments_broadcastStatus();

// =========== TESTS ===========
/**
 * This performs two tests:
 * First, it checks that the reference voltage is correct.
 * Then, it measures cells 7/13 (index 6/12) on ADC1/2 and ADC2/3 respectively.
 * If all tests pass, by transitivity, all ADCs must be working correctly.
 */
ExitCode app_segments_ADCAccuracyTest();

/**
 * Performs a voltage self test: it sends a command, which forces a generator to put specific values into the registers
 * This gives us confidence that there are no internal digital ADC problems in the ltc
 */
void app_segments_voltageSelftest();

/**
 * Performs a aux self test: it sends a command, which forces a generator to put specific values into the registers
 * This gives us confidence that there are no internal digital ADC problems in the ltc
 */
void app_segments_auxSelftest();

/**
 * Performs a status self test: it sends a command, which forces a generator to put specific values into the registers
 * This gives us confidence that there are no internal digital ADC problems in the ltc
 */
void app_segments_statusSelftest();

/**
 * Checks for open wires
 * It does this by pulling up then pulling down the GPIOs
 * @note That this function cleans up after itself, it will pull it back down once it is done
 * @note This takes about 1 second to run. Make sure to not do this unless you absolutely need to.
 */
ExitCode app_segments_openWireCheck();
