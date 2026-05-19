#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"
#include <cstring>

namespace io::adbms
{
result<void> writeConfigReg(const Segments<SegmentConfig> &config)
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

Segments<result<SegmentConfig>> readConfigReg()
{
    const Segments<result<RegBuffer>> a_regs = readRegGroup(RDCFGA);
    const Segments<result<RegBuffer>> b_regs = readRegGroup(RDCFGB);
    static_assert(sizeof(RegBuffer) == sizeof(CFGA));
    static_assert(sizeof(RegBuffer) == sizeof(CFGB));

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
} // namespace io::adbms
