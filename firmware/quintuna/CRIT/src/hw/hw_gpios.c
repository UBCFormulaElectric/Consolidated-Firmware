#include "hw_gpios.h"
#include "main.h"

const Gpio boot                 = { .port = BOOT_GPIO_Port, .pin = BOOT_Pin };
const Gpio led                  = { .port = LED_GPIO_Port, .pin = LED_Pin };
const Gpio telem_sig            = { .port = TELEM_SIG_GPIO_Port, .pin = LED_Pin };
const Gpio push_drive_sig       = { .port = PUSH_DRIVE_SIG_GPIO_Port, .pin = PUSH_DRIVE_SIG_Pin };
const Gpio rot_s                = { .port = ROT_S_GPIO_Port, .pin = ROT_S_Pin };
const Gpio rot_b                = { .port = ROT_B_GPIO_Port, .pin = ROT_B_Pin };
const Gpio rot_a                = { .port = ROT_A_GPIO_Port, .pin = ROT_A_Pin };
const Gpio launch_control_sig   = { .port = LAUNCH_CONTROL_SIG_GPIO_Port, .pin = LAUNCH_CONTROL_SIG_Pin };
const Gpio torque_vectoring_sig = { .port = TORQUE_VECTORING_SIG_GPIO_Port, .pin = TORQUE_VECTORING_SIG_Pin };
const Gpio regen_sig            = { .port = REGEN_SIG_GPIO_Port, .pin = REGEN_SIG_Pin };
const Gpio led_rck              = { .port = LED_RCK_GPIO_Port, .pin = LED_RCK_Pin };
const Gpio seven_seg_rck        = { .port = SEVEN_SEG_RCK_GPIO_Port, .pin = SEVEN_SEG_RCK_Pin };
const Gpio seven_seg_dimming    = { .port = SEVEN_SEG_DIMMING_GPIO_Port, .pin = SEVEN_SEG_DIMMING_Pin };
const Gpio led_dimming          = { .port = LED_DIMMING_GPIO_Port, .pin = LED_DIMMING_Pin };
const Gpio led_serin            = { .port = LED_SERIN_GPIO_Port, .pin = LED_SERIN_Pin};
const Gpio seven_seg_serin      = { .port = SEVEN_SEG_SERIN_GPIO_Port, .pin = SEVEN_SEG_SERIN_Pin};