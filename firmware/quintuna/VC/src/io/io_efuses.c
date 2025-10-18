#include "hw_adcs.h"
#include "hw_gpios.h"
#include "hw_i2cs.h"
#include "io_efuses.h"
#include "io_efuse/io_efuse_ST/io_efuse_ST.h"
#include "io_efuse/io_efuse_TI/io_efuse_TI.h"

static ST_Efuse f_inv_st_efuse     = { .stby_reset_gpio = &fr_stby_inv };
static ST_Efuse rsm_st_efuse       = { .stby_reset_gpio = &fr_stby_inv };
static ST_Efuse bms_st_efuse       = { .stby_reset_gpio = &fr_stby_front };
static ST_Efuse r_inv_st_efuse     = { .stby_reset_gpio = &fr_stby_front };
static ST_Efuse dam_st_efuse       = { .stby_reset_gpio = &fr_stby_rear };
static ST_Efuse front_st_efuse     = { .stby_reset_gpio = &fr_stby_rear };
static TI_Efuse rl_pump_ti_efuse   = { .pgood = &rl_pump_pgood };
static TI_Efuse rr_pump_ti_efuse   = { .pgood = &rr_pump_pgood };
static TI_Efuse f_pump_ti_efuse   = { .pgood = &f_pump_pgood };
static ST_Efuse r_rad_fan_st_efuse = { .stby_reset_gpio = &fr_stby_rad };

const Efuse f_inv_efuse = { .enable_gpio     = &f_inv_en,
                            .sns_adc_channel = &inv_f_pwr_i_sns,
                            .st              = &f_inv_st_efuse,
                            .efuse_functions = &st_efuse_functions };

const Efuse rsm_efuse = { .enable_gpio     = &rsm_en,
                          .sns_adc_channel = &rsm_i_sns,
                          .st              = &f_inv_st_efuse,
                          .efuse_functions = &st_efuse_functions };

const Efuse bms_efuse = { .enable_gpio     = &bms_en,
                          .sns_adc_channel = &bms_i_sns,
                          .st              = &bms_st_efuse,
                          .efuse_functions = &st_efuse_functions };

const Efuse r_inv_efuse = { .enable_gpio     = &r_inv_en,
                            .sns_adc_channel = &inv_r_pwr_i_sns,
                            .st              = &r_inv_st_efuse,
                            .efuse_functions = &st_efuse_functions };

const Efuse dam_efuse = { .enable_gpio     = &dam_en,
                          .sns_adc_channel = &dam_i_sns,
                          .st              = &dam_st_efuse,
                          .efuse_functions = &st_efuse_functions };

const Efuse front_efuse = { .enable_gpio     = &front_en,
                            .sns_adc_channel = &front_i_sns,
                            .st              = &front_st_efuse,
                            .efuse_functions = &st_efuse_functions };

const Efuse rl_pump_efuse = { .enable_gpio     = &rl_pump_en,
                              .sns_adc_channel = &pump_rl_pwr_i_sns,
                              .ti              = &rl_pump_ti_efuse,
                              .efuse_functions = &ti_efuse_functions };

const Efuse rr_pump_efuse = { .enable_gpio     = &rr_pump_en,
                              .sns_adc_channel = &pump_rr_pwr_i_sns,
                              .ti              = &rr_pump_ti_efuse,
                              .efuse_functions = &ti_efuse_functions }; 
                              
const Efuse f_pump_efuse = { .enable_gpio     = &f_pump_en,
                              .sns_adc_channel = &pump_f_pwr_i_sns,
                              .ti              = &f_pump_ti_efuse,
                              .efuse_functions = &ti_efuse_functions };

const Efuse r_rad_fan_efuse = { .enable_gpio     = &rr_rad_fan_en,
                                .sns_adc_channel = &r_rad_fan_i_sns,
                                .st              = &r_rad_fan_st_efuse,
                                .efuse_functions = &st_efuse_functions };

const Efuse *const efuse_channels[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV] = &f_inv_efuse,     [EFUSE_CHANNEL_RSM] = &rsm_efuse,
    [EFUSE_CHANNEL_BMS] = &bms_efuse,         [EFUSE_CHANNEL_R_INV] = &r_inv_efuse,
    [EFUSE_CHANNEL_DAM] = &dam_efuse,         [EFUSE_CHANNEL_FRONT] = &front_efuse,
    [EFUSE_CHANNEL_RL_PUMP] = &rl_pump_efuse, [EFUSE_CHANNEL_RR_PUMP] = &rr_pump_efuse, 
    [EFUSE_CHANNEL_F_PUMP] = &f_inv_efuse, [EFUSE_CHANNEL_R_RAD] = &r_rad_fan_efuse
};
