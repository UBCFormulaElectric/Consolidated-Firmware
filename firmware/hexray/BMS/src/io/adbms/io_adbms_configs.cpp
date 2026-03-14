#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

#include <cstring>

using namespace std;

namespace io::adbms
{
expected<void, ErrorCode> writeConfigReg(array<SegmentConfig, NUM_SEGMENTS> &config)
{
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> cfga_regs{};
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> cfgb_regs{};

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        memcpy(cfga_regs[seg].data(), &config[seg].reg_a, sizeof(CFGA));
        memcpy(cfgb_regs[seg].data(), &config[seg].reg_b, sizeof(CFGB));
    }

    if (const auto err = writeRegGroup(WRCFGA, cfga_regs); not err)
        return err;
    if (const auto err = writeRegGroup(WRCFGB, cfgb_regs); not err)
        return err;
    return {};
}

void readConfigReg(array<SegmentConfig, NUM_SEGMENTS> &configs, array<expected<void, ErrorCode>, NUM_SEGMENTS> &success)
{
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_a{};
    array<expected<void, ErrorCode>, NUM_SEGMENTS>      success_a{};
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_b{};
    array<expected<void, ErrorCode>, NUM_SEGMENTS>      success_b{};

    readRegGroup(RDCFGA, regs_a, success_a);
    readRegGroup(RDCFGB, regs_b, success_b);

    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        if (!success_a[seg])
        {
            success[seg] = success_a[seg];
            continue;
        }
        if (!success_b[seg])
        {
            success[seg] = success_b[seg];
            continue;
        }

        memcpy(&configs[seg].reg_a, regs_a[seg].data(), sizeof(CFGA));
        memcpy(&configs[seg].reg_b, regs_b[seg].data(), sizeof(CFGB));
        success[seg] = {};
    }
}
} // namespace io::adbms
