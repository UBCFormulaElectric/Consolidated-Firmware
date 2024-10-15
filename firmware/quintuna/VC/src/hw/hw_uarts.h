#pragma once
#include "io_telemMessage.h"
#include "hw_uart.h"
#include "main.h"

namespace hw::uart
{
const Uart chimera_uart{ &huart7 };
const Uart sbg_uart{ &huart2 };
const Uart modem2G4_uart{ &huart3 };
const Uart modem900_uart{ &huart1 };
// TODO: once io_telemMessage is refactored
// const Modem modem = { &modem2G4_uart, &modem900_uart };
} // namespace hw::uart

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == hw::uart::chimera_uart.getHandle())
    {
        io::chimera::msgRxCallback();
    }
}
