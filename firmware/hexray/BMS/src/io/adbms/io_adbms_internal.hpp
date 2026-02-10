#pragma once
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <span>

namespace io::adbms
{

// Command Codes (Table 50)
constexpr uint8_t CMD_BYTES = 2;
constexpr uint8_t PEC_BYTES = 2;

// Config
inline constexpr uint16_t WRCFGA = 0x0001U;
inline constexpr uint16_t WRCFGB = 0x0024U;
inline constexpr uint16_t RDCFGA = 0x0002U;
inline constexpr uint16_t RDCFGB = 0x0026U;

// Cell voltages (C-ADC)
inline constexpr uint16_t RDCVA   = 0x0004U;
inline constexpr uint16_t RDCVB   = 0x0006U;
inline constexpr uint16_t RDCVC   = 0x0008U;
inline constexpr uint16_t RDCVD   = 0x000AU;
inline constexpr uint16_t RDCVE   = 0x0009U;
inline constexpr uint16_t RDCVF   = 0x000BU;
inline constexpr uint16_t RDCVALL = 0x000CU;

// AUX & Status
inline constexpr uint16_t RDAUXA  = 0x0019U;
inline constexpr uint16_t RDAUXB  = 0x001AU;
inline constexpr uint16_t RDAUXC  = 0x001BU;
inline constexpr uint16_t RDAUXD  = 0x001FU;
inline constexpr uint16_t RDSTATA = 0x0018U;
inline constexpr uint16_t RDSTATB = 0x0019U;
inline constexpr uint16_t RDSTATC = 0x001AU;
inline constexpr uint16_t RDSTATD = 0x001BU;
inline constexpr uint16_t RDSTATE = 0x001CU;
inline constexpr uint16_t RDASALL = 0x001DU;

// PWM
inline constexpr uint16_t WRPWMA = 0x0020U;
inline constexpr uint16_t WRPWMB = 0x0021U;
inline constexpr uint16_t RDPWMA = 0x0022U;
inline constexpr uint16_t RDPWMB = 0x0023U;

// Clear
inline constexpr uint16_t CLRCELL = 0x0711U;
inline constexpr uint16_t CLRAUX  = 0x0712U;
inline constexpr uint16_t CLRFLAG = 0x747U;
inline constexpr uint16_t CLOVUV  = 0x547U;

// Poll
inline constexpr uint16_t PLADC  = 0x0718U;
inline constexpr uint16_t PLCADC = 0x071DU;
inline constexpr uint16_t PLAUX  = 0x071EU;
inline constexpr uint16_t PLAUX2 = 0x071FU;

// Discharge
inline constexpr uint16_t MUTE   = 0x0028U;
inline constexpr uint16_t UNMUTE = 0x0029U;

// Start
inline constexpr uint16_t ADCV_BASE  = 0x0260U;
inline constexpr uint16_t ADSV_BASE  = 0x0168U;
inline constexpr uint16_t ADAX_BASE  = 0x0410U;
inline constexpr uint16_t ADAX2_BASE = 0x0400U;

// ADCV and ADSV
inline constexpr uint16_t RD   = (1U << 8); // redundant C + S ADC
inline constexpr uint16_t CONT = (1U << 7); // continuous mode
inline constexpr uint16_t DCP  = (1U << 4); // discharge during conversion
inline constexpr uint16_t RSTF = (1U << 2); // reset IIR filters
inline constexpr uint16_t OW1  = (1U << 1); // open wire
inline constexpr uint16_t OW0  = (1U << 0);

// ADAX and ADAX2
inline constexpr uint16_t OW  = (1U << 8); // open wire
inline constexpr uint16_t PUP = (1U << 7); // pull up or down current sources
inline constexpr uint16_t CH4 = (1U << 6); // channel select
inline constexpr uint16_t CH3 = (1U << 3);
inline constexpr uint16_t CH2 = (1U << 2);
inline constexpr uint16_t CH1 = (1U << 1);
inline constexpr uint16_t CH0 = (1U << 0);

/**
 * Sends the given command.
 * @param cmd The command to send
 * @return success of operation
 */
std::expected<void, ErrorCode> sendCmd(uint16_t cmd);

/**
 * Send a poll command (see Table 45)
 * @param cmd The command to send
 * @param poll_buf Buffer to store the polled data
 */
std::expected<void, ErrorCode> poll(uint16_t cmd, std::span<uint8_t> poll_buf);

/**
 * Send a read reg group command and receive the values (see Table 47).
 * @param cmd The command to send
 * @param regs Buffer to store the received register values
 * @param comm_success Buffer to store the success of communication for each segment
 */
void readRegGroup(
    uint16_t                                                       cmd,
    std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs,
    std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      &comm_success);

/**
 * Send a write reg group command to write the values (see Table 46)
 * @param cmd The command to send
 * @param regs Buffer containing the register values to write
 * @return success of operation
 */
std::expected<void, ErrorCode>
    writeRegGroup(uint16_t cmd, std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs);
} // namespace io::adbms
