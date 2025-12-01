#pragma once
#include <stdint.h>
#include "io_adbms.hpp"
#include "util_errorCodes.h"

namespace io::adbms
{

// ADC conversion commands.
constexpr uint16_t RD    = (0x00 << 8);                       // 1 to enable redundant conversion (C-ADC and S-ADC)
constexpr uint16_t CONT  = (0x00 << 7);                       // 1 to enable continuous conversion mode
constexpr uint16_t DCP   = (0x00 << 4);                       // 1 to enable discharge/balance during conversion
constexpr uint16_t RSTF  = (0x00 << 3);                       // 1 to reset IIR filter
constexpr uint16_t C_ADC = (0x0260 | RD | CONT | DCP | RSTF); // Start cell voltage ADC conversion

// Clear registers commands.
constexpr uint16_t CLRCELL = 0x0711; // Cells

// Poll status commands.
constexpr uint16_t PLADC = 0x0718; // Poll Any ADC Status

// Write configuration register commands.
constexpr uint16_t WRCFGA = 0x0001;
constexpr uint16_t WRCFGB = 0x0024;

// Read configuration register commands.
constexpr uint16_t RDCFGA = 0x0002;
constexpr uint16_t RDCFGB = 0x0026;

// Read cell voltage register commands.
constexpr uint16_t RDCVA = 0x04U;
constexpr uint16_t RDCVB = 0x06U;
constexpr uint16_t RDCVC = 0x08U;
constexpr uint16_t RDCVD = 0x0AU;
constexpr uint16_t RDCVE = 0x09U;

// Read auxillary register commands.
constexpr uint16_t RDAUXA = 0x0019U;
constexpr uint16_t RDAUXB = 0x001AU;
constexpr uint16_t RDAUXC = 0x001BU;
constexpr uint16_t RDAUXD = 0x001FU;

// mute/unmute commands.
constexpr uint16_t MUTE   = 0x28;
constexpr uint16_t UNMUTE = 0x29;

/**
 * Sends the given comand
 * @param command The command to send
 * @return success of operation
 */
ExitCode sendCommand(uint16_t command);

/**
 * Send a poll command (see Table 45)
 * @param cmd The command to send
 * @param poll_buf Buffer to store the polled data
 * @param poll_buf_len Length of the poll buffer
 */
ExitCode poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len);

/**
 * Send a read reg group command and recieve the values (see Table 47)
 * @param cmd The command to send
 * @param regs Buffer to store the recieved register values
 * @param comm_success Buffer to store the success of communication for each segment
 */
void readRegGroup(
    uint16_t cmd,
    uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP],
    ExitCode comm_success[io::NUM_SEGMENTS]);

/**
 * Send a write reg group command to write the values (see Table 46)
 * @param cmd The command to send
 * @param regs Buffer containing the register values to write
 * @param comm_sucess success of operation
 */
ExitCode writeRegGroup(uint16_t cmd, const uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP]);
} // namespace io::adbms