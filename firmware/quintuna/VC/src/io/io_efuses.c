#include "hw_adcs.h"
#include "hw_gpios.h"
#include "hw_i2cs.h"
#include "io_efuses.h"
#include "io_efuse/io_efuse_ST_VND5/io_efuse_ST_VND5.h"
#include "io_efuse/io_efuse_TI_TPS25/io_efuse_TI_TPS25.h"

static ST_VND5_Efuse  f_inv_ST_VND5_Efuse     = { .stby_reset_gpio = &fr_stby_inv };
static ST_VND5_Efuse  rsm_ST_VND5_Efuse       = { .stby_reset_gpio = &fr_stby_inv };
static ST_VND5_Efuse  bms_ST_VND5_Efuse       = { .stby_reset_gpio = &fr_stby_front };
static ST_VND5_Efuse  r_inv_ST_VND5_Efuse     = { .stby_reset_gpio = &fr_stby_front };
static ST_VND5_Efuse  dam_ST_VND5_Efuse       = { .stby_reset_gpio = &fr_stby_rear };
static ST_VND5_Efuse  front_ST_VND5_Efuse     = { .stby_reset_gpio = &fr_stby_rear };
static TI_TPS25_Efuse rl_pump_TI_TPS25_Efuse  = { .pgood = &rl_pump_pgood };
static ST_VND5_Efuse  r_rad_fan_ST_VND5_Efuse = { .stby_reset_gpio = &fr_stby_rad };

const Efuse f_inv_efuse = { .enable_gpio     = &f_inv_en,
                            .sns_adc_channel = &inv_f_pwr_i_sns,
                            .st_vnd5         = &f_inv_ST_VND5_Efuse,
                            .efuse_functions = &ST_VND5_Efuse_functions };

const Efuse rsm_efuse = { .enable_gpio     = &rsm_en,
                          .sns_adc_channel = &rsm_i_sns,
                          .st_vnd5         = &f_inv_ST_VND5_Efuse,
                          .efuse_functions = &ST_VND5_Efuse_functions };

const Efuse bms_efuse = { .enable_gpio     = &bms_en,
                          .sns_adc_channel = &bms_i_sns,
                          .st_vnd5         = &bms_ST_VND5_Efuse,
                          .efuse_functions = &ST_VND5_Efuse_functions };

const Efuse r_inv_efuse = { .enable_gpio     = &r_inv_en,
                            .sns_adc_channel = &inv_r_pwr_i_sns,
                            .st_vnd5         = &r_inv_ST_VND5_Efuse,
                            .efuse_functions = &ST_VND5_Efuse_functions };

const Efuse dam_efuse = { .enable_gpio     = &dam_en,
                          .sns_adc_channel = &dam_i_sns,
                          .st_vnd5         = &dam_ST_VND5_Efuse,
                          .efuse_functions = &ST_VND5_Efuse_functions };

const Efuse front_efuse = { .enable_gpio     = &front_en,
                            .sns_adc_channel = &front_i_sns,
                            .st_vnd5         = &front_ST_VND5_Efuse,
                            .efuse_functions = &ST_VND5_Efuse_functions };

const Efuse rl_pump_efuse = { .enable_gpio     = &rl_pump_en,
                              .sns_adc_channel = &pump_rl_pwr_i_sns,
                              .ti_tps25        = &rl_pump_TI_TPS25_Efuse,
                              .efuse_functions = &TI_TPS25_Efuse_functions };

const Efuse r_rad_fan_efuse = { .enable_gpio     = &rr_rad_fan_en,
                                .sns_adc_channel = &r_rad_fan_i_sns,
                                .st_vnd5         = &r_rad_fan_ST_VND5_Efuse,
                                .efuse_functions = &TI_TPS25_Efuse_functions };

const Efuse *const efuse_channels[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV] = &f_inv_efuse,     [EFUSE_CHANNEL_RSM] = &rsm_efuse,
    [EFUSE_CHANNEL_BMS] = &bms_efuse,         [EFUSE_CHANNEL_R_INV] = &r_inv_efuse,
    [EFUSE_CHANNEL_DAM] = &dam_efuse,         [EFUSE_CHANNEL_FRONT] = &front_efuse,
    [EFUSE_CHANNEL_RL_PUMP] = &rl_pump_efuse, [EFUSE_CHANNEL_R_RAD] = &r_rad_fan_efuse
};
