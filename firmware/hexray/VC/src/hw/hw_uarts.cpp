#include "hw_uarts.hpp"
#include "main.h"
#include <cassert>

namespace hw
{
Uart sbg_ellipse_uart(&huart8);

Uart &hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle)
{
    assert(handle == sbg_ellipse_uart.getHandle());
    return sbg_ellipse_uart;
}
} // namespace hw
