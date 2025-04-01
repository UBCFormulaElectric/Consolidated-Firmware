#pragma once
#include <stdbool.h>
#include <stdint.h>

// physical constants
#define NUM_SEGMENTS 1
#define CELLS_PER_SEGMENT 16
#define THERMISTORS_PER_SEGMENT 8

// LTC6813 realities
#define VOLTAGE_REGISTER_GROUPS 6
#define THERMISTOR_REGISTER_GROUPS 3

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
 * Sends a command to all segments on the daisy chain
 * In particular, it sends a single command, which is not shifted. However, it will affect all segments
 * @param command command to send
 * @note that I decided not to use enums for command, as there are certain commands which need to built on the fly
 * @return success of the operation
 */
bool io_ltc6813_sendCommand(uint16_t command);

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
 * Reads all temperatures from all segments
 * @param cell_temps The resultant cell temperatures
 * @param success This parameter will be assumed to contain all falses at the beginning
 * @return
 */
void io_ltc6813_readTemperatures(
    float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][THERMISTOR_REGISTER_GROUPS]);

/**
 *
 * @return success of the operation
 */
bool io_ltc6813_startCellsAdcConversion(void);

/**
 *
 * @return success of the operation
 */
bool io_ltc6813_startThermistorsAdcConversion(void);

/**
 *
 * @return success of the operation
 */
bool io_ltc6813_pollAdcConversions();

// CONVENIENCE FUNCTIONS

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