#include "adbms.h"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

#include <cassert>

static io::adbms::SegmentConfig       configs[io::NUM_SEGMENTS];
static std::expected<void, ErrorCode> success[io::NUM_SEGMENTS];
static io::adbms::SegmentConfig       configReg[io::NUM_SEGMENTS];
io::adbms::SegmentConfig              default_config{};

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

    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigurationRegisters(configReg));
}

CFUNC void adbms_tick()
{
    LOG_INFO("tick!");
    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigurationRegisters(configReg));
    io::adbms::readConfigurationRegisters(configs, success);
    //assert(io::adbms::sendBalanceCmd());
    LOG_INFO("done!");
}
