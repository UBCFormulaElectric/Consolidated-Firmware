/**
 * @file io_ltc6813.h
 * @brief This file contains the interface to the LTC6813 driver
 * @note all implementation is in the ltc6813 directory
 */
#pragma once
#include <stdbool.h>

// physical constants
#define NUM_SEGMENTS 1
#define CELLS_PER_SEGMENT 14
#define THERMISTORS_PER_SEGMENT 8

// LTC6813 realities
#define VOLTAGE_REGISTER_GROUPS 5
#define THERMISTOR_REGISTER_GROUPS 3

/**
 * @file ltc6813/io_ltc6813_configs.c
 */

/**
 * Reads the configuration registers, and returns them into the pointer you give it
 * @return success of the operation
 */
bool io_ltc6813_readConfigurationRegisters();

/**
 * Writes a configuration to all segments on the daisy chain.
 * We have a default configuration. All elements that are not in the default configuration must be
 * present in the config variable passed into the function.
 * @param balance_config The balance configuration to write to the LTCs
 * @return success of the operation
 */
bool io_ltc6813_writeConfigurationRegisters(bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT]);

/**
 * @file ltc6813/io_ltc6813_cells.c
 */

/**
 * Reads all voltages from all segments
 * @param cell_voltages The resultant cell voltages
 * @param success This parameter will be assumed to contain all falses at the beginning
 * @return
 */
void io_ltc6813_readVoltages(
    float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS]);

/**
 *
 * @return success of the operation
 */
bool io_ltc6813_startCellsAdcConversion(void);

/**
 * @file ltc6813/io_ltc6813_temps.c
 */

/**
 * Reads all temperatures from all segments
 * @param cell_temps The resultant cell temperatures
 * @param success This parameter will be assumed to contain all falses at the beginning
 * @return
 */
void io_ltc6813_readTemperatures(
    float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][THERMISTOR_REGISTER_GROUPS]);

/**
 * sends a command to read all temperatures from all segments
 * @return success of the operation
 */
bool io_ltc6813_startThermistorsAdcConversion(void);

/**
 * @file ltc6813/io_ltc6813_utils.c
 */

/**
 * polls the LTC6813 for the completion of the ADC conversions
 * @return success of the operation
 */
bool io_ltc6813_pollAdcConversions();

/**
 * @file ltc6813/io_ltc6813_balance.c
 */

/**
 * Sends a command to enable balancing
 * @return success of operation
 */
bool io_ltc6813_sendBalanceCommand(void);

/**
 * Send a command to disable balancing
 * @return success of operation
 */
bool io_ltc6813_sendStopBalanceCommand(void);

/**
 * @file ltc6813/io_ltc6813_owc.c
 */

typedef enum
{
    PULL_DOWN,
    PULL_UP,
} PullDirection;
/**
 * Sends command to initiate open wire check
 * @param pull_direction The pull up or pull down phase of the open wire check
 * @return success of operation
 */
bool io_ltc6813CellVoltages_owc(PullDirection pull_direction);

/**
 * @file ltc6813/io_ltc6813_tests.c
 */

/**
 * self tests the cell adcs (muxes i think as well)
 * @returns if the test is successful
 * @note that the test can also fail due to comms issues
 */
bool io_ltc6813_sendSelfTestVoltages(void);

/**
 * self tests the aux adcs
 * @returns if the test is successful
 * @note that the test can also fail due to comms issues
 */
bool io_ltc6813_sendSelfTestAux(void);

/**
 * self tests the stat registers
 * @returns if the test is successful
 * @note that the test can also fail due to comms issues
 */
bool io_ltc6813_sendSelfTestStat(void);