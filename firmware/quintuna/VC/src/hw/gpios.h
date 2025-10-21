#pragma once

#include "hw/gpio.h"
#include <SEGGER_SYSVIEW.h>

extern const Gpio pgood;
extern const Gpio f_inv_en;
extern const Gpio r_inv_en;
extern const Gpio dam_en;
extern const Gpio rsm_en;
extern const Gpio imu_int1;
extern const Gpio imu_int2;
extern const Gpio pcm_en;
extern const Gpio pwr_mtr_nalert;
extern const Gpio bat_chrg_nshdn;
extern const Gpio bat_chrg_mode;
extern const Gpio front_en;
extern const Gpio fr_stby_front;
extern const Gpio fr_stby_rear;
extern const Gpio fr_stby_inv;
extern const Gpio fr_stby_f_pump;
extern const Gpio fr_stby_rl_pump;
extern const Gpio fr_stby_rr_pump;
extern const Gpio fr_stby_rad;
extern const Gpio bat_mtr_nalert;
extern const Gpio bms_en;
extern const Gpio rl_pump_en;
extern const Gpio rr_pump_en;
extern const Gpio f_pump_en;
extern const Gpio rl_rad_fan_en;
extern const Gpio rr_rad_fan_en;
extern const Gpio sb_shdn_3v3_out;
extern const Gpio interia_3v3_out;
extern const Gpio tsms_3v3_out;
extern const Gpio rr_ilck_3v3_out;
extern const Gpio led;
extern const Gpio boot_led;
extern const Gpio rl_pump_pgood;
extern const Gpio rr_pump_pgood;
extern const Gpio f_pump_pgood;