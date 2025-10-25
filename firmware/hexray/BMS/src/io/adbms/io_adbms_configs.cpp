#include "io_adbms.hpp"
#include "util_errorCodes.h"
#include "io_adbms_internal.hpp"

#include <string.h>

namespace io::adbms {
    ExitCode writeConfigurationRegisters(const SegmentConfig config[io::NUM_SEGMENTS]) {
        uint16_t cfga_regs[io::NUM_SEGMENTS];
        uint16_t cfgb_regs[io::NUM_SEGMENTS];

        for (uint8_t seg_idx = 0U; seg_idx < io::NUM_SEGMENTS; seg_idx++) {
            memcpy(&cfga_regs[seg_idx], &config[seg_idx].reg_a, sizeof(CFGAR));
            memcpy(&cfgb_regs[seg_idx], &config[seg_idx].reg_b, sizeof(CFGBR));
        }

        // Write to configuration registers
        RETURN_IF_ERR(io_adbms_writeRegGroup(WRCFGA, cfga_regs));
        RETURN_IF_ERR(io_adbms_writeRegGroup(WRCFGB, cfgb_regs));
        return EXIT_CODE_OK;
    }
    
    void readConfigurationRegisters(SegmentConfig configs[io::NUM_SEGMENTS], ExitCode success[io::NUM_SEGMENTS]) {
        uint16_t regs_a[io::NUM_SEGMENTS];
        ExitCode success_a[io::NUM_SEGMENTS];
        io_adbms_readRegGroup(RDCFGA, regs_a, success_a);

        uint16_t regs_b[io::NUM_SEGMENTS];
        ExitCode success_b[io::NUM_SEGMENTS];
        io_adbms_readRegGroup(RDCFGB, regs_b, success_b);

        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++) {
            if (IS_EXIT_ERR(success_a[seg_idx])) {
                success[seg_idx] = success_a[seg_idx];
                continue;
            } else if (IS_EXIT_ERR(success_b[seg_idx])) {
                success[seg_idx] = success_b[seg_idx];
                continue;
            }

            memcpy(&configs[seg_idx].reg_a, &regs_a[seg_idx], sizeof(CFGAR));
            memcpy(&configs[seg_idx].reg_b, &regs_b[seg_idx], sizeof(CFGBR));
            success[seg_idx] = EXIT_CODE_OK;
        }
    }
}