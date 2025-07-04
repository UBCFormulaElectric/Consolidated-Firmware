#include "hw_gpios.h"
#include "hw_gpio.h"
#include "main.h"

const Gpio pgood           = { .port = PGOOD_GPIO_Port, .pin = PGOOD_Pin };
const Gpio f_inv_en        = { .port = F_INV_EN_GPIO_Port, .pin = F_INV_EN_Pin };
const Gpio r_inv_en        = { .port = R_INV_EN_GPIO_Port, .pin = R_INV_EN_Pin };
const Gpio dam_en          = { .port = DAM_EN_GPIO_Port, .pin = DAM_EN_Pin };
const Gpio rsm_en          = { .port = RSM_EN_GPIO_Port, .pin = RSM_EN_Pin };
const Gpio imu_int1        = { .port = IMU_INT1_GPIO_Port, .pin = IMU_INT1_Pin };
const Gpio imu_int2        = { .port = IMU_INT2_GPIO_Port, .pin = IMU_INT2_Pin };
const Gpio pcm_en          = { .port = PCM_EN_GPIO_Port, .pin = PCM_EN_Pin };
const Gpio pwr_mtr_nalert  = { .port = PWR_MTR_nALERT_GPIO_Port, .pin = PWR_MTR_nALERT_Pin };
const Gpio bat_chrg_nshdn  = { .port = BAT_CHRG_nSHDN_GPIO_Port, .pin = BAT_CHRG_nSHDN_Pin };
const Gpio bat_chrg_mode   = { .port = BAT_CHRG_MODE_GPIO_Port, .pin = BAT_CHRG_MODE_Pin };
const Gpio front_en        = { .port = FRONT_EN_GPIO_Port, .pin = FRONT_EN_Pin };
const Gpio fr_stby_front   = { .port = FR_STBY_FRONT_GPIO_Port, .pin = FR_STBY_FRONT_Pin };
const Gpio fr_stby_rear    = { .port = FR_STBY_REAR_GPIO_Port, .pin = FR_STBY_REAR_Pin };
const Gpio fr_stby_inv     = { .port = FR_STBY_INV_GPIO_Port, .pin = FR_STBY_INV_Pin };
const Gpio fr_stby_f_pump  = { .port = F_PUMP_PGOOD_GPIO_Port, .pin = F_PUMP_PGOOD_Pin };
const Gpio fr_stby_rl_pump = { .port = RL_PUMP_PGOOD_GPIO_Port, .pin = RL_PUMP_PGOOD_Pin };
const Gpio fr_stby_rr_pump = { .port = RR_PUMP_PGOOD_GPIO_Port, .pin = RR_PUMP_PGOOD_Pin };
const Gpio fr_stby_rad     = { .port = RAD_FAN_FR_STBY_GPIO_Port, .pin = RAD_FAN_FR_STBY_Pin };
const Gpio bat_mtr_nalert  = { .port = BAT_MTR_nALERT_GPIO_Port, .pin = BAT_MTR_nALERT_Pin };
const Gpio bms_en          = { .port = BMS_EN_GPIO_Port, .pin = BMS_EN_Pin };
const Gpio rl_pump_en      = { .port = RL_PUMP_EN_GPIO_Port, .pin = RL_PUMP_EN_Pin };
const Gpio rr_pump_en      = { .port = RR_PUMP_EN_GPIO_Port, .pin = RR_PUMP_EN_Pin };
const Gpio f_pump_en       = { .port = F_PUMP_EN_GPIO_Port, .pin = F_PUMP_EN_Pin };
const Gpio rl_rad_fan_en   = { .port = L_RAD_FAN_EN_GPIO_Port, .pin = L_RAD_FAN_EN_Pin };
const Gpio rr_rad_fan_en   = { .port = R_RAD_FAN_EN_GPIO_Port, .pin = R_RAD_FAN_EN_Pin };
const Gpio sb_shdn_3v3_out = { .port = SB_SHDN_3V3_OUT_GPIO_Port, .pin = SB_SHDN_3V3_OUT_Pin };
const Gpio interia_3v3_out = { .port = INTERIA_3V3_OUT_GPIO_Port, .pin = INTERIA_3V3_OUT_Pin };
const Gpio tsms_3v3_out    = { .port = TSMS_3V3_OUT_GPIO_Port, .pin = TSMS_3V3_OUT_Pin };
const Gpio rr_ilck_3v3_out = { .port = RR_ILCK_3V3_OUT_GPIO_Port, .pin = RR_ILCK_3V3_OUT_Pin };
const Gpio led             = { .port = LED_GPIO_Port, LED_Pin };
const Gpio boot_led        = { .port = BOOT_LED_GPIO_Port, BOOT_LED_Pin };
const Gpio rl_pump_pgood   = { .port = RL_PUMP_PGOOD_GPIO_Port, .pin = RL_PUMP_PGOOD_Pin };
const Gpio rr_pump_pgood   = { .port = RR_PUMP_PGOOD_GPIO_Port, .pin = RR_PUMP_PGOOD_Pin };
const Gpio f_pump_pgood    = { .port = F_PUMP_PGOOD_GPIO_Port, .pin = F_PUMP_PGOOD_Pin };
