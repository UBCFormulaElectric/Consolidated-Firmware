#include "app_segments.hpp"
#include "app_segments_internal.hpp"

#include <cstring>

inline constexpr uint8_t NUM_CONFIG_SYNC_TRIES = 20;
inline constexpr uint16_t VUV = 0x800; //VUV × 16 × 150 μV + 1.5 V (TO DO)
inline constexpr uint16_t VOV = 0x7FF; //VOV × 16 × 150 μV + 1.5 V (TO DO)

static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> segment_config;

namespace app::segments {
    void setDefaultConfig() {
        for (auto &[reg_a,reg_b] : segment_config) {
            reg_a.cth       = 0x01;
            reg_a.ref_on    = 0x01;
            reg_a.gpio_1_8  = 0xFF;
            reg_a.gpio_9_10 = 0x03;

            reg_b.vuv_0_7   = static_cast<uint8_t>(VUV & 0xFF);
            reg_b.vuv_8_11  = static_cast<uint8_t>(VUV >> 8 & 0x0F);
            reg_b.vov_0_3   = static_cast<uint8_t>(VOV & 0x0F);
            reg_b.vov_4_11  = static_cast<uint8_t>(VOV >> 4 & 0xFF);
        }
    }

    void setBalanceConfig(std::array<std::array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> balance_config) {
        for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++) {
            auto &[reg_a, reg_b] = segment_config[seg];
            uint16_t dcc_bits = 0U;

            (void)reg_a;
            for (uint8_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++) {
                dcc_bits |= static_cast<uint16_t>((balance_config[seg][cell] ? 1U : 0U) << cell);
            }

            reg_b.dcc_1_8   = static_cast<uint8_t>(dcc_bits & 0xFF);
            reg_b.dcc_9_16  = static_cast<uint8_t>(dcc_bits >> 8 & 0xFF);
        }
    }

    void setThermistorConfig(ThermistorMux mux) {
        for (auto &[reg_a, reg_b] : segment_config) {
            (void)reg_b;

            if (mux == ThermistorMux::THERMISTOR_MUX_0_7) {
                reg_a.gpio_1_8 = 0xFF;
                reg_a.gpio_9_10 = 0x02;
            } else {
                reg_a.gpio_1_8 = 0xFF;
                reg_a.gpio_9_10 = 0x03;
            }
        }
    }

    static std::expected<void, ErrorCode> isConfigEqual() {
        std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> segment_config_buf;
        std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS> segment_success_buf;

        io::adbms::readConfigReg(segment_config_buf, segment_success_buf);

        for (uint8_t seg = 0U; seg < io::NUM_SEGMENTS; seg++) {
            if (!segment_success_buf[seg]) {
                return segment_success_buf[seg];
            } else {
                if (std::memcmp(&segment_config_buf[seg], &segment_config[seg], sizeof(segment_config[seg])) != 0) {
                    return std::unexpected(ErrorCode::CHECKSUM_FAIL);
                }
            }
        }
        return {};
    }

    std::expected<void, ErrorCode> configSync() {
        for (uint8_t tries = 0; tries < NUM_CONFIG_SYNC_TRIES; tries++) {
            const auto write_ok = io::adbms::writeConfigReg(segment_config);
            if (!write_ok) {
                continue;
            } else {
                if (const auto equal = isConfigEqual(); equal.has_value()) {
                    return {};
                }
            }
        }
        return std::unexpected(ErrorCode::RETRY_FAILED);
    }
}
