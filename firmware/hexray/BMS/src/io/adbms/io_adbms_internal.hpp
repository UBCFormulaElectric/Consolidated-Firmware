#pragma once

#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <span>

using namespace std;

namespace io::adbms
{
// Shared scratch buffers used by low-level register transactions.
extern array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> shared_reg_group;
extern array<expected<void, ErrorCode>, NUM_SEGMENTS>      shared_reg_group_success;

// Transaction framing constants.
inline constexpr uint8_t CMD_BYTES = 2;
inline constexpr uint8_t PEC_BYTES = 2;

// Config
inline constexpr uint16_t WRCFGA = 0x0001U;
inline constexpr uint16_t WRCFGB = 0x0024U;
inline constexpr uint16_t RDCFGA = 0x0002U;
inline constexpr uint16_t RDCFGB = 0x0026U;

// Cell voltages (C-ADC)
inline constexpr uint16_t RDCVA = 0x0004U;
inline constexpr uint16_t RDCVB = 0x0006U;
inline constexpr uint16_t RDCVC = 0x0008U;
inline constexpr uint16_t RDCVD = 0x000AU;
inline constexpr uint16_t RDCVE = 0x0009U;
inline constexpr uint16_t RDCVF = 0x000BU;

// Redundant cell voltages (S-ADC)
inline constexpr uint16_t RDSVA = 0x0003U;
inline constexpr uint16_t RDSVB = 0x0005U;
inline constexpr uint16_t RDSVC = 0x0007U;
inline constexpr uint16_t RDSVD = 0x000DU;
inline constexpr uint16_t RDSVE = 0x000EU;
inline constexpr uint16_t RDSVF = 0x000FU;

// Filtered cell voltages
inline constexpr uint16_t RDFCA = 0x0012U;
inline constexpr uint16_t RDFCB = 0x0013U;
inline constexpr uint16_t RDFCC = 0x0014U;
inline constexpr uint16_t RDFCD = 0x0015U;
inline constexpr uint16_t RDFCE = 0x0016U;
inline constexpr uint16_t RDFCF = 0x0017U;

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
inline constexpr uint16_t CLRFC   = 0x714U;

// Poll
inline constexpr uint16_t PLADC  = 0x0718U;
inline constexpr uint16_t PLCADC = 0x071CU;
inline constexpr uint16_t PLSADC = 0x071DU;
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

// Snapshot
inline constexpr uint16_t SNAP   = 0x002DU;
inline constexpr uint16_t UNSNAP = 0x002FU;

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

// Poll commands return a packed readiness bitmap.
inline constexpr uint32_t POLL_STATUS_READY = __builtin_bswap32(0xFFFFFFFFU >> (2 * NUM_SEGMENTS));

// Raw command helpers.
expected<void, ErrorCode> sendCmd(uint16_t cmd);
expected<void, ErrorCode> poll(uint16_t cmd, span<uint8_t> poll_buf);
void                      readRegGroup(
                         uint16_t                                             cmd,
                         array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs,
                         array<expected<void, ErrorCode>, NUM_SEGMENTS>      &comm_success);
expected<void, ErrorCode> writeRegGroup(uint16_t cmd, const array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs);
expected<void, ErrorCode> pollTempAdcConversion();
} // namespace io::adbms
