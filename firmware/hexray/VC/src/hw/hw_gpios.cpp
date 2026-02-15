#include "hw_gpios.hpp"
#include "main.h"

hw::Gpio f_inv_en(F_INV_EN_GPIO_Port, F_INV_EN_Pin);
hw::Gpio f_inv_pg(F_INV_PG_GPIO_Port, F_INV_PG_Pin);
hw::Gpio r_inv_en(R_INV_EN_GPIO_Port, R_INV_EN_Pin);
hw::Gpio r_inv_pg(R_INV_PG_GPIO_Port, R_INV_PG_Pin);
hw::Gpio dam_en(DAM_EN_GPIO_Port, DAM_EN_Pin);
hw::Gpio dam_pg(DAM_PG_GPIO_Port, DAM_PG_Pin);
hw::Gpio bms_en(BMS_EN_GPIO_Port, BMS_EN_Pin);
hw::Gpio bms_pg(BMS_PG_GPIO_Port, BMS_PG_Pin);
hw::Gpio boost_pg(BOOST_PG_GPIO_Port, BOOST_PG_Pin);
hw::Gpio rsm_en(RSM_EN_GPIO_Port, RSM_EN_Pin);
hw::Gpio rsm_pg(RSM_PG_GPIO_Port, RSM_PG_Pin);
hw::Gpio rr_pump_en(RR_PUMP_EN_GPIO_Port, RR_PUMP_EN_Pin);
hw::Gpio rr_pump_pgood(RR_PUMP_PGOOD_GPIO_Port, RR_PUMP_PGOOD_Pin);
hw::Gpio rl_pump_en(RL_PUMP_EN_GPIO_Port, RL_PUMP_EN_Pin);
hw::Gpio rl_pump_pgood(RL_PUMP_PGOOD_GPIO_Port, RL_PUMP_PGOOD_Pin);
hw::Gpio pcm_en(PCM_EN_GPIO_Port, PCM_EN_Pin);
hw::Gpio l_rad_fan_en(L_RAD_FAN_EN_GPIO_Port, L_RAD_FAN_EN_Pin);
hw::Gpio l_rad_fan_pg(L_RAD_FAN_PG_GPIO_Port, L_RAD_FAN_PG_Pin);
hw::Gpio r_rad_fan_en(R_RAD_FAN_EN_GPIO_Port, R_RAD_FAN_EN_Pin);
hw::Gpio r_rad_fan_pg(R_RAD_FAN_PG_GPIO_Port, R_RAD_FAN_PG_Pin);
hw::Gpio front_en(FRONT_EN_GPIO_Port, FRONT_EN_Pin);
hw::Gpio front_pg(FRONT_PG_GPIO_Port, FRONT_PG_Pin);
hw::Gpio sb_shdn_3v3_out(SB_SHDN_3V3_OUT_GPIO_Port, SB_SHDN_3V3_OUT_Pin);
hw::Gpio bat_chrg_mode(BAT_CHRG_MODE_GPIO_Port, BAT_CHRG_MODE_Pin);
hw::Gpio bat_chrg_nSHDN(BAT_CHRG_nSHDN_GPIO_Port, BAT_CHRG_nSHDN_Pin);
hw::Gpio imu_fsync(IMU_FSYNC_GPIO_Port, IMU_FSYNC_Pin);
hw::Gpio pwr_mtr_nalert(PWR_MTR_nALERT_GPIO_Port, PWR_MTR_nALERT_Pin);
hw::Gpio boot(BOOT_GPIO_Port, BOOT_Pin);
hw::Gpio led(LED_GPIO_Port, LED_Pin);
hw::Gpio rr_ilck_3v3_out(RR_ILCK_3V3_OUT_GPIO_Port, RR_ILCK_3V3_OUT_Pin);
hw::Gpio tsms_3v3_out(TSMS_3V3_OUT_GPIO_Port, TSMS_3V3_OUT_Pin);
hw::Gpio inertia_3v3_out(INERTIA_3V3_OUT_GPIO_Port,INERTIA_3V3_OUT_Pin);
hw::Gpio imu_cs1(IMU_CS1_GPIO_Port, IMU_CS1_Pin);
hw::Gpio imu_cs2(IMU_CS2_GPIO_Port, IMU_CS2_Pin);
hw::Gpio imu_cs3(IMU_CS3_GPIO_Port, IMU_CS3_Pin);

// hw::Gpio inertia_switch_pin{};