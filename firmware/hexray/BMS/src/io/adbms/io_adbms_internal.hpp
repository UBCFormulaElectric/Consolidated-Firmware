#pragma once

#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include <array>
#include <bitset>

using namespace std;

namespace io::adbms
{

// Transaction framing constants.
inline constexpr uint8_t CMD_BYTES = 2;
inline constexpr uint8_t PEC_BYTES = 2;

// Serial Number
inline constexpr uint16_t RDSID = 0x002CU;

// Config
inline constexpr uint16_t WRCFGA = 0x0001U;
inline constexpr uint16_t WRCFGB = 0x0024U;
inline constexpr uint16_t RDCFGA = 0x0002U;
inline constexpr uint16_t RDCFGB = 0x0026U;

// Cell voltages (C-ADC) — A..E only; 14 cells → Group F unused.
inline constexpr uint16_t RDCVA = 0x0004U;
inline constexpr uint16_t RDCVB = 0x0006U;
inline constexpr uint16_t RDCVC = 0x0008U;
inline constexpr uint16_t RDCVD = 0x000AU;
inline constexpr uint16_t RDCVE = 0x0009U;

// Redundant cell voltages (S-ADC) — A..E only.
inline constexpr uint16_t RDSVA = 0x0003U;
inline constexpr uint16_t RDSVB = 0x0005U;
inline constexpr uint16_t RDSVC = 0x0007U;
inline constexpr uint16_t RDSVD = 0x000DU;
inline constexpr uint16_t RDSVE = 0x000EU;

// Filtered cell voltages — A..E only.
inline constexpr uint16_t RDFCA = 0x0012U;
inline constexpr uint16_t RDFCB = 0x0013U;
inline constexpr uint16_t RDFCC = 0x0014U;
inline constexpr uint16_t RDFCD = 0x0015U;
inline constexpr uint16_t RDFCE = 0x0016U;

// AUX & Status
inline constexpr uint16_t RDAUXA  = 0x0019U;
inline constexpr uint16_t RDAUXB  = 0x001AU;
inline constexpr uint16_t RDAUXC  = 0x001BU;
inline constexpr uint16_t RDAUXD  = 0x001FU;
inline constexpr uint16_t RDSTATA = 0x0030U;
inline constexpr uint16_t RDSTATB = 0x0031U;
inline constexpr uint16_t RDSTATC = 0x0032U;
inline constexpr uint16_t RDSTATD = 0x0033U;
inline constexpr uint16_t RDSTATE = 0x0034U;
inline constexpr uint16_t RDASALL = 0x0035U;

// PWM
inline constexpr uint16_t WRPWMA = 0x0020U;
inline constexpr uint16_t WRPWMB = 0x0021U;
inline constexpr uint16_t RDPWMA = 0x0022U;
inline constexpr uint16_t RDPWMB = 0x0023U;

// Clear
inline constexpr uint16_t CLRCELL = 0x0711U;
inline constexpr uint16_t CLRAUX  = 0x0712U;
inline constexpr uint16_t CLRFLAG = 0x0717U;
inline constexpr uint16_t CLRSPIN = 0x0716U;
inline constexpr uint16_t CLOVUV  = 0x0715U;
inline constexpr uint16_t CLRFC   = 0x0714U;

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
inline constexpr uint16_t ADCV_BASE  = 0x0270U;
inline constexpr uint16_t ADSV_BASE  = 0x0168U;
inline constexpr uint16_t ADAX_BASE  = 0x0410U;
inline constexpr uint16_t ADAX2_BASE = 0x0400U;

// Snapshot
inline constexpr uint16_t SNAP   = 0x002DU;
inline constexpr uint16_t UNSNAP = 0x002FU;

// ADCV and ADSV
inline constexpr uint16_t RD   = 1U << 8; // redundant C + S ADC
inline constexpr uint16_t CONT = 1U << 7; // continuous mode
inline constexpr uint16_t DCP  = 1U << 4; // discharge during conversion
inline constexpr uint16_t RSTF = 1U << 2; // reset IIR filters
inline constexpr uint16_t OW1  = 1U << 1; // open wire
inline constexpr uint16_t OW0  = 1U << 0;

// ADAX and ADAX2
inline constexpr uint16_t OW  = 1U << 8; // open wire
inline constexpr uint16_t PUP = 1U << 7; // pull up or down current sources
inline constexpr uint16_t CH4 = 1U << 6; // channel select
inline constexpr uint16_t CH3 = 1U << 3;
inline constexpr uint16_t CH2 = 1U << 2;
inline constexpr uint16_t CH1 = 1U << 1;
inline constexpr uint16_t CH0 = 1U << 0;

// Poll commands return a packed readiness bitmap
inline constexpr uint32_t POLL_STATUS_READY =
    __builtin_bswap32(0xFFFFFFFFU >> (2 * NUM_SEGMENTS)); // TEST THIS SMTH SKEACHY GOING ON
inline constexpr uint8_t POLL_RETRIES        = 5U;        // TEST THIS SMTH SKEACHY GOING ON
inline constexpr uint8_t POLL_RETRY_DELAY_MS = 3U;        // TEST THIS SMTH SKEACHY GOING ON

inline constexpr uint16_t DEFAULT_REGISTER_VALUE = 0x8000;

// Raw command helpers
/**
 * @param cmd Command to send
 * @return Success if the command was acknowledged by the chip, or an error code if the SPI transaction failed
 */
[[nodiscard]] result<void> sendCmd(uint16_t cmd);
/**
 * @param cmd Poll command to send (e.g. PLAUX)
 * @return A bitmap indicating which segments are ready, where bit 0 corresponds to segment A, bit 1 to segment B, etc.;
 * @note Generally, you will check against POLL_STATUS_READY to see if all segments are ready, but you can also check
 * individual bits to see which segments are ready
 */
[[nodiscard]] result<std::bitset<32>> poll(uint16_t cmd);
/**
 * Reads a register group
 * If the SPI transaction fails, each segment's result will contain the error code
 * @throws ErrorCode::CHECKSUM_FAIL if the PEC check fails for a segment; that segment's result will contain the error
 * code
 * @throws ErrorCode::CMD_COUNT_MISMATCH if the command count byte doesn't match the expected value for a segment; that
 * segment's result will contain the error code and the expected count will be resynced to the received count
 * @param cmd the command to read the register group (e.g. RDCVA)
 * @return a Segments of results, one per segment, containing either the register values or an error code if the read
 * failed
 */
[[nodiscard]] Segments<result<RegBuffer>> readRegGroup(uint16_t cmd);
/**
 * Writes a register group
 * @param cmd the command to write the register group (e.g. WRCFGA)
 * @param regs the register values to write, organized by segment; the caller is responsible for ensuring the correct
 * values are
 * @return Success if the write was acknowledged by the chip, or an error code if the SPI transaction failed
 */
[[nodiscard]] result<void> writeRegGroup(uint16_t cmd, array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs);

// Returns a per-segment bitmap of command counter mismatches observed during
// the last readRegGroup. Cleared/overwritten on each readRegGroup call.
// Segments<bool> getCmdCountMismatches();
} // namespace io::adbms
