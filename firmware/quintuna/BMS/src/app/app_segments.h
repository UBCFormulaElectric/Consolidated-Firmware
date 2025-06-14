#pragma once
#include "io_ltc6813.h"

#define NUM_CYCLES_TO_SETTLE (30U)

// =========== CONFIGS ===========
/**
 * @file segments/app_segments_configs.c
 */

typedef enum
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
} ThermistorMux;

/**
 * Writes the default config to the internal config
 */
void app_segments_setDefaultConfig(void);

/**
 * Sets the balancing configuration of all segments
 * @param balance_config the balancing configuration in question
 */
void app_segments_setBalanceConfig(const bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT]);

/**
 * Sets the thermistor MUX config of all segments by writing to GPIO9.
 * @param mux MUX config.
 */
void app_segments_setThermistorMuxConfig(ThermistorMux mux);

/**
 * Syncs configs on LTC with the internal configs
 * @note you can use this to wake up the chip
 * @note This will block until it succeeds, during this time it will broadcast module comm error
 */
ExitCode app_segments_configSync(void);

ExitCode app_segments_writeConfig(void);

/**
 * @file segments/app_segments_conversions.c
 */

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

/**
 * @file segments/app_segments_broadcast.c
 */

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

/**
 * Broadcast stats (min, max, avg) for voltages.
 */
void app_segments_broadcastVoltageStats(void);

/**
 * Broadcast stats (min, max, avg) for temps.
 */
void app_segments_broadcastTempStats(void);

/**
 * @file segments/app_segments_faults.c
 */

/**
 * Init faults module (init debounce timers).
 */
void app_segments_initFaults(void);

/**
 * Returns true if a warning is present.
 */
bool app_segments_checkWarnings(void);

/**
 * Returns true if there is a rules-required BMS fault present that we need to latch BMS_OK false.
 */
bool app_segments_checkFaults(void);

/**
 * @file segments/app_segments_balancing.c
 */

void app_segments_balancingInit(void);
void app_segments_balancingTick(bool enabled);

/**
 * @file segments/app_segments_getters.c
 */

typedef struct
{
    uint8_t segment;
    uint8_t cell;
    float   value;
} CellParam;

/**
 * Get the accumulator's total voltage
 */
float app_segments_getPackVoltage(void);

/**
 * Get the highest voltage of any cell in the accumulator
 */
CellParam app_segments_getMaxCellVoltage(void);

/**
 * Get the lowest voltage of any cell in the accumulator
 */
CellParam app_segments_getMinCellVoltage(void);

/**
 * Get the highest temp of any cell in the accumulator
 */
CellParam app_segments_getMaxCellTemp(void);

/**
 * Get the lowest temp of any cell in the accumulator
 */
CellParam app_segments_getMinCellTemp(void);
