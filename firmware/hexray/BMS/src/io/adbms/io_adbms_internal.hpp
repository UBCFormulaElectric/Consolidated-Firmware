#pragma once
#include <stdint.h>
#include "io_adbms.hpp"
#include "util_errorCodes.h"

namespace io::adbms
{
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
ExitCode io_adbms_sendCommand(uint16_t command);

/**
 * Send a poll command (see Table 45)
 * @param cmd The command to send
 * @param poll_buf Buffer to store the polled data
 * @param poll_buf_len Length of the poll buffer
 */
ExitCode io_adbms_poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len);

/**
 * Send a read reg group command and recieve the values (see Table 47)
 * @param cmd The command to send
 * @param regs Buffer to store the recieved register values
 * @param comm_success Buffer to store the success of communication for each segment
 */
void io_adbms_readRegGroup(
    uint16_t cmd,
    uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP],
    ExitCode comm_success[io::NUM_SEGMENTS]);

/**
 * Send a write reg group command to write the values (see Table 46)
 * @param cmd The command to send
 * @param regs Buffer containing the register values to write
 * @param comm_sucess success of operation
 */
void io_adbms_writeRegGroup(
    uint16_t       cmd,
    const uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP],
    ExitCode       comm_success[io::NUM_SEGMENTS]);
} // namespace io::adbms