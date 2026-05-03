#include "io_efuses.hpp"

#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

#include <array>
#include <functional>

// tps28 efuses
const io::TI_TPS28_Efuse f_inv_efuse(f_inv_en, adc_f_inv, f_inv_pg);
const io::TI_TPS28_Efuse r_inv_efuse(r_inv_en, adc_r_inv, r_inv_pg);
const io::TI_TPS28_Efuse bms_efuse(bms_en, adc_bms, bms_pg);
const io::TI_TPS28_Efuse rsm_efuse(rsm_en, adc_rsm, rsm_pg);
// constexpr io::TI_TPS28_Efuse misc_fuse_efuse(misc_fuse_en, adc_misc_fuse, misc_fuse_pg, misc_fuse_en);
// (potential addee)
const io::TI_TPS28_Efuse dam_efuse(dam_en, adc_dam, dam_pg);
const io::TI_TPS28_Efuse front_efuse(front_en, adc_front, front_pg);
const io::TI_TPS28_Efuse l_rad_fan_efuse(l_rad_fan_en, adc_l_rad_fan, l_rad_fan_pg);
const io::TI_TPS28_Efuse r_rad_fan_efuse(r_rad_fan_en, adc_r_rad_fan, r_rad_fan_pg);

// tps25 efuses
const io::TI_TPS25_Efuse rl_pump_efuse(rl_pump_en, adc_rl_pump, rl_pump_pgood);
const io::TI_TPS25_Efuse rr_pump_efuse(rr_pump_en, adc_rr_pump, rr_pump_pgood);

std::array<std::reference_wrapper<const io::Efuse>, NUM_EFUSE_CHANNELS> efuses = {
    { f_inv_efuse, r_inv_efuse, bms_efuse, rsm_efuse, dam_efuse, front_efuse, l_rad_fan_efuse, r_rad_fan_efuse,
      rl_pump_efuse, rr_pump_efuse }
}; // POTENTIAL ADD: misc fuse
