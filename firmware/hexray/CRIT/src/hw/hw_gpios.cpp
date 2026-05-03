#include "hw_gpios.hpp"
#include "main.h"

const hw::gpio boot_led(BOOT_GPIO_Port, BOOT_Pin);
const hw::gpio status_led(LED_GPIO_Port, LED_Pin);
const hw::gpio push_drive_sig(PUSH_DRIVE_SIG_GPIO_Port, PUSH_DRIVE_SIG_Pin);
const hw::gpio telem_sig(TELEM_SIG_GPIO_Port, TELEM_SIG_Pin);
const hw::gpio regen_sig(REGEN_SIG_GPIO_Port, REGEN_SIG_Pin);
const hw::gpio torque_vectoring_sig(TORQUE_VECTORING_SIG_GPIO_Port, TORQUE_VECTORING_SIG_Pin);
const hw::gpio launch_control_sig(LAUNCH_CONTROL_SIG_GPIO_Port, LAUNCH_CONTROL_SIG_Pin);
const hw::gpio rot_s(ROT_S_GPIO_Port, ROT_S_Pin);
const hw::gpio rot_b(ROT_B_GPIO_Port, ROT_B_Pin);
const hw::gpio rot_a(ROT_A_GPIO_Port, ROT_A_Pin);
const hw::gpio led_rck(LED_RCK_GPIO_Port, LED_RCK_Pin);
const hw::gpio seven_seg_rck(_7SEG_RCK_GPIO_Port, _7SEG_RCK_Pin);

#ifndef USE_CHIMERA
// Rotary GPIO interrupt handler.
#include "io_rotary.hpp"
void HAL_GPIO_EXTI_Callback(const uint16_t GPIO_Pin)
{
    if (GPIO_Pin == rot_a.getPin() || GPIO_Pin == rot_b.getPin())
    {
        io::rotary::rotA_rotB_IRQHandler();
    }
    else if (GPIO_Pin == rot_s.getPin())
    {
        io::rotary::push_IRQHandler();
    }
}
#endif