#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#include "hw_spi.h"
#endif

// clang-format off
// Time that a SPI transaction should wait for until until an error is returned
#define LTC6813_SPI_TIMEOUT_MS (10U)

typedef enum
{
    ACCUMULATOR_SEGMENT_0 = 0U,
    ACCUMULATOR_SEGMENT_1,
    ACCUMULATOR_SEGMENT_2,
    ACCUMULATOR_SEGMENT_3,
    ACCUMULATOR_SEGMENT_4,
    ACCUMULATOR_NUM_SEGMENTS,
} AccumulatorSegment;

#define ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT (16U)
#define ACCUMULATOR_NUM_SERIES_CELLS_TOTAL (ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT * ACCUMULATOR_NUM_SEGMENTS)

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)
#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * V_PER_100UV)

// Indexes for data to write/read from the register group in byte format
typedef enum
{
    REG_GROUP_BYTE_0 = 0U,
    REG_GROUP_BYTE_1,
    REG_GROUP_BYTE_2,
    REG_GROUP_BYTE_3,
    REG_GROUP_BYTE_4,
    REG_GROUP_BYTE_5,
    NUM_REG_GROUP_PAYLOAD_BYTES,
} RegGroupByteFormat;

// Number of PEC15 bytes sent/received from the LTC6813 chip
#define PEC15_SIZE_BYTES (2U)

// Number of bytes sent/read to/from register group: 6 bytes payload + 2 bytes PEC15,
// which can also be represented as 3 words payload + 1 word PEC15
#define TOTAL_NUM_REG_GROUP_BYTES (NUM_REG_GROUP_PAYLOAD_BYTES + PEC15_SIZE_BYTES)
#define TOTAL_NUM_REG_GROUP_WORDS (TOTAL_NUM_REG_GROUP_BYTES >> 1U)

// Reading data back from a register group for all LTC6813's connected in a daisy chain
#define NUM_REG_GROUP_RX_BYTES (TOTAL_NUM_REG_GROUP_BYTES * ACCUMULATOR_NUM_SEGMENTS)
#define NUM_REG_GROUP_RX_WORDS (TOTAL_NUM_REG_GROUP_WORDS * ACCUMULATOR_NUM_SEGMENTS)

// Indexing for data to write/read from the register group in word format
#define REG_GROUP_WORD_PEC_INDEX (3U)

// GPIO Selection for ADC conversion
#define CHG (0U)

// Discharge permitted
#define DCP (0U)

// Cell selection for ADC conversion
#define CH (0U)

// ADC mode selection
#define MD (1U)

// ADOW mode selection
#define PUP_PU (1U) // Pull-up current
#define PUP_PD (0U) // Pull-down current

// clang-format on

// Each command sent includes: 1 cmd word + 1 PEC15 word, which is
// equal to: 2 cmd bytes + 2 PEC15 bytes
typedef enum
{
    CMD_WORD = 0U,
    CMD_PEC15,
    NUM_CMD_WORDS,
} CmdFormat;

#define CMD_SIZE_BYTES (2U)
#define PEC15_SIZE_BYTES (2U)
#define TOTAL_NUM_CMD_BYTES (CMD_SIZE_BYTES + PEC15_SIZE_BYTES)

// Number of readings (cell voltages or temperatures) per each register group
// Each register group consists of 48 bytes, 3x 16 bytes for each reading
typedef enum
{
    REG_GROUP_READING_0 = 0U,
    REG_GROUP_READING_1,
    REG_GROUP_READING_2,
    NUM_OF_READINGS_PER_REG_GROUP
} NumReadingsPerRegGroup;

#ifdef TARGET_EMBEDDED
/**
 * Initialize the SPI handle used to communicate with the LTC6813
 * @param spi The given SPI handle for the LTC6813 daisy chain.
 */
void io_ltc6813Shared_init(const SpiInterface *spi);
#endif

/**
 * Calculate and pack PEC15 bytes for commands sent to the LTC6813
 * @param tx_cmd The buffer containing the command used to calculate and pack
 * PEC15 bytes
 */
void io_ltc6813Shared_packCmdPec15(uint16_t *tx_cmd);

/**
 * Calculate and pack PEC15 bytes for data written to a given register groups
 * @param tx_cfg The buffer containing data used to calculate and pack PEC15
 * bytes
 */
void io_ltc6813Shared_packRegisterGroupPec15(uint8_t *tx_cfg);

/**
 * Calculate the PEC15 value for data to write/read back from a register group
 * on the LTC6813
 * @param data_buffer The buffer containing data to write/read to calculate the
 * PEC15 bytes from
 * @return The PEC15 code generated from the contents of data_buffer
 */
uint16_t io_ltc6813Shared_calculateRegGroupPec15(const uint8_t *data_buffer);

/**
 * Send a command to all LTC6813 chips on the daisy chain
 * @param cmd The command that is transmitted to the LTC6813 chips on
 * the daisy chain
 * @return True if the command was transmitted successfully. Else, false
 */
bool io_ltc6813Shared_sendCommand(uint16_t cmd);

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
