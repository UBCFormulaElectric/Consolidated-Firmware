#include <cmath>

#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "util_retry.hpp"

namespace {

    constexpr float UVOV_LSB_V    = 16.0f * 150e-6f;
    constexpr float UVOV_OFFSET_V = 1.5f;

    constexpr std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> createSegmentConfig() {
        std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> config{};
        for (auto &[reg_a, reg_b] : config) {
            reg_a.cth       = 0x01;
            reg_a.ref_on    = 0x01;
            reg_a.gpio_1_8  = 0xFF;
            reg_a.gpio_9_10 = 0x03;
            reg_a.fc        = 0x03;
        }
        return config;
    }

    constexpr std::array<io::adbms::PWMConfig, NUM_SEGMENTS> createPwmConfig() {
        std::array<io::adbms::PWMConfig, NUM_SEGMENTS> config{};
        for (auto &[_reg_a, reg_b] : config)
            reg_b.res = 0xFFFFFFFFu;
        return config;
    }   

    io::adbms::Segments<io::adbms::SegmentConfig> segment_config = createSegmentConfig();
    io::adbms::Segments<io::adbms::PWMConfig>     pwm_config     = createPwmConfig();
}

namespace app::pack::config {
    result<void> setSegmentConfig(const io::adbms::ThermistorMux mux) {
        //not sure if i should do this (need to constexpr it)
        const auto vuv = static_cast<uint16_t>(std::lround((V_FAULT_UV - UVOV_OFFSET_V) / UVOV_LSB_V));
        const auto vov = static_cast<uint16_t>(std::lround((V_FAULT_OV - UVOV_OFFSET_V) / UVOV_LSB_V));

        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            auto &[reg_a, reg_b] = segment_config[seg];

            reg_a.gpio_1_8  = 0xFF;
            reg_a.gpio_9_10 = 0x2 | (mux == io::adbms::ThermistorMux::ODD);

            reg_b.vuv_0_7  = static_cast<uint8_t>(vuv & 0xFF);
            reg_b.vuv_8_11 = static_cast<uint8_t>(vuv >> 8 & 0x0F);
            reg_b.vov_0_3  = static_cast<uint8_t>(vov & 0x0F);
            reg_b.vov_4_11 = static_cast<uint8_t>(vov >> 4 & 0xFF);
        }

        return io::adbms::write::configReg(segment_config);
    }

    result<void> setPWMConfig(const io::adbms::Cells<uint8_t> &duty) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            const auto &d   = duty[seg];
            pwm_config[seg] = {
                .reg_a = { static_cast<uint8_t>(d[0] & 0x0F), static_cast<uint8_t>(d[1] & 0x0F),
                           static_cast<uint8_t>(d[2] & 0x0F), static_cast<uint8_t>(d[3] & 0x0F),
                           static_cast<uint8_t>(d[4] & 0x0F), static_cast<uint8_t>(d[5] & 0x0F),
                           static_cast<uint8_t>(d[6] & 0x0F), static_cast<uint8_t>(d[7] & 0x0F),
                           static_cast<uint8_t>(d[8] & 0x0F), static_cast<uint8_t>(d[9] & 0x0F),
                           static_cast<uint8_t>(d[10] & 0x0F), static_cast<uint8_t>(d[11] & 0x0F) },
                .reg_b = { static_cast<uint8_t>(d[12] & 0x0F), static_cast<uint8_t>(d[13] & 0x0F), 0, 0,
                           0xFFFFFFFFu },
            };
        }

        return io::adbms::write::pwmReg(pwm_config);
    }

    result<void> checkSegmentConfig() {
        const io::adbms::Segments<result<io::adbms::SegmentConfig>> readback = io::adbms::read::configReg();

        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            if (!readback[seg]) {
                LOG_WARN(
                    "Config readback failed on seg %u: %s", static_cast<unsigned>(seg),
                    error_code_to_string(readback[seg].error()));
                return std::unexpected(readback[seg].error());
            }
            if (readback[seg].value() != segment_config[seg]) {
                LOG_WARN("Config mismatch on seg %u", static_cast<unsigned>(seg));
                return std::unexpected(ErrorCode::MISMATCH);
            }
        }

        return {};
    }

    result<void> checkPWMConfig() {
        const io::adbms::Segments<result<io::adbms::PWMConfig>> readback = io::adbms::read::pwmReg();

        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            if (!readback[seg]) {
                LOG_WARN(
                    "PWM readback failed on seg %u: %s", static_cast<unsigned>(seg),
                    error_code_to_string(readback[seg].error()));
                return std::unexpected(readback[seg].error());
            }
            if (readback[seg].value() != pwm_config[seg]) {
                LOG_WARN("PWM mismatch on seg %u", static_cast<unsigned>(seg));
                return std::unexpected(ErrorCode::MISMATCH);
            }
        }

        return {};
    }
}