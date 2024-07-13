#pragma once
#include "hw_uart.h"
#include "main.h"

namespace hw::uart
{
extern const Uart chimera_uart{ &huart2 };
}
