#include "io_loadswitches.h"
#include "hw_adcs.h"
#include "hw_gpios.h"
#include "hw_i2cs.h"
#include "io_loadswitch.h"

static const Efuse f_inv_efuse     = { .enable_gpio = &f_inv_en, .sns_adc_channel = &inv_f_pwr_i_sns };
static const Efuse rsm_efuse       = { .enable_gpio = &rsm_en, .sns_adc_channel = &rsm_i_sns };
static const Efuse bms_efuse       = { .enable_gpio = &bms_en, .sns_adc_channel = &bms_i_sns };
static const Efuse r_inv_efuse     = { .enable_gpio = &r_inv_en, .sns_adc_channel = &inv_r_pwr_i_sns };
static const Efuse rr_pump_efuse   = { .enable_gpio = &rr_pump_en, .sns_adc_channel = &pump_rr_pwr_i_sns };
static const Efuse f_pump_efuse    = { .enable_gpio = &f_pump_en, .sns_adc_channel = &pump_f_pwr_i_sns };
static const Efuse dam_efuse       = { .enable_gpio = &dam_en, .sns_adc_channel = &dam_i_sns };
static const Efuse front_efuse     = { .enable_gpio = &front_en, .sns_adc_channel = &front_i_sns };
static const Efuse rl_pump_efuse   = { .enable_gpio = &rl_pump_en, .sns_adc_channel = &pump_rl_pwr_i_sns };
static const Efuse r_rad_fan_efuse = { .enable_gpio = &rr_rad_fan_en, .sns_adc_channel = &r_rad_fan_i_sns };
static const Efuse l_rad_fan_efuse = { .enable_gpio = &rl_rad_fan_en, .sns_adc_channel = &l_rad_fan_i_sns };


const ST_LoadSwitch inv_rsm_loadswitch = { .efuse1          = &f_inv_efuse,
                                           .efuse2          = &rsm_efuse,
                                           .stby_reset_gpio = &fr_stby_inv };
const ST_LoadSwitch inv_bms_loadswitch = { .efuse1          = &bms_efuse,
                                           .efuse2          = &r_inv_efuse,
                                           .stby_reset_gpio = &fr_stby_front };
const ST_LoadSwitch front_loadswitch   = { .efuse1          = &dam_efuse,
                                           .efuse2          = &front_efuse,
                                           .stby_reset_gpio = &fr_stby_rear };
const TI_LoadSwitch rr_pump_loadswitch = { .efuse = &rr_pump_efuse, .pgood = &rr_pump_pgood };
const TI_LoadSwitch f_pump_loadswitch  = { .efuse = &f_pump_efuse, .pgood = &f_pump_pgood };
const TI_LoadSwitch rl_pump_loadswitch = { .efuse = &rl_pump_efuse, .pgood = &rl_pump_pgood };
const ST_LoadSwitch rad_fan_loadswitch = { .efuse1          = &l_rad_fan_efuse,
                                           .efuse2          = &r_rad_fan_efuse,
                                           .stby_reset_gpio = &fr_stby_rad };

const Efuse *const efuse_channels[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV] = &f_inv_efuse,     [EFUSE_CHANNEL_RSM] = &rsm_efuse,
    [EFUSE_CHANNEL_BMS] = &bms_efuse,         [EFUSE_CHANNEL_R_INV] = &r_inv_efuse,
    [EFUSE_CHANNEL_DAM] = &dam_efuse,         [EFUSE_CHANNEL_FRONT] = &front_efuse,
    [EFUSE_CHANNEL_RL_PUMP] = &rl_pump_efuse, [EFUSE_CHANNEL_R_RAD] = &r_rad_fan_efuse,
    [EFUSE_CHANNEL_RR_PUMP] = &rr_pump_efuse, [EFUSE_CHANNEL_F_PUMP] = &f_pump_efuse
};
