#include "hw/gpios.h"
#include "io/log.h"
#include "main.h"
#include "io/rotary.h"

// Rotary GPIO interrupt handler.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == rot_a.pin || GPIO_Pin == rot_b.pin)
    {
        io_rotary_rotA_rotB_IRQHandler();
    }
    else if (GPIO_Pin == rot_s.pin)
    {
        io_rotary_push_IRQHandler();
    }
}

const Gpio boot               = { .port = BOOT_GPIO_Port, .pin = BOOT_Pin };
const Gpio led                = { .port = LED_GPIO_Port, .pin = LED_Pin };
const Gpio telem_sig          = { .port = TELEM_SIG_GPIO_Port, .pin = TELEM_SIG_Pin };
const Gpio push_drive_sig     = { .port = PUSH_DRIVE_SIG_GPIO_Port, .pin = PUSH_DRIVE_SIG_Pin };
const Gpio rot_s              = { .port = ROT_S_GPIO_Port, .pin = ROT_S_Pin };
const Gpio rot_b              = { .port = ROT_B_GPIO_Port, .pin = ROT_B_Pin };
const Gpio rot_a              = { .port = ROT_A_GPIO_Port, .pin = ROT_A_Pin };
const Gpio launch_control_sig = { .port = LAUNCH_CONTROL_SIG_GPIO_Port, .pin = LAUNCH_CONTROL_SIG_Pin };
const Gpio vanilla_sig        = { .port = VANILLA_SIG_GPIO_Port, .pin = VANILLA_SIG_Pin };
const Gpio regen_sig          = { .port = REGEN_SIG_GPIO_Port, .pin = REGEN_SIG_Pin };
const Gpio led_rck            = { .port = LED_RCK_GPIO_Port, .pin = LED_RCK_Pin };
const Gpio seven_seg_rck      = { .port = SEVEN_SEG_RCK_GPIO_Port, .pin = SEVEN_SEG_RCK_Pin };
const Gpio led_srck           = { .port = LED_SRCK_GPIO_Port, .pin = LED_SRCK_Pin };
const Gpio seven_seg_srck     = { .port = SEVEN_SEG_SRCK_GPIO_Port, .pin = SEVEN_SEG_SRCK_Pin };
const Gpio led_serin          = { .port = LED_SERIN_GPIO_Port, .pin = LED_SERIN_Pin };
const Gpio seven_seg_serin    = { .port = SEVEN_SEG_SERIN_GPIO_Port, .pin = SEVEN_SEG_SERIN_Pin };
