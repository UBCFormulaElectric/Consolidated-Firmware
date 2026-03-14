#include "hw_uarts.hpp"

#include "main.h"
#include <cassert>

const hw::Uart _900k_uart(&huart2);

const hw::Uart &getUartFromHandle(const UART_HandleTypeDef *handle)
{
    assert(handle == _900k_uart.getHandle());
    return _900k_uart;
}
