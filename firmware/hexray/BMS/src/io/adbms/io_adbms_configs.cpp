#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"
#include <cstring>

namespace io::adbms
{
result<void> write::configReg(const Segments<SegmentConfig> &config)
{
    Segments<RegBuffer> cfga_regs{};
    Segments<RegBuffer> cfgb_regs{};

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        memcpy(cfga_regs[seg].data(), &config[seg].reg_a, sizeof(CFGA));
        memcpy(cfgb_regs[seg].data(), &config[seg].reg_b, sizeof(CFGB));
    }

    RETURN_IF_ERR_SILENT(writeRegGroup(WRCFGA, cfga_regs));
    RETURN_IF_ERR_SILENT(writeRegGroup(WRCFGB, cfgb_regs));

    return {};
}

Segments<result<SegmentConfig>> read::configReg()
{
    const Segments<result<RegBuffer>> a_regs = readRegGroup(RDCFGA);
    const Segments<result<RegBuffer>> b_regs = readRegGroup(RDCFGB);

    Segments<result<SegmentConfig>> out;
    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        if (!a_regs[seg])
        {
            out[seg] = unexpected(a_regs[seg].error());
            continue;
        }
        if (!b_regs[seg])
        {
            out[seg] = unexpected(b_regs[seg].error());
            continue;
        }
        out[seg] = SegmentConfig{
            .reg_a = *reinterpret_cast<const CFGA *>(a_regs[seg]->data()),
            .reg_b = *reinterpret_cast<const CFGB *>(b_regs[seg]->data()),
        };
    }
    return out;
}

result<void> write::pwmReg(const array<PWMConfig, NUM_SEGMENTS> &pwm_config)
{
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> pwma_regs{};
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> pwmb_regs{};

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        memcpy(pwma_regs[seg].data(), &pwm_config[seg].reg_a, sizeof(PWMA));
        memcpy(pwmb_regs[seg].data(), &pwm_config[seg].reg_b, sizeof(PWMB));
    }

    RETURN_IF_ERR(writeRegGroup(WRPWMA, pwma_regs));
    RETURN_IF_ERR(writeRegGroup(WRPWMB, pwmb_regs));
    return {};
}

Segments<result<PWMConfig>> read::pwmReg()
{
    Segments<result<PWMConfig>> pwm_configs;

    const Segments<result<RegBuffer>> regs_a = readRegGroup(RDPWMA);
    const Segments<result<RegBuffer>> regs_b = readRegGroup(RDPWMB);

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        if (!regs_a[seg])
        {
            pwm_configs[seg] = unexpected(regs_a[seg].error());
            continue;
        }
        if (!regs_b[seg])
        {
            pwm_configs[seg] = unexpected(regs_b[seg].error());
            continue;
        }
        pwm_configs[seg] = {
            .reg_a = *reinterpret_cast<const PWMA *>(regs_a[seg]->data()),
            .reg_b = *reinterpret_cast<const PWMB *>(regs_b[seg]->data()),
        };
    }
    return pwm_configs;
}

Segments<result<uint64_t>> read::serialNum()
{
    Segments<result<uint64_t>> serial_nums{};

    const Segments<result<RegBuffer>> raw_serial = readRegGroup(RDSID);
    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        if (!raw_serial[seg])
        {
            serial_nums[seg] = unexpected(raw_serial[seg].error());
            continue;
        }

        const RegBuffer &bytes  = raw_serial[seg].value();
        uint64_t         serial = 0U;
        for (size_t i = 0U; i < REG_GROUP_SIZE; ++i)
        {
            serial |= static_cast<uint64_t>(bytes[i]) << (8U * i);
        }
        serial_nums[seg] = serial;
    }
    return serial_nums;
}
} // namespace io::adbms
