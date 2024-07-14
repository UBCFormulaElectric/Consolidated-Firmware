#pragma once
#include "hw_uart.h"
#include "main.h"

namespace hw::uart
{
const Uart chimera_uart{ &huart2 };
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == hw::uart::chimera_uart.getHandle())
    {
        io::chimera::msgRxCallback();
    }
}
