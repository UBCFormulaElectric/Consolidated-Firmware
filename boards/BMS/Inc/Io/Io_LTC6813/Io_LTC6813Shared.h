#pragma once

#include <stdbool.h>
#include <stm32f3xx_hal.h>
#include "App_SharedConstants.h"

// clang-format off

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)

// Indexes for data to write/read from the register group in byte format
enum RegGroupByteFormat
{
    REG_GROUP_BYTE_0 = 0U,
    REG_GROUP_BYTE_1,
    REG_GROUP_BYTE_2,
    REG_GROUP_BYTE_3,
    REG_GROUP_BYTE_4,
    REG_GROUP_BYTE_5,
    NUM_REG_GROUP_PAYLOAD_BYTES,
};

// Number of PEC15 bytes sent/received from the LTC6813 chip
#define PEC15_SIZE_BYTES    (2U)

// Number of bytes sent/read to/from register group: 6 bytes payload + 2 bytes PEC15,
// which can also be represented as 3 words payload + 1 word PEC15
#define TOTAL_NUM_REG_GROUP_BYTES (NUM_REG_GROUP_PAYLOAD_BYTES + PEC15_SIZE_BYTES)
#define TOTAL_NUM_REG_GROUP_WORDS (TOTAL_NUM_REG_GROUP_BYTES >> 1U)

// Reading data back from a register group for all LTC6813's connected in a daisy chain
#define NUM_REG_GROUP_RX_BYTES    (TOTAL_NUM_REG_GROUP_BYTES * NUM_OF_ACCUMULATOR_SEGMENTS)
#define NUM_REG_GROUP_RX_WORDS    (TOTAL_NUM_REG_GROUP_WORDS * NUM_OF_ACCUMULATOR_SEGMENTS)

// Indexing for data to write/read from the register group in word format
#define REG_GROUP_WORD_PEC_INDEX (3U)

// GPIO Selection for ADC conversion
#define CHG (0U)

// Discharge permitted
#define DCP (0U)

// Cell selection for ADC conversion
#define CH  (0U)

// ADC mode selection
#define MD  (1U)

// clang-format on

// Each command sent includes: 1 cmd word + 1 PEC15 word, which is
// equal to: 2 cmd bytes + 2 PEC15 bytes
enum CmdFormat
{
    CMD_WORD = 0U,
    CMD_PEC15,
    NUM_OF_CMD_WORDS,
};
#define CMD_SIZE_BYTES (2U)
#define TOTAL_NUM_CMD_BYTES (NUM_OF_CMD_WORDS << 1U)

// Number of readings (cell voltages, temperatures) per each register group
enum NumReadingsPerRegGroup
{
    REG_GROUP_READING_0 = 0U,
    REG_GROUP_READING_1,
    REG_GROUP_READING_2,
    NUM_OF_READINGS_PER_REG_GROUP
};

/**
 * Initialize the SPI handle used to communicate with the LTC6813
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 */
void Io_LTC6813Shared_InitSpiHandle(SPI_HandleTypeDef *spi_handle);

/**
 * Calculate and pack PEC15 bytes for commands sent to the LTC6813
 * @param tx_cmd The buffer containing the command used to calculate and pack
 * PEC15 bytes
 */
void Io_LTC6813Shared_PackCmdPec15(uint16_t *tx_cmd);

/**
 * Calculate and pack PEC15 bytes for data written to a given register groups
 * @param tx_cfg The buffer containing data used to calculate and pack PEC15
 * bytes
 */
void Io_LTC6813Shared_PackRegisterGroupPec15(uint8_t *tx_cfg);

/**
 * Calculate the PEC15 value for data to write/read back from a register group
 * on the LTC6813
 * @param data_buffer The buffer containing data to write/read to calculate the
 * PEC15 bytes from
 * @return The PEC15 code generated from the contents of data_buffer
 */
uint16_t Io_LTC6813Shared_CalculateRegGroupPec15(const uint8_t *data_buffer);

/**
 * Send a command to all LTC6813 chips on the daisy chain
 * @param cmd The command that is transmitted to the LTC6813 chips on
 * the daisy chain
 * @return True if the command was transmitted successfully. Else, false
 */
bool Io_LTC6813Shared_SendCommand(uint16_t cmd);

/**
 * Wait for the completion of all ADC conversions for the LTC6813 chips on the
 * daisy chain.
 * @return True if all ADC conversions have completed successfully. Else, false.
 */
bool Io_LTC6813Shared_PollAdcConversions(void);

/**
 * Set configuration register to default settings. Refer to ltc6813_configs in
 * Io_LTC6813.c for exact register configurations used. Users can also consult
 * Table 56 (Memory Bit Descriptions) of the LTC6813 datasheet for more
 * information.
 * @return True if configuration registers are set successfully. Else, false
 */
bool Io_LTC6813Shared_SetCfgRegsToDefaultSettings(void);

/**
 * Write to configuration registers, namely CFGRA and CFGRB
 * @return True if the commands used to write to the configuration registers can
 * be sent successfully. Else, false
 */
bool Io_LTC6813Shared_WriteConfigurationRegisters(void);

/**
 * Enable LTC6813 cell discharge
 * @return True if the command to enable cell discharge is sent successfully.
 * Else, false
 */
bool Io_LTC6813Shared_EnableDischarge(void);

/**
 * Disable LTC6813 cell discharge
 * @return True if the command to enable cell discharge is sent successfully.
 * Else, false
 */
bool Io_LTC6813Shared_DisableDischarge(void);
