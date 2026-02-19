/**
 * @file io_adbms.hpp
 * @brief This file contains the interface to the ADBMS driver
 */
#pragma once
#include "util_errorCodes.hpp"

namespace io
{
inline constexpr uint8_t NUM_SEGMENTS            = 1;
inline constexpr uint8_t CELLS_PER_SEGMENT       = 14;
inline constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;
namespace adbms
{
    inline constexpr uint8_t REG_GROUP_SIZE      = 6; // bytes
    inline constexpr uint8_t NUM_VOLT_REG_GROUPS = 5; // A..E only (F not used; 14 cells measured)
    inline constexpr uint8_t NUM_STAT_REG_GROUPS = 5; // A..E

    /**
     * @file adbms/io_adbms_configs.cpp
     */

    // Configuration Register Group A (Table 55 and 102)
    struct __attribute__((packed)) CFGA
    {
        // byte 1
        uint8_t cth : 3;    // C-ADC vs. S-ADC comparison voltage threshold
        uint8_t : 4;        // unused bits
        uint8_t ref_on : 1; // reference powered up, enables REFUP state
        // byte 2
        uint8_t flag_d; // asserts flags in Status Reg C for latent fault detection (pg. 70)
        // byte 3
        uint8_t : 3;
        uint8_t owa : 3;     // open wire soak times
        uint8_t owrng : 1;   // soak time range
        uint8_t soak_on : 1; // enables soak on AUX ADCs
        // byte 4
        uint8_t gpio_1_8; // GPIOx pin control
        // byte 5
        uint8_t gpio_9_10 : 2; // GPIOx pin control
        uint8_t : 6;           // unused bits
        // byte 6
        uint8_t fc : 3;      // IIR filter parameter, see table 21
        uint8_t comm_bk : 1; // communication break, propagation communication prevention
        uint8_t mute_st : 1; // 1 = mute is active, discharging disabled
        uint8_t snap_st : 1; // 1 = snapshot is activated, result registers are frozen
        uint8_t : 2;
    };
    static_assert(sizeof(CFGA) == REG_GROUP_SIZE);

    // Configuration Register Group B (Table 56 and 103)
    struct __attribute__((packed)) CFGB
    {
        // byte 1
        uint8_t vuv_0_7; // cell undervoltage threshold = vuv * 16 * 150uV + 1.5V
        // byte 2
        uint8_t vuv_8_11 : 4;
        uint8_t vov_0_3 : 4; // cell overvoltage threshold = vov * 16 * 150uV + 1.5V
        // byte 3
        uint8_t vov_4_11;
        // byte 4
        uint8_t dcto_0_5 : 6; // discharge timeout value
        uint8_t dtrng : 1;    // discharge timer range setting
        uint8_t dtmen : 1;    // enable discharge timer monitor
        // byte 5
        uint8_t dcc_1_8; // discharge cell x
        // byte 6
        uint8_t dcc_9_16; // discharge cell x
    };
    static_assert(sizeof(CFGB) == REG_GROUP_SIZE);

    struct SegmentConfig
    {
        CFGA reg_a;
        CFGB reg_b;
    };

    struct __attribute__((packed)) STATA
    {
    };

    struct __attribute__((packed)) STATB
    {
    };

    struct __attribute__((packed)) STATC
    {
    };

    struct __attribute__((packed)) STATD
    {
    };

    struct __attribute__((packed)) STATE
    {
    };

    struct StatusGroups
    {
        STATA stat_a;
        STATB stat_b;
        STATC stat_c;
        STATD stat_d;
        STATE stat_e;
    };

    enum class OpenWireSwitch
    {
        OddChannels,
        EvenChannels
    };

    void                           readConfigReg(SegmentConfig configs[NUM_SEGMENTS], std::expected<void, ErrorCode> success[NUM_SEGMENTS]);
    std::expected<void, ErrorCode> writeConfigReg(SegmentConfig config[NUM_SEGMENTS]);
    std::expected<void, ErrorCode> sendBalanceCmd();
    std::expected<void, ErrorCode> sendStopBalanceCmd();
    void                           readVoltageRegisters(uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT], std::expected<void, ErrorCode> comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT]);
    std::expected<void, ErrorCode> startCellsAdcConversion();
    std::expected<void, ErrorCode> pollCellsAdcConversion();
    std::expected<void, ErrorCode> wakeup();
    std::expected<void, ErrorCode> pollAdcConversions();

} // namespace adbms
} // namespace io
