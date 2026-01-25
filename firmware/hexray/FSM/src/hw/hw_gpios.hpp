#pragma once
extern "C"
{
#include "main.h"
}
#include "hw_gpio.hpp"

namespace hw::gpios
{
extern constexpr Gpio bots_3v3;
extern constexpr Gpio cockpit_shdn_3v3;
extern constexpr Gpio imu_fsync;
extern constexpr Gpio nsusp_fl_ocsc;
extern constexpr Gpio nsusp_fr_ocsc;
extern constexpr Gpio fr_int_3v3;
extern constexpr Gpio fl_int_3v3;
extern constexpr Gpio debug_led;
extern constexpr Gpio nstr_angle_ocsc;
extern constexpr Gpio nbps_f_ocsc;
extern constexpr Gpio boot_led;
} // namespace hw::gpios
