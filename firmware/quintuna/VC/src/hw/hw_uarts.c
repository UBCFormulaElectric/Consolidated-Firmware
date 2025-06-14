#include "hw_uarts.h"
#include "main.h"
#include "io_sbgEllipse.h"

UartDevice sbg_ellipse_uart = { .config = { .handle             = &huart8,
                                            .polling_timeout_ms = 0, // Doesn't use polling
                                            .callback_dma       = true,
                                            .transmit_callback  = NULL, // Doesn't use TX callbacks
                                            .receive_callback   = io_sbgEllipse_msgRxCallback } };

UartDevice *hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle)
{
    if (handle == &huart8)
    {
        return &sbg_ellipse_uart;
    }
    else
    {
        return NULL;
    }
}
