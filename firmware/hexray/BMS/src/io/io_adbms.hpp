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

// Physical daisy-chain dimensions for this ECU.
inline constexpr uint8_t NUM_SEGMENTS            = 4;
inline constexpr uint8_t CELLS_PER_SEGMENT       = 14;
inline constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;
inline constexpr uint8_t THERM_GPIOS_PER_SEGMENT = 8;

namespace io::adbms
{

inline constexpr uint8_t REG_GROUP_SIZE = 6;

using RegBuffer = std::array<uint8_t, REG_GROUP_SIZE>;

enum class OpenWireSwitch
{
    ODD_CHANNELS,
    EVEN_CHANNELS,
    CHANNEL_COUNT
};

// Configuration Register Group A (Table 55 and 102)
struct __attribute__((packed)) CFGA
{
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
    bool operator==(const CFGA &cfga) const = default;
};
static_assert(sizeof(CFGA) == REG_GROUP_SIZE);
static_assert(sizeof(RegBuffer) == sizeof(CFGA));

// Configuration Register Group B (Table 56 and 103)
struct __attribute__((packed)) CFGB
{
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
static_assert(sizeof(RegBuffer) == sizeof(CFGB));

struct SegmentConfig
{
    CFGA reg_a;
    CFGB reg_b;
    bool operator==(const SegmentConfig &) const = default;
};

// PWM Register Group A: per-cell duty cycle for cells 0–11
struct __attribute__((packed)) PWMA
{
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

// PWM Register Group B: per-cell duty cycle for cells 12–13
struct __attribute__((packed)) PWMB
{
    uint8_t  pwm13 : 4;
    uint8_t  pwm14 : 4;
    uint8_t  pwm15 : 4;
    uint8_t  pwm16 : 4;
    uint32_t res : 32;
    bool     operator==(const PWMB &other) const { return std::memcmp(this, &other, sizeof(PWMB)) == 0; }
};
static_assert(sizeof(PWMB) == REG_GROUP_SIZE);

struct PWMConfig
{
    PWMA reg_a;
    PWMB reg_b;
    bool operator==(const PWMConfig &) const = default;
};

struct __attribute__((packed)) STATA
{
    int16_t  vref2;
    int16_t  itmp;
    uint16_t reserved;
};
static_assert(sizeof(STATA) == REG_GROUP_SIZE);

struct __attribute__((packed)) STATB
{
    int16_t vd;
    int16_t va;
    int16_t vres;
};
static_assert(sizeof(STATB) == REG_GROUP_SIZE);

struct __attribute__((packed)) STATC
{
    uint16_t csflt;

    uint8_t : 3;
    uint16_t ct : 11;
    uint8_t  cts : 2;

    uint8_t va_ov : 1;
    uint8_t va_uv : 1;
    uint8_t vd_ov : 1;
    uint8_t vd_uv : 1;
    uint8_t ced : 1;
    uint8_t cmed : 1;
    uint8_t sed : 1;
    uint8_t smed : 1;

    uint8_t vdel : 1;
    uint8_t vde : 1;
    uint8_t comp : 1;
    uint8_t spiflt : 1;
    uint8_t sleep : 1;
    uint8_t thsd : 1;
    uint8_t tmodchk : 1;
    uint8_t oscchk : 1;
};
#ifdef TARGET_EMBEDDED
static_assert(sizeof(STATC) == REG_GROUP_SIZE);
#endif

struct __attribute__((packed)) STATD
{
    uint32_t covuv;
    uint8_t : 8;
    uint8_t oc_cntr;
};
static_assert(sizeof(STATD) == REG_GROUP_SIZE);

struct __attribute__((packed)) STATE
{
    uint32_t : 32;
    uint16_t gpi : 10;
    uint8_t : 2;
    uint8_t rev : 4;
};
#ifdef TARGET_EMBEDDED
static_assert(sizeof(STATE) == REG_GROUP_SIZE);
#endif

struct StatusGroupsRes
{
    result<STATA> stat_a;
    result<STATB> stat_b;
    result<STATC> stat_c;
    result<STATD> stat_d;
    result<STATE> stat_e;
};

// Base array types. Kept inside io::adbms so generic names don't collide at global scope.
template <typename T> using Segments          = std::array<T, NUM_SEGMENTS>;
template <typename T> using SegmentCells      = std::array<T, CELLS_PER_SEGMENT>;
template <typename T> using SegmentTherms     = std::array<T, THERMISTORS_PER_SEGMENT>;
template <typename T> using SegmentThermGpios = std::array<T, THERM_GPIOS_PER_SEGMENT>;
template <typename T> using Cells             = Segments<SegmentCells<T>>;
template <typename T> using Therms            = Segments<SegmentTherms<T>>;
template <typename T> using ThermGpios        = Segments<SegmentThermGpios<T>>;

namespace write
{
    [[nodiscard]] result<void> pwmReg(const Segments<PWMConfig> &pwm_config);
    [[nodiscard]] result<void> configReg(const Segments<SegmentConfig> &config);
} // namespace write

namespace read
{
    [[nodiscard]] Segments<result<SegmentConfig>> configReg();
    [[nodiscard]] Segments<result<PWMConfig>>     pwmReg();
    /**
     * Reads C-ADCs
     * @return
     */
    [[nodiscard]] Cells<result<int16_t>> cellVoltage();
    /**
     * Reads S-ADCs
     * @return
     */
    [[nodiscard]] Cells<result<int16_t>>      secondaryCellVoltage();
    [[nodiscard]] Segments<result<int16_t>>   segVoltage();
    [[nodiscard]] ThermGpios<result<int16_t>> thermGpioVoltage();
    [[nodiscard]] Segments<StatusGroupsRes>   status();
} // namespace read

namespace command
{
    [[nodiscard]] result<void> startCellsAdc();
    [[nodiscard]] result<void> startAuxAdc();
    /**
     * Polls for ADCs conversion, with retries. If returns successfully, S-ADCs are ready to be read.
     * @throws ErrorCode poll() errors if poll fails
     * @throws ErrorCode::POLL_INVALID if ADC is not ready after retries
     * @return
     */
    [[nodiscard]] result<void> pollSecondaryCellsAdc();
    /**
     * Polls for ADCs conversion, with retries. If returns successfully, C-ADCs are ready to be read.
     * @throws ErrorCode poll() errors if poll fails
     * @throws ErrorCode::POLL_INVALID if ADC is not ready after retries
     * @return
     */
    [[nodiscard]] result<void> pollCellsAdc();
    /**
     * Polls for ADCs conversion, with retries. If returns successfully, Aux ADCs are ready to be read.
     * @throws ErrorCode poll() errors if poll fails
     * @throws ErrorCode::POLL_INVALID if ADC is not ready after retries
     * @return
     */
    [[nodiscard]] result<void> pollAuxAdc();
    [[nodiscard]] result<void> startBalance();
    [[nodiscard]] result<void> stopBalance();
    [[nodiscard]] result<void> wakeup();
    [[nodiscard]] result<void> owcCells(OpenWireSwitch owcSwitch);
} // namespace command

namespace clear
{
    [[nodiscard]] result<void> aux();
    [[nodiscard]] result<void> cell();
    [[nodiscard]] result<void> filteredCell();
    [[nodiscard]] result<void> stat();
} // namespace clear

// random bullshit
Segments<uint8_t> getCmdCountMismatches();
} // namespace io::adbms
