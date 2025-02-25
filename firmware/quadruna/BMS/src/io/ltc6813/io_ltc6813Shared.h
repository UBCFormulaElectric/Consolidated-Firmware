#pragma once

#include <stdbool.h>
#include <stdint.h>

// Time that a SPI transaction should wait for until until an error is returned
#define LTC6813_SPI_TIMEOUT_MS (10U)

#define ACCUMULATOR_NUM_SEGMENTS 4
#define ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT (16U)
#define ACCUMULATOR_NUM_SERIES_CELLS_TOTAL (ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT * ACCUMULATOR_NUM_SEGMENTS)

// Number of readings (cell voltages or temperatures) per each register group
// Each register group consists of 48 bits, 3x 16 bits for each reading
#define NUM_READINGS_PER_REG_GROUP 3

// Each command sent includes: 1 cmd word + 1 PEC15 word, which is
// equal to: 2 cmd bytes + 2 PEC15 bytes
typedef struct
{
    uint16_t cmd;
    uint16_t pec15;
} LtcCmdPayload;

typedef struct
{
    union
    {
        uint16_t words[NUM_READINGS_PER_REG_GROUP];
        uint8_t  bytes[NUM_READINGS_PER_REG_GROUP * 2];
    };
    uint16_t pec15;
} LtcRegGroupPayload;

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)
#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * V_PER_100UV)

/**
 * Calculate the PEC15 value for data to write/read back from a register group
 * on the LTC6813
 * @param data_buffer The buffer containing data to write/read to calculate the
 * PEC15 bytes from
 * @return The PEC15 code generated from the contents of data_buffer
 */
bool io_ltc6813Shared_checkRegGroupPec15(const LtcRegGroupPayload *data_buffer);

/**
 * Send a command to all LTC6813 chips on the daisy chain
 * @param cmd The command that is transmitted to the LTC6813 chips on
 * the daisy chain
 * @return True if the command was transmitted successfully. Else, false
 */
bool io_ltc6813Shared_sendCommand(uint16_t cmd);

bool io_ltc6813Shared_readRegGroup(uint16_t cmd, LtcRegGroupPayload *rx_buffer);

/**
 * Wait for the completion of all ADC conversions for the LTC6813 chips on the
 * daisy chain.
 * @return True if all ADC conversions have completed successfully. Else, false.
 */
bool io_ltc6813Shared_pollAdcConversions(void);

/**
 * Set configuration register to default settings. Refer to ltc6813_configs in
 * Io_LTC6813.c for exact register configurations used. Users can also consult
 * Table 56 (Memory Bit Descriptions) of the LTC6813 datasheet for more
 * information.
 * @return True if configuration registers are set successfully. Else, false
 */
bool io_ltc6813Shared_setCfgRegsToDefaultSettings(void);

/**
 * Write to configuration registers, namely CFGRA and CFGRB to set which cells to balance.
 * @param enable_balance True to enable balancing, False to disable all balancing
 * @return True if the commands used to write to the configuration registers can
 * be sent successfully. Else, false
 */
bool io_ltc6813Shared_writeConfigurationRegisters(bool enable_balance);

/**
 * Enable LTC6813 cell balancing (discharge)
 * @return True if the command to enable cell balancing is sent successfully.
 * Else, false
 */
bool io_ltc6813Shared_enableBalance(void);

/**
 * Disable LTC6813 cell balancing (discharge)
 * @return True if the command to enable cell balancing is sent successfully.
 * Else, false
 */
bool io_ltc6813Shared_disableBalance(void);

/**
 * Mark cell for cell balancing discharge status
 */
void io_ltc6813Shared_markCellForDischarge(uint8_t segment, uint8_t cell, bool needs_discharging);
