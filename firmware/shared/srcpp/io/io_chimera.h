#pragma once

#include "hw_uart.h"
#include "hw_gpio.h"

#define CASERETURN(A, B) \
    case A:              \
        return B

namespace io::chimera
{
void init(const hw::Uart *serial_uart, uint32_t net_name_gpio, uint32_t net_name_adc, const hw::Gpio *bootup_gpio);
void msgRxCallback();
void sleepTaskIfEnabled();
} // namespace io::chimera
