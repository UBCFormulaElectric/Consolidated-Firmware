#include "hw_gpio.h"

namespace hw::gpio
{
// switches
extern const Gpio start_switch_gpio;
extern const Gpio regen_switch_gpio;
extern const Gpio torquevec_switch_gpio;

// binary leds
extern const Gpio imd_r_pin;
extern const Gpio bspd_r_pin;
extern const Gpio ams_r_pin;

extern const Gpio start_led_pin;
extern const Gpio regen_led_pin;
extern const Gpio torquevec_led_pin;

// rgb leds
extern const Gpio shdn_r_pin;
extern const Gpio shdn_g_pin;
extern const Gpio shdn_b_pin_fake;

extern const Gpio bms_r_pin;
extern const Gpio bms_g_pin;
extern const Gpio bms_b_pin;

extern const Gpio fvc_r_pin;
extern const Gpio fvc_g_pin;
extern const Gpio fvc_b_pin;

extern const Gpio rvc_r_pin;
extern const Gpio rvc_g_pin;
extern const Gpio rvc_b_pin;

extern const Gpio rsm_r_pin;
extern const Gpio rsm_g_pin;
extern const Gpio rsm_b_pin;

extern const Gpio aux_r_pin;
extern const Gpio aux_g_pin;
extern const Gpio aux_b_pin;

extern const Gpio crit_r_pin;
extern const Gpio crit_g_pin;
extern const Gpio crit_b_pin;

// drive mode pins
extern const Gpio n_drive_mode_0_pin;
extern const Gpio n_drive_mode_1_pin;
extern const Gpio n_drive_mode_2_pin;
extern const Gpio n_drive_mode_3_pin;

// shutdown sensors
extern const Gpio cockpit_estop_sen_pin;
extern const Gpio inertia_sen_pin;
extern const Gpio n_program_pin;
extern const Gpio n_chimera_pin;
extern const Gpio led_pin;
} // namespace hw::gpio