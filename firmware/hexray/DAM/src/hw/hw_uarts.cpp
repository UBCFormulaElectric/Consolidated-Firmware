#include "hw_uarts.hpp"

extern "C"
{
    #include "main.h"
}

// Not sure about this part

extern UART_HandleTypeDef huart1;  // declared in CubeMX autogen code?

hw::Uart uart1(&huart1);

// UartDevice _900k_uart = { .config = {
//                               .handle             = &huart2,
//                               .polling_timeout_ms = 1000,
//                               .callback_dma       = true,
//                               .transmit_callback  = NULL, // Doesn't use TX callbacks
//                               .receive_callback   = NULL  // Doesn't use RX callbacks
//                           } };

// UartDevice *hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle)
// {
//     if (handle == &huart2)
//     {
//         return &_900k_uart;
//     }
//     else
//     {
//         return NULL;
//     }
// }