#pragma once

#include <stdbool.h>
#include <stm32f3xx_hal.h>

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)

// Data stored in register group is 2 bytes wide
#define NUM_BYTES_REG_GROUP_DATA (2U)

// Number of registers in a single register group
#define NUM_OF_REGS_IN_GROUP (6U)

enum ConfigurationRegister_E
{
    CONFIG_REG_A = 0,
    CONFIG_REG_B,
    NUM_OF_CFG_REGISTERS,
};

// The calculated PEC15 for the LTC6813 is 2 bytes wide
typedef enum
{
    PEC15_BYTE_0 = 0,
    PEC15_BYTE_1,
    NUM_OF_PEC15_BYTES,
} Ltc6813Pec15Format_E;

// Each command on the LTC6813 is 2 bytes wide
typedef enum
{
    CMD_BYTE_0 = 0U,
    CMD_BYTE_1,
    CMD_BYTE_PEC0,
    CMD_BYTE_PEC1,
    TOTAL_NUM_CMD_BYTES,
} Ltc6813CmdFormat_E;

// LTC6813 registers are 5 bytes wide
typedef enum
{
    REG_GROUP_0 = 0U,
    REG_GROUP_1,
    REG_GROUP_2,
    REG_GROUP_3,
    REG_GROUP_4,
    REG_GROUP_5,
    REG_GROUP_PEC0,
    REG_GROUP_PEC1,
    NUM_REG_GROUP_BYTES,
} Ltc6813RegGroupFormat_E;

#define TOTAL_NUM_OF_REG_BYTES \
    (NUM_REG_GROUP_BYTES * NUM_OF_ACCUMULATOR_SEGMENTS)
#define TOTAL_NUM_OF_REG_WORDS \
    ((NUM_REG_GROUP_BYTES >> 1U) * NUM_OF_ACCUMULATOR_SEGMENTS)

// Pack the word into bytes (big endian)
static inline void Io_LTC6813_PackWordInBytes(uint8_t u8[2], uint16_t u16)
{
    u8[0] = (uint8_t)(u16 >> 8);
    u8[1] = (uint8_t)u16;
}

static inline void Io_LTC6813_ChangeEndianness(uint16_t *val)
{
    *val = (uint16_t)((*val >> 8U) | (*val << 8U));
}

void Io_LTC6813_PrepareCmdNew(uint16_t tx_cmd[2U]);

/**
 * Pack PEC15 command into the tx_data buffer before transmission to the
 * LTC6813 chips
 * @param tx_data The command or register group to pack PEC15 bytes
 * @param size The size of the command or register group to transmit
 */
void Io_LTC6813_PackPec15(uint8_t *tx_data, uint8_t size);

/**
 * Pack command into two bytes
 * @param tx_cmd An array used to store the packed command
 * @param cmd  The command to pack
 */
void Io_LTC6813_PrepareCmd(uint8_t *tx_cmd, uint16_t cmd);

/**
 * Initialize the SPI handle used to communicate with the LTC6813
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 */
void Io_LTC6813_InitSpiHandle(SPI_HandleTypeDef *spi_handle);

/**
 * Calculate the PEC15 value for data to write OR data read back from a register
 * group
 * @param data_buffer
 * @return
 */
uint16_t Io_LTC6813_CalculateRegGroupPec15(uint8_t *data_buffer);

/**
 * Send a command to all LTC6813 chips on the daisy chain
 * @param cmd The command that is transmitted to the LTC6813 chips on
 * the daisy chain
 * @return True if the command was transmitted successfully. Else, false
 */
bool Io_LTC6813_SendCommand(uint16_t cmd);

/**
 * Wait for the completion of all ADC conversions for the LTC6813 chips on the
 * daisy chain.
 * @return True if all ADC conversions have completed successfully. Else, false.
 */
bool Io_LTC6813_PollAdcConversions(void);

/**
 * Set configuration register to default settings
 * @return True if configuration registers are set successfully. Else, false
 */
bool Io_LTC6813_SetCfgRegsToDefaultSettings(void);

/**
 * Write to configuration register
 * @return
 */
bool Io_LTC6813_WriteConfigurationRegisters(void);

/**
 * Enable LTC6813 cell discharge
 * @return True if the command to enable cell discharge is sent successfully.
 * Else, false
 */
bool Io_LTC813_EnableDischarge(void);

/**
 * Disable LTC6813 cell discharge
 * @return True if the command to enable cell discharge is sent successfully.
 * Else, false
 */
bool Io_LTC813_DisableDischarge(void);
