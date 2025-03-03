#pragma once
#include <stdbool.h>
#include <stdint.h>

// Global Variables
#define NUM_SEGMENTS 4
#define CELLS_PER_SEGMENT 16
#define VOLTAGE_REGISTER_GROUPS 6
#define THERMISTORS_PER_SEGMENT 8
#define THERMISTOR_REGISTER_GROUPS 3
#define TOTAL_NUM_OF_THERMISTORS (NUM_SEGMENTS * THERMISTORS_PER_SEGMENT)
// #define TOTAL_PACK_CELLS (ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT * ACCUMULATOR_NUM_SEGMENTS)

typedef struct {
  bool (*balance_config)[NUM_SEGMENTS][CELLS_PER_SEGMENT]; // balancing enabled if non-null, otherwise disabled.
} LTCConfig;
/**
 * Writes a configuration to all segments on the daisy chain.
 * We have a default configuration. All elements that are not in the default configuration must be
 * present in the config variable passed into the function.
 * @param config Configurations we want to write
 * @return success of the operation
 */
bool io_ltc6813_writeConfigurationRegisters(LTCConfig config);

typedef enum
{
  MUTE = 0x2800U,
  UNMUTE = 0x2900U
} LTCCommand;
/**
 * Sends a command to all segments on the daisy chain
 * In particular, it sends a single command, which is not shifted. However, it will affect all segments
 * @param command command to send
 * @return success of the operation
 */
bool io_ltc6813_sendCommand(LTCCommand command);

/**
 * Reads all voltages from all segments
 * @param cell_voltages The resultant cell voltages
 * @param success This parameter will be assumed to contain all falses at the beginning
 * @return
 */
void io_ltc6813_readVoltages(float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT], bool success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS]);

/**
 * Reads all temperatures from all segments
 * @param cell_temps The resultant cell temperatures
 * @param success This parameter will be assumed to contain all falses at the beginning
 * @return
 */
void io_ltc6813_readTemperatures(float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT], bool success[NUM_SEGMENTS][THERMISTOR_REGISTER_GROUPS]);

/**
 *
 * @return success of the operation
 */
bool io_ltc6813_startAdcConversion(void);

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

// JUST TO SHUT UP EDITOR ERRORS
bool io_ltc6813CellVoltages_getGlobalOpenWireFault(void);
bool io_ltc6813CellVoltages_getOpenWireFault(uint8_t segment, uint8_t cell);
void io_ltc6813CellVoltages_owcStart(bool a);
bool io_ltc6813CellVoltages_owcReadVoltages(bool a);
void io_ltc6813CellVoltages_checkOpenWireStatus();