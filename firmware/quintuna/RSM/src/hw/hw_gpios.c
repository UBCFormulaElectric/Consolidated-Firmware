#include "hw_gpios.h"
#include "main.h"

const Gpio led_pin = { .port = LED_GPIO_Port, .pin = LED_Pin };

const Gpio brake_light_en_pin = { .port = BRAKE_LIGHT_EN_GPIO_Port, .pin = BRAKE_LIGHT_EN_Pin };

const Gpio rl_int_pin = { .port = RL_INT_3V3_SENS_GPIO_Port, .pin = RL_INT_3V3_SENS_Pin };

const Gpio imu_int1_pin = { .port = IMU_INT1_GPIO_Port, .pin = IMU_INT1_Pin };

const Gpio imu_int2_pin = { .port = IMU_INT2_GPIO_Port, .pin = IMU_INT2_Pin };

const Gpio boot_pin = { .port = BOOT_GPIO_Port, .pin = BOOT_Pin };

const Gpio susp_travel_rr_ocsc_pin = { .port = SUSP_TRAVEL_RR_OCSC_GPIO_Port, .pin = SUSP_TRAVEL_RR_OCSC_Pin };

const Gpio susp_travel_rl_ocsc_pin = { .port = SUSP_TRAVEL_RL_OCSC_GPIO_Port, .pin = SUSP_TRAVEL_RL_OCSC_Pin };