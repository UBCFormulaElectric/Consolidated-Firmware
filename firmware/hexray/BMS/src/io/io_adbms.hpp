/**
 * @file io_adbms.hpp
 * @brief This file contains the interface to the ADBMS driver
 */
#pragma once
#include "util_errorCodes.hpp"
#include <array>

namespace io
{
inline constexpr uint8_t NUM_SEGMENTS            = 1;
inline constexpr uint8_t CELLS_PER_SEGMENT       = 14;
inline constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;

namespace adbms
{
    inline constexpr uint8_t REG_GROUP_SIZE      = 6; // bytes
    inline constexpr uint8_t NUM_VOLT_REG_GROUPS = 5; // A..E only (F not used; 14 cells measured)
    inline constexpr uint8_t NUM_TEMP_REG_GROUPS = 4; // A..D only
    inline constexpr uint8_t NUM_STAT_REG_GROUPS = 5;
    inline constexpr uint8_t GPIOS_PER_SEGMENT   = 10;
    inline constexpr uint8_t THERM_GPIOS_PER_SEGMENT = 8;

    /**
     * @file adbms/io_adbms_configs.cpp
     */

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
    };

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
    };

    struct SegmentConfig
    {
        CFGA reg_a;
        CFGB reg_b;
    };

    struct __attribute__((packed)) STATA
    {
        uint16_t vref2;
        uint16_t itmp;
        uint16_t reserved;
    };

    struct __attribute__((packed)) STATB
    {
        uint16_t vd;
        uint16_t va;
        uint16_t : 16;
    };

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

    struct __attribute__((packed)) STATD
    {
        uint32_t covuv;
        uint8_t : 8;
        uint8_t oc_cntr;
    };

    struct __attribute__((packed)) STATE
    {
        uint32_t : 32;
        uint16_t gpi : 10;
        uint8_t : 2;
        uint8_t rev : 4;
    };

    struct StatusGroups
    {
        STATA stat_a;
        STATB stat_b;
        STATC stat_c;
        STATD stat_d;
        STATE stat_e;
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
    };

    // PWM Register Group B: per-cell duty cycle for cells 12–13
    struct __attribute__((packed)) PWMB
    {
        uint8_t pwm13 : 4;
        uint8_t pwm14 : 4;
        uint8_t pwm15 : 4;
        uint8_t pwm16 : 4;
        uint32_t : 32;
    };

    struct PWMConfig
    {
        PWMA reg_a;
        PWMB reg_b;
    };

    enum class OpenWireSwitch
    {
        OddChannels,
        EvenChannels
    };

    std::expected<void, ErrorCode> writeConfigReg(std::array<SegmentConfig, NUM_SEGMENTS> &config);
    std::expected<void, ErrorCode> writePwmReg(std::array<PWMConfig, NUM_SEGMENTS> &pwm_config);

    void                           readConfigReg(
                                  std::array<SegmentConfig, NUM_SEGMENTS>                  &configs,
                                  std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS> &success);
    void                           readCellVoltageReg(
                                  std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS>                       &cell_voltage_regs,
                                  std::array<std::array<std::expected<void, ErrorCode>, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success);
    void                           readFilteredCellVoltageReg(
                                  std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS>                       &filtered_cell_voltage_regs,
                                  std::array<std::array<std::expected<void, ErrorCode>, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success);   
    void                           readRedundantCellVoltageReg(
                                  std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS>                       &redundant_cell_voltage_regs,
                                  std::array<std::array<std::expected<void, ErrorCode>, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success);                             
    void                           readCellTempReg(
                                  std::array<std::array<uint16_t, THERM_GPIOS_PER_SEGMENT>, NUM_SEGMENTS>                       &cell_temp_regs,
                                  std::array<std::array<std::expected<void, ErrorCode>, THERM_GPIOS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success);
    void                           readStatusReg(
                                    std::array<StatusGroups, NUM_SEGMENTS>                   &stat_regs,
                                    std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS> &stat_regs_success);

    std::expected<void, ErrorCode> baselineCells();
    std::expected<void, ErrorCode> owcCells(OpenWireSwitch owcSwitch);
    std::expected<void, ErrorCode> owcTherms(OpenWireSwitch owcSwitch);

    std::expected<void, ErrorCode> startCellsAdcConversion();
    std::expected<void, ErrorCode> startTempAdcConversion();
    std::expected<void, ErrorCode> pollCellsAdcConversion();
    std::expected<void, ErrorCode> pollAdcConversions();
    std::expected<void, ErrorCode> sendBalanceCmd();
    std::expected<void, ErrorCode> sendStopBalanceCmd();
    std::expected<void, ErrorCode> wakeup();

    std::expected<void, ErrorCode> clearCellTempReg();

} // namespace adbms
} // namespace io
