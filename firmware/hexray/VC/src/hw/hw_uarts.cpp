#include "hw_uarts.hpp"
#ifndef USE_CHIMERA
#include "io_sbgEllipse.hpp"
#endif
#include "main.h"
#include <cassert>

const hw::Uart sbg_ellipse_uart{ huart8, true,
#ifdef USE_CHIMERA
                                 []() {}
#else
                                 io::sbgEllipse::msgRxCallback
#endif
};

const hw::Uart &hw::getUartFromHandle(const UART_HandleTypeDef *handle)
{
    assert(handle == &huart8);
    return sbg_ellipse_uart;
}
