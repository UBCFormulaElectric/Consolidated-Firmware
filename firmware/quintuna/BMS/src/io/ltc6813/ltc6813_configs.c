#include "app/utils.h"
#include "io/ltc6813.h"

#include <string.h>

#include "io/ltc6813/ltc6813_internal.h"

ExitCode io_ltc6813_writeConfigurationRegisters(const SegmentConfig config[NUM_SEGMENTS])
{
    uint16_t cfga_regs[NUM_SEGMENTS][REGS_PER_GROUP];
    uint16_t cfgb_regs[NUM_SEGMENTS][REGS_PER_GROUP];

    for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
    {
        memcpy(cfga_regs[seg_idx], &config[seg_idx].reg_a, sizeof(CFGAR));
        memcpy(cfgb_regs[seg_idx], &config[seg_idx].reg_b, sizeof(CFGBR));
    }

    // Write to configuration registers
    RETURN_IF_ERR(io_ltc6813_writeRegGroup(WRCFGA, cfga_regs));
    RETURN_IF_ERR(io_ltc6813_writeRegGroup(WRCFGB, cfgb_regs));
    return EXIT_CODE_OK;
}

void io_ltc6813_readConfigurationRegisters(SegmentConfig configs[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
{
    uint16_t regs_a[NUM_SEGMENTS][REGS_PER_GROUP];
    ExitCode success_a[NUM_SEGMENTS];
    io_ltc6813_readRegGroup(RDCFGA, regs_a, success_a);

    uint16_t regs_b[NUM_SEGMENTS][REGS_PER_GROUP];
    ExitCode success_b[NUM_SEGMENTS];
    io_ltc6813_readRegGroup(RDCFGB, regs_b, success_b);

    for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
    {
        if (IS_EXIT_ERR(success_a[seg_idx]))
        {
            success[seg_idx] = success_a[seg_idx];
            continue;
        }
        else if (IS_EXIT_ERR(success_b[seg_idx]))
        {
            success[seg_idx] = success_b[seg_idx];
            continue;
        }

        memcpy(&configs[seg_idx].reg_a, regs_a[seg_idx], sizeof(CFGAR));
        memcpy(&configs[seg_idx].reg_b, regs_b[seg_idx], sizeof(CFGBR));
        success[seg_idx] = EXIT_CODE_OK;
    }
}
