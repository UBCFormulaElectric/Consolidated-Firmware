#include "hw_uarts.hpp"
#include "main.h"

namespace hw
{
Uart sbg_ellipse_uart(huart8);

const Uart &getUartFromHandle(const UART_HandleTypeDef *handle)
{
    UNUSED(handle);
    return sbg_ellipse_uart;
}
} // namespace hw
