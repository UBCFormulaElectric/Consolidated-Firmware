#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

#include <cstring>

namespace io::adbms
{
std::expected<void, ErrorCode> writeConfigReg(SegmentConfig config[NUM_SEGMENTS])
{
    static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> cfga_regs;
    static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> cfgb_regs;

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        std::memcpy(&cfga_regs[seg], &config[seg].reg_a, sizeof(CFGA));
        std::memcpy(&cfgb_regs[seg], &config[seg].reg_b, sizeof(CFGB));
    }
    if (const auto err = writeRegGroup(WRCFGA, cfga_regs); not err)
        return err;
    if (const auto err = writeRegGroup(WRCFGB, cfgb_regs); not err)
        return err;
    return {};
}

void readConfigReg(
    SegmentConfig                  configs[NUM_SEGMENTS],
    std::expected<void, ErrorCode> success[NUM_SEGMENTS])
{
    static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_a;
    static std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      success_a;
    readRegGroup(RDCFGA, regs_a, success_a);

    static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_b;
    static std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      success_b;
    readRegGroup(RDCFGB, regs_b, success_b);

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (not success_a[seg])
        {
            success[seg] = success_a[seg];
            continue;
        }
        if (not success_b[seg])
        {
            success[seg] = success_b[seg];
            continue;
        }

        std::memcpy(&configs[seg].reg_a, &regs_a[seg], sizeof(CFGA));
        std::memcpy(&configs[seg].reg_b, &regs_b[seg], sizeof(CFGB));
        success[seg] = {};
    }

}
} // namespace io::adbms
