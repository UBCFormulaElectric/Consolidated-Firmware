#include "hw_gpios.hpp"
#include "hw_gpio.hpp"
#include "main.h"

const hw::gpio brake_ocsc_ok(BRAKE_OCSC_OK_3V3_GPIO_Port, BRAKE_OCSC_OK_3V3_Pin);
const hw::gpio nbspd_brake_pressed_3v3(NBSPD_BRAKE_PRESSED_3V3_GPIO_Port, NBSPD_BRAKE_PRESSED_3V3_Pin);
const hw::gpio rl_int(RL_INT_3V3_SENS_GPIO_Port, RL_INT_3V3_SENS_Pin);
const hw::gpio susp_travel_rl_ocsc(SUSP_TRAVEL_RL_OCSC_GPIO_Port, SUSP_TRAVEL_RL_OCSC_Pin);
const hw::gpio susp_travel_rr_ocsc(SUSP_TRAVEL_RR_OCSC_GPIO_Port, SUSP_TRAVEL_RR_OCSC_Pin);
const hw::gpio brake_light_en(BRAKE_LIGHT_EN_3V3_GPIO_Port, BRAKE_LIGHT_EN_3V3_Pin);
const hw::gpio led_pin(LED_GPIO_Port, LED_Pin);
const hw::gpio imu_cs(IMU_CS_GPIO_Port, IMU_CS_Pin);
const hw::gpio imu_int(IMU_INT_GPIO_Port, IMU_INT_Pin);
const hw::gpio imu_fsync(IMU_FSYNC_GPIO_Port, IMU_FSYNC_Pin);
const hw::gpio d_p_pullup(D_P_PULLUP_GPIO_Port, D_P_PULLUP_Pin);
const hw::gpio boot_led(BOOT_LED_GPIO_Port, BOOT_LED_Pin);
