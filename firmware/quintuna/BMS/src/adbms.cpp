#include "adbms.h"
#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "app_segments.hpp"
#include "util_errorCodes.hpp"
#include "io_time.hpp"

#include <cassert>

[[maybe_unused]] static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configs;
[[maybe_unused]] static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS> success;
[[maybe_unused]] static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configReg;

[[maybe_unused]] static std::array<std::array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_regs;
[[maybe_unused]] static std::array<std::array<std::expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>
    cell_voltage_success;

CFUNC void adbms_init()
{
    for (auto &[reg_a, reg_b] : configReg)
    {
        reg_a.cth       = 1;
        reg_a.ref_on    = 1;
        reg_a.flag_d    = 0;
        reg_a.owa       = 0;
        reg_a.owrng     = 0;
        reg_a.soak_on   = 0;
        reg_a.gpio_1_8  = 0b11111111;
        reg_a.gpio_9_10 = 0b11;
        reg_a.fc        = 0;
        reg_a.comm_bk   = 0;
        reg_a.mute_st   = 0;
        reg_a.snap_st   = 0;

        constexpr uint16_t vuv = 0b110100001;
        constexpr uint16_t vov = 0b10001100101;
        reg_b.vuv_0_7          = static_cast<uint8_t>(vuv & 0xFF);
        reg_b.vuv_8_11         = static_cast<uint8_t>(vuv >> 8 & 0x0F);
        reg_b.vov_0_3          = static_cast<uint8_t>(vov & 0x0F);
        reg_b.vov_4_11         = static_cast<uint8_t>(vov >> 4 & 0xFF);
        reg_b.dcto_0_5         = 0;
        reg_b.dtrng            = 0;
        reg_b.dtmen            = 0;
        reg_b.dcc_1_8          = 0xFF;
        reg_b.dcc_9_16         = 0xFF;
    }

    io::adbms::wakeup();
    io::adbms::writeConfigReg(configReg);
    // io::adbms::startCellsAdcConversion();
}

CFUNC void adbms_tick()
{
    LOG_INFO("tick!");
    io::adbms::wakeup();
    io::adbms::writeConfigReg(configReg);
    io::adbms::readConfigReg(configs, success);
    io::adbms::sendBalanceCmd();
    // io::adbms::startCellsAdcConversion();
    // io::time::delay(2);
    // io::adbms::sendCmd(io::adbms::SNAP);
    // io::adbms::readCellVoltageReg(cell_voltage_regs, cell_voltage_success);
    // io::adbms::sendCmd(io::adbms::UNSNAP);
    // io::adbms::readCellVoltageReg(cell_voltage_regs, comm_success);
    // assert(io::adbms::sendBalanceCmd());
    LOG_INFO("done!");
}
