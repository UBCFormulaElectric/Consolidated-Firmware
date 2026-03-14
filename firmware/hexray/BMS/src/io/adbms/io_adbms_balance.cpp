#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"

#include <cstring>

using namespace std;

namespace io::adbms
{
expected<void, ErrorCode> sendBalanceCmd()
{
    return sendCmd(UNMUTE);
}

expected<void, ErrorCode> sendStopBalanceCmd()
{
    return sendCmd(MUTE);
}

expected<void, ErrorCode> writePwmReg(array<PWMConfig, NUM_SEGMENTS> &pwm_config)
{
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> pwma_regs{};
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> pwmb_regs{};

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        memcpy(pwma_regs[seg].data(), &pwm_config[seg].reg_a, sizeof(PWMA));
        memcpy(pwmb_regs[seg].data(), &pwm_config[seg].reg_b, sizeof(PWMB));
    }

    if (const auto err = writeRegGroup(WRPWMA, pwma_regs); !err)
        return err;
    if (const auto err = writeRegGroup(WRPWMB, pwmb_regs); !err)
        return err;
    return {};
}

void readPwmReg(array<PWMConfig, NUM_SEGMENTS> &configs, array<expected<void, ErrorCode>, NUM_SEGMENTS> &success) {
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_a{};
    array<expected<void, ErrorCode>, NUM_SEGMENTS>      success_a{};
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_b{};
    array<expected<void, ErrorCode>, NUM_SEGMENTS>      success_b{};

    readRegGroup(RDPWMA, regs_a, success_a);
    readRegGroup(RDPWMB, regs_b, success_b);

    for (size_t seg=0U; seg < NUM_SEGMENTS; ++seg) {
        if (!success_a[seg]) {
            success[seg] = success_a[seg];
            continue;
        }
        if (!success_b[seg]) {
            success[seg] = success_b[seg];
            continue;
        }

        memcpy(&configs[seg].reg_a, regs_a[seg].data(), sizeof(PWMA));
        memcpy(&configs[seg].reg_b, regs_b[seg].data(), sizeof(PWMB));
        success[seg] = {};
    }
}

} // namespace io::adbms
