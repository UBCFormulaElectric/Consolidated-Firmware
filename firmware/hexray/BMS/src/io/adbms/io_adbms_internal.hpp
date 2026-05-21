#pragma once

#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <bitset>

using namespace std;

namespace io::adbms
{

inline constexpr uint8_t REG_GROUP_SIZE = 6;

// Transaction framing constants.
inline constexpr uint8_t CMD_BYTES = 2;
inline constexpr uint8_t PEC_BYTES = 2;

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
inline constexpr uint16_t ADCV_BASE  = 0x0260U;
inline constexpr uint16_t ADSV_BASE  = 0x0168U;
inline constexpr uint16_t ADAX_BASE  = 0x0410U;
inline constexpr uint16_t ADAX2_BASE = 0x0400U;

// Snapshot
inline constexpr uint16_t SNAP   = 0x002DU;
inline constexpr uint16_t UNSNAP = 0x002FU;

// Counter resets — RSTCC clears the command counter, SRST puts the daisy chain
// into sleep (which also resets CC to 0 on wake). Both are INC = No.
inline constexpr uint16_t RSTCC = 0x002EU;
inline constexpr uint16_t SRST  = 0x0027U;

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
inline constexpr uint32_t POLL_STATUS_READY = __builtin_bswap32(0xFFFFFFFFU >> (2 * NUM_SEGMENTS));
inline constexpr uint8_t POLL_RETRIES = 5U;
inline constexpr uint8_t POLL_RETRY_DELAY_MS = 1U;

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

struct SegmentConfig
{
    CFGA reg_a;
    CFGB reg_b;
    bool operator==(const SegmentConfig &) const = default;
};

struct __attribute__((packed)) STATA
{
    uint16_t vref2;
    uint16_t itmp;
    uint16_t reserved;
};
static_assert(sizeof(STATA) == REG_GROUP_SIZE);

struct __attribute__((packed)) STATB
{
    uint16_t vd;
    uint16_t va;
    uint16_t vres;
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
static_assert(sizeof(STATC) == REG_GROUP_SIZE);

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
static_assert(sizeof(STATE) == REG_GROUP_SIZE);

struct StatusGroups
{
    result<STATA> stat_a;
    result<STATB> stat_b;
    result<STATC> stat_c;
    result<STATD> stat_d;
    result<STATE> stat_e;
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
    uint8_t pwm13 : 4;
    uint8_t pwm14 : 4;
    uint8_t pwm15 : 4;
    uint8_t pwm16 : 4;
    uint32_t : 32;
    bool operator==(const PWMB &other) const { return std::memcmp(this, &other, sizeof(PWMB)) == 0; }
};
static_assert(sizeof(PWMB) == REG_GROUP_SIZE);

struct PWMConfig
{
    PWMA reg_a;
    PWMB reg_b;
    bool operator==(const PWMConfig &) const = default;
};

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
[[nodiscard]] result<void> writeRegGroup(uint16_t cmd, const array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs);
} // namespace io::adbms
