#include "io_loadswitches.hpp"
using namespace hw::adcs;
io::TI_TPS25_Efuse RR_PUMP_Efuse{ rr_pump_en, adc_rr_pump, rr_pump_pgood };
io::TI_TPS25_Efuse RL_PUMP_Efuse{ rl_pump_en, adc_rl_pump, rl_pump_pgood };
io::TI_TPS25_Efuse R_RAD_Efuse{ r_rad_fan_en, adc_r_rad_fan, r_rad_fan_pg };
io::TI_TPS25_Efuse L_RAD_Efuse{ l_rad_fan_en, adc_l_rad_fan, l_rad_fan_pg };
io::TI_TPS28_Efuse F_INV_Efuse{ f_inv_en, adc_f_inv, f_inv_pg };
io::TI_TPS28_Efuse R_INV_Efuse{ r_inv_en, adc_r_inv, r_inv_pg };
io::TI_TPS28_Efuse RSM_Efuse{ rsm_en, adc_rsm, rsm_pg };
io::TI_TPS28_Efuse BMS_Efuse{ bms_en, adc_bms, bms_pg };
io::TI_TPS28_Efuse DAM_Efuse{ dam_en, adc_dam, dam_pg };
io::TI_TPS28_Efuse FRONT_Efuse{ front_en, adc_front, front_pg };
