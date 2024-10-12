#pragma once

namespace hw::bootup
{
/**
 * Due to quirks of STM32's generated code, this **must** be invoked at the top of main.c if using our CAN bootloader.
 * Otherwise interrupts will not work.
 */
void enableInterruptsForApp();
} // namespace hw::bootup
