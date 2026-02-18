#include "hw_gpios.hpp"
#include "main.h"

const hw::Gpio bots_3v3{ BOTS_3V3_GPIO_Port, BOTS_3V3_Pin };
const hw::Gpio cockpit_shdn_3v3{ COCKPIT_SHDN_3V3_GPIO_Port, COCKPIT_SHDN_3V3_Pin };
const hw::Gpio imu_nss{ IMU_CS_GPIO_Port, IMU_CS_Pin };
const hw::Gpio imu_fsync{ IMU_FSYNC_GPIO_Port, IMU_FSYNC_Pin };
const hw::Gpio nsusp_fl_ocsc{ nSUSP_FL_OCSC_GPIO_Port, nSUSP_FL_OCSC_Pin };
const hw::Gpio nsusp_fr_ocsc{ nSUSP_FR_OCSC_GPIO_Port, nSUSP_FR_OCSC_Pin };
const hw::Gpio fr_int_3v3{ FR_INT_3V3_GPIO_Port, FR_INT_3V3_Pin };
const hw::Gpio fl_int_3v3{ FL_INT_3V3_GPIO_Port, FL_INT_3V3_Pin };
const hw::Gpio debug_led{ DEBUG_LED_GPIO_Port, DEBUG_LED_Pin };
const hw::Gpio nstr_angle_ocsc{ nSTR_ANGLE_OCSC_GPIO_Port, nSTR_ANGLE_OCSC_Pin };
const hw::Gpio nbps_f_ocsc{ nBPS_F_OCSC_GPIO_Port, nBPS_F_OCSC_Pin };
const hw::Gpio boot_led{ BOOT_LED_GPIO_Port, BOOT_LED_Pin };