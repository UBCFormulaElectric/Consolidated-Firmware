#pragma once
#include "io_led.h"
#include "hw_gpios.h"

namespace io::leds
{
const io::binary_led imd_led{ hw::gpio::imd_r_pin };
const io::binary_led bspd_led{ hw::gpio::bspd_r_pin };
const io::binary_led ams_led{ hw::gpio::ams_r_pin };
const io::binary_led start_led{ hw::gpio::start_led_pin };
const io::binary_led regen_led{ hw::gpio::regen_led_pin };
const io::binary_led torquevec_led{ hw::gpio::torquevec_led_pin };
const io::binary_led debug_led{ hw::gpio::debug_led_pin };

const io::led shutdown_status_led{ hw::gpio::shdn_r_pin, hw::gpio::shdn_g_pin, hw::gpio::shdn_b_pin_fake };

const io::led bms_status_led{ hw::gpio::bms_r_pin, hw::gpio::bms_g_pin, hw::gpio::bms_b_pin };
const io::led fvc_status_led{ hw::gpio::fvc_r_pin, hw::gpio::fvc_g_pin, hw::gpio::fvc_b_pin };
const io::led rvc_status_led{ hw::gpio::rvc_r_pin, hw::gpio::rvc_g_pin, hw::gpio::rvc_b_pin };
const io::led rsm_status_led{ hw::gpio::rsm_r_pin, hw::gpio::rsm_g_pin, hw::gpio::rsm_b_pin };
const io::led aux_status_led{ hw::gpio::aux_r_pin, hw::gpio::aux_g_pin, hw::gpio::aux_b_pin };
const io::led crit_status_led{ hw::gpio::crit_r_pin, hw::gpio::crit_g_pin, hw::gpio::crit_b_pin };
} // namespace io::leds