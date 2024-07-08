#pragma once
#include "io_led.h"
#include "hw_gpios.h"

namespace io::leds
{
const io::binary_led imd_led{ hw::imd_r_pin };
const io::binary_led bspd_led{ hw::bspd_r_pin };
const io::binary_led ams_led{ hw::ams_r_pin };
const io::binary_led start_led{ hw::start_led_pin };
const io::binary_led regen_led{ hw::regen_led_pin };
const io::binary_led torquevec_led{ hw::torquevec_led_pin };

const io::led shutdown_status_led{ hw::shdn_r_pin, hw::shdn_g_pin, hw::shdn_b_pin_fake };

const io::led bms_status_led{ hw::bms_r_pin, hw::bms_g_pin, hw::bms_b_pin };
const io::led fvc_status_led{ hw::fvc_r_pin, hw::fvc_g_pin, hw::fvc_b_pin };
const io::led rvc_status_led{ hw::rvc_r_pin, hw::rvc_g_pin, hw::rvc_b_pin };
const io::led rsm_status_led{ hw::rsm_r_pin, hw::rsm_g_pin, hw::rsm_b_pin };
const io::led aux_status_led{ hw::aux_r_pin, hw::aux_g_pin, hw::aux_b_pin };
const io::led crit_status_led{ hw::crit_r_pin, hw::crit_g_pin, hw::crit_b_pin };
} // namespace io::leds