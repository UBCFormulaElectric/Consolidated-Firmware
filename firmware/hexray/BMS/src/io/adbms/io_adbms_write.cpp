#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include <bit>

namespace io::adbms::write
{
result<void> configReg(const Segments<CFGA> &config_a)
{
    return reversible::writeRegGroup(WRCFGA, std::bit_cast<Segments<RegGroup>>(config_a));
}

result<void> configReg(const Segments<CFGB> &config_b)
{
    return reversible::writeRegGroup(WRCFGB, std::bit_cast<Segments<RegGroup>>(config_b));
}

result<void> pwmReg(const Segments<PWMA> &pwm_a)
{
    return reversible::writeRegGroup(WRPWMA, std::bit_cast<Segments<RegGroup>>(pwm_a));
}

result<void> pwmReg(const Segments<PWMB> &pwm_b)
{
    return reversible::writeRegGroup(WRPWMB, std::bit_cast<Segments<RegGroup>>(pwm_b));
}

result<void> clearAllFlags()
{
    // Need to write 1s to clear flags/uvov
    Segments<RegGroup> clr_regs;
    for (RegGroup &reg : clr_regs)
    {
        reg.fill(0xFFU);
    }
    RETURN_IF_ERR_SILENT(reversible::writeRegGroup(CLRFLAG, clr_regs));
    RETURN_IF_ERR_SILENT(reversible::writeRegGroup(CLOVUV, clr_regs));
    return {};
}

result<void> clearCsFlags()
{
    // Bytes 0-1 (CS1FLT-CS16FLT) are set to clear
    Segments<RegGroup> clr_regs{};
    for (RegGroup &reg : clr_regs)
    {
        reg[0] = 0xFFU;
        reg[1] = 0xFFU;
    }
    return reversible::writeRegGroup(CLRFLAG, clr_regs);
}
} // namespace io::adbms::write
