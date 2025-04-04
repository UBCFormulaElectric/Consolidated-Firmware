#include "hw_adcs.h"
#include "hw_i2cs.h"
#include "vc.pb.h"
#include "shared.pb.h"
#include "hw_gpios.h"
#include "hw_chimeraConfig_v2.h"

// Chimera V2 enums to GPIO peripherals.
const Gpio *id_to_gpio[] = {
    [vc_GpioNetName_GPIO_SB_SHDN_3V3_OUT] = &sb_shdn_3v3_out,
    [vc_GpioNetName_GPIO_INTERIA_3V3_OUT] = &interia_3v3_out,
    [vc_GpioNetName_GPIO_FR_STBY_INV]     = &fr_stby_inv,
    [vc_GpioNetName_GPIO_TSMS_3V3_OUT]    = &tsms_3v3_out,
    [vc_GpioNetName_GPIO_RR_ILCK_3V3_OUT] = &rr_ilck_3v3_out,
    [vc_GpioNetName_GPIO_R_INV_EN]        = &r_inv_en,
    [vc_GpioNetName_GPIO_LED]             = &led,
    [vc_GpioNetName_GPIO_BOOT_LED]        = &boot_led,
    [vc_GpioNetName_GPIO_BMS_EN]          = &bms_en,
    [vc_GpioNetName_GPIO_FR_STBY_FRONT]   = &fr_stby_front,
    [vc_GpioNetName_GPIO_PGOOD]           = &pgood,
    [vc_GpioNetName_GPIO_BAT_CHRG_MODE]   = &bat_chrg_mode,
    [vc_GpioNetName_GPIO_BAT_CHRG_nSHDN]  = &bat_chrg_nshdn,
    [vc_GpioNetName_GPIO_BAT_MTR_nALERT]  = &bat_mtr_nalert,
    [vc_GpioNetName_GPIO_FR_STBY_REAR]    = &fr_stby_rear,
    [vc_GpioNetName_GPIO_FRONT_EN]        = &front_en,
    [vc_GpioNetName_GPIO_RSM_EN]          = &rsm_en,
    [vc_GpioNetName_GPIO_IMU_INT1]        = &imu_int1,
    [vc_GpioNetName_GPIO_IMU_INT2]        = &imu_int2,
    [vc_GpioNetName_GPIO_RAD_FAN_FR_STBY] = &fr_stby_rad,
    [vc_GpioNetName_GPIO_PWR_MTR_nALERT]  = &pwr_mtr_nalert,
    [vc_GpioNetName_GPIO_RL_PUMP_PGOOD]   = &rl_pump_pgood,
    [vc_GpioNetName_GPIO_F_INV_EN]        = &f_inv_en,
    [vc_GpioNetName_GPIO_DAM_EN]          = &dam_en,
    [vc_GpioNetName_GPIO_RR_PUMP_EN]      = &rr_pump_en,
    [vc_GpioNetName_GPIO_RR_PUMP_PGOOD]   = &rr_pump_pgood,
    [vc_GpioNetName_GPIO_F_PUMP_PGOOD]    = &f_pump_pgood,
    [vc_GpioNetName_GPIO_PCM_EN]          = &pcm_en,
    [vc_GpioNetName_GPIO_R_RAD_FAN_EN]    = &rr_rad_fan_en,
    [vc_GpioNetName_GPIO_RL_PUMP_EN]      = &rl_pump_en,
    [vc_GpioNetName_GPIO_L_RAD_FAN_EN]    = &rl_rad_fan_en,
};

// Chimera V2 enums to GPIO peripherals.
const AdcChannel *id_to_adc[] = {
    [vc_AdcNetName_ADC_F_PUMP]    = &pump_f_pwr_i_sns,
    [vc_AdcNetName_ADC_RR_PUMP]   = &pump_rr_pwr_i_sns,
    [vc_AdcNetName_ADC_DAM]       = &dam_i_sns,
    [vc_AdcNetName_ADC_L_RAD_FAN] = &l_rad_fan_i_sns,
    [vc_AdcNetName_ADC_R_RAD_FAN] = &r_rad_fan_i_sns,
    [vc_AdcNetName_ADC_RL_PUMP]   = &pump_rl_pwr_i_sns,
    [vc_AdcNetName_ADC_F_INV]     = &inv_f_pwr_i_sns,
    [vc_AdcNetName_ADC_R_INV]     = &inv_r_pwr_i_sns,
    [vc_AdcNetName_ADC_BMS]       = &bms_i_sns,
    [vc_AdcNetName_ADC_FRONT]     = &front_i_sns,
    [vc_AdcNetName_ADC_RSM]       = &rsm_i_sns,
};

const I2cDevice *id_to_i2c[] = { [vc_I2cNetName_I2C_IMU_I2C]    = &imu,
                                 [vc_I2cNetName_I2C_R_PUMP_I2C] = &pumps,
                                 [vc_I2cNetName_I2C_PWR_MTR]    = &pwr_mtr,
                                 [vc_I2cNetName_I2C_BAT_MTR]    = &bat_mtr };

hw_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_vc_net_name_tag,
                                           .id_to_gpio        = id_to_gpio,
                                           .adc_net_name_tag  = AdcNetName_vc_net_name_tag,
                                           .id_to_adc         = id_to_adc,
                                           .i2c_net_name_tag  = I2cNetName_vc_net_name_tag,
                                           .id_to_i2c         = id_to_i2c };