/**
 * @file io_ltc6813_internal.h
 * @note this file SHOULD NOT BE imported outside of ltc drivers.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "utils.h"
#include "io_ltc6813.h"

#define CMD_SIZE (2U) // 2 bytes
#define PEC_SIZE (2U) // 2 bytes

// Misc. bit definitions.
#define CHG (0x0 << 0)
#define MD_NORMAL (0x2 << 7)   // 7kHz, Needs ADCOPT=0
#define MD_FILTERED (0x3 << 7) // 26Hz, Needs ADCOPT=1
#define CH (0x0 << 0)
#define DCP (0x0 << 4)
#define CHST (0x0 << 0)
#define ST (0x2 << 5)

// Write configuration register commands.
#define WRCFGA (0x0001)
#define WRCFGB (0x0024)

// Read configuration register commands.
#define RDCFGA (0x0002)
#define RDCFGB (0x0026)

// Read cell voltage register commands.
#define RDCVA (0x04U)
#define RDCVB (0x06U)
#define RDCVC (0x08U)
#define RDCVD (0x0AU)
#define RDCVE (0x09U)

// Read auxillary register commands.
#define RDAUXA (0x000CU)
#define RDAUXB (0x000EU)
#define RDAUXC (0x000DU)

// Read status register commands.
#define RDSTATA (0x0010)
#define RDSTATB (0x0012)

// Start ADC conversions.
#define ADCV (0x260 | MD_NORMAL | DCP | CH)        // Cells
#define ADAX (0x460 | MD_NORMAL | CHG)             // Auxillary
#define ADSTAT (0x0468 | MD_NORMAL | CHST)         // Status
#define ADOW_PU (0x0228U | MD_FILTERED | DCP | CH) // Open wire pull up
#define ADOW_PD (0x0268U | MD_FILTERED | DCP | CH) // Open wire pull down

// Start self test commands.
#define CVST (0x0207U | MD_NORMAL | ST)
#define AXST (0x407U | MD_NORMAL | ST)
#define STATST (0x40F | MD_NORMAL | ST)
#define ADOL (0x0201 | MD_NORMAL | DCP)

// Clear registers commands.
#define CLRCELL (0x0711) // Cells
#define CLRAUX (0x0712)  // Auxillary
#define CLRSTAT (0x0713) // Status

// Poll status commands.
#define DIAGN (0x0715)
#define PLADC (0x0714U)

// Mute/unmute discharge.
#define UNMUTE (0x0029U)
#define MUTE (0x0028U)

/**
 * sends the given command
 * @param command command to send
 * @return success of operation
 */
ExitCode io_ltc6813_sendCommand(uint16_t command);

/**
 * Send a polling-type command (see Table 32).
 * @param cmd command to send
 * @param poll_buf poll data buffer
 * @param poll_buf_len number of poll data bytes to receive
 * @return Success of operation
 */
ExitCode io_ltc6813_poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len);

/**
 * Send a read reg group command, and receive the values (see Table 34).
 * A reg group consists of 3x 16-bit registers.
 * @param cmd command to send
 * @param regs Reg group buffer to read into
 * @param comm_success comm_success[seg] gives the exit code for the particular segment
 */
void io_ltc6813_readRegGroup(
    uint16_t cmd,
    uint16_t regs[NUM_SEGMENTS][REGS_PER_GROUP],
    ExitCode comm_success[NUM_SEGMENTS]);

/**
 * Send a write reg group command, and write the values (see Table 33).
 * A reg group consists of 3x 16-bit registers.
 * @param cmd command to send
 * @param regs Reg group data to write
 * @param comm_success comm_success[seg] gives the exit code for the particular segment
 */
ExitCode io_ltc6813_writeRegGroup(uint16_t cmd, uint16_t regs[NUM_SEGMENTS][REGS_PER_GROUP]);
