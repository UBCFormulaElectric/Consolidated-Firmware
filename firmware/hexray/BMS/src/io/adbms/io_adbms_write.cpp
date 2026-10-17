#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include <bit>

namespace io::adbms::write {
    result<void> configReg(const Segments<CFGA> &config_a, const Segments<CFGB> &config_b) {
        RETURN_IF_ERR_SILENT(reversible::writeRegGroup(WRCFGA, std::bit_cast<Segments<RegGroup>>(config_a)));
        RETURN_IF_ERR_SILENT(reversible::writeRegGroup(WRCFGB, std::bit_cast<Segments<RegGroup>>(config_b)));
        return {};
    }

    result<void> pwmReg(const Segments<PWMA> &pwm_a, const Segments<PWMB> &pwm_b) {
        RETURN_IF_ERR_SILENT(reversible::writeRegGroup(WRPWMA, std::bit_cast<Segments<RegGroup>>(pwm_a)));
        RETURN_IF_ERR_SILENT(reversible::writeRegGroup(WRPWMB, std::bit_cast<Segments<RegGroup>>(pwm_b)));
        return {};
    }

    result<void> clearFlags() {
        Segments<RegGroup> clr_regs;
        for (RegGroup &reg : clr_regs) {
            reg.fill(0xFFU);
        }
        RETURN_IF_ERR_SILENT(reversible::writeRegGroup(CLRFLAG, clr_regs));
        RETURN_IF_ERR_SILENT(reversible::writeRegGroup(CLOVUV, clr_regs));
        return {};
    }
}
