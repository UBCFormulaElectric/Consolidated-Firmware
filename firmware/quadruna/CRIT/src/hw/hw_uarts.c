#include "hw_uarts.h"

#include "io_chimera.h"
#include "main.h"

UartDevice chimera_uart = { .config = { .handle             = &huart2,
                                        .polling_timeout_ms = osWaitForever,
                                        .callback_dma       = false,
                                        .transmit_callback  = NULL,   // Doesn't use TX callbacks
                                        .receive_callback   = NULL } }; // Doesn't use RX callbacks

UartDevice *hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle)
{
    if (handle == &huart2)
    {
        return &chimera_uart;
    }
    else
    {
        return NULL;
    }
}
