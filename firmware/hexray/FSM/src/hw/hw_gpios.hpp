#pragma once
extern "C"
{
#include "main.h"
}
#include "hw_gpio.hpp"

namespace hw::gpios
{
extern const Gpio bots_3v3;
extern const Gpio cockpit_shdn_3v3;
extern const Gpio imu_nss;
extern const Gpio imu_fsync;
extern const Gpio nsusp_fl_ocsc;
extern const Gpio nsusp_fr_ocsc;
extern const Gpio fr_int_3v3;
extern const Gpio fl_int_3v3;
extern const Gpio debug_led;
extern const Gpio nstr_angle_ocsc;
extern const Gpio nbps_f_ocsc;
extern const Gpio boot_led;
} // namespace hw::gpios
