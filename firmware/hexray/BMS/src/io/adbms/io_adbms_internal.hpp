#pragma once
#include <cstdint>
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{

// Command Codes (Table 50)
constexpr uint8_t CMD_BYTES = 2;
constexpr uint8_t PEC_BYTES = 2;

// Config
constexpr uint16_t WRCFGA = 0x0001U;
constexpr uint16_t WRCFGB = 0x0024U;
constexpr uint16_t RDCFGA = 0x0002U;
constexpr uint16_t RDCFGB = 0x0026U;

// Cell voltages (C-ADC)
constexpr uint16_t RDCVA   = 0x0004U;
constexpr uint16_t RDCVB   = 0x0006U;
constexpr uint16_t RDCVC   = 0x0008U;
constexpr uint16_t RDCVD   = 0x000AU;
constexpr uint16_t RDCVE   = 0x0009U;
constexpr uint16_t RDCVF   = 0x000BU;
constexpr uint16_t RDCVALL = 0x000CU;

// AUX & Status
constexpr uint16_t RDAUXA  = 0x0019U;
constexpr uint16_t RDAUXB  = 0x001AU;
constexpr uint16_t RDAUXC  = 0x001BU;
constexpr uint16_t RDAUXD  = 0x001FU;
constexpr uint16_t RDSTATA = 0x0018U;
constexpr uint16_t RDSTATB = 0x0019U;
constexpr uint16_t RDSTATC = 0x001AU;
constexpr uint16_t RDSTATD = 0x001BU;
constexpr uint16_t RDSTATE = 0x001CU;
constexpr uint16_t RDASALL = 0x001DU;

// PWM
constexpr uint16_t WRPWMA = 0x0020U;
constexpr uint16_t WRPWMB = 0x0021U;
constexpr uint16_t RDPWMA = 0x0022U;
constexpr uint16_t RDPWMB = 0x0023U;

// Clear
constexpr uint16_t CLRCELL = 0x0711U;
constexpr uint16_t CLRAUX  = 0x0712U;
constexpr uint16_t CLRFLAG = 0x747U;
constexpr uint16_t CLOVUV  = 0x547U;

// Poll
constexpr uint16_t PLADC  = 0x0718U;
constexpr uint16_t PLCADC = 0x071DU;
constexpr uint16_t PLAUX  = 0x071EU;
constexpr uint16_t PLAUX2 = 0x071FU;

// Discharge
constexpr uint16_t MUTE   = 0x0028U;
constexpr uint16_t UNMUTE = 0x0029U;

// Start
constexpr uint16_t ADCV_BASE  = 0x0260U;
constexpr uint16_t ADSV_BASE  = 0x0168U;
constexpr uint16_t ADAX_BASE  = 0x0410U;
constexpr uint16_t ADAX2_BASE = 0x0400U;

// ADCV and ADSV
constexpr uint16_t RD   = (1U << 8); // redundant C + S ADC
constexpr uint16_t CONT = (1U << 7); // continuous mode
constexpr uint16_t DCP  = (1U << 4); // discharge during conversion
constexpr uint16_t RSTF = (1U << 2); // reset IIR filters
constexpr uint16_t OW1  = (1U << 1); // open wire
constexpr uint16_t OW0  = (1U << 0);

// ADAX and ADAX2
constexpr uint16_t OW  = (1U << 8); // open wire
constexpr uint16_t PUP = (1U << 7); // pull up or down current sources
constexpr uint16_t CH4 = (1U << 6); // channel select
constexpr uint16_t CH3 = (1U << 3);
constexpr uint16_t CH2 = (1U << 2);
constexpr uint16_t CH1 = (1U << 1);
constexpr uint16_t CH0 = (1U << 0);

/**
 * Sends the given command.
 * @param command The command to send
 * @return success of operation
 */
ExitCode sendCmd(uint16_t command);

/**
 * Send a poll command (see Table 45)
 * @param cmd The command to send
 * @param poll_buf Buffer to store the polled data
 * @param poll_buf_len Length of the poll buffer
 */
ExitCode poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len);

/**
 * Send a read reg group command and receive the values (see Table 47).
 * @param cmd The command to send
 * @param regs Buffer to store the received register values
 * @param comm_success Buffer to store the success of communication for each segment
 */
void readRegGroup(
    uint16_t cmd,
    uint8_t  regs[io::NUM_SEGMENTS][io::adbms::REG_GROUP_SIZE],
    ExitCode comm_success[io::NUM_SEGMENTS]);

/**
 * Send a write reg group command to write the values (see Table 46)
 * @param cmd The command to send
 * @param regs Buffer containing the register values to write
 * @return success of operation
 */
ExitCode writeRegGroup(uint16_t cmd, const uint8_t regs[io::NUM_SEGMENTS][io::adbms::REG_GROUP_SIZE]);
} // namespace io::adbms
