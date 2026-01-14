#include "hw_gpios.hpp"
#include "hw_gpio.hpp"
#include "main.h"

namespace hw::gpio
{
const Gpio brake_ocsc_ok(BRAKE_OCSC_OK_3V3_GPIO_Port, BRAKE_OCSC_OK_3V3_Pin);
const Gpio rl_int(RL_INT_3V3_SENS_GPIO_Port, RL_INT_3V3_SENS_Pin);
const Gpio susp_travel_rl_ocsc(SUSP_TRAVEL_RL_OCSC_GPIO_Port, SUSP_TRAVEL_RL_OCSC_Pin);
const Gpio susp_travel_rr_ocsc(SUSP_TRAVEL_RR_OCSC_GPIO_Port, SUSP_TRAVEL_RR_OCSC_Pin);
const Gpio flow_meter_5v(FLOW_METER_5V_GPIO_Port, FLOW_METER_5V_Pin);
const Gpio brake_light_en(BRAKE_LIGHT_EN_3V3_GPIO_Port, BRAKE_LIGHT_EN_3V3_Pin);
const Gpio led(LED_GPIO_Port, LED_Pin);
const Gpio imu_cs(IMU_CS_GPIO_Port, IMU_CS_Pin);
const Gpio imu_int(IMU_INT_GPIO_Port, IMU_INT_Pin);
const Gpio imu_fsync(IMU_FSYNC_GPIO_Port, IMU_FSYNC_Pin);
const Gpio d_p_pullup(D_P_PULLUP_GPIO_Port, D_P_PULLUP_Pin);
} // namespace hw::gpio
