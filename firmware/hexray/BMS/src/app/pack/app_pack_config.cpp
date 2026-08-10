#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "util_retry.hpp"

namespace {
    constexpr uint8_t NUM_CONFIG_SYNC_TRIES = 3;

    constexpr std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> createSegmentConfig() {
        std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> config{};
        for (auto &[reg_a, reg_b] : config) {
            reg_a.cth       = 0x01;
            reg_a.ref_on    = 0x01;
            reg_a.gpio_1_8  = 0xFF;
            reg_a.gpio_9_10 = 0x03;
            reg_a.fc        = 0x03;

            reg_b.vuv_0_7  = static_cast<uint8_t>(VUV & 0xFF);
            reg_b.vuv_8_11 = static_cast<uint8_t>(VUV >> 8 & 0x0F);
            reg_b.vov_0_3  = static_cast<uint8_t>(VOV & 0x0F);
            reg_b.vov_4_11 = static_cast<uint8_t>(VOV >> 4 & 0xFF);
        }
        return config;
    }

    constexpr std::array<io::adbms::PWMConfig, NUM_SEGMENTS> createPwmConfig() {
        std::array<io::adbms::PWMConfig, NUM_SEGMENTS> config{};
        for (auto &[_reg_a, reg_b] : config)
            reg_b.res = 0xFFFFFFFFu;
        return config;
    }   

    Segments<io::adbms::SegmentConfig> segment_config = createSegmentConfig();
    Segments<io::adbms::PWMConfig> pwm_config = createPWMConfig();
}   

namespace app::pack::config {
    result<void> setThermMuxConfig(const ThermMux mux) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            segment_config[seg].reg_a.gpio_1_8  = 0xFF;
            segment_config[seg].reg_a.gpio_9_10 = 0x2 | (mux == ThermMux::MUX_8_13);
        }

        RETURN_IF_ERR_SILENT(io::adbms::write::configReg(segment_config));
        return {};
    }

    result<void> setBalanceConfig(const bool balancing_muted, const io::adbms::Cells<uint8_t> &duty) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            segment_config[seg].reg_a.mute_st = balancing_muted;

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

        RETURN_IF_ERR_SILENT(io::adbms::write::configReg(segment_config));
        RETURN_IF_ERR_SILENT(io::adbms::write::pwmReg(pwm_config));
        return {};
    }

    bool checkSegmentConfig() {
        const io::adbms::Segments<result<io::adbms::SegmentConfig>> readback = io::adbms::read::configReg();

        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            if (!readback[seg]) {
                LOG_WARN(
                    "Config readback failed on seg %u: %s", static_cast<unsigned>(seg),
                    error_code_to_string(readback[seg].error()));
                return false;
            }
            if (readback[seg].value() != segment_config[seg]) {
                LOG_WARN("Config mismatch on seg %u", static_cast<unsigned>(seg));
                return false;
            }
        }

        return true;
    }

    bool checkPwmConfig() {
        const io::adbms::Segments<result<io::adbms::PWMConfig>> readback = io::adbms::read::pwmReg();

        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            if (!readback[seg]) {
                LOG_WARN(
                    "PWM readback failed on seg %u: %s", static_cast<unsigned>(seg),
                    error_code_to_string(readback[seg].error()));
                return false;
            }
            if (readback[seg].value() != pwm_config[seg]) {
                LOG_WARN("PWM mismatch on seg %u", static_cast<unsigned>(seg));
                return false;
            }
        }

        return true;
    }

    result<void> syncSegmentConfig() {
        return util::retry(
            []() -> result<void> {
                RETURN_IF_ERR_SILENT(io::adbms::write::configReg(segment_config));
                if (not checkSegmentConfig())
                    return std::unexpected(ErrorCode::RETRY_FAILED);
                return {};
            },
            NUM_CONFIG_SYNC_TRIES);
    }

    result<void> syncPwmConfig() {
        return util::retry(
            []() -> result<void> {
                RETURN_IF_ERR_SILENT(io::adbms::write::pwmReg(pwm_config));
                if (not checkPwmConfig())
                    return std::unexpected(ErrorCode::RETRY_FAILED);
                return {};
            },
            NUM_CONFIG_SYNC_TRIES);
    }
}