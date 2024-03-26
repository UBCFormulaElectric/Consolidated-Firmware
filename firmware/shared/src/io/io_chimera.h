#pragma once

#include "hw_uart.h"
#include "hw_gpio.h"

void io_chimera_init(UART *serial_uart, uint32_t net_name_gpio, uint32_t net_name_adc, const Gpio *bootup_gpio);
void io_chimera_msgRxCallback(void);
void io_chimera_sleepTaskIfEnabled(void);