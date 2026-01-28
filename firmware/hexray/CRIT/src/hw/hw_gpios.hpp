#pragma once

#include "hw_gpio.hpp"
#include "hw_utils.hpp"

extern hw::Gpio boot_led;
extern hw::Gpio status_led;
extern hw::Gpio push_drive_sig;
extern hw::Gpio telem_sig;
extern hw::Gpio regen_sig;
extern hw::Gpio torque_vectoring_sig;
extern hw::Gpio launch_control_sig;
extern hw::Gpio rot_s;
extern hw::Gpio rot_b;
extern hw::Gpio rot_a;
extern hw::Gpio led_rck;
extern hw::Gpio seven_seg_rck;