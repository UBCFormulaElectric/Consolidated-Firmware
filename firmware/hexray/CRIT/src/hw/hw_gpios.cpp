#include "hw_gpios.hpp"
#include "main.h"

hw::Gpio boot_led(BOOT_GPIO_Port, BOOT_Pin);
hw::Gpio status_led(LED_GPIO_Port, LED_Pin);
hw::Gpio push_drive_sig(PUSH_DRIVE_SIG_GPIO_Port, PUSH_DRIVE_SIG_Pin);
hw::Gpio telem_sig(TELEM_SIG_GPIO_Port, TELEM_SIG_Pin);
hw::Gpio regen_sig(REGEN_SIG_GPIO_Port, REGEN_SIG_Pin);
hw::Gpio torque_vectoring_sig(TORQUE_VECTORING_SIG_GPIO_Port, TORQUE_VECTORING_SIG_Pin);
hw::Gpio launch_control_sig(LAUNCH_CONTROL_SIG_GPIO_Port, LAUNCH_CONTROL_SIG_Pin);
hw::Gpio rot_s(ROT_S_GPIO_Port, ROT_S_Pin);
hw::Gpio rot_b(ROT_B_GPIO_Port, ROT_B_Pin);
hw::Gpio rot_a(ROT_A_GPIO_Port, ROT_A_Pin);

hw::spi::SpiDevice pwr_chg_device
