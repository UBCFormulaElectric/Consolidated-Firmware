#include "io_efuses.hpp"

#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

#include <array>
#include <functional>

namespace io
{
// tps28 efuses
TI_TPS28_Efuse f_inv_efuse(f_inv_en, hw::adcs::adc_f_inv, f_inv_pg);
TI_TPS28_Efuse r_inv_efuse(r_inv_en, hw::adcs::adc_r_inv, r_inv_pg);
TI_TPS28_Efuse bms_efuse(bms_en, hw::adcs::adc_bms, bms_pg);
TI_TPS28_Efuse rsm_efuse(rsm_en, hw::adcs::adc_rsm, rsm_pg);
// constexpr io::TI_TPS28_Efuse misc_fuse_efuse(misc_fuse_en, hw::adcs::adc_misc_fuse, misc_fuse_pg, misc_fuse_en);
// (potential addee)
TI_TPS28_Efuse dam_efuse(dam_en, hw::adcs::adc_dam, dam_pg);
TI_TPS28_Efuse front_efuse(front_en, hw::adcs::adc_front, front_pg);
TI_TPS28_Efuse l_rad_fan_efuse(l_rad_fan_en, hw::adcs::adc_l_rad_fan, l_rad_fan_pg);
TI_TPS28_Efuse r_rad_fan_efuse(r_rad_fan_en, hw::adcs::adc_r_rad_fan, r_rad_fan_pg);

// tps25 efuses
TI_TPS25_Efuse rl_pump_efuse(rl_pump_en, hw::adcs::adc_rl_pump, rl_pump_pgood);
TI_TPS25_Efuse rr_pump_efuse(rr_pump_en, hw::adcs::adc_rr_pump, rr_pump_pgood);

std::array<std::reference_wrapper<Efuse>, NUM_EFUSE_CHANNELS> efuses = {
    { f_inv_efuse, r_inv_efuse, bms_efuse, rsm_efuse, dam_efuse, front_efuse, l_rad_fan_efuse, r_rad_fan_efuse,
      rl_pump_efuse, rr_pump_efuse }
}; // POTENTIAL ADD: misc fuse
} // namespace io
