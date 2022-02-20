#pragma once

#include <stdbool.h>
#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

#define MD (1U)
#define DCP (0U)
#define CH (0U)
#define CHG (0U)
#define CHST (0U)

// Default configurations for CFGRA
#define WRCFGA (0x0001U)
#define VUV (0x4E1U)
#define VOV (0x8CAU)
#define ADCOPT (1U)
#define REFON (0U << 2U)
#define DTEN (0U << 1U)
#define ENABLE_ALL_CFGRA_GPIO (0x001FU << 3U)

// Default configurations for CFGRB
#define WRCFGB (0x0024U)
#define ENABLE_ALL_CFGRB_GPIO (0x000FU)

// Data stored in register group is 2 bytes wide
#define REG_GROUP_DATA_SIZE (2U)
// Number of registers in a single register group
#define NUM_OF_REGS_IN_GROUP (6U)

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)

// Pack byte into word
#define BYTES_TO_WORD(h_byte, l_byte) (uint16_t)(((h_byte) << 8) | (l_byte))

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

typedef enum
{
    CONFIG_REG_A = 0,
    CONFIG_REG_B,
    NUM_OF_CFG_REGISTERS,
} Ltc6813CfgRegs_E;

#define TOTAL_NUM_OF_REG_BYTES \
    (NUM_REG_GROUP_BYTES * NUM_OF_ACCUMULATOR_SEGMENTS)

// Pack the word into bytes (big endian)
static inline void Io_LTC6813_PackWordInBytes(uint8_t *u8, uint16_t u16)
{
    *u8       = (uint8_t)(u16 >> 8);
    *(u8 + 1) = (uint8_t)u16;
}

/**
 * Pack PEC15 command into the tx_data buffer before transmission to the LTC6813
 * chips
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
 *
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
 * Configure register A for all LTC6813 chips on the daisy chain
 * @return True if all chips on the daisy chain are configured successfully.
 * Else, false.
 */
bool Io_LTC6813_ConfigureRegisterA(void);

bool Io_LTC6813_ConfigureRegister(Ltc6813CfgRegs_E cfg_reg);

/**
 * Start ADC conversion for all LTC6813 chips on the daisy chain
 * @return True if all chips on the daisy have started ADC conversions. Else
 * false.
 */
bool Io_LTC6813_StartADCConversion(void);

bool Io_LTC6813_SetConfigurationRegister(
    Ltc6813CfgRegs_E cfg_reg,
    uint8_t          min_cell_segment,
    uint16_t         min_cell_loc);
