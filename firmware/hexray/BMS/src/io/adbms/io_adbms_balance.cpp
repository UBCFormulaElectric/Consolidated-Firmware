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

expected<void, ErrorCode> writePwmReg(const array<PWMConfig, NUM_SEGMENTS> &pwm_config)
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

Segments<expected<PWMConfig, ErrorCode>> readPwmReg()
{
    Segments<expected<PWMConfig, ErrorCode>> configs;

    const auto regs_a = readRegGroup(RDPWMA);
    const auto regs_b = readRegGroup(RDPWMB);

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        if (!regs_a[seg])
        {
            configs[seg] = unexpected(regs_a[seg].error());
            continue;
        }
        if (!regs_b[seg])
        {
            configs[seg] = unexpected(regs_b[seg].error());
            continue;
        }
        configs[seg]->reg_a = *reinterpret_cast<const PWMA *>(regs_a[seg]->data());
        configs[seg]->reg_b = *reinterpret_cast<const PWMB *>(regs_b[seg]->data());
    }
    return configs;
}
} // namespace io::adbms
