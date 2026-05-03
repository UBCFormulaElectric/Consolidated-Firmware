#include "hw_uarts.hpp"
#include "main.h"
#include <cassert>

namespace hw
{
constexpr Uart sbg_ellipse_uart{ huart8 };

const Uart &getUartFromHandle(const UART_HandleTypeDef *handle)
{
    assert(handle == &huart8);
    return sbg_ellipse_uart;
}
} // namespace hw
