#include "hw_uarts.h"

#include "io_chimera.h"
#include "io_sbgEllipse.h"
#include "main.h"

UartDevice chimera_uart     = { .config = { .handle             = &huart7,
                                            .polling_timeout_ms = osWaitForever,
                                            .callback_dma       = false,
                                            .transmit_callback  = NULL, // Doesn't use TX callbacks
                                            .receive_callback   = io_chimera_msgRxCallback } }; // Doesn't use RX callbacks
UartDevice sbg_ellipse_uart = { .config = { .handle             = &huart2,
                                            .polling_timeout_ms = 0, // Doesn't use polling
                                            .callback_dma       = true,
                                            .transmit_callback  = NULL, // Doesn't use TX callbacks
                                            .receive_callback   = io_sbgEllipse_msgRxCallback } };
UartDevice modem_2g4_uart   = { .config = { .handle             = &huart3,
                                            .polling_timeout_ms = 100,
                                            .callback_dma       = false,
                                            .transmit_callback  = NULL,   // Doesn't use TX callbacks
                                            .receive_callback   = NULL } }; // Doesn't use RX callbacks
UartDevice modem_900k_uart  = { .config = { .handle             = &huart1,
                                            .polling_timeout_ms = 100,
                                            .callback_dma       = false,
                                            .transmit_callback  = NULL,   // Doesn't use TX callbacks
                                            .receive_callback   = NULL } }; // Doesn't use RX callbacks

UartDevice *hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle)
{
    if (handle == &huart7)
    {
        return &chimera_uart;
    }
    else if (handle == &huart2)
    {
        return &sbg_ellipse_uart;
    }
    else if (handle == &huart3)
    {
        return &modem_2g4_uart;
    }
    else if (handle == &huart1)
    {
        return &modem_900k_uart;
    }
    else
    {
        return NULL;
    }
}
