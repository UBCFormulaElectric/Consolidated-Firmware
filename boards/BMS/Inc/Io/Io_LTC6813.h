#pragma once

#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

#define MD (1U)
#define DCP (0U)
#define CH (0U)
#define CHG (0U)
#define CHST (0U)

// TODO: Determine the ADC conversion timeout threshold #674
// Driver will send a command to check if ADC conversions are finished on the
// LTC6813 up to 10 times
#define MAX_NUM_OF_CONV_COMPLETE_CHECKS (10U)

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

#define NUM_TX_CMD_BYTES (NUM_OF_PEC15_BYTES + NUM_OF_CMD_BYTES)
#define NUM_REG_BYTES (NUM_OF_PEC15_BYTES + NUM_OF_REGS_IN_GROUP)

static inline void
    Io_LTC6813_PackCmd(uint8_t dest[NUM_TX_CMD_BYTES], uint16_t cmd)
{
    dest[LTC6813_CMD_BYTE_0] = (uint8_t)(cmd >> 8);
    dest[LTC6813_CMD_BYTE_1] = (uint8_t)cmd;
}

/**
 * Initialize all chips on the LTC6813 daisy chain.
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 * @param nss_port The given GPIO port of the NSS pin.
 * @param nss_pin The given GPIO NSS pin.
 */
void Io_LTC6813_Init(SPI_HandleTypeDef *spi_handle);

void Io_LTC6813_PackPec15(uint8_t *cfg_reg, uint8_t size);

/**
 * Calculate the 15-bit packet error code (PEC15) for the given data buffer.
 * @param data_buffer A pointer to the buffer containing data used to calculate
 * the PEC15 code.
 * @param size The number of data elements used to calculate the PEC15 code
 * @note The size can be a positive integer less than or equal to the size
 * of the data buffer.
 * @return The calculated PEC15 code for the given data buffer.
 */
uint16_t Io_LTC6813_CalculatePec15(uint8_t *data_buffer, uint32_t size);

/**
 * Send a command to all LTC6813 chips on the daisy chain
 * @param tx_cmd The given command that is transmitted to the LTC6813 chips on
 * the daisy chain
 * @return EXIT_CODE_OK if the command was transmitted successfully. Else,
 * EXIT_CODE_ERROR
 */
bool Io_LTC6813_SendCommand(uint16_t cmd);

/**
 * Wait for the completion of all ADC conversions for the LTC6813 chips on the
 * daisy chain.
 * @return EXIT_CODE_OK if all ADC conversions have completed successfully.
 * Else, EXIT_CODE_TIMEOUT.
 */
bool Io_LTC6813_PollAdcConversions(void);

/**
 * Configure register A for all LTC6813 chips on the LTC6813 daisy chain with
 * the following properties: (1) [1.8, 2.2] discharge time, (2)
 * @return EXIT_CODE_OK if all chips on the daisy chain are configured
 * successfully. Else, EXIT_CODE_ERROR.
 */
bool Io_LTC6813_ConfigureRegisterA(void);

// Used to wake up the LTC6813, whcih is required by app code
bool Io_LTC6813_StartADCConversion(void);
