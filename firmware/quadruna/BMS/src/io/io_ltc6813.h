/**
 * @file io_ltc6813.h
 * @brief This file contains the interface to the LTC6813 driver
 * @note all implementation is in the ltc6813 directory
 */
#pragma once
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>

// physical constants
#define NUM_SEGMENTS 1
#define CELLS_PER_SEGMENT 14
#define THERMISTORS_PER_SEGMENT 8

// LTC6813 realities
#define VOLTAGE_REGISTER_GROUPS 5
#define AUX_REGISTER_GROUPS 3
#define AUX_REGS_PER_SEGMENT 9

/**
 * @file ltc6813/io_ltc6813_configs.c
 */

/**
 * Reads the configuration registers, and returns them into the pointer you give it
 * @return success of the operation
 */
ExitCode io_ltc6813_readConfigurationRegisters(void);

/**
 * Writes a configuration to all segments on the daisy chain.
 * We have a default configuration. All elements that are not in the default configuration must be
 * present in the config variable passed into the function.
 * @param balance_config The balance configuration to write to the LTCs
 * @return success of the operation
 */
ExitCode io_ltc6813_writeConfigurationRegisters(bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT]);

typedef enum
{
    ADCSpeed_422Hz = 0,
    ADCSpeed_27kHz = 1,
    ADCSpeed_7kHz  = 2,
    ADCSpeed_26Hz  = 3,
    ADCSpeed_1kHz  = 4,
    ADCSpeed_14kHz = 5,
    ADCSpeed_3kHz  = 6,
    ADCSpeed_2kHz  = 7,
} ADCSpeed;

/**
 * @file ltc6813/io_ltc6813_cells.c
 */

/**
 * Gets the value of the cell voltage registers, shares success of operation
 * @param cell_voltage_regs value of the cell voltage registers
 * @param comm_success success of operation on a certain segment and register group
 */
void io_ltc6813_readVoltageRegisters(
    uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool     comm_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS]);

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
 * sends a command to read all voltages from all segments
 * @return success of the operation
 */
ExitCode io_ltc6813_startCellsAdcConversion(ADCSpeed speed);

/**
 * @file ltc6813/io_ltc6813_temps.c
 */

/**
 * Directly reads the auxillary registers
 * @param aux_regs Auxillary register values to populate
 * @param comm_success Success of communications
 */
void io_ltc6813_readAuxRegisters(
    uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
    bool     comm_success[NUM_SEGMENTS][AUX_REGISTER_GROUPS]);

/**
 * Reads all temperatures from all segments
 * @param cell_temps The resultant cell temperatures
 * @param vref The 2ND reference voltage
 * @param success This parameter will be assumed to contain all falses at the beginning
 * @return
 */
void io_ltc6813_readTemperatures(
    float  cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    float *vref,
    bool   success[NUM_SEGMENTS][AUX_REGISTER_GROUPS]);

/**
 * sends a command to read all temperatures from all segments
 * @return success of the operation
 */
ExitCode io_ltc6813_startThermistorsAdcConversion(ADCSpeed speed);

/**
 * @file ltc6813/io_ltc6813_utils.c
 */

/**
 * polls the LTC6813 for the completion of the ADC conversions
 * @return success of the operation
 */
ExitCode io_ltc6813_pollAdcConversions(void);

/**
 * @file ltc6813/io_ltc6813_balance.c
 */

/**
 * Sends a command to enable balancing
 * @return success of operation
 */
ExitCode io_ltc6813_sendBalanceCommand(void);

/**
 * Send a command to disable balancing
 * @return success of operation
 */
ExitCode io_ltc6813_sendStopBalanceCommand(void);

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
bool io_ltc6813CellVoltages_owcPull(PullDirection pull_direction);

/**
 * @file ltc6813/io_ltc6813_tests.c
 */

/**
 * dispatches a command to test the cell adcs
 * @returns if the dispatch is successful
 * @note this does not mean that the test has passed.
 * you still need to assert the values in the reg groups
 */
bool io_ltc6813_sendSelfTestVoltages(ADCSpeed speed);

/**
 * dispatches a command to test the aux adcs
 * @returns if the dispatch is successful
 * @note this does not mean that the test has passed.
 * you still need to assert the values in the reg groups
 */
bool io_ltc6813_sendSelfTestAux(ADCSpeed speed);

/**
 * dispatches a command to test the stat registers
 * @returns if the dispatch is successful
 * @note this does not mean that the test has passed.
 * you still need to assert the values in the reg groups
 */
bool io_ltc6813_sendSelfTestStat(ADCSpeed speed);

/**
 * Sends a command to diagnose the MUX. Populates the MUXFAIL bit in the status register
 * @return success of operation
 */
bool io_ltc6813_diagnoseMUX();

/**
 * Sends a command to test the ADC overlap. In particular
 * it measures cell 7 on ADC1/2 and cell 13 on ADC2/3.
 * @note that the values of the operations are placed in
 * positions 7/8 and 13/14 respectively (1 indexed)
 * @param speed speed of adc
 * @return success of operation
 */
bool io_ltc6813_overlapADCTest(ADCSpeed speed);

/**
 * @file ltc6813/io_ltc6813_status.c
 */

/**
 * Gets the status registers from all the segments
 * @param success success of operations
 */
void io_ltc6813_getStatus(bool success[NUM_SEGMENTS]);
