#pragma once
#include "hw_gpio.h"

extern const Gpio boot;
extern const Gpio led;

extern const Gpio telem_sig;
extern const Gpio push_drive_sig;

extern const Gpio rot_s;
extern const Gpio rot_b;

extern const Gpio rot_a;
extern const Gpio launch_control_sig;
// deleted Vanilla_sig as it was a hotfix at comp
//new
extern const Gpio torque_vectoring_sig;
extern const Gpio regen_sig;

extern const Gpio led_srck;
extern const Gpio seven_seg_srck;

extern const Gpio led_rck;
extern const Gpio seven_seg_rck;

extern const Gpio led_serin;
extern const Gpio seven_seg_serin;