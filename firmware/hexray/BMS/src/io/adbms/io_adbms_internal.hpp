#pragma once

#include "io_adbms.hpp"
#include "hw_spi.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <cstdint>
#include <span>
#include <bit>

namespace io::adbms {

    inline constexpr uint16_t WRCFGA = 0x0001U;
    inline constexpr uint16_t WRCFGB = 0x0024U;
    inline constexpr uint16_t WRPWMA = 0x0020U;
    inline constexpr uint16_t WRPWMB = 0x0021U;

    // Read
    inline constexpr uint16_t RDCVA   = 0x0004U;
    inline constexpr uint16_t RDCVB   = 0x0006U;
    inline constexpr uint16_t RDCVC   = 0x0008U;
    inline constexpr uint16_t RDCVD   = 0x000AU;
    inline constexpr uint16_t RDCVE   = 0x0009U;
    inline constexpr uint16_t RDSTATC = 0x0032U;

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
    inline constexpr uint16_t OW1  = 1U << 1; // open wire odd channels
    inline constexpr uint16_t OW0  = 1U << 0; // open wire even channels

    // Poll
    inline constexpr uint16_t PLSADC = 0x071DU;
    inline constexpr uint16_t PLAUX  = 0x071EU;

    // Snapshot
    inline constexpr uint16_t SNAP   = 0x002DU;
    inline constexpr uint16_t UNSNAP = 0x002FU;

    // Discharge
    inline constexpr uint16_t MUTE   = 0x0028U;
    inline constexpr uint16_t UNMUTE = 0x0029U;

    inline constexpr uint32_t POLL_STATUS_READY = std::byteswap(0xFFFFFFFFU >> (2 * NUM_SEGMENTS));

    using RegGroup = std::array<uint8_t, REG_GROUP_SIZE>;

    namespace spi {
        result<void>     sendCmd(const hw::spi::device &port, uint16_t cmd);
        result<uint32_t> poll(const hw::spi::device &port, uint16_t cmd);
        result<void>     readRegGroup(const hw::spi::device &port, uint16_t cmd, std::span<RegGroup> rx);
        result<void>     writeRegGroup(const hw::spi::device &port, uint16_t cmd, std::span<const RegGroup> tx);
    }

    namespace reversible {
        result<void> sendCmd(uint16_t cmd);
        result<void> poll(uint16_t cmd);
        result<void> readRegGroup(uint16_t cmd, Segments<RegGroup> &rx);
        result<void> writeRegGroup(uint16_t cmd, const Segments<RegGroup> &tx);
    }

}

