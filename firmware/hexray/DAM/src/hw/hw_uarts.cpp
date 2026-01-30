#include "hw_uarts.hpp"

extern "C"
{
#include "main.h"
}

hw::Uart _900k_uart(&huart2);

hw::Uart *hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle)
{
    if (handle == &huart2)
    {
        return &_900k_uart;
    }

    return nullptr;
}
