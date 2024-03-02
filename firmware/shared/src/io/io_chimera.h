#pragma once

#include "hw_uart.h"

void io_chimera_init(UART *serial_uart, uint32_t net_name_gpio, uint32_t net_name_adc);
void io_chimera_msgRxCallback(void);