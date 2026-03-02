#include "adbms.h"
#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "app_segments.hpp"
#include "util_errorCodes.hpp"
#include "io_time.hpp"

#include <cassert>

static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configs;
static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS> success;
static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configReg;

static std::array<std::array<uint8_t, 42>, io::NUM_SEGMENTS>         cell_voltage_regs;
static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS>  comm_success;

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
        reg_b.dcc_1_8          = 0x00;
        reg_b.dcc_9_16         = 0x00;
    }

    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigReg(configReg));
}

CFUNC void adbms_tick()
{
    LOG_INFO("tick!");
    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigReg(configReg));
    io::adbms::readConfigReg(configs, success);
    io::adbms::startCellsAdcConversion();
    io::time::delay(2);
    io::adbms::pollCellsAdcConversion();
    io::adbms::sendCmd(io::adbms::SNAP);
    io::adbms::readRegGroup<42>(io::adbms::RDCVALL, cell_voltage_regs, comm_success);
    io::adbms::sendCmd(io::adbms::UNSNAP);
    //io::adbms::readCellVoltageReg(cell_voltage_regs, comm_success);
    //assert(io::adbms::sendBalanceCmd());
    LOG_INFO("done!");
}

