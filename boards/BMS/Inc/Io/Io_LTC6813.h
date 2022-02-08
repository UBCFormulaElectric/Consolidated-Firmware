#pragma once

#include <stdbool.h>
#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

#define MD (1U)
#define DCP (0U)
#define CH (0U)
#define CHG (0U)
#define CHST (0U)

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)

// The calculated PEC15 for the LTC6813 is 2 bytes wide
enum Ltc6813Pec15Format
{
    LTC6813_PEC15_BYTE_0 = 0,
    LTC6813_PEC15_BYTE_1,
    NUM_OF_PEC15_BYTES,
};

// Each command on the LTC6813 is 2 bytes wide
enum Ltc6813CmdFormat
{
    LTC6813_CMD_BYTE_0 = 0U,
    LTC6813_CMD_BYTE_1,
    NUM_OF_CMD_BYTES,
};

// LTC6813 registers are 5 bytes wide
enum Ltc6813RegGroupFormat
{
    LTC6813_REG_0 = 0U,
    LTC6813_REG_1,
    LTC6813_REG_2,
    LTC6813_REG_3,
    LTC6813_REG_4,
    LTC6813_REG_5,
    NUM_OF_REGS_IN_GROUP,
};

typedef enum
{
    CONFIG_REGISTER_A = 0,
    CONFIG_REGISTER_B,
    NUM_OF_CONFIG_REGISTERS,
} ConfigurationRegisters_E;

#define NUM_TX_CMD_BYTES (NUM_OF_PEC15_BYTES + NUM_OF_CMD_BYTES)
#define NUM_REG_GROUP_BYTES (NUM_OF_PEC15_BYTES + NUM_OF_REGS_IN_GROUP)
#define TOTAL_NUM_OF_REG_BYTES \
    (NUM_REG_GROUP_BYTES * NUM_OF_ACCUMULATOR_SEGMENTS)

// Data stored in register group is 2 bytes wide
#define REG_GROUP_DATA_SIZE (2U)

/**
 * Pack command into two bytes
 * @param dest An array used to store the packed command
 * @param cmd  The command to pack
 */
static inline void
    Io_LTC6813_PackCmd(uint8_t dest[NUM_TX_CMD_BYTES], uint16_t cmd)
{
    dest[LTC6813_CMD_BYTE_0] = (uint8_t)(cmd >> 8);
    dest[LTC6813_CMD_BYTE_1] = (uint8_t)cmd;
}

/**
 * Initialize the SPI handle used to communicate with the LTC6813
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 */
void Io_LTC6813_InitSpiHandle(SPI_HandleTypeDef *spi_handle);

/**
 * Pack PEC15 command into the tx_data buffer before transmission to the LTC6813
 * chips
 * @param tx_data The command or register group to pack PEC15 bytes
 * @param size The size of the command or register group to transmit
 */
void Io_LTC6813_PackPec15(uint8_t *tx_data, uint8_t size);

/**
 * Calculate the 15-bit packet error code (PEC15) for the given data buffer.
 * @param data_buffer A pointer to the buffer containing data used to calculate
 * the PEC15 code.
 * @param size The number of data elements used to calculate the PEC15 code
 * @note The size can be a positive integer less than or equal to the size
 * of the data buffer.
 * @return The calculated PEC15 code for the given data buffer.
 */
uint16_t Io_LTC6813_CalculatePec15(uint8_t *data_buffer, uint8_t size);

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
 * Configure register A for all LTC6813 chips on the daisy chain
 * @return True if all chips on the daisy chain are configured successfully.
 * Else, false.
 */
bool Io_LTC6813_ConfigureRegisterA(void);

bool Io_LTC6813_ConfigureRegisterB(void);

bool Io_LTC6813_ConfigureRegister(ConfigurationRegisters_E cfg_reg);

/**
 * Start ADC conversion for all LTC6813 chips on the daisy chain
 * @return True if all chips on the daisy have started ADC conversions. Else
 * false.
 */
bool Io_LTC6813_StartADCConversion(void);

void Io_LTC6813_EnterReadyState(void);
