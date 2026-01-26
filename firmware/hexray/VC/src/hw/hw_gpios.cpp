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
