#include "hw_uarts.hpp"
#include "io_sbgEllipse.hpp"
#include "main.h"
#include <cassert>

const hw::Uart sbg_ellipse_uart{ huart8, true, io::sbgEllipse::msgRxCallback };

const hw::Uart &hw::getUartFromHandle(const UART_HandleTypeDef *handle)
{
    assert(handle == &huart8);
    return sbg_ellipse_uart;
}
