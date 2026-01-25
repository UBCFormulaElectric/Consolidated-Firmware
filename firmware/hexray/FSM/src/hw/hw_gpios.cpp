#include "hw_gpios.hpp"

namespace hw::gpios
{
constexpr Gpio bots_3v3         = Gpio(BOTS_3V3_GPIO_Port, BOTS_3V3_Pin);
constexpr Gpio cockpit_shdn_3v3 = Gpio(COCKPIT_SHDN_3V3_GPIO_Port, COCKPIT_SHDN_3V3_Pin);
constexpr Gpio imu_fsync        = Gpio(IMU_FSYNC_GPIO_Port, IMU_FSYNC_Pin); // imu_int2
constexpr Gpio nsusp_fl_ocsc    = Gpio(nSUSP_FL_OCSC_GPIO_Port, nSUSP_FL_OCSC_Pin);
constexpr Gpio nsusp_fr_ocsc    = Gpio(nSUSP_FR_OCSC_GPIO_Port, nSUSP_FR_OCSC_Pin);
constexpr Gpio fr_int_3v3       = Gpio(FR_INT_3V3_GPIO_Port, FR_INT_3V3_Pin);
constexpr Gpio fl_int_3v3       = Gpio(FL_INT_3V3_GPIO_Port, FL_INT_3V3_Pin);
constexpr Gpio debug_led        = Gpio(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
constexpr Gpio nstr_angle_ocsc  = Gpio(nSTR_ANGLE_OCSC_GPIO_Port, nSTR_ANGLE_OCSC_Pin);
constexpr Gpio nbps_f_ocsc      = Gpio(nBPS_F_OCSC_GPIO_Port, nBPS_F_OCSC_Pin);
constexpr Gpio boot_led         = Gpio(BOOT_LED_GPIO_Port, BOOT_LED_Pin);
} // namespace hw::gpios
