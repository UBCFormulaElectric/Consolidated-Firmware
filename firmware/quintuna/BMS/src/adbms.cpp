#include "adbms.h"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

static io::adbms::SegmentConfig configs[io::NUM_SEGMENTS];
static ExitCode                 success[io::NUM_SEGMENTS];
static io::adbms::SegmentConfig configReg[io::NUM_SEGMENTS];
io::adbms::SegmentConfig default_config{};


CFUNC void adbms_init()
{
    for (uint8_t segment = 0; segment < io::NUM_SEGMENTS; segment++)
    {
        configReg[segment].reg_a.cth = 1;
        configReg[segment].reg_a.ref_on = 1;
        configReg[segment].reg_a.flag_d = 0;
        configReg[segment].reg_a.owa = 0;
        configReg[segment].reg_a.owrng = 0;
        configReg[segment].reg_a.soak_on = 0;
        configReg[segment].reg_a.gpio_1_8 = 0b11111111;
        configReg[segment].reg_a.gpio_9_10 = 0b11;
        configReg[segment].reg_a.fc = 0;
        configReg[segment].reg_a.comm_bk = 0;
        configReg[segment].reg_a.mute_st = 0;
        configReg[segment].reg_a.snap_st = 0;

        const uint16_t vuv = 0b110100001;
        const uint16_t vov = 0b10001100101;
        configReg[segment].reg_b.vuv_0_7 = static_cast<uint8_t>(vuv & 0xFF);
        configReg[segment].reg_b.vuv_8_11 = static_cast<uint8_t>((vuv >> 8) & 0x0F);
        configReg[segment].reg_b.vov_0_3 = static_cast<uint8_t>(vov & 0x0F);
        configReg[segment].reg_b.vov_4_11 = static_cast<uint8_t>((vov >> 4) & 0xFF);
        configReg[segment].reg_b.dcto_0_5 = 0;
        configReg[segment].reg_b.dtrng = 0;
        configReg[segment].reg_b.dtmen = 0;
        configReg[segment].reg_b.dcc_1_8 = 0;
        configReg[segment].reg_b.dcc_9_16 = 0;
    }

    io::adbms::wakeup();
    io::adbms::writeConfigurationRegisters(configReg);
}

CFUNC void adbms_tick()
{
    io::adbms::wakeup();
    io::adbms::writeConfigurationRegisters(configReg);
    io::adbms::readConfigurationRegisters(configs, success);
}
