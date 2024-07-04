#include "hw_gpio.h"

namespace hw
{
// switches
extern const gpio start_switch_gpio;
extern const gpio regen_switch_gpio;
extern const gpio torquevec_switch_gpio;

// binary leds
extern const gpio imd_r_pin;
extern const gpio bspd_r_pin;
extern const gpio ams_r_pin;

extern const gpio start_led_pin;
extern const gpio regen_led_pin;
extern const gpio torquevec_led_pin;

// rgb leds
extern const gpio shdn_r_pin;
extern const gpio shdn_g_pin;
extern const gpio shdn_b_pin_fake;

extern const gpio vc_r_pin;
extern const gpio vc_g_pin;
extern const gpio vc_b_pin;

extern const gpio bms_r_pin;
extern const gpio bms_g_pin;
extern const gpio bms_b_pin;

extern const gpio fsm_r_pin;
extern const gpio fsm_g_pin;
extern const gpio fsm_b_pin;

extern const gpio rsm_r_pin;
extern const gpio rsm_g_pin;
extern const gpio rsm_b_pin;

extern const gpio aux_r_pin;
extern const gpio aux_g_pin;
extern const gpio aux_b_pin;

extern const gpio crit_r_pin;
extern const gpio crit_g_pin;
extern const gpio crit_b_pin;

// drive mode pins
extern const gpio n_drive_mode_0_pin;
extern const gpio n_drive_mode_1_pin;
extern const gpio n_drive_mode_2_pin;
extern const gpio n_drive_mode_3_pin;

// shutdown sensors
extern const gpio cockpit_estop_sen_pin;
extern const gpio inertia_sen_pin;
} // namespace hw