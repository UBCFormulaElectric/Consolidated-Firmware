#include "hw_gpios.h"
#include "main.h"

const Gpio pgood             = { .port = PGOOD_GPIO_Port, .pin = PGOOD_Pin };
const Gpio f_inv_en          = { .port = F_INV_EN_GPIO_Port, .pin = F_INV_EN_Pin };
const Gpio r_inv_en          = { .port = R_INV_EN_GPIO_Port, .pin = R_INV_EN_Pin };
const Gpio dam_en            = { .port = DAM_EN_GPIO_Port, .pin = DAM_EN_Pin };
const Gpio rsm_en            = { .port = RSM_EN_GPIO_Port, .pin = RSM_EN_Pin };
const Gpio imu_int1          = { .port = IMU_INT1_GPIO_Port, .pin = IMU_INT1_Pin };
const Gpio imu_int2          = { .port = IMU_INT2_GPIO_Port, .pin = IMU_INT2_Pin };
const Gpio pcm_en            = { .port = PCM_EN_GPIO_Port, .pin = PCM_EN_Pin };
const Gpio pwr_mtr_nalert    = { .port = PWR_MTR_nALERT_GPIO_Port, .pin = PWR_MTR_nALERT_Pin };
const Gpio bat_chrg_nshdn    = { .port = BAT_CHRG_nSHDN_GPIO_Port, .pin = BAT_CHRG_nSHDN_Pin };
const Gpio bat_chrg_mode     = { .port = BAT_CHRG_MODE_GPIO_Port, .pin = BAT_CHRG_MODE_Pin };
const Gpio front_en          = { .port = FRONT_EN_GPIO_Port, .pin = FRONT_EN_Pin };
const Gpio fr_stby_front     = { .port = FR_STBY_FRONT_GPIO_Port, .pin = FR_STBY_FRONT_Pin };
const Gpio fr_stby_rear      = { .port = FR_STBY_REAR_GPIO_Port, .pin = FR_STBY_REAR_Pin };
const Gpio fr_stby_inv       = { .port = FR_STBY_INV_GPIO_Port, .pin = FR_STBY_INV_Pin };
const Gpio fr_stby_f_pump    = { .port = F_PUMP_PGOOD_GPIO_Port, .pin = F_PUMP_PGOOD_Pin };
const Gpio fr_stby_rl_pump   = { .port = RL_PUMP_PGOOD_GPIO_Port, .pin = RL_PUMP_PGOOD_Pin };
const Gpio fr_stby_rr_pump   = { .port = RR_PUMP_PGOOD_GPIO_Port, .pin = RR_PUMP_PGOOD_Pin };
const Gpio fr_stby_rad       = { .port = 0, .pin = 0 };
const Gpio extra_en          = { .port = 0, .pin = 0 };
const Gpio bat_mtr_alert     = { .port = 0, .pin = 0 };
const Gpio bat_mtr_rst_shut  = { .port = 0, .pin = 0 };
const Gpio bms_en            = { .port = 0, .pin = 0 };
const Gpio rl_pump_en        = { .port = 0, .pin = 0 };
const Gpio rr_pump_en        = { .port = 0, .pin = 0 };
const Gpio f_pump_en         = { .port = 0, .pin = 0 };
const Gpio l_rad_fan_en      = { .port = 0, .pin = 0 };
const Gpio r_rad_fan_en      = { .port = 0, .pin = 0 };