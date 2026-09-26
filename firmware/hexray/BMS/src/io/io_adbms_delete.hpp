/**
 * @file io_adbms.hpp
 * @brief Public interface for the Hexray BMS ADBMS driver.
 */
#pragma once

#include "util_errorCodes.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <expected>

inline constexpr uint8_t NUM_SEGMENTS            = 10;
inline constexpr uint8_t CELLS_PER_SEGMENT       = 14;
inline constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;

namespace app::pack {
    struct VoltStats;
    struct TempStats;
    struct OwcStats;
    struct ADBMS6830Diag;
}

namespace io::adbms {

    inline constexpr uint8_t REG_GROUP_SIZE = 6;

    template <typename T> using Segments      = std::array<T, NUM_SEGMENTS>;
    template <typename T> using SegmentCells  = std::array<T, CELLS_PER_SEGMENT>;
    template <typename T> using SegmentTherms = std::array<T, THERMISTORS_PER_SEGMENT>;
    template <typename T> using Cells         = Segments<SegmentCells<T>>;
    template <typename T> using Therms        = Segments<SegmentTherms<T>>;

    enum class OpenWireParity {
        ODD,
        EVEN,
        NONE,
    };

    enum class ThermistorMux {
        ODD,
        EVEN,
    };

    struct __attribute__((packed)) CFGA {
        uint8_t cth : 3;    // C-ADC vs. S-ADC comparison voltage threshold
        uint8_t : 4;        // unused bits
        uint8_t ref_on : 1; // reference powered up, enables REFUP state

        uint8_t flag_d; // asserts flags in Status Reg C for latent fault detection (pg. 70)

        uint8_t : 3;
        uint8_t owa : 3;     // open wire soak times
        uint8_t owrng : 1;   // soak time range
        uint8_t soak_on : 1; // enables soak on AUX ADCs

        uint8_t gpio_1_8; // GPIOx pin control

        uint8_t gpio_9_10 : 2; // GPIOx pin control
        uint8_t : 6;           // unused bits

        uint8_t fc : 3;      // IIR filter parameter, see table 21
        uint8_t comm_bk : 1; // communication break, propagation communication prevention
        uint8_t mute_st : 1; // 1 = mute is active, discharging disabled
        uint8_t snap_st : 1; // 1 = snapshot is activated, result registers are frozen
        uint8_t : 2;
        bool operator==(const CFGA &cfga) const
        {
            return cth == cfga.cth && ref_on == cfga.ref_on && flag_d == cfga.flag_d && owa == cfga.owa &&
                   owrng == cfga.owrng && soak_on == cfga.soak_on && gpio_1_8 == cfga.gpio_1_8 &&
                   gpio_9_10 == cfga.gpio_9_10 && fc == cfga.fc && comm_bk == cfga.comm_bk;
        };
    };
    static_assert(sizeof(CFGA) == REG_GROUP_SIZE);

    struct __attribute__((packed)) CFGB {
        uint8_t vuv_0_7; // cell undervoltage threshold = vuv * 16 * 150uV + 1.5V

        uint8_t vuv_8_11 : 4;
        uint8_t vov_0_3 : 4; // cell overvoltage threshold = vov * 16 * 150uV + 1.5V

        uint8_t vov_4_11;

        uint8_t dcto_0_5 : 6; // discharge timeout value
        uint8_t dtrng : 1;    // discharge timer range setting
        uint8_t dtmen : 1;    // enable discharge timer monitor

        uint8_t dcc_1_8; // discharge cell x

        uint8_t dcc_9_16; // discharge cell x
        bool    operator==(const CFGB &cfgb) const = default;
    };
    static_assert(sizeof(CFGB) == REG_GROUP_SIZE);

    struct SegmentConfig {
        CFGA reg_a;
        CFGB reg_b;
        bool operator==(const SegmentConfig &) const = default;
    };

    struct __attribute__((packed)) PWMA {
        uint8_t pwm1 : 4;
        uint8_t pwm2 : 4;
        uint8_t pwm3 : 4;
        uint8_t pwm4 : 4;
        uint8_t pwm5 : 4;
        uint8_t pwm6 : 4;
        uint8_t pwm7 : 4;
        uint8_t pwm8 : 4;
        uint8_t pwm9 : 4;
        uint8_t pwm10 : 4;
        uint8_t pwm11 : 4;
        uint8_t pwm12 : 4;
        bool    operator==(const PWMA &other) const { return std::memcmp(this, &other, sizeof(PWMA)) == 0; }
    };
    static_assert(sizeof(PWMA) == REG_GROUP_SIZE);

    struct __attribute__((packed)) PWMB {
        uint8_t  pwm13 : 4;
        uint8_t  pwm14 : 4;
        uint8_t  pwm15 : 4;
        uint8_t  pwm16 : 4;
        uint32_t res : 32;
        bool     operator==(const PWMB &other) const { return std::memcmp(this, &other, sizeof(PWMB)) == 0; }
    };
    static_assert(sizeof(PWMB) == REG_GROUP_SIZE);

    struct PWMConfig {
        PWMA reg_a;
        PWMB reg_b;
        bool operator==(const PWMConfig &) const = default;
    };

    namespace write {
        [[nodiscard]] result<void> configReg(const Segments<SegmentConfig> &config);
        [[nodiscard]] result<void> pwmReg(const Segments<PWMConfig> &pwm_config);
    } // namespace write

    namespace read {
        [[nodiscard]] Segments<result<SegmentConfig>> configReg();
        [[nodiscard]] Segments<result<PWMConfig>> pwmReg();
        [[nodiscard]] result<void> Cadc(bool redundant, app::pack::VoltStats &stats);
        [[nodiscard]] result<void> Sadc(OpenWireParity parity, app::pack::OwcStats &stats);
        [[nodiscard]] result<void> Xadc(ThermistorMux mux, app::pack::TempStats &stats);
        [[nodiscard]] result<void> flags(app::pack::ADBMS6830Diag &diag);
    } // namespace read

    namespace command {
        [[nodiscard]] result<void> startCadc(bool redundant);
        [[nodiscard]] result<void> startSadc(bool continuous, OpenWireParity parity);
        [[nodiscard]] result<void> startAuxadc();
        [[nodiscard]] result<void> pollSadc();
        [[nodiscard]] result<void> pollXadc();
        [[nodiscard]] result<void> snap();
        [[nodiscard]] result<void> unsnap();
        [[nodiscard]] result<void> startBalance();
        [[nodiscard]] result<void> stopBalance();
        [[nodiscard]] result<void> clearFlags();
    } // namespace command

} // namespace io::adbms
