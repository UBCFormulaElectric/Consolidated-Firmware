#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

#include <cstring>

static std::array<std::array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> regs_a;
static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS>      success_a;
static std::array<std::array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> regs_b;
static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS>      success_b;

namespace io::adbms
{
std::expected<void, ErrorCode> writeConfigReg(std::array <SegmentConfig, NUM_SEGMENTS> &config) {
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
    std::array<SegmentConfig, NUM_SEGMENTS>       &configs,
    std::array<std::expected<void, ErrorCode>,NUM_SEGMENTS> &success)
{
    readRegGroup<6>(RDCFGA, regs_a, success_a);
    readRegGroup<6>(RDCFGB, regs_b, success_b);

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
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

        std::memcpy(&configs[seg].reg_a, &regs_a[seg], sizeof(CFGA));
        std::memcpy(&configs[seg].reg_b, &regs_b[seg], sizeof(CFGB));
        success[seg] = {};

        
        
    }

}
} // namespace io::adbms
