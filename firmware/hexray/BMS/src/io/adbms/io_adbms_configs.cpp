#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

#include <cstring>

namespace io::adbms
{
std::expected<void, ErrorCode> writeConfigurationRegisters(SegmentConfig config[NUM_SEGMENTS])
{
    static uint8_t cfga_regs[NUM_SEGMENTS][REG_GROUP_SIZE];
    static uint8_t cfgb_regs[NUM_SEGMENTS][REG_GROUP_SIZE];

    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        std::memcpy(&cfga_regs[segment], &config[segment].reg_a, sizeof(CFGA));
        std::memcpy(&cfgb_regs[segment], &config[segment].reg_b, sizeof(CFGB));
    }
    if (const auto err = writeRegGroup(WRCFGA, cfga_regs); not err)
        return err;
    if (const auto err = writeRegGroup(WRCFGB, cfgb_regs); not err)
        return err;
    return {};
}

void readConfigurationRegisters(
    SegmentConfig                  configs[NUM_SEGMENTS],
    std::expected<void, ErrorCode> success[NUM_SEGMENTS])
{
    static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_a;
    static std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      success_a;
    readRegGroup(RDCFGA, regs_a, success_a);

    static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs_b;
    static std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      success_b;
    readRegGroup(RDCFGB, regs_b, success_b);

    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        if (not success_a[segment])
        {
            success[segment] = success_a[segment];
            continue;
        }
        if (not success_b[segment])
        {
            success[segment] = success_b[segment];
            continue;
        }

        std::memcpy(&configs[segment].reg_a, &regs_a[segment], sizeof(CFGA));
        std::memcpy(&configs[segment].reg_b, &regs_b[segment], sizeof(CFGB));
        success[segment] = {};
    }
}
} // namespace io::adbms
