#include "hw_gpios.hpp"
#include "main.h"

const hw::gpio bots_3v3{ BOTS_3V3_GPIO_Port, BOTS_3V3_Pin };
const hw::gpio cockpit_shdn_3v3{ COCKPIT_SHDN_3V3_GPIO_Port, COCKPIT_SHDN_3V3_Pin };
const hw::gpio imu_nss{ IMU_CS_GPIO_Port, IMU_CS_Pin };
const hw::gpio imu_fsync{ IMU_FSYNC_GPIO_Port, IMU_FSYNC_Pin };
const hw::gpio nsusp_fl_ocsc{ nSUSP_FL_OCSC_GPIO_Port, nSUSP_FL_OCSC_Pin };
const hw::gpio nsusp_fr_ocsc{ nSUSP_FR_OCSC_GPIO_Port, nSUSP_FR_OCSC_Pin };
const hw::gpio fr_int_3v3{ FR_INT_3V3_GPIO_Port, FR_INT_3V3_Pin };
const hw::gpio fl_int_3v3{ FL_INT_3V3_GPIO_Port, FL_INT_3V3_Pin };
const hw::gpio debug_led{ DEBUG_LED_GPIO_Port, DEBUG_LED_Pin };
const hw::gpio nstr_angle_ocsc{ nSTR_ANGLE_OCSC_GPIO_Port, nSTR_ANGLE_OCSC_Pin };
const hw::gpio nbps_f_ocsc{ nBPS_F_OCSC_GPIO_Port, nBPS_F_OCSC_Pin };
const hw::gpio boot_led{ BOOT_LED_GPIO_Port, BOOT_LED_Pin };
const hw::gpio d_p_pullup(D_P_PULLUP_GPIO_Port, D_P_PULLUP_Pin);