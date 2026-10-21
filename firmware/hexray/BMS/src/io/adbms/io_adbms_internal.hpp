#pragma once

#include "io_adbms.hpp"
#include "hw_spi.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <cstdint>
#include <span>
#include <bit>

namespace io::adbms
{

// Write
inline constexpr uint16_t WRCFGA = 0x0001U;
inline constexpr uint16_t WRCFGB = 0x0024U;
inline constexpr uint16_t WRPWMA = 0x0020U;
inline constexpr uint16_t WRPWMB = 0x0021U;

// Read
inline constexpr uint16_t RDCFGA  = 0x0002U;
inline constexpr uint16_t RDCFGB  = 0x0026U;
inline constexpr uint16_t RDPWMA  = 0x0022U;
inline constexpr uint16_t RDPWMB  = 0x0023U;
inline constexpr uint16_t RDCVA   = 0x0004U;
inline constexpr uint16_t RDCVB   = 0x0006U;
inline constexpr uint16_t RDCVC   = 0x0008U;
inline constexpr uint16_t RDCVD   = 0x000AU;
inline constexpr uint16_t RDCVE   = 0x0009U;
inline constexpr uint16_t RDSVA   = 0x0003U;
inline constexpr uint16_t RDSVB   = 0x0005U;
inline constexpr uint16_t RDSVC   = 0x0007U;
inline constexpr uint16_t RDSVD   = 0x000DU;
inline constexpr uint16_t RDSVE   = 0x000EU;
inline constexpr uint16_t RDAUXA  = 0x0019U;
inline constexpr uint16_t RDAUXB  = 0x001AU;
inline constexpr uint16_t RDAUXC  = 0x001BU;
inline constexpr uint16_t RDAUXD  = 0x001FU;
inline constexpr uint16_t RDSTATA = 0x0030U;
inline constexpr uint16_t RDSTATB = 0x0031U;
inline constexpr uint16_t RDSTATC = 0x0032U;
inline constexpr uint16_t RDSTATD = 0x0033U;

// Clear
inline constexpr uint16_t CLOVUV  = 0x0715U;
inline constexpr uint16_t CLRFLAG = 0x0717U;

// Start
inline constexpr uint16_t ADCV_BASE = 0x0270U;
inline constexpr uint16_t ADSV_BASE = 0x0168U;
inline constexpr uint16_t ADAX_BASE = 0x0410U;

// ADCV and ADSV
inline constexpr uint16_t RD   = 1U << 8; // redundant C + S ADC
inline constexpr uint16_t CONT = 1U << 7; // continuous mode
inline constexpr uint16_t OW1 =
    1U << 1; // open wire on odd channels (C1, C3, ... in datasheet; even cell indices in code)
inline constexpr uint16_t OW0 =
    1U << 0; // open wire on even channels (C2, C4, ... in datasheet; odd cell indices in code)

// Poll
inline constexpr uint16_t PLSADC = 0x071DU;
inline constexpr uint16_t PLAUX  = 0x071EU;

// Snapshot
inline constexpr uint16_t SNAP   = 0x002DU;
inline constexpr uint16_t UNSNAP = 0x002FU;

// Discharge
inline constexpr uint16_t MUTE   = 0x0028U;
inline constexpr uint16_t UNMUTE = 0x0029U;

// Reset command counter
inline constexpr uint16_t RSTCC = 0x002EU;

inline constexpr uint32_t POLL_STATUS_READY = std::byteswap(0xFFFFFFFFU >> (2 * NUM_SEGMENTS));

using RegGroup = std::array<uint8_t, REG_GROUP_SIZE>;

namespace spi
{
    result<void>     sendCmd(const hw::spi::device &port, uint16_t cmd);
    result<uint32_t> poll(const hw::spi::device &port, uint16_t cmd);
    result<void>     readRegGroup(const hw::spi::device &port, uint16_t cmd, std::span<result<RegGroup>> rx);
    result<void>     writeRegGroup(const hw::spi::device &port, uint16_t cmd, std::span<const RegGroup> tx);
} // namespace spi

namespace reversible
{
    result<void> sendCmd(uint16_t cmd);
    result<void> poll(uint16_t cmd);
    result<void> readRegGroup(uint16_t cmd, Segments<result<RegGroup>> &rx);
    result<void> writeRegGroup(uint16_t cmd, const Segments<RegGroup> &tx);
} // namespace reversible

} // namespace io::adbms
